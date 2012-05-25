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

using namespace std;
using namespace origin;

int main()
{
  vector<int> v = {1, 0, 2, 0, 3, 0, 4};
  vector<int> out(3);
  auto p = extract(v.begin(), v.end(), out.begin(), 0);
  
  int nz[] = {1, 2, 3, 4};
  int z[] = {0, 0, 0};
  
  assert(( lexicographical_equal(v.begin(), p.first, nz, nz + 4) ));
  assert(( lexicographical_equal(out.begin(), p.second, z, z + 3) ));
}
