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

// This file contains lists of tuples, as required by table constraints. This
// is required as these tuples can get big, and it's important to keep them
// as compactly as possible.

class LiteralSpecificLists;
class Nightingale;
class TupleTrieArray;

class TupleList
{

  LiteralSpecificLists* litlists;
  Nightingale* nightingale;
  TupleTrieArray* triearray;
  
  int* tuple_data;
  int tuple_length;
  int number_of_tuples;
  bool tuples_locked;
 
  
  public:
  
  LiteralSpecificLists* getLitLists();
  Nightingale* getNightingale();
  TupleTrieArray* getTries();
  
  /// Get raw pointer to the tuples.
  int* getPointer()
  { return tuple_data; }
  
  /// Get number of tuples.
  int size() const
  { return number_of_tuples; }
  
  // Get size of a particular tuple.
  int tuple_size() const
  { return tuple_length; }
  
  // This function is temporary while the new interface is being designed.
  vector<int> get_vector(int pos) const
  {
    vector<int> vec(tuple_length);
	for(int i = 0; i < tuple_length; ++i)
	  vec[i] = tuple_data[pos * tuple_length + i];
	return vec;
  }
  
  TupleList(const vector<vector<int> >& tuple_list) : litlists(NULL), 
    tuples_locked(false), nightingale(NULL)
  {
    number_of_tuples = tuple_list.size();
	tuple_length = tuple_list[0].size();
	tuple_data = new int[number_of_tuples * tuple_length];
    for(int i = 0; i < number_of_tuples; ++i)
	  for(int j = 0; j < tuple_length; ++j)
	  { tuple_data[i * tuple_length + j] = tuple_list[i][j]; }
  }
  
  TupleList(int _numtuples, int _tuplelength) : litlists(NULL),
    tuples_locked(false), nightingale(NULL),
    number_of_tuples(_numtuples), tuple_length(_tuplelength)
  { tuple_data = new int[number_of_tuples * tuple_length]; }
  
  const int* operator[](int pos) const
  { return get_tupleptr(pos); }
  
  const int* get_tupleptr(int pos) const
  { 
	D_ASSERT(pos >= 0 && pos < number_of_tuples);
	return tuple_data + pos*tuple_length;
  }
  

  
 /// Original smallest value from each domain.
  vector<int> dom_smallest;
  /// Original size of each domain.
  vector<int> dom_size;
  
   /// Total number of literals in the variables at the start of search.
   int literal_num;
  
  /// Used by get_literal.
  vector<vector<int> > _map_vars_to_literal;
  
  /// Used to get a variable/value pair from a literal
  vector<pair<int,int> > _map_literal_to_vars;
  
  /// Maps a variable/value pair to a literal.
  int get_literal(int var_num, int dom_num)
  {
     D_ASSERT(var_num >= 0 && var_num < tuple_size());
	 D_ASSERT(dom_num >= dom_smallest[var_num] && 
			  dom_num < dom_smallest[var_num] + dom_size[var_num]);
    return _map_vars_to_literal[var_num][dom_num - dom_smallest[var_num]]; 
  }
  
  pair<int,int> get_varval_from_literal(int literal)
  { return _map_literal_to_vars[literal]; }

   /// Sets up the variable/value pair to literal mapping, used by @get_literal.
  void finalise_tuples()
  {
    if(tuples_locked)
	  return;
	tuples_locked = true;
	
    int arity = tuple_size();	
    
   	  // Set up the table of tuples.
	  for(unsigned i = 0; i < arity; ++i)
	  {
		int min_val = get_tupleptr(0)[i];
		int max_val = get_tupleptr(0)[i];
		for(unsigned j = 1; j < size(); ++j)
		{
		  min_val = mymin(min_val, get_tupleptr(j)[i]);
		  max_val = mymax(max_val, get_tupleptr(j)[i]);
		}
		dom_smallest.push_back(min_val);
		dom_size.push_back(max_val - min_val + 1);
	  }
	  
	 


	_map_vars_to_literal.resize(dom_size.size());
	// For each variable / value pair, get a literal
	int literal_count = 0;
	for(unsigned i = 0; i < dom_size.size(); ++i)
	{
	  _map_vars_to_literal[i].resize(dom_size[i]);
	  for(int j = 0; j < dom_size[i]; ++j)
	  {
		_map_vars_to_literal[i][j] = literal_count;
		_map_literal_to_vars.push_back(make_pair(i, j + dom_smallest[i]));
		D_ASSERT(get_literal(i, j + dom_smallest[i]) == literal_count);
		D_ASSERT(get_varval_from_literal(literal_count).first == i);
		D_ASSERT(get_varval_from_literal(literal_count).second == j + dom_smallest[i]); 
		++literal_count;
	  }
	}
	 literal_num = literal_count;
  }
  
};



VARDEF(std::vector<TupleList*> Internal_TupleList);

inline TupleList* getNewTupleList(int numtuples, int tuplelength)
{
  TupleList* tuplelist_ptr = new TupleList(numtuples, tuplelength);
  Internal_TupleList.push_back(tuplelist_ptr);
  return tuplelist_ptr;
}

inline TupleList* getNewTupleList(const vector<vector<int> >& tuples)
{ 
  TupleList* tuplelist_ptr = new TupleList(tuples);
  Internal_TupleList.push_back(tuplelist_ptr);
  return tuplelist_ptr;
}

inline TupleList* getTupleList(int num)
{ return Internal_TupleList[num]; }

inline int getNumOfTupleLists()
{ return Internal_TupleList.size(); }


/// The first GACtable implementation.
struct LiteralSpecificLists
{
  TupleList* tuples;
  
  /// For each literal, a list of the tuples it is present in.  
  vector<vector<vector<int> > > literal_specific_tuples;
  
  LiteralSpecificLists(TupleList* _tuples) : tuples(_tuples)
  { 
	tuples->finalise_tuples();
	  // For each literal, store the set of tuples which it allows.
	  for(unsigned i = 0; i < tuples->dom_size.size(); ++i)
	  {
		for(int j = tuples->dom_smallest[i]; 
				j < tuples->dom_smallest[i] + tuples->dom_size[i];
				 ++j)
		{
		  vector<vector<int> > specific_tuples;
		  for(unsigned k = 0; k < (*tuples).size(); ++k)
		  {
			if((*tuples)[k][i] == j)
			  specific_tuples.push_back((*tuples).get_vector(k));
		  }
		  literal_specific_tuples.push_back(specific_tuples);
		  //D_ASSERT(literal_specific_tuples.size() - 1 == get_literal(i,j));
		}
	  }
  }
};

inline LiteralSpecificLists* TupleList::getLitLists()
{
  if(litlists == NULL)
    litlists = new LiteralSpecificLists(this);
  return litlists;
}