// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>

#include <origin/type/typestr.hpp>
#include <origin/math/matrix/matrix.hpp>

using namespace std;
using namespace origin;

template <size_t N>
  ostream& operator<<(ostream& os, const matrix_descriptor<N>& d)
  {
    os << d.size << '\n';
    for (auto n : d.extents)
      os << n << ' ';
    os << '\n';
    for (auto n : d.strides)
      os << n << ' ';
    os << '\n';
    return os;
  }


int main()
{
  matrix_descriptor<3> d1;
  // cout << d1 << "-----\n";

  matrix_descriptor<3> d2 (d1);
  // cout << d2 << "-----\n";

  matrix_descriptor<3> d3 = std::move(d2);
  // cout << d3 << "-----\n";

  matrix_descriptor<3> d4({3, 4, 2});

  for (size_t i = 0; i != d4.extents[0]; ++i) {
    for (size_t j = 0; j != d4.extents[1]; ++j)
      for (size_t k = 0; k != d4.extents[2]; ++k)
        cout << d4(i, j, k) << '\n';
  }
  cout << '\n';
}
