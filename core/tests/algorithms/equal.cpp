// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
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

int main()
{
  vector<int> v = {1, 2, 3, 4};
  list<int> l = {1, 2, 3, 4};
  vector<int> x = {1, 2, 3};
  
  assert(( equal(v, l) ));
  assert(( !equal(v, x) ));
}
