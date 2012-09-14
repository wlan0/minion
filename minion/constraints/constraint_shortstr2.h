/*
* Minion http://minion.sourceforge.net
* Copyright (C) 2006-09
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

/** @help constraints;shortstr2 Description
Another type of table constraint.
*/

/** @help constraints;shortstr2 Example 

ShortSTR2 is an implementation of STR2 by Christophe Lecoutre, adapted for
short supports.

shortstr2([x,y,z], [[1,2,3], [1,3,2]])

*/

/** @help constraints;shortstr2 Notes
This constraint enforces generalized arc consistency.
*/

#ifndef CONSTRAINT_STR2_H
#define CONSTRAINT_STR2_H



/*
* Minion http://minion.sourceforge.net
* Copyright (C) 2006-09
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
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <utility>

using namespace std;

template<typename VarArray>
struct ShortSTR2
{
    VarArray vars;
    
    bool constraint_locked;
    
    vector<int> tupindices;
    
    TupleList* tups;
    
    ReversibleInt limit;   // In tupindices, indices less than limit are not known to be invalid.
    
    int numtups;
    
    ShortSupportsGAC(StateObj* _stateObj, const VarArray& _var_array, TupleList* tuples) : AbstractConstraint(_stateObj), 
    vars(_var_array), constraint_locked(false), tups(tuples), limit(_stateObj)
    {
        numtups=tuples->size();   //
        limit=numtups;
        
        tupindices.resize(limit);
        for(int i=0; i<numtups; i++) {
            tupindices[i]=i;
        }
        
    }
    
    virtual string constraint_name()
    { 
        return "ShortSTR2";
    }
    
    
    // Set up triggers.
    virtual triggerCollection setup_internal()
    {
        triggerCollection t;
        int array_size = vars.size();
        for(int i = 0; i < array_size; ++i) {
            t.push_back(make_trigger(vars[i], Trigger(this, i), DomainChanged));
        }
        return t;
    }
    
    
    
    virtual void propagate(int prop_var, DomainDelta)
    {
        if(!constraint_locked)
        {
            constraint_locked = true;
            getQueue(stateObj).pushSpecialTrigger(this);
        }
    }
    
    
    
    virtual void special_unlock() { constraint_locked = false; }
    
    virtual void special_check()
    {
        constraint_locked = false;
        
        if(getState(stateObj).isFailed())
        {
            return;
        }
        
        do_prop();
    }
    
    
    
    void do_prop() {
        
        
        
        
        
    }
    
    
};


#endif
