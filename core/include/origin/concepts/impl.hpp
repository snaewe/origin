// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONCEPTS_IMPL_HPP
#define ORIGIN_CONCEPTS_IMPL_HPP

#include <origin/concepts/impl.hpp>

namespace origin
{
  /**
   * The concept check metafunction evaluates to true if all of the given
   * predicates are true. If not, the metafunction evaluates to false.
   *
   * The concept check cl
   */
  template<typename... Args> struct concept_check;

  template<typename Model>
  struct concept_check<Model>
    : bool_constant<Model::value>
  { };

  template<typename Model, typename... Args>
  struct concept_check<Model, Args...>
    : std::conditional<
        Model::value, typename concept_check<Args...>::type, std::false_type
      >::type
  { };

  // A specialization for tuples, just to help out.
  template<typename... Args>
  struct concept_check<std::tuple<Args...>>
    : concept_check<Args...>::type
  { };

} // namespace origin

#endif
