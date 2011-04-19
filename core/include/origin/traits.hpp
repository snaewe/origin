// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_TRAITS_HPP
#define ORIGIN_TRAITS_HPP

#include <origin/traits/type.hpp>
#include <origin/traits/constructors.hpp>
#include <origin/traits/operators.hpp>

namespace origin
{
  /**
   * @ingroup traits
   *
   * Deduce the result of the expression f(args...) where f has type T and
   * args is a parameter pack.
   */
  //@{
  // FIXME: GCC is preventing me from writing this in a variadic way. I
  // suspcect that the reason can be traced to a combination of variadic
  // templates + forward + declval. A weird combo.
  template<typename T, typename... Args> struct get_call_result;

  template<typename T>
  struct get_call_result<T>
  {
  private:
    template<typename X>
    static auto check(X&& x) -> decltype(x());

    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>())) type;
  };

  template<typename T, typename Arg>
  struct get_call_result<T, Arg>
  {
  private:
    template<typename X, typename A>
    static auto check(X&& x, A&& a) -> decltype(x(std::forward<A>(a)));

    static substitution_failure check(...);
  public:
    typedef decltype(
      check(std::declval<T>(), std::forward<Arg>(std::declval<Arg>()))
    ) type;
  };


  template<typename T, typename Arg1, typename Arg2>
  struct get_call_result<T, Arg1, Arg2>
  {
  private:
    // FIXME: I should be forwarding the argument type, but GCC is crashing
    // with an ICE if I try. I don't think that this will cause too many
    // projects.
    template<typename F, typename A1, typename A2>
    static auto check(F&& f, A1&& a1, A2&& a2) -> decltype(f(a1, a2));

    static substitution_failure check(...);
  public:
    typedef decltype(
      check(std::declval<T>(),
            std::forward<Arg1>(std::declval<Arg1>()),
            std::forward<Arg2>(std::declval<Arg2>()))
      ) type;
  };
  //@}

  /**
   * @ingroup traits
   *
   * Determine the validity of the expression f(args...) where f has type F
   * and args is a parameter pack.
   */
  template<typename F, typename... Args>
  struct is_callable
    : substitution_succeeded<typename get_call_result<F, Args...>::type>
  { };


  /**
   * @ingroup traits
   *
   * Deduce the result of the expression x[y] where x has type T and y has type
   * U. If the expression is invalid, return substitution_failure.
   */
  template<typename T, typename U>
  struct get_subscript_result
  {
  private:
    template<typename X, typename Y>
    static auto check(X x, Y y) -> decltype(x[y]);

    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * @ingroup traits
   *
   * Determine the validity of the expression x[y] where x has type T and y
   * has type U. Return true if valid, false otherwise.
   */
  template<typename T, typename U>
  struct has_subscript
    : substitution_succeeded<typename get_subscript_result<T, U>::type>
  { };

} // namespace origin

#endif
