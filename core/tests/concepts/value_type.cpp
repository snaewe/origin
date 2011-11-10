// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>
#include <vector>
#include <string>

#include <origin/concepts.hpp>

using namespace std;
using namespace origin;

struct failure { };

int main()
{
  typedef vector<int> V;
  static_assert(Has_value_type<V>(), "");
  static_assert(Same<Value_type<V>, int>(), "");
  
  static_assert(!Has_value_type<failure>(), "");

  
  // Check adaptation for pointers.
  static_assert(Has_value_type<int*>(), "");
  static_assert(Same<Value_type<int*>, int>(), "");
  static_assert(Has_value_type<int const*>(), "");
  static_assert(Same<Value_type<int const*>, int>(), "");
}

