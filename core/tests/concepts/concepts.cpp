// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <string>
#include <complex>
#include <iostream>

#include <origin/utility/typestr.hpp>
#include <origin/concepts.hpp>

#include <boost/logic/tribool.hpp>

#include "check.hpp"

using namespace std;
using namespace origin;

int main()
{
  static_assert(tSame<int, int>::value, "");
  static_assert(!tSame<int, int const>::value, "");

  static_assert(tCommon<int, int, char>::value, "");
  static_assert(!tCommon<string, float>::value, "");

  static_assert(tInt<int>::value, "");
  static_assert(!tInt<float>::value, "");

  static_assert(tLogical_And<bool>::value, "");
  static_assert(tLogical_And<boost::tribool>::value, "");
  static_assert(!tLogical_And<string>::value, "");

  static_assert(cOrdered<int>::value, "");
  static_assert(cOrdered<float>::value, "");
  static_assert(cOrdered<string>::value, "");
  static_assert(!cOrdered<complex<float>>::value, "");  // No natural order
  
  static_assert(cMoveable<int>::value, "");
  static_assert(cMoveable<int*>::value, "");

  static_assert(cRegular<char>::value, "");
  static_assert(!cRegular<char const>::value, "");

  static_assert(cBoolean<bool>::value, "");
  static_assert(!cBoolean<string>::value, "");
  static_assert(!cBoolean<int*>::value, "");            // !Constructible<int*, bool>
  static_assert(!cBoolean<boost::tribool>::value, "");  // tribool is not Ordered
  
}

