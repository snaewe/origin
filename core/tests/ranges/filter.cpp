// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <vector>
#include <iostream>

#include <origin/range/filter.hpp>

using namespace std;
using namespace origin;

// Return truen if n is not 0.
bool non_zero(int n) { return n != 0; }

template<typename R>
  void print(R const& r)
  {
    for(auto x : filter(r, non_zero))
      cout << x << " ";
    cout << "\n";
  }

int main()
{
  vector<int> v = {0, 1, 2, 0, 3, 4, 0, 5, 6, 0};
  int a[5] = {1, 0, 3, 0, 5};
  
  print(v);
  
  // Explicitly treat the array as a range.
  print(arr(a));  
}
