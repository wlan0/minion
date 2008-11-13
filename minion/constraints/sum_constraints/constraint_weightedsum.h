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

/** @help constraints;weightedsumleq Description
The constraint

   weightedsumleq(constantVec, varVec, total)

ensures that constantVec.varVec <= total, where constantVec.varVec is
the scalar dot product of constantVec and varVec.
*/

/** @help constraints;weightedsumleq Reifiability
This constraint is reifiable and reifyimply'able.
*/

/** @help constraints;weightedsumleq References
help constraints weightedsumgeq
help constraints sumleq
help constraints sumgeq
*/

/** @help constraints;weightedsumgeq Description
The constraint

   weightedsumgeq(constantVec, varVec, total)

ensures that constantVec.varVec >= total, where constantVec.varVec is
the scalar dot product of constantVec and varVec.
*/

/** @help constraints;weightedsumgeq Reifiability
This constraint is reifiable and reifyimply'able.
*/

/** @help constraints;weightedsumgeq References
help constraints weightedsumleq
help constraints sumleq
help constraints sumgeq
*/

#ifndef CONSTRAINT_WEIGHTSUM_H
#define CONSTRAINT_WEIGHTSUM_H

#include "constraint_sum.h"
 
template<typename T1, typename T2>
AbstractConstraint*
LeqWeightedSum(StateObj* stateObj, light_vector<T1> vec, const light_vector<T2>& t2, const ConstraintBlob& b)
{
  vector<int> scale = b.constants[0];
  // Preprocess to remove any multiplications by 0, both for efficency
  // and correctness
  if(scale.size() != vec.size())
  {
    FAIL_EXIT("In a weighted sum constraint, the vector of weights must have the same length to the vector of variables.");
  }
  for(unsigned i = 0; i < scale.size(); ++i)
  {
    if(scale[i] == 0)
    {
      scale.erase(scale.begin() + i);
      vec.erase(vec.begin() + i);
      --i; // So we don't miss an element.
    }
  }

  BOOL multipliers_size_one = true;
  for(unsigned i = 0; i < scale.size(); ++i)
  {
    if(scale[i] != 1 && scale[i] != -1)
    {
      multipliers_size_one = false;
      i = scale.size();
    }
  }

  if(multipliers_size_one)
  {
    light_vector<SwitchNeg<T1> > mult_vars(vec.size());
    for(unsigned int i = 0; i < vec.size(); ++i)
      mult_vars[i] = SwitchNeg<T1>(vec[i], scale[i]);
    return LessEqualSumCon(stateObj, mult_vars, t2);
  }
  else
  {
    light_vector<MultiplyVar<T1> > mult_vars(vec.size());
    for(unsigned int i = 0; i < vec.size(); ++i)
      mult_vars[i] = MultiplyVar<T1>(vec[i], scale[i]);
    return LessEqualSumCon(stateObj, mult_vars, t2);
  }
}

// Don't pass in the vectors by reference, as we might need to copy them.
template<typename T1, typename T2>
AbstractConstraint*
  GeqWeightedSum(StateObj* stateObj, light_vector<T1> vec, const light_vector<T2>& t2, const ConstraintBlob& b)
{
  vector<int> scale = b.constants[0];
  // Preprocess to remove any multiplications by 0, both for efficency
  // and correctness
  if(scale.size() != vec.size())
  {
    FAIL_EXIT("In a weighted sum constraint, the vector of weights must have the same length as the vector of variables.");
  }
  for(unsigned i = 0; i < scale.size(); ++i)
  {
    if(scale[i] == 0)
    {
      scale.erase(scale.begin() + i);
      vec.erase(vec.begin() + i);
      --i; // So we don't miss an element.
    }
  }

  BOOL multipliers_size_one = true;  
  for(unsigned i = 0; i < scale.size(); ++i)
  {
    if(scale[i] != 1 && scale[i] != -1)
    {
      multipliers_size_one = false;
      i = scale.size();
    }
  }

  if(multipliers_size_one)
  {
    light_vector<SwitchNeg<T1> > mult_vars(vec.size());
    for(unsigned int i = 0; i < vec.size(); ++i)
      mult_vars[i] = SwitchNeg<T1>(vec[i], scale[i]);
    return GreaterEqualSumCon(stateObj, mult_vars, t2);
  }
  else
  {
    light_vector<MultiplyVar<T1> > mult_vars(vec.size());
    for(unsigned int i = 0; i < vec.size(); ++i)
      mult_vars[i] = MultiplyVar<T1>(vec[i], scale[i]);
    return GreaterEqualSumCon(stateObj, mult_vars, t2);
  }
}

BUILD_CONSTRAINT2_WITH_BLOB(CT_WEIGHTLEQSUM, LeqWeightedSum)
BUILD_CONSTRAINT2_WITH_BLOB(CT_WEIGHTGEQSUM, GeqWeightedSum)

#endif