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
  

  /**
   * @internal
   * 
   * This metafunction always returns false. It is useful when a static 
   * assertion must be made dependent.
   * 
   * @todo Move this into meta. This seems like it could be useful in other
   * places.
   */
  template<typename T>
    struct always_false
      : bool_constant<!std::is_same<T, T>::value>
    { };
    

  /**
   * @ingroup axioms
   * 
   * The logical implication operator returns true if x is true and the y is
   * true. Note that if x is false, then the result is also trivially true.
   */
  template<typename T, typename U>
    inline auto implies(T const& p, U const& q)
      -> decltype(!p || q)
    { 
      return !p || q; 
    }

  /**
   * @ingroup axioms
   * The logical equality (spelled iff, or if and only iff) operator returns 
   * true if x implies y and y implies x. Said otherwise, x is logically equal 
   * to y if x equals y.
   */
  template<typename T, typename U>
    inline auto iff(T const& p, T const& q)
      -> decltype((p && q) || (!p && !q))
    { 
      return (p && q) || (!p && !q); 
    }
    
    
  /**
   * @ingroup concepts
   * 
   * The cExplicit concept implements a concept that is always invalid for the 
   * given argument. In essence, it forces a programmer to create a 
   * specialization to satisfy any requirements that depend on this concept.
   */
  template<typename T>
    struct cExplicit
    {
      cExplicit()
      {
        static_assert(always_false<T>::value, "Explicit concept required");
      }
      
      typedef std::tuple<always_false<T>> requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };
    
  /**
   * @ingroup concepts
   * 
   * The cModel class is always a valid model. This is intended to be used as
   * a base class for specializations of Explicit requirements.
   */
  struct cModel
    : std::true_type
  {
    typedef std::tuple<> requirements;
  };

} // namespace origin

#endif
