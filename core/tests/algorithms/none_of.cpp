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

struct negative
{
  bool operator()(int n) const { return n < 0; }
};

int main()
{
  vector<int> v1 = {1, 2, 3};
  assert(none_of(v1.begin(), v1.end(), negative{}));
  assert(none_of(v1, negative{}));
  assert(none_of({1, 2, 3}, negative{}));

  vector<bool> v2 = {false, false, false};
  assert(none_of(v2.begin(), v2.end()));
  assert(none_of(v2));
  assert(none_of({false, false, false}));
}
