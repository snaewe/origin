// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/range.hpp>

using namespace origin;

template <typename R>
  void print(const R& range)
  {
    for (auto x : range)
      std::cout << x << ' ';
    std::cout << '\n';
  }


int main()
{
  print(range(5));
  print(range(1, 5));

  print(range(0, 3, 3)); // [0]
  print(range(0, 4, 3)); // [0, 3]
  print(range(0, 5, 3)); // [0, 4]
  print(range(0, 6, 3)); // [0, 5]

  print(range(5, 20, 5)); // [5, 10, 15)

  int a[] {1, 2, 3, 4, 5};
  print(a);
  print(range(a, a + 3));
  print(range(a + 2, a + 5));
  print(range(a, a + 5, 2));
  print(range(a + 1, a + 5, 2));
}

