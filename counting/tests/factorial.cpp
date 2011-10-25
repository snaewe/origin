// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/counting.hpp>

using namespace origin;

int main()
{
  assert(( factorial(0) == 1 ));
  assert(( factorial(1) == 1 ));
  assert(( factorial(2) == 2 ));
  assert(( factorial(3) == 6 ));
  assert(( factorial(4) == 24 ));
  assert(( factorial(10) == 3628800 ));
}
