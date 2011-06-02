// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_FUNCTIONAL_HPP
#define ORIGIN_FUNCTIONAL_HPP

#include <origin/concepts.hpp>

namespace origin
{
  /**
   * @ingroup logic
   *
   * The truth function object is a constant Function that returns a value
   * that represents the notion of "true" for the given type.
   *
   * @tparam T  A Boolean type
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
   * @tparam T  A Boolean type
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

  // FIXME: Optimize with EBO?
  /**
   * The incomparable_to operation is defined in terms of a strict ordering
   * comparison on T, less by default. The operation returns true if, for
   * objects x and y, it is neither the case that x < y, nor y < x. 
   * 
   * Note that if the  template parameter T is a total order, then this is 
   * equivalent to the equal_to function.
   *
   * @tparam T      A type
   * @tparam Comp   A Strict Partial Order on T
   */
  template<typename T, typename Comp>
  class incomparable_to
    : cRelation<Comp, T, T>
  {
    // aStrict_Partial_Order<Comp, T>
  public:
    incomparable(Comp c)
      : comp{c}
    { }
    
    bool operator()(T const& x, T const& y) const
    {
      return !comp(x, y) && !comp(y, x);
    }
    
  private:
    Comp comp;
  };
    
} // namespace origin

#endif
