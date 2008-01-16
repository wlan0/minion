/* Minion Constraint Solver
   http://minion.sourceforge.net
   
   For Licence Information see file LICENSE.txt 

   $Id$
*/

/* Minion
* Copyright (C) 2006
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/


/// Internal type used by AnyVarRef.
struct AnyVarRef_Abstract
{
  virtual BOOL isBound() = 0;
  virtual BOOL isAssigned() = 0;  
  virtual int getAssignedValue() = 0;
  virtual BOOL isAssignedValue(int i) = 0;
  virtual BOOL inDomain(int b) = 0;
  virtual BOOL inDomain_noBoundCheck(int b) = 0;
  virtual int getMax() = 0;
  virtual int getMin() = 0;
  virtual int getInitialMax() const = 0;
  virtual int getInitialMin() const = 0;
  virtual void setMax(int i) = 0;
  virtual void setMin(int i) = 0;
  virtual void uncheckedAssign(int b) = 0;
  virtual void propogateAssign(int b) = 0;
  virtual void removeFromDomain(int b) = 0;
  virtual void addTrigger(Trigger t, TrigType type) = 0;

  virtual string virtual_to_string() = 0;
  
  virtual ~AnyVarRef_Abstract()
  {}
  
  virtual int getDomainChange(DomainDelta d) = 0;
#ifdef DYNAMICTRIGGERS
  virtual void addDynamicTrigger(DynamicTrigger* t, TrigType type, int pos = -999) = 0;
#endif
};

/// Internal type used by AnyVarRef.
template<typename VarRef>
struct AnyVarRef_Concrete : public AnyVarRef_Abstract
{

  virtual BOOL isBound()
  { return data.isBound();}
  
  VarRef data;
  AnyVarRef_Concrete(const VarRef& _data) : data(_data)
  {}
  
  AnyVarRef_Concrete() 
  {}
  
  AnyVarRef_Concrete(const AnyVarRef_Concrete& b) : data(b.data)
  {}
  
  virtual BOOL isAssigned()
  { return data.isAssigned(); }
  
  virtual int getAssignedValue()
  { return data.getAssignedValue(); }
  
  virtual BOOL isAssignedValue(int i)
  { return data.isAssignedValue(i); }
  
  virtual BOOL inDomain(int b)
  { return data.inDomain(b); }
  
  virtual BOOL inDomain_noBoundCheck(int b)
  { return data.inDomain_noBoundCheck(b); }
  
  virtual int getMax()
  { return data.getMax(); }
  
  virtual int getMin()
  { return data.getMin(); }

  virtual int getInitialMax() const
  { return data.getInitialMax(); }
  
  virtual int getInitialMin() const
  { return data.getInitialMin(); }
  
  virtual void setMax(int i)
  { data.setMax(i); }
  
  virtual void setMin(int i)
  { data.setMin(i); }
  
  virtual void uncheckedAssign(int b)
  { data.uncheckedAssign(b); }
  
  virtual void propogateAssign(int b)
  { data.propogateAssign(b); }
  
  virtual void removeFromDomain(int b)
  { data.removeFromDomain(b); }
  
  virtual void addTrigger(Trigger t, TrigType type)
  { data.addTrigger(t, type); }
  
  virtual string virtual_to_string()
  { return string(data); }
  
  virtual ~AnyVarRef_Concrete()
  {}
  
  int getDomainChange(DomainDelta d)
  { return data.getDomainChange(d); }

#ifdef DYNAMICTRIGGERS
  void addDynamicTrigger(DynamicTrigger* t, TrigType type, int pos = -999)
  {  data.addDynamicTrigger(t, type, pos); }
#endif
};

/// Provides a method of wrapping any variable type in a general wrapper.
struct AnyVarRef
{
  static const BOOL isBool = false;
  static const BoundType isBoundConst = Bound_Maybe;
  shared_ptr<AnyVarRef_Abstract> data;
  
  BOOL isBound()
  { return data->isBound();}
  
  template<typename VarRef>
    AnyVarRef(const VarRef& _data) 
  { data = shared_ptr<AnyVarRef_Abstract>(new AnyVarRef_Concrete<VarRef>(_data)); }
  
  AnyVarRef() 
  {}
  
  AnyVarRef(const AnyVarRef& b) : data(b.data)
  {}
  
  BOOL isAssigned()
  { return data->isAssigned(); }
  
  int getAssignedValue()
  { return data->getAssignedValue(); }
  
  BOOL isAssignedValue(int i)
  { 
    return data->isAssigned() &&
    data->getAssignedValue() == i;
  }
  
  BOOL inDomain(int b)
  { return data->inDomain(b); }

  BOOL inDomain_noBoundCheck(int b)
  { return data->inDomain_noBoundCheck(b); }
  
  int getMax()
  { return data->getMax(); }
  
  int getMin()
  { return data->getMin(); }

  int getInitialMax() const
  { return data->getInitialMax(); }
  
  int getInitialMin() const
  { return data->getInitialMin(); }
  
  void setMax(int i)
  { data->setMax(i); }
  
  void setMin(int i)
  { data->setMin(i); }
  
  void uncheckedAssign(int b)
  { data->uncheckedAssign(b); }
  
  void propogateAssign(int b)
  { data->propogateAssign(b); }
  
  void removeFromDomain(int b)
  { data->removeFromDomain(b); }

  void addTrigger(Trigger t, TrigType type)
  { data->addTrigger(t, type); }
  
  operator string()
  { return "AnyVRef:" + data->virtual_to_string(); }
  
  int getDomainChange(DomainDelta d)
  { return data->getDomainChange(d); }
  
#ifdef DYNAMICTRIGGERS
  void addDynamicTrigger(DynamicTrigger* t, TrigType type, int pos = -999)
  {  data->addDynamicTrigger(t, type, pos); }
#endif
};


