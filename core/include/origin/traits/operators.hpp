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
   * @name Arithmetic Operators
   */
  //@{
  /**
   * Deduce the result type of the expression x + y.
   */
  template<typename T, typename U>
  struct deduce_plus
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x + y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Return true if the expression x + y is valid for the types of x and  y.
   */
  template<typename T, typename U>
  struct has_plus
    : substitution_succeeded<typename deduce_plus<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x - y.
   */
  template<typename T, typename U>
  struct deduce_minus
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x - y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Return true if the expression x - y is valid for the types of x and  y.
   */
  template<typename T, typename U>
  struct has_minus
    : substitution_succeeded<typename deduce_minus<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x * y.
   */
  template<typename T, typename U>
  struct deduce_multiplies
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x * y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Return true if the expression x * y is valid for the types of x and  y.
   */
  template<typename T, typename U>
  struct has_multiplies
    : substitution_succeeded<typename deduce_multiplies<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x / y.
   */
  template<typename T, typename U>
  struct deduce_divides
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x / y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Return true if the expression x / y is valid for the types of x and  y.
   */
  template<typename T, typename U>
  struct has_divides
    : substitution_succeeded<typename deduce_divides<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x % y.
   */
  template<typename T, typename U>
  struct deduce_modulus
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x % y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Return true if the expression x % y is valid for the types of x and y.
   */
  template<typename T, typename U>
  struct has_modulus
    : substitution_succeeded<typename deduce_modulus<T, U>::type>
  { };

  /**
   * Deduce the result type of the unary expression +x.
   */
  template<typename T>
  struct deduce_unary_plus
  {
  private:
    template<typename X>
    static auto check(X const& x) -> decltype(+x);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>())) type;
  };

  /**
   * Return true if the expression +x is valid for the types of x.
   */
  template<typename T>
  struct has_unary_plus
    : substitution_succeeded<typename deduce_unary_plus<T>::type>
  { };

  /**
   * Deduce the result type of the unary expression -x.
   */
  template<typename T>
  struct deduce_unary_minus
  {
  private:
    template<typename X>
    static auto check(X const& x) -> decltype(-x);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>())) type;
  };

  /**
   * Return true if the expression -x is valid for the types of x.
   */
  template<typename T>
  struct has_unary_minus
    : substitution_succeeded<typename deduce_unary_minus<T>::type>
  { };
  //@}

  /**
   * @ingroup traits
   * @name Bitwise Operators
   */
  //@{
  /**
   * Deduce the result type of the expression x & y.
   */
  template<typename T, typename U>
  struct deduce_bit_and
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x & y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Return true if the expression x & y is valid for the types of x and y.
   */
  template<typename T, typename U>
  struct has_bit_and
    : substitution_succeeded<typename deduce_bit_and<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x | y.
   */
  template<typename T, typename U>
  struct deduce_bit_or
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x | y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Return true if the expression x | y is valid for the types of x and y.
   */
  template<typename T, typename U>
  struct has_bit_or
    : substitution_succeeded<typename deduce_bit_or<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x % y.^
   */
  template<typename T, typename U>
  struct deduce_bit_xor
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x ^ y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Return true if the expression x ^ y is valid for the types of x and y.
   */
  template<typename T, typename U>
  struct has_bit_xor
    : substitution_succeeded<typename deduce_bit_xor<T, U>::type>
  { };

  // NOTE: The check functions accept rvalue references so that we can match
  // more loosely when lvalue references are required. Essentially, this is
  // loosened so that we can write checks against I/O streams.
  /**
   * Deduce the result type of the expression x << y.
   */
  template<typename T, typename U>
  struct deduce_left_shift
  {
  private:
    template<typename X, typename Y>
    static auto check(X&& x, Y&& y) -> decltype(x << y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Return true if the expression x << y is valid for the types of x and y.
   */
  template<typename T, typename U>
  struct has_left_shift
    : substitution_succeeded<typename deduce_left_shift<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x >> y.
   */
  template<typename T, typename U>
  struct deduce_right_shift
  {
  private:
    template<typename X, typename Y>
    static auto check(X&& x, Y&& y) -> decltype(x >> y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Return true if the expression x >> y is valid for the types of x and y.
   */
  template<typename T, typename U>
  struct has_right_shift
    : substitution_succeeded<typename deduce_right_shift<T, U>::type>
  { };

  /**
   * Deduce the result type of the unary expression ~x.
   */
  template<typename T>
  struct deduce_complement
  {
  private:
    template<typename X>
    static auto check(X const& x) -> decltype(~x);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>())) type;
  };

  /**
   * Return true if the expression ~x is valid for the types of x.
   */
  template<typename T>
  struct has_complement
    : substitution_succeeded<typename deduce_complement<T>::type>
  { };
  //@}

  /**
   * @ingroup traits
   * @name Logical Operators
   */
  //@{
  /**
   * Deduce the result type of the expression x && y where x has type T and y
   * has type U. If no such operation is supported, the result type is
   * substitution_failure.
   */
  template<typename T, typename U>
  struct get_logical_and_result
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x && y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Determine the validity of the expression x < y where x has type T and y
   * has type U. Return true if valid, false otherwise.
   */
  template<typename T, typename U>
  struct has_logical_and
    : substitution_succeeded<typename get_logical_and_result<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x || y where x has type T and y
   * has type U. If no such operation is supported, the result type is
   * substitution_failure.
   */
  template<typename T, typename U>
  struct get_logical_or_result
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x || y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Determine the validity of the expression x < y where x has type T and y
   * has type U. Return true if valid, false otherwise.
   */
  template<typename T, typename U>
  struct has_logical_or
    : substitution_succeeded<typename get_logical_or_result<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression !x where x has type T. If no
   * such operation is supported, the result type is substitution_failure.
   */
  template<typename T>
  struct get_logical_not_result
  {
  private:
    template<typename X>
    static auto check(X const& x) -> decltype(!x);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>())) type;
  };

  /**
   * Determine the validity of the expression !x where x has type T. Return
   * true if valid, false otherwise.
   */
  template<typename T>
  struct has_logical_not
    : substitution_succeeded<typename get_logical_not_result<T>::type>
  { };
  //@}

  /**
   * @ingroup traits
   * @name Relational Operators
   *
   * Traits in this group evaluate the presence of the equality and ordering
   * relational operators.
   */
  //@{
  /**
   * Deduce the result type of the expression x == y where x has type
   * T and y has type U. If no such operation is supported, the result type
   * is substitution_failure.
   */
  template<typename T, typename U>
  struct get_equal_result
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x == y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Determine the validity of the expression x == y where x has type T and y
   * has type U. Return true if valid, false otherwise.
   */
  template<typename T, typename U>
  struct has_equal
    : substitution_succeeded<typename get_equal_result<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x != y.
   */
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
   * Deduce the result type of the expression x < y where x has type T and y
   * has type U. If no such operation is supported, the result type is
   * substitution_failure.
   */
  template<typename T, typename U>
  struct get_less_result
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x < y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Determine the validity of the expression x < y where x has type T and y
   * has type U. Return true if valid, false otherwise.
   */
  template<typename T, typename U>
  struct has_less
    : substitution_succeeded<typename get_less_result<T, U>::type>
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
  //@{
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

  /**
   * @ingroup traits
   * @name Arithmetic Operators
   */
  //@{
  /**
   * Deduce the result type of the expression ++x for the type of x.
   */
  template<typename T>
  struct deduce_pre_increment
  {
  private:
    template<typename X>
    static auto check(X& x) -> decltype(++x);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T&>())) type;
  };

  /**
   * Return true if the expression ++x is valid for the type of x.
   */
  template<typename T>
  struct has_pre_increment
    : substitution_succeeded<typename deduce_pre_increment<T>::type>
  { };

  /**
   * Deduce the result type of the expression x++ for the type of x.
   */
  template<typename T>
  struct deduce_post_increment
  {
  private:
    template<typename X>
    static auto check(X& x) -> decltype(x++);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T&>())) type;
  };

  /**
   * Return true if the expression x++ is valid for the type of x.
   */
  template<typename T>
  struct has_post_increment
    : substitution_succeeded<typename deduce_post_increment<T>::type>
  { };

  /**
   * Deduce the result type of the expression --x for the type of x.
   */
  template<typename T>
  struct deduce_pre_decrement
  {
  private:
    template<typename X>
    static auto check(X& x) -> decltype(--x);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T&>())) type;
  };

  /**
   * Return true if the expression --x is valid for the type of x.
   */
  template<typename T>
  struct has_pre_decrement
    : substitution_succeeded<typename deduce_pre_decrement<T>::type>
  { };

  /**
   * Deduce the result type of the expression x-- for the type of x.
   */
  template<typename T>
  struct deduce_post_decrement
  {
  private:
    template<typename X>
    static auto check(X& x) -> decltype(x--);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T&>())) type;
  };

  /**
   * Return true if the expression x-- is valid for the type of x.
   */
  template<typename T>
  struct has_post_decrement
    : substitution_succeeded<typename deduce_post_decrement<T>::type>
  { };
  //@}

  /**
   * @ingroup traits
   * @name Miscellaneous Operators
   */
  //@{
  /**
   * Deduce the result type of the *x for the type of x.
   */
  template<typename T>
  struct deduce_dereference
  {
  private:
    template<typename X>
    static auto check(X& x) -> decltype(*x);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T&>())) type;
  };

  /**
   * Return true if the expression *x is valid for the type of x.
   */
  template<typename T>
  struct has_dereference
    : substitution_succeeded<typename deduce_dereference<T>::type>
  { };
  //@}

} // namespace origin

#endif
