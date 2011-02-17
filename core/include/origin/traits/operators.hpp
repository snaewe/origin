// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_TRAITS_OPERATORS_HPP
#define ORIGIN_TRAITS_OPERATORS_HPP

#include <origin/utility/meta.hpp>

namespace origin
{
  /**
   * @ingroup traits
   * @name Logical Operators
   */
  //@{
  /**
   * Deduce the result type of the expression x && y.
   */
  template<typename T, typename U>
  struct deduce_logical_and
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x && y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Return true if the expression x && y is valid for the types of x and  y.
   */
  template<typename T, typename U>
  struct has_logical_and
    : substitution_succeeded<typename deduce_logical_and<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x || y.
   */
  template<typename T, typename U>
  struct deduce_logical_or
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x || y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Return true if the expression x || y is valid for the types of x and  y.
   */
  template<typename T, typename U>
  struct has_logical_or
    : substitution_succeeded<typename deduce_logical_or<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression !x.
   */
  template<typename T>
  struct deduce_logical_not
  {
  private:
    template<typename X>
    static auto check(X const& x) -> decltype(!x);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>())) type;
  };

  /**
   * Return true if the expression !x is valid for the types of x.
   */
  template<typename T>
  struct has_logical_not
    : substitution_succeeded<typename deduce_logical_not<T>::type>
  { };
  //@}

  /**
   * @ingroup traits
   * @name Relational Operators
   * Traits in this group evaluate the presence of the equality and ordering
   * relational operators.
   */
  /** Deduce the result type of the expression x == y. */
  template<typename T, typename U>
  struct deduce_equal
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x == y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Return true if the expression x == y is valid for the types of x and  y.
   */
  template<typename T, typename U>
  struct has_equal
    : substitution_succeeded<typename deduce_equal<T, U>::type>
  { };

  /** Deduce the result type of the expression x != y. */
  template<typename T, typename U>
  struct deduce_not_equal
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x != y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Return true if the expression x == y is valid for the types of x and  y.
   */
  template<typename T, typename U>
  struct has_not_equal
    : substitution_succeeded<typename deduce_not_equal<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x < y for the types of x and y.
   */
  template<typename T, typename U>
  struct deduce_less
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x < y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Return true if the expression x < y is valid for the types of x and y.
   */
  template<typename T, typename U>
  struct has_less
    : substitution_succeeded<typename deduce_less<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x > y for the types of x and y.
   */
  template<typename T, typename U>
  struct deduce_greater
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x > y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Return true if the expression x > y is valid for the types of x and y.
   */
  template<typename T, typename U>
  struct has_greater
    : substitution_succeeded<typename deduce_greater<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x <= y for the types of x and y.
   */
  template<typename T, typename U>
  struct deduce_less_equal
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x <= y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Return true if the expression x <= y is valid for the types of x and y.
   */
  template<typename T, typename U>
  struct has_less_equal
    : substitution_succeeded<typename deduce_less_equal<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x >= y for the types of x and y.
   */
  template<typename T, typename U>
  struct deduce_greater_equal
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x >= y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Return true if the expression x <= y is valid for the types of x and y.
   */
  template<typename T, typename U>
  struct has_greater_equal
    : substitution_succeeded<typename deduce_greater_equal<T, U>::type>
  { };
  //@}

  // FIXME: Some forms of assignment are checked in the std namespace,
  // specifically, those dealing with triviality of copy and move operators.
  //
  // FIXME: Implement the rest of these traits!
  /**
   * @ingroup traits
   * @name Assignment oeprators
   * Traits for querying common assignment expressions.
   */
  /**
   * Deduce the result type of the x = y for the types of x and y.
   */
  template<typename T, typename U>
  struct deduce_assign
  {
  private:
    template<typename X, typename Y>
    static auto check(X& x, Y const& y) -> decltype(x = y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T&>(), std::declval<U>())) type;
  };

  /**
   * Return true if the expression x = y is valid for the types of x and y.
   */
  template<typename T, typename U>
  struct has_assign
    : substitution_succeeded<typename deduce_assign<T, U>::type>
  { };
  //@}

} // namespace origin

#endif
