// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <vector>
#include <iostream>

#include <origin/utility/typestr.hpp>
#include <origin/range.hpp>

using namespace std;
using namespace origin;

int main()
{
  int a[10];
  assert(( begin(a) == a ));
  assert(( size(a) == 10 ));
  assert(( !empty(a) ));
  
  vector<int> v;
  assert(( begin(v) == v.begin() ));
  assert(( size(v) == 0 ));
  assert(( empty(v) ));
}
