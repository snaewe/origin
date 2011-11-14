// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/ordinal_map.hpp>

using namespace std;
using namespace origin;

struct num {
  num() = default;

  num(size_t n)
    : n(n)
  { }
  
  friend bool operator==(num x, num y)
  {
    return x.n == y.n;
  }

  friend bool operator<(num x, num y)
  {
    return x.n < y.n;
  }

  size_t n;
};

size_t ord(num n)
{
  return n.n;
}

int main()
{
  ordinal_map<num, char> v;
  assert(( v.empty() ));
  assert(( v.size() == 0 ));

  num zero = 0;
  v.insert({zero, 'a'});
  assert(( !v.empty() ));
  assert(( v.size() == 1));
  assert(( v[zero] == 'a' ));
  assert(( v.find(zero) != v.end() ));
  assert(( v.count(zero) == 1 ));
  
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
