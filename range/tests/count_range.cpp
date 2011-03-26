// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <vector>
#include <iostream>

#include <origin/utility/typestr.hpp>
#include <origin/range/count_range.hpp>

#include "range_test.hpp"

using namespace std;
using namespace origin;


int main()
{
  // Positive ranges
  assert(( check(range(3), {0, 1, 2}) ));
  assert(( check(range(0, 3), {0, 1, 2}) ));
  assert(( check(range<3>(0, 9), {0, 3, 6}) ));
  assert(( check(range<3>(1, 9), {1, 4, 7 }) ));
  assert(( check(range<3>(2, 9), {2, 5, 8 }) ));
  assert(( check(range<3>(3, 9), {3, 6 }) ));

  // Negative ranges
  assert(( check(range<-1>(3), {3, 2, 1}) ));
  assert(( check(range<-1>(3, 0), {3, 2, 1}) ));
  assert(( check(range<-3>(9, 0), {9, 6, 3}) ));
  assert(( check(range<-3>(8, 0), {8, 5, 2}) ));
  assert(( check(range<-3>(7, 0), {7, 4, 1}) ));
  assert(( check(range<-3>(6, 0), {6, 3}) ));

  // Empty ranges
  assert(( distance(range(1, 0)) == 0 ));
  assert(( distance(range<-1>(0, 1)) == 0 ));

  // Step ranges
  assert(( check(range(0, 3, 1), {0, 1, 2}) ));
  assert(( check(range(3, 0, -1), {3, 2, 1}) ));
}

