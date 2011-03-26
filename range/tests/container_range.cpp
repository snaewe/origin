// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>
#include <vector>
#include <type_traits>

#include <origin/utility/typestr.hpp>
#include <origin/range/container_range.hpp>

using namespace std;
using namespace origin;

int main()
{
  typedef vector<int> Vector;
  typedef vector<int> const ConstVector;
  Vector v = {1, 2, 3};
  ConstVector cv = {1, 2, 3};

  // Const-qualified containers will generate constant iterators, which
  // have const-qualified value types. The result should be the same as
  // calling begin(c) where c is a possibly const-qualified container.
  typedef container_range<Vector> Range;
  typedef container_range<ConstVector> ConstRange;

  assert(( is_same<Vector::iterator, Range::iterator>::value ));
  assert(( is_same<ConstVector::const_iterator, ConstRange::iterator>::value ));
}
