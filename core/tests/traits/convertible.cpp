// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/traits.hpp>

using namespace std;
using namespace origin;

int main()
{
  static_assert(Convertible<char, int>(), "");
  static_assert(Convertible<int, double>(), "");
  
  // Check qualifiers
  static_assert(Convertible<int const, int>(), "");
  static_assert(Convertible<int, int const>(), "");
  
  static_assert(Convertible<int volatile, int>(), "");
  static_assert(Convertible<int, int volatile>(), "");

  // NOTE: You can't convert an int to an int& because that would require
  // binding an lvalue reference to an rvalue -- doesn't work. You can bind
  // a const lvalue reference to an rvalue, though.
  static_assert(Convertible<int&, int>(), "");
  static_assert(!Convertible<int, int&>(), "");
  static_assert(Convertible<int const&, int>(), "");
  static_assert(Convertible<int, int const&>(), "");
  
  
  
  // FIXME: Write tests on custom conversion types (strings, etc.) and
  // operators.
}
