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
  template<typename T> struct Boolean;

  /**
   * @defgroup logic
   * This module defines several generic truth functions. In general, these
   * algorithms are parameterized over a single Boolean type. More general
   * two-type versions are also provided to help satisfy the requirements of
   * expression template systems that define Boolean operators.
   *
   * The implies and iff functions are used extensively in axiomiatic testing.
   * They are defined in terms of Boolean operators rather than if statements
   * or equality in order to prevent the accrual of additional constraints
   * on the parameter types.
   */

  /**
   * @ingroup logic
   * The logical implication operator returns true if x is true and the y is
   * true. Note that ff x is false, then the result is also trivially true.
   */
  template<typename T>
  inline auto implies(T const& p, T const& q)
    -> decltype(!p || q)
  {
    Boolean<T>{};
    return !p || q;
  }

  // This version satisfies requirements for expression templates.
  // FIXME: Constrain this template using a trait.
  template<typename T, typename U>
  inline auto implies(T const& p, U const& q)
    -> decltype(!p || q)
  { return !p || q; }

  /**
   * @ingroup logic
   * The logical equality (spelled iff, or if and only iff) operator
   * returns true if x implies y and y implies x. Said otherwise, x is
   * logically equal to y if x equals y.
   */
  template<typename T>
  inline auto iff(T const& p, T const& q)
    -> decltype((p && q) || (!p && !q))
  {
    Boolean<T>{};
    return (p && q) || (!p && !q);
  }

  // This overload satsifies requirements for expression templates.
  // FIXME: Constrain this template using a trait.
  template<typename T, typename U>
  inline auto iff(T const& p, T const& q)
    -> decltype((p && q) || (!p && !q))
  { return (p && q) || (!p && !q); }

} // namespace origin

#endif
