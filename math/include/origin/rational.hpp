// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RATIONAL_HPP
#define ORIGIN_RATIONAL_HPP

#include <origin/integer.hpp>
#include <origin/fraction.hpp>

namespace origin
{
  // Rational
  // A rational number number is a fraction of integers.
  using rational = fraction<integer>;

} // namespace origin

#endif
