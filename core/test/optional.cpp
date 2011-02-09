// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>

#include <origin/utility/typestr.hpp>
#include <origin/optional.hpp>

using namespace std;
using namespace origin;

int main()
{
  typedef optional<int> T;
  T x1;
  assert(( !x1 ));

  T x2 = 5;
  assert(( x2 ));
  assert(( *x2 == 5 ));

  T x3 = x1;
  assert(( !x3 ));
  x3 = x2;
  assert(( x2 == x3 ));
  x3 = nullptr;
  assert(( !x3 ));

  // Test relational operators.
  T x4 = 3;
  assert(( x2 == x2 ));
  assert(( x1 == x1 ));
  assert(( x2 != x4 ));
  assert(( x1 != x2 ));

  assert(( x4 < x2 ));
  assert(( x2 > x4 ));
  assert(( x2 <= x2 ));
  assert(( x2 >= x2 ));
  assert(( x1 < x4 ));
  assert(( !(x1 < x1) ));
  assert(( x1 <= x1 ));

  // Test relation operators for underlying value types.
  assert(( x3 == nullptr ));
  assert(( nullptr == x3 ));
  assert(( x2 != nullptr ));
  assert(( nullptr != x2 ));
  assert(( x2 == 5 ));
  assert(( 5 == x2 ));
  assert(( x2 != 3 ));
  assert(( 3 != x2 ));

  x2 < nullptr;
  nullptr < x2;
}
