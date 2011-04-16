// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONCEPTS_IMPL_HPP
#define ORIGIN_CONCEPTS_IMPL_HPP

#include <tuple>

#include <origin/utility/meta.hpp>
#include <origin/traits.hpp>
#include <origin/concepts/fwd.hpp>

namespace origin
{
  /**
   * @defgroup concepts
   */

  /**
   * @internal
   *
   * The concept check metafunction evaluates to true if all of the given
   * predicates are true. If not, the metafunction evaluates to false.
   */
  template<typename... Args> struct requires_all;

  // Empty argument sequences are trivially true.
  template<> struct requires_all<> : std::true_type { };

  template<typename Model>
  struct requires_all<Model>
    : bool_constant<Model::value>
  { };

  template<typename Model, typename... Args>
  struct requires_all<Model, Args...>
    : std::conditional<
        Model::value, typename requires_all<Args...>::type, std::false_type
      >::type
  { };

  // A specialization for tuples, just to help out.
  template<typename... Args>
  struct requires_all<std::tuple<Args...>>
    : requires_all<Args...>::type
  { };

} // namespace origin

#endif
