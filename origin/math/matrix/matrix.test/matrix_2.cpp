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

void test_init()
{
  matrix<int, 2> m1 {
    {1, 2, 3, 4},
    {5, 6, 7, 8}
  };
  assert(m1.extent(0) == 2);
  assert(m1.extent(1) == 4);
  assert(m1.size() == 8);
  cout << m1 << '\n';

  // Using parens calls the size initializer
  matrix<int, 2> m2(5, 3);
  assert(m2.extent(0) == 5);
  assert(m2.extent(1) == 3);
  cout << m2 << '\n';

  // Make sure that we can be completely ambiguous and still get the
  // right thing.
  matrix<std::size_t, 2> m3(3, 2);
  assert(m3.extent(0) == 3);
  assert(m3.extent(1) == 2);
  cout << m3 << '\n';

  // Same as above. Parents are always value initialization.
  matrix<std::size_t, 2> m4(3ul, 2);
  assert(m4.extent(0) == 3);
  assert(m4.extent(1) == 2);
  cout << m4 << '\n';

  // You cannot value initialize from a flat list. There is no way to deduce
  // the intended extents.
  // matrix<int, 2> m5 {1, 2, 3, 4, 5, 6};
}

void test_access()
{
  matrix<int, 2> m {
    {0, 1, 2, 3},
    {4, 5, 6, 7}
  };

  // Check indexing.
  {
    int n = 0;
    for (size_t i = 0; i < m.extent(0); ++i) {
      for (size_t j = 0; j < m.extent(1); ++j) {
        assert(m(i, j) == n);
        ++n;
      }
    }
  }

  for (size_t i = 0; i < m.extent(0); ++i)
    cout << m.row(i) << '\n';
  for (size_t i = 0; i < m.extent(1); ++i)
    cout << m.col(i) << '\n';
}

void test_ops()
{
  matrix<int, 2> m {
    {0, 1, 2, 3},
    {4, 5, 6, 7}
  };

  // Test ops on elements.
  m(0, 0) = 1;
  assert(m(0, 0) == 1);
  m(0, 0) += 1;
  assert(m(0, 0) == 2);
  m(0, 0) -= 1;
  assert(m(0, 0) == 1);
  m(0, 0) *= 10;
  assert(m(0, 0) == 10);
  m(0, 0) /= 2;
  assert(m(0, 0) == 5);
  m(0, 0) %= 5;
  assert(m(0, 0) == 0);

  // Test broadcast operations
  auto m1 = m;
  m1 += 1; // {1, 2, 3, 4}, {5, 6, 7, 8}
  m1 -= 1;
  assert(m1 == m);
  m1 *= 5;
  m1 /= 5;
  assert(m1 == m);

  // Test matrix addition/subtraction.
  matrix<int, 2> m2 {
    {7, 6, 5, 4},
    {3, 2, 1, 0}
  };
  m += m2; // {{7, 7, 7, 7}, {7, 7, 7, 7}}
  cout << m << '\n';
  m -= m2; // Same as initial m.
  cout << m << '\n';

  // Operations on slices
  auto m3 = m;
  m3[0] += 10;
  m3[0] -= 10;
  assert(m3 == m);
  m3[1] *= 3;
  m3[1] /= 3;
  assert(m3 == m);



  // Operations on slices
  m3.col(1) += 10;
  m3.col(1) -= 10;
  assert(m3 == m);
  m3.col(2) *= 3;
  m3.col(2) /= 3;
  assert(m3 == m);
}


int main()
{
  test_init();
  test_access();
  test_ops();
}
