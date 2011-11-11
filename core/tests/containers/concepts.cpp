// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <memory>
#include <string>
#include <vector>
#include <iostream>

#include <origin/container.hpp>

using namespace std;
using namespace origin;

int main()
{
  using Vec = vector<double>;
  static_assert(Same<Value_type<Vec>, double>(), "");
  static_assert(Same<Iterator_type<Vec>, vector<double>::iterator>(), "");
  static_assert(Same<Iterator_type<Vec const>, vector<double>::const_iterator>(), "");
  static_assert(Same<Size_type<Vec>, size_t>(), "");
  static_assert(Same<Distance_type<Vec>, ptrdiff_t>(), "");
  static_assert(Range<Vec>(), "");
  static_assert(Range<Vec const>(), "");
  static_assert(Container<Vec>(), "");
  
  using Arr = double[5];
  static_assert(Same<Value_type<Arr>, double>(), "");
  static_assert(Same<Iterator_type<Arr>, double*>(), "");
  static_assert(Same<Iterator_type<Arr const>, double const*>(), "");
  static_assert(Same<Size_type<Arr>, size_t>(), "");
  static_assert(Same<Distance_type<Arr>, ptrdiff_t>(), "");
  static_assert(Range<Arr>(), "");
  static_assert(Range<Arr const>(), "");
//   static_assert(Container<Vec>(), "");
}

