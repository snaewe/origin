// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>
#include <vector>
#include <list>

#include <origin/algorithm.hpp>

using namespace std;
using namespace origin;

struct my_pair
{
  int m;
  char n;
  
  bool operator==(const my_pair& x) { return m == x.m && n == x.n; }
  bool operator!=(const my_pair& x) { return !(*this == x); }
};

int main()
{
  vector<int> v = {1, 2, 3, 4};
  list<int> l = {1, 2, 3, 4};
  vector<int> x = {1, 2, 3};
  int a[] = {1, 2, 3, 4, 5};
  
  assert(( equal(v, l) ));
  assert(( !equal(v, x) ));

  // These should use the optimized version.
  // Is there some way to test this?
  assert(o_equal(a, a + 5, a)); 
  assert(o_equal(v.begin(), v.end(), v.begin()));
  
  // These should use the optimized version also.
  assert(equal(a, a));
  assert(equal(v, v));
  
  // This should not use the optimized version.
  vector<my_pair> p = { {0, 'a'}, {1, 'b'} };
  assert(o_equal(&p[0], &p[2], &p[0]));
  
}
