// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_TRAITS_CONSTRUCTORS_HPP
#define ORIGIN_TRAITS_CONSTRUCTORS_HPP

#include <utility>
#include <origin/utility/meta.hpp>

namespace origin
{
  /**
   * @ingroup traits
   * @name Construction and Destruction
   *
   * @todo These will go away whenever the the standard library catches up,
   * and I will need to replace all of their uses in the regularity concepts.
   * Currently, evalating the std versions of these functions results in a
   * compiler error.
   */
  //@{
  template<typename T, typename... Args>
  struct deduce_constructible;

  // Check for default constructors
  template<typename T>
  struct deduce_constructible<T>
  {
  private:
    template<typename X>
    static auto check(X&&) -> decltype(new X());
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>())) type;
  };

  // Check for unary constructors
  template<typename T, typename Arg>
  struct deduce_constructible<T, Arg>
  {
  private:
    template<typename X, typename Y>
    static auto check(X&&, Y y) -> decltype(new X(y));
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<Arg>())) type;
  };


  template<typename T, typename... Args>
  struct is_constructible;

  // Check for default constructors
  template<typename T>
  struct is_constructible<T>
    : substitution_succeeded<typename deduce_constructible<T>::type>
  { };

  // Check for nullary constructors
  template<typename T, typename Arg>
  struct is_constructible<T, Arg>
    : substitution_succeeded<typename deduce_constructible<T, Arg>::type>
  { };
  //@}

} // namespace origin

#endif
