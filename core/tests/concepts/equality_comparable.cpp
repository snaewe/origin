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

#include <origin/concepts.hpp>

using namespace std;
using namespace origin;

struct failure { int x; };

int main()
{
  static_assert(Equality_comparable<int>(), "");
  static_assert(!Equality_comparable<failure>(), "");

  static_assert(Equality_comparable<char, int>(), "");
  static_assert(Equality_comparable<string, const char*>(), "");
  static_assert(!Equality_comparable<string, int>(), "");
}

