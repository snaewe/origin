// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RELATION_HPP
#define ORIGIN_RELATION_HPP

#include <origin/concepts/traits.hpp>

namespace origin
{
  // FIXME: Should this go somewhere else? Should this be in an Order module
  // as opposed to general relations.

  /**
   * The indifferent_to function object defines an "indifference" operation
   * built in terms of an Ordered type T. Two objects x and y are indifferent
   * (or incomparable) if neither x < y nor y < x.
   *
   * Note that for totally ordered types, indifference is the same as
   * equivalence.
   *
   * @note The requirement (Less) for this type is purely syntactic. We do not
   * need to specify order semantics for this function template. That is
   * typically the purpose of some other component.
   */
  template<typename T>
  struct indifferent_to
    : tLess<T>
  {
    bool operator()(T const& x, T const& y) const
    {
      return !(x < y) && !(y < x);
    }
  };

} // namespace origin

#endif
