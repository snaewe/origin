// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_TRAITS_TYPE_HPP
#define ORIGIN_TRAITS_TYPE_HPP

#include <type_traits>

namespace origin
{
  /**
   * @internal
   * @ingroup traits
   *
   * Return the common type of the sequence of arguments. This is defined in
   * terms of a conditionally enabled conditional operator. The implementation
   * is fragile.
   */
  //@{
  template<typename... Args> struct get_conditional_result;

  template<typename T>
  struct get_conditional_result<T>
  {
    typedef T type;
  };

  template<typename T, typename U>
  struct get_conditional_result<T, U>
  {
  private:
    template<typename X, typename Y>
    static auto test(X x, Y y) -> decltype(true ? x : y);
    
    static substitution_failure test(...);
  public:
    typedef decltype(test(std::declval<T>(), std::declval<U>())) type;
  };

  template<typename T, typename... Args>
  struct get_conditional_result<T, Args...>
  {
  private:
    typedef typename front_type<Args...>::type Head;
    typedef typename get_conditional_result<T, Head>::type X;
    typedef typename get_conditional_result<Args...>::type Y;
  public:
    typedef typename get_conditional_result<X, Y>::type type;
  };
  //@}

  /**
   * @ingroup traits
   *
   * Deduce the esult of the use of the conditional operator. The resulting 
   * type is not a reference type.
   */
  template<typename... Args>
  struct deduce_conditional_result
  {
    typedef typename std::remove_reference<
      typename get_conditional_result<Args...>::type
    >::type type;
  };

  /**
   * @ingroup traits
   *
   * Return true if a result can be deduced from the conditional operator.
   */
  template<typename... Args>
  struct has_conditional_result
    : substitution_succeeded<
        typename deduce_conditional_result<Args...>::type
      >::type
  { };

  /**
   * @ingroup traits
   *
   * Return true if the given arguments share a common type. 
   *
   * This is equivalent to has_conditional_result.
   */
  template<typename... Args>
  struct has_common_type
    : has_conditional_result<Args...>::type
  { };

} // namespace origin

#endif
