// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>

#include <origin/math/matrix.hpp>

using namespace std;
using namespace origin;


int main()
{
  typedef matrix<int, 1> Matrix;
  
  Matrix m1;
  assert(( m1.size() == 0 ));
  
  Matrix m2(3);
  
  Matrix m3(3, 1);
  
  m3 += 2;
  assert(( m3 == Matrix(3, 3) ));
  
  Matrix m4 = m3 + 3;
  assert(( m4 == Matrix(3, 6) ));

  Matrix m5 = m3 * 3;
  assert(( m5 == Matrix{9, 9, 9} ));
  
  Matrix m6 = {1, 2, 3, 4};
  assert(( m6 % 2 == Matrix{1, 0, 1, 0} ));
}
