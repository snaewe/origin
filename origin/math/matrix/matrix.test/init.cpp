// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/type/type.hpp>
#include <origin/math/matrix/matrix.hpp>

using namespace std;
using namespace origin;

// Test the initialization patterns of matrix and matrix_ref.

int main()
{
  using Matrix = matrix<int, 2>;
  using Matrix_ref = matrix_ref<int, 2>;
  using Const_matrix_ref = matrix_ref<const int, 2>;

  Matrix m {{0, 1}, {2, 3}};

  auto a = m + m;
  static_assert(Same<Matrix, decltype(a)>(), "");

  const Matrix& cm = m;
  Const_matrix_ref cr1 = cm;
  Const_matrix_ref cr2 = m;
  assert(cr1 == cr2);
  assert(cr1.data() == cr2.data());

  Matrix_ref r1 = m;
  assert (r1 == cr1);
  assert(r1.data() == cr1.data());

  // This should fail -- deleted function.
  // Matrix_ref m5 = m + m;
}

