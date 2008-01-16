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

#ifndef DEBUG_H
#define DEBUG_H

struct parse_exception : public exception
{
  string error;
  parse_exception(string s) : error(s)
  {}
  
  virtual const char* what() const throw() 
  { return error.c_str(); }
  
  virtual ~parse_exception() throw()
  {}
};

#define D_FATAL_ERROR(s) { D_FATAL_ERROR2(s,  __FILE__, to_string(__LINE__)); abort(); }

void inline D_FATAL_ERROR2(string s, string file, string line);


void inline D_FATAL_ERROR2(string s, string file, string line)
{ 
  cerr << "Sorry, there has been some kind of error." << endl;
  cerr << "This could be caused by a misformed input file, or by an internal bug." << endl;
  cerr << "If you can't figure out what is causing the problem, please report it at http://www.sourceforge.net/projects/minion." << endl;
  cerr << "Either on the bug tracker, or on the mailing list." << endl;
  cerr << endl;
  cerr << "The generated error message was: " << s << endl;
  cerr << "The error was in the file " << file << " on line " << line << endl;
#ifndef NO_DEBUG 
   int* i = NULL;
   *i = 0;
#endif
  abort();
}

template<typename T>
std::string
print_vec(T t)
{
  std::ostringstream streamOut;
  streamOut << "(";
  for(unsigned int i=0;i<t.size();i++)
    streamOut << t[i] << ",";
  streamOut << ")";
  return streamOut.str();
}

template<typename T>
std::string
to_string(T t)
{
  std::ostringstream streamOut;
  streamOut << t;
  return streamOut.str();
}

template<typename T1, typename T2>
std::string
to_string(T1 t1, T2 t2)
{
  std::ostringstream streamOut1;
  streamOut1 << t1;
  std::ostringstream streamOut2;
  streamOut2 << t2;
  return streamOut1.str() + ":" + streamOut2.str();
}

template<typename T1, typename T2, typename T3>
std::string
to_string(T1 t1, T2 t2, T3 t3)
{
  std::ostringstream streamOut1;
  streamOut1 << t1;
  std::ostringstream streamOut2;
  streamOut2 << t2;
  std::ostringstream streamOut3;
  streamOut3 << t3;
  
  return streamOut1.str() + ":" + streamOut2.str() + ":" + streamOut3.str();
}


#ifndef NO_DEBUG

struct assert_fail {};

inline void assert_function(bool x, string a, string f, int line)
{ 
  if(!x) 
  {
    cerr << "Assert Error!" << endl;
	cerr << "Test '" << a << "' failed." << endl;
	cerr << "In file " << f << ", line " << line << endl;
    cerr << "\n";
	cout << "\n";
    cout.flush();
	cerr.flush();
    throw new assert_fail;
  }
}

#define FAIL_EXIT() { int* nullvar = NULL; *nullvar = 0; exit(1); }
#define D_ASSERT(x) {assert_function(x, #x, __FILE__, __LINE__);}



#define D_DATA(x) x


enum debug_types
{ DI_SOLVER, DI_SUMCON, DI_BOOLCON, DI_ANDCON, DI_ARRAYAND, DI_QUEUE, DI_REIFY,
  DI_LEXCON, DI_TABLECON, DI_TEST, DI_DYSUMCON, DI_DYNAMICTRIG, DI_DYELEMENT, 
  DI_INTCONTAINER, DI_GACELEMENT, DI_CHECKCON, DI_VECNEQ };
  
#define DEBUG_CASE(x) case x: std::cerr << #x; break;

#ifdef NO_PRINT
#define D_INFO(x,y,z)
#else
// importance: 0 = trivial, 1 = important, 2 = v. important.
  inline void D_INFO(int importance, debug_types x, std::string s)
  {
    std::cerr << importance << ",";
    switch(x)
    {
      DEBUG_CASE(DI_SOLVER);
      DEBUG_CASE(DI_SUMCON);
      DEBUG_CASE(DI_BOOLCON);
      DEBUG_CASE(DI_ARRAYAND);
      DEBUG_CASE(DI_QUEUE);
      DEBUG_CASE(DI_ANDCON);
      DEBUG_CASE(DI_REIFY);
      DEBUG_CASE(DI_LEXCON);
      DEBUG_CASE(DI_TABLECON);
	  DEBUG_CASE(DI_TEST);
	  DEBUG_CASE(DI_DYSUMCON);
	  DEBUG_CASE(DI_DYNAMICTRIG);
	  DEBUG_CASE(DI_DYELEMENT);
	  DEBUG_CASE(DI_INTCONTAINER);
	  DEBUG_CASE(DI_GACELEMENT);
	  DEBUG_CASE(DI_CHECKCON);
	  DEBUG_CASE(DI_VECNEQ);
      default:
	D_FATAL_ERROR("Missing debug case. Go add it to line 125(ish) in debug.h");
    };
    
    std::cerr << ": " << s << std::endl;
  }
#endif
  
  
#else
#define D_INFO(x,y,z)
#define D_DATA(x)
#define D_ASSERT(x)
#define FAIL_EXIT() { exit(1); }
#endif

#endif //DEBUG_H