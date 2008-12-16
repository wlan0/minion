/* Minion Constraint Solver
   http://minion.sourceforge.net
   
   For Licence Information see file LICENSE.txt 

   $Id: constraint_gcc.h 668 2007-09-26 15:14:50Z pete_n $
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


/** @help constraints;gcc Description 
The Generalized Cardinality Constraint (GCC) constrains the number of each value
that a set of variables can take.

gcc([primary variables], [values of interest], [capacity variables])

For each value of interest, there must be a capacity variable, which specifies
the number of occurrences of the value in the primary variables.

This constraint is new, and its syntax and implementation are not finalised.
*/

/** @help constraints;gcc Example 
Suppose the input file had the following vectors of variables defined:

DISCRETE myVec[9] {1..9}
BOUND cap[9] {0..2}

The following constraint would restrict the occurrence of values 1..9 in myVec
to be at most 2 each initially, and finally equal to the values of the cap
vector.

gcc(myVec, [1,2,3,4,5,6,7,8,9], cap)
*/

/** @help constraints;gcc Reifiability
This constraint is reifyimply'able but not reifiable.
*/

/** @help constraints;gcc Notes 
This constraint enforces a hybrid consistency. It reads the bounds of the
capacity variables, then enforces GAC over the primary variables only.  Then the
bounds of the capacity variables are updated by counting values in the domains
of the primary variables.
*/

#ifndef CONSTRAINT_GCC_H_PQWOEI
#define CONSTRAINT_GCC_H_PQWOEI

#include "gcc_common.h"

template<typename VarArray1, typename VarArray2>
AbstractConstraint*
GlobalCardCon(StateObj* stateObj, const VarArray1& var_array, const VarArray2& cap_array, ConstraintBlob& b)
{ return new GCC<VarArray1, VarArray2>(stateObj, var_array, cap_array, b); }


BUILD_CONSTRAINT2_WITH_BLOB(CT_GCC, GlobalCardCon)

#endif
