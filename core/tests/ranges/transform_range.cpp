// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <vector>
#include <iostream>

#include <origin/algorithm.hpp>
#include <origin/range/transform_range.hpp>

using namespace std;
using namespace origin;

struct twice
{
  int operator()(int n) const { return 2 * n; }
};

int main()
{
  vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8};
  vector<int> a(v.size());
  vector<int> b(v.size());

  transform(v.begin(), v.end(), a.begin(), twice{});
//   copy(transformed(v, twice{}), b);
  
  auto r = transformed(v, twice{});
  using R = decltype(r);
  cout << Input_range<R>() << "\n";
  
  for(auto x : transformed(v, twice{}))
    cout << x << ' ';
  cout << '\n';
}
