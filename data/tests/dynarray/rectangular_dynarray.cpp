// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/dynarray/rectangular_dynarray.hpp>

using namespace std;
using namespace origin;

int main()
{
  {
    rectangular_dynarray<int> x;
    assert(( x.empty() ));
  }

  {
    rectangular_dynarray<int> m{2, 3};
    assert(( m.rows() == 2 ));
    assert(( m.cols() == 3 ));
  }

  // Check 2d initializer list init
  {
    rectangular_dynarray<int> m = {
      {1, 2},
      {3, 4},
      {5, 6}
    };
    assert(( m.rows() == 3 ));
    assert(( m.cols() == 2 ));
    assert(( m(0, 0) == 1 ));
    assert(( m(0, 1) == 2 ));
    assert(( m(1, 0) == 3 ));
    assert(( m(1, 1) == 4 ));
    assert(( m(2, 0) == 5 ));
    assert(( m(2, 1) == 6 ));
  }
}
