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
  assert(( rising_factorial(5, 0) == 1 ));
  assert(( rising_factorial(5, 1) == 5 ));
  assert(( rising_factorial(5, 2) == 30 ));
  assert(( rising_factorial(5, 3) == 210 ));
  assert(( rising_factorial(5, 4) == 1680 ));
  assert(( rising_factorial(5, 5) == 15120 ));
}
