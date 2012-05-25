// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/vector.hpp>
#include <origin/algorithm.hpp>

using namespace origin;

bool neg(int n) { return n < 0; }

int main()
{
  vector<int> v1 = {1, -1, 2, -2, 3, -3};
  vector<int> v2 = {-1, -2, -3, 1, 2, 3};

  partition(v1, neg);
  for(auto x : v1) 
    std::cout << x << ' '; 
  std::cout << '\n';
  assert(is_partitioned(v1, neg));
  assert(is_permutation(v1, v2));
}
