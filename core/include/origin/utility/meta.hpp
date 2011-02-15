// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_UTILITY_META_HPP
#define ORIGIN_UTILITY_META_HPP

#include <type_traits>

namespace origin
{
  /**
   * @defgroup meta Metaprogramming Support
   * The metaprogramming module contains a number of metafunctions, constants,
   * and type traits that complement the suite of type traits and
   * metaprogramming utilities provided in the C++ standard.
   */

  /**
   * @ingroup meta
   * The bool constant is an alias for the type integral_constant<bool, X>.
   * This type is provided purely for convenience.
   */
  template<bool B>
  struct bool_constant
    : std::integral_constant<bool, B>
  { };

  /**
   * @ingroup meta
   * This trait returns true if the types T and U are not the same. This
   * is the logical inverse of the is_same type trait.
   */
  template<typename T, typename U>
  struct is_different
    : bool_constant<!std::is_same<T, U>::value>
  { };

  /**
   * @defgroup sfinae
   * @ingroup meta
   * Types and traits for handling SFINAE-related queries.
   */

  /**
   * @internal
   * @ingroup sfinae
   * The substitution failure type represents the result of failed name lookup
   * and is used to support queries about the existence or state of expressions
   * that might fail.
   */
  struct substitution_failure { };

  /**
   * @internal
   * @ingroup sfinae
   * The substitution failed trait returns true if the the given type indicates
   * a failed substitution.
   */
  template<typename T>
  struct substitution_failed
    : std::false_type
  { };

  template<>
  struct substitution_failed<substitution_failure>
    : std::true_type
  { };

  /**
   * @internal
   * @ingroup sfinae
   * The substitution succeeded trait is true if the given type does not
   * indicate a failed substitution.
   */
  template<typename T>
  struct substitution_succeeded : std::true_type
  { };

  template<>
  struct substitution_succeeded<substitution_failure>
    : std::false_type
  { };



} // namespace origin

#endif

