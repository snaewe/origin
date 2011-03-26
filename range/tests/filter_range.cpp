// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <vector>
#include <iostream>

#include <origin/range/iterator_range.hpp>
#include <origin/range/filter_range.hpp>

using namespace std;
using namespace origin;

int main()
{
  auto nonzero = [](int x) { return x != 0; };

  vector<int> v = {1, 0, 2, 0, 3, 0, 4, 0, 5};
  for(int x : v) {
    cout << x << " ";
  }
  cout << "\n";

  {
    for(int x : filter(v, [](int x) { return x != 0; })) {
      cout << x << " ";
    }
    cout << "\n";
  }

  {
    for(int x : filter(v, [](int x) { return x == 0; })) {
      cout << x << " ";
    }
    cout << "\n";
  }

  {
    vector<int> const& cv = v;
    for(int x : filter(cv, nonzero)) {
      cout << x << ' ';
    }
    cout << '\n';
  }

}

