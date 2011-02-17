// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_TRAITS_HPP
#define ORIGIN_TRAITS_HPP

#include <origin/traits/constructors.hpp>
#include <origin/traits/operators.hpp>

namespace origin {

  /**
   * Deduce the result type of the expression f(arg...) where f is a callable
   * type and args... is a sequence of arguments or arbitrary type.
   */
  template<typename F, typename... Args>
  struct deduce_callable
  {
  private:
    template<typename X, typename... Y>
    static auto check(X x, Y&&... y) -> decltype(x(std::forward<Y>(y)...));
    static substitution_failure check(...);
  public:
    typedef decltype(
      check(std::declval<F>(), std::forward<Args>(std::declval<Args>())...)
    ) type;
  };

  /**
   * Return true if the expression f(args...) is valid where f is a callable
   * type and args... is a sequence of arguments or arbitrary type.
   */
  template<typename F, typename... Args>
  struct is_callable
    : substitution_succeeded<typename deduce_callable<F, Args...>::type>
  { };

} // namespace origin

#endif
