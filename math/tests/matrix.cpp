// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/utility/typestr.hpp>
#include <origin/math/matrix.hpp>
#include <origin/math/square_matrix.hpp>

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
    matrix<int> m{5, 3, 3};
    assert(( m.rows() == 3 ));
    assert(( m.cols() == 3 ));

    auto r1 = m.row(0);
    cout << r1[0] << " " << r1[1] << " " << r1[2] << "\n";

    auto r2 = m.row(1);
    cout << r2[0] << " " << r2[1] << " " << r2[2] << "\n";

    auto r3 = m.row(2);
    cout << r3[0] << " " << r3[1] << " " << r3[2] << "\n";

//     for(size_t i = 0; i < m.rows(); ++i) {
//       for(size_t j = 0; j < m.cols(); ++j) {
//         cout << m(i, j) << " ";
//       }
//       cout << "\n";
//     }
  }

  /*
  {
    square_matrix<int> m{5, 3};
    assert(( m.rows() == 3 ));
    assert(( m.cols() == 3 ));
    for(size_t i = 0; i < m.rows(); ++i) {
      for(size_t j = 0; j < m.cols(); ++j) {
        cout << m(i, j) << " ";
      }
      cout << "\n";
    }
  }
  */

  return 0;
}
