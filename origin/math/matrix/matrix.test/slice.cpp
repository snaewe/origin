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
  matrix_base<int, 2> b({3, 2}, {2, 1});

  for(int i = 0; i < 3; ++i) {
    for (int j = 0; j < 2; ++j) {
      cout << b(i, j) << '\n';
    }
  }
}
