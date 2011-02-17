// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_LOGIC_HPP
#define ORIGIN_LOGIC_HPP

#include <cassert>

namespace origin
{
  /**
   * @defgroup logic
   * This module defines several functions that return the results of logical
   * truth tables. The purpose of these functions is to provide a framework of
   * logical operators that can be used to express axioms in such a way that no
   * firm requirements are made about the convertability of expressions into a
   * boolean type.
   *
   * By default, these operators are defined in terms of boolean types. Any
   * type that is implicitly convertible to bool, will implicitly bind to these
   * operators. If not such conversion is available for the argument types,
   * then the type provider must give overloads.
   */

  /**
   * @ingroup logic
   * The logical implication operator returns true if x is true and the y is
   * true. Note that ff x is false, then the result is also trivially true.
   */
  inline bool implies(bool x, bool y)
  { return x ? y : true; }

  /**
   * @ingroup logic
   * The logical equality (spelled iff, or if and only iff) operator
   * returns true if x implies y and y implies x. Said otherwise, x is
   * logically equal to y if x equals y.
   */
  inline bool iff(bool x, bool y)
  { return x == y; }

} // namespace origin

#endif
