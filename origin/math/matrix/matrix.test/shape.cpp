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

int main()
{
  using Shape_2 = matrix_shape<size_t, 2>;
  Shape_2 a {3, 4};
  Shape_2 b = a;
  assert(a == b);
}
