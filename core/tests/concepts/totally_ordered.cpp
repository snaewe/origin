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

struct failure { };

int main()
{
  static_assert(Totally_ordered<int>(), "");
  static_assert(!Totally_ordered<failure>(), "");

  static_assert(Totally_ordered<char, int>(), "");
  static_assert(Totally_ordered<string, const char*>(), "");
  static_assert(!Totally_ordered<string, int>(), "");
}

