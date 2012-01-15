// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <vector>
#include <iostream>

#include <origin/iterator/transform_iterator.hpp>

using namespace std;
using namespace origin;

struct twice
{
  int operator()(int x) const { return 2 * x; }
};

int main()
{
  vector<int> v = {1, 2, 3};
  auto i = xform(v.begin(), twice{});
  auto j = xform(v.end(), twice{});
  
  while(i != j) {
    cout << *i << ' ';
    ++i;
  }
  cout << '\n';
}
