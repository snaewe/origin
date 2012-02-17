// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <vector>

#include <origin/algorithm.hpp>

using namespace std;
using namespace origin;

struct equivalent_div_3
{
  bool operator()(int a, int b) const { return a / 3 == b / 3; }
};

int main()
{
  vector<int> v1 = {0, 0, 0};
  assert(all_equal(v1.begin(), v1.end(), 0));
  assert(all_equal(v1, 0));
  assert(all_equal({0, 0, 0}, 0));

  vector<int> v2 = {0, 1, 2};
//   assert(all_equal(v2.begin(), v2.end(), equivalent_div_3{}));
//   assert(all_equal(v2, equivalent_div_3{}));
//   assert(all_equal({0, 1, 2}, equivalent_div_3{}));
}
