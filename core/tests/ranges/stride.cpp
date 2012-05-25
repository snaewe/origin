// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/vector.hpp>
#include <origin/range/stride.hpp>

using namespace origin;

template <typename R>
  void print(const R& range)
  {
    for (auto x : range)
      std::cout << x << ' ';
    std::cout << '\n';
  }

// Return true if n != 0.
bool non_zero(int n) { return n != 0; }

int main()
{
  vector<int> v = {1, 0, 0, 3, 0, 0, 5, 0, 0};
  print(stride(v, 3));
  print(stride(v, 2));
  print(stride(v, integral_constant<int, 3>{}));
}
