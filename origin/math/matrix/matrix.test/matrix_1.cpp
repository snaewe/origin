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

// Tests for 1D use of matrices:

int main()
{
  matrix<int, 1> m1 { 1, 2, 3, 4 };
  assert(m1.extent(0) == 4);
  assert(m1.size() == 4);
  cout << m1 << '\n';
}
