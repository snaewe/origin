// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>

#include <origin/math/matrix/matrix.hpp>

using namespace std;
using namespace origin;

// Tests for 2D use of matrices:

int main()
{
  matrix<int, 2> m1 {
    {1, 2, 3, 4},
    {5, 6, 7, 8}
  };
  assert(m1.extent(0) == 2);
  assert(m1.extent(1) == 4);
  assert(m1.size() == 8);
  cout << m1 << '\n';

#ifndef ORIGIN_MATRIX_USE_SHAPE_CTOR
  // Using parens calls the size initializer
  matrix<int, 2> m2(5, 3);
  assert(m2.extent(0) == 5);
  assert(m2.extent(1) == 3);

  // Make sure that we can be completely ambiguous and still get the
  // right thing.
  matrix<std::size_t, 2> m3(3, 2);
  assert(m3.extent(0) == 3);
  assert(m3.extent(1) == 2);

  matrix<std::size_t, 2> m4(3ul, 2);
  assert(m4.extent(0) == 3);
  assert(m4.extent(1) == 2);
#endif

}
