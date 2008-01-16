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

/// V1 + ... Vn <= X
template<typename VarRef, std::size_t size, typename VarSum, bool is_reversed = false>
struct LightLessEqualSumConstraint : public Constraint
{
  //typedef BoolLessSumConstraint<VarArray, VarSum,1-VarToCount> NegConstraintType;
 
  
  array<VarRef, size> var_array;  
  VarSum var_sum;
  LightLessEqualSumConstraint(const array<VarRef, size>& _var_array, const VarSum& _var_sum) :
    var_array(_var_array), var_sum(_var_sum)
  { }
  
  virtual triggerCollection setup_internal()
  {
    D_INFO(2,DI_SUMCON,"Setting up Constraint");
    triggerCollection t;
    
    int array_size = var_array.size();
    for(int i = 0; i < array_size; ++i)
    { t.push_back(make_trigger(var_array[i], Trigger(this, i), LowerBound)); }
    
    t.push_back(make_trigger(var_sum, Trigger(this, -1), UpperBound));
    return t;    
  }
  
  PROPAGATE_FUNCTION(int prop_val, DomainDelta)
  {
    int min_sum = 0;
    for(unsigned i = 0; i < size; ++i)
      min_sum += var_array[i].getMin();
    
    if(prop_val >= 0)
    { var_sum.setMin(min_sum); }
    
    int slack = var_sum.getMax() - min_sum;
    for(unsigned i = 0; i < size; ++i)
      var_array[i].setMax(var_array[i].getMin() + slack);
  }
  
  virtual bool full_check_unsat()
  { return check_unsat(0, 0); }
  
  virtual bool check_unsat(int, DomainDelta)
  {
    int min_sum = 0;
    for(unsigned i = 0; i < size; ++i)
      min_sum += var_array[i].getMin();
    return min_sum > var_sum.getMax();
  }
  
  virtual void full_propogate()
  {
    propogate(-1,0);
    propogate(0,0);
  }
  
  virtual bool check_assignment(vector<int> v)
  {
    D_ASSERT(v.size() == var_array.size() + 1);
    int sum = 0;
    int v_size = v.size();
    for(int i = 0; i < v_size - 1; i++)
      sum += v[i];
    return sum <= v.back();
  }
  
  virtual vector<AnyVarRef> get_vars()
  { 
    vector<AnyVarRef> array_copy;
	array_copy.reserve(var_array.size() + 1);
    for(unsigned i = 0; i < var_array.size(); ++i)
      array_copy.push_back(var_array[i]);
    array_copy.push_back(var_sum);
    return array_copy;
  }

 virtual Constraint* reverse_constraint()
  { return reverse_constraint_helper<is_reversed,int>::fun(*this); }

// BUGFIX: The following two class definitions have a 'T=int' just to get around a really stupid parsing bug
// in g++ 4.0.x. Hopefully eventually we'll be able to get rid of it.

/// These classes are just here to avoid infinite recursion when calculating the reverse of the reverse
/// of a constraint.
  template<bool reversed, typename T>
    struct reverse_constraint_helper    
  {
    static Constraint* fun(LightLessEqualSumConstraint& con)
    {
      typedef array<typename NegType<VarRef>::type, size> VarArray;
      VarArray new_var_array;
      for(unsigned i = 0; i < con.var_array.size(); ++i)
        new_var_array[i] = VarNegRef(con.var_array[i]);
      
      typedef typename ShiftType<typename NegType<VarSum>::type, compiletime_val<-1> >::type SumType;
      SumType new_sum = ShiftVarRef(VarNegRef(con.var_sum), compiletime_val<-1>());
      
      return new LightLessEqualSumConstraint<typename NegType<VarRef>::type, size, SumType, true>
        (new_var_array, new_sum);   
    }
  };
  
  template<typename T>
    struct reverse_constraint_helper<true, T>
  {
    static Constraint* fun(LightLessEqualSumConstraint&)
    { 
      // This should never be reached, unless we try reversing an already reversed constraint.
      // We have this code here as the above case makes templates, which if left would keep instansiating
      // recursively and without bound.
      FAIL_EXIT();
    }
  };
  
};

template<typename VarRef, std::size_t size, typename VarSum>
Constraint*
LightLessEqualSumCon(const array<VarRef,size>& _var_array,  const VarSum& _var_sum)
{ 
  return (new LightLessEqualSumConstraint<VarRef, size, VarSum>(_var_array,_var_sum)); 
}


template<typename VarRef, std::size_t size, typename VarSum>
Constraint*
LightGreaterEqualSumCon(const array<VarRef,size>& _var_array, const VarSum& _var_sum)
{ 
  return 
  (new LightLessEqualSumConstraint<typename NegType<VarRef>::type, size, 
   typename NegType<VarSum>::type>(VarNegRef(_var_array), VarNegRef(_var_sum))); 
}

