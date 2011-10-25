// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>
#include <vector>

#include <origin/algorithm.hpp>
#include <origin/range.hpp>

using namespace std;
using namespace origin;

int main()
{
  vector<int> v = {1, 0, 2, 0, 3, 0};
  
  assert(( count_not_equal(v.begin(), v.end(), 0) == 3 ));
  assert(( count_not_equal(v, 0) == 3 ));
}
