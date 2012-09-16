// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#define private public

#include <iostream>

#include <origin/math/matrix/matrix.hpp>

using namespace std;
using namespace origin;

// Tests for 2D use of matrices:

int main()
{
  using T = matrix<int, 2>;
  matrix<T, 2> m {
    { // row 0
      {{1, 2}, {3, 4}}, // col 0
      {{4, 5}, {6, 7}}, // col 1
    },
    { // row 1
      {{8, 9},     {0xa, 0xb}}, // col 0
      {{0xc, 0xd}, {0xe, 0xf}}, // col 1
    },
    { // row 3
      {{1, 2}, {3, 4}}, // col 0
      {{4, 5}, {6, 7}}, // col 1
    }
  };
  assert(m.extent(0) == 3);
  assert(m.extent(1) == 2);

  cout << m(0, 0) << '\n';
  cout << m(0, 1) << '\n';
  cout << m(1, 0) << '\n';
  cout << m(1, 1) << '\n';
  cout << m(2, 0) << '\n';
  cout << m(2, 1) << '\n';
}
