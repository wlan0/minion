/*
 *  triggers.h
 *  cutecsp
 *
 *  Created by Chris Jefferson on 25/03/2006.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

/// Container for a range of triggers
struct TriggerRange
{
  /// Start of triggers
  Trigger* start;
  /// End of triggers
  Trigger* end;
  /// The domain delta from the domain change.
  /** This may not contain the actual delta, but contains data from which a variable can
   construct it, by passing it to getDomainChange. */
  short data;
  TriggerRange(Trigger* s, Trigger* e, int _data) : start(s), end(e), data(_data)
  { 
    D_ASSERT(data >= std::numeric_limits<short>::min());
    D_ASSERT(data <= std::numeric_limits<short>::max());
  }
};



///The classes which are used to build the queue.
struct Trigger
{ 
  /// The constraint to be propagated.
  Constraint* constraint;
#ifdef FUNCTIONPOINTER_TRIGGER
  typedef void(*fun_ptr)(Constraint*, int, DomainDelta);
  fun_ptr constraint_function_ptr;
#endif  
  /// The first value to be passed to the propagate function.
  int info;
  
  template<typename T>
    Trigger(T* _sc, int _info) : constraint(_sc), info(_info)
#ifdef FUNCTIONPOINTER_TRIGGER
  { 
    typedef void(T::*prop_ptr_type)(int, DomainDelta);
	prop_ptr_type ptr = &T::propogate;
    memcpy(&constraint_function_ptr,&ptr,sizeof(fun_ptr)); 
	D_ASSERT(constraint_function_ptr != NULL);
  }
#else
  {  }
#endif
  
  Trigger(const Trigger& t) : constraint(t.constraint), info(t.info) 
#ifdef FUNCTIONPOINTER_TRIGGER
  , constraint_function_ptr(t.constraint_function_ptr)
#endif
  {}
  
  Trigger() : constraint(NULL)
  {}
  
  void propogate(DomainDelta domain_data);
  void full_propogate();
  // In function_defs.hpp.
};

/// Abstract Type that represents any Trigger Creator.
struct AbstractTriggerCreator
{
  Trigger trigger;
  TrigType type;
  virtual void post_trigger() = 0;
  AbstractTriggerCreator(Trigger t, TrigType _type) : trigger(t), type(_type) {}
  virtual ~AbstractTriggerCreator()
  { }
};

/**
 * @brief Concrete Trigger Creators.
 * Allows a trigger to be passed around before being imposed. This is here so reification works.
 */
template<typename VarRef>
struct TriggerCreator : public AbstractTriggerCreator
{
  VarRef ref;
  int val;
  TriggerCreator(VarRef& v, Trigger t, TrigType _type, int _val = -999) :
    AbstractTriggerCreator(t, _type),  ref(v), val(_val)
  {}
  
  virtual void post_trigger()
  { ref.addTrigger(trigger, type, val); }
  
  virtual ~TriggerCreator()
  { }
};

template<typename VarRef>
inline shared_ptr<AbstractTriggerCreator> 
make_trigger(VarRef v, Trigger t, TrigType trigger_type, int val = -999)
{ return shared_ptr<AbstractTriggerCreator>(new TriggerCreator<VarRef>(v,t, trigger_type, val));}
