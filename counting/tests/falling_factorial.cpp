// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/counting.hpp>

using namespace std;
using namespace origin;

int main()
{
  assert(( falling_factorial(5, 0) == 1 ));
  assert(( falling_factorial(5, 1) == 5 ));
  assert(( falling_factorial(5, 2) == 20 ));
  assert(( falling_factorial(5, 3) == 60 ));
  assert(( falling_factorial(5, 4) == 120 ));
  assert(( falling_factorial(5, 5) == 120 ));
}
