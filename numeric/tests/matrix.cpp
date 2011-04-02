// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/numeric/matrix.hpp>

using namespace std;
using namespace origin;

int main()
{
  {
    matrix<int> m{2, 3};
    assert(( m.rows() == 2 ));
    assert(( m.cols() == 3 ));
    for(auto x : m) {
      assert(( x == 0 ));
    }
  }

  {
    matrix<int> m{3, 2, 5};
    assert(( m.rows() == 3 ));
    assert(( m.cols() == 2 ));
    for(size_t i = 0; i < m.rows(); ++i) {
      for(size_t j = 0; j < m.cols(); ++j) {
        cout << m(i, j) << " ";
      }
      cout << "\n";
    }
  }

  {
    typedef matrix<int, square_dynarray<int>> Matrix;
    Matrix m{2};
    assert(( m.rows() == 2 ));
    assert(( m.cols() == 2 ));
    for(auto x : m) {
      assert(( x == 0 ));
    }
  }

  {
    matrix<int> m{4, 4, 5};
    for(size_t i = 0; i < m.rows(); ++i) {
      for(size_t j = 0; j < m.cols(); ++j) {
        cout << m(i, j) << " ";
      }
      cout << "\n";
    }
  }
  return 0;
}
