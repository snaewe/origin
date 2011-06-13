// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/vector_map.hpp>

using namespace std;
using namespace origin;


int main()
{
  vector_map<char> v;
  assert(( v.empty() ));
  assert(( v.size() == 0 ));

  v.insert({0, 'a'});
  assert(( !v.empty() ));
  assert(( v.size() == 1));
  assert(( v[0] == 'a' ));
  assert(( v.find(0) != v.end() ));
  assert(( v.count(0) == 1 ));
  
  v.insert({1, 'b'});
  assert(( v.size() == 2 ));
  assert(( v[1] == 'b' ));
  assert(( v.find(1) != v.end() ));
  assert(( v.count(1) == 1 ));
  
  v[0] = 'z';
  assert(( v[0] == 'z' ));
  
  v[2] = 'c';
  assert(( v.size() == 3 ));
  assert(( v[2] == 'c' ));
  assert(( v.count(2) == 1 ));
}
