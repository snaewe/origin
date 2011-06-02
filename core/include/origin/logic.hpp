// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_LOGIC_HPP
#define ORIGIN_LOGIC_HPP

#include <origin/concepts/fwd.hpp>

namespace origin
{
  /**
   * @ingroup logic
   *
   * The truth function object is a constant Function that returns a value
   * that represents the notion of "true" for the given type.
   *
   * @note T is generally required to be Boolean, but we relax the requirement
   * to simply Constructible<T, bool> to avoid recursive definitions.
   */
  template<typename T>
    struct truth
      : tConstructible<T, bool>
    {
      T operator()() const
      {
        return T{true};
      }
    };

  /**
   * @ingroup logic
   *
   * The falsity function object is a constant Function that returns a value
   * that represents the notion of "false" for the given type.
   *
   * @note T is generally required to be Boolean, but we relax the requirement
   * to simply Constructible<T, bool> to avoid recursive definitions.
   */
  template<typename T>
    struct falsity
      : tConstructible<T, bool>
    {
      T operator()() const
      {
        return T{false};
      }
    };

} // namespace origin

#endif
