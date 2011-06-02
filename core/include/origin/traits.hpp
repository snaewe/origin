// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_TRAITS_HPP
#define ORIGIN_TRAITS_HPP

#include <type_traits>
#include <origin/utility/meta.hpp>

namespace origin
{
  /**
   * @name Arithmetic Operators
   * @ingroup traits
   */
  //@{
  /**
   * Deduce the result type of the expression x + y where x has type T and
   * y has type U. If the operation is not supported for the tyeps T and U,
   * the result type is substitution_failure.
   */
  template<typename T, typename U>
  struct get_plus_result
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x + y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Determine the validity of the expression x + y where x has type T and y
   * has type U. Return true if valid, false otherwise.
   */
  template<typename T, typename U>
  struct has_plus
    : substitution_succeeded<typename get_plus_result<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x - y where x has type T and
   * y has type U. If the operation is not supported for the tyeps T and U,
   * the result type is substitution_failure.
   */
  template<typename T, typename U>
  struct get_minus_result
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x - y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Determine the validity of the expression x - y where x has type T and y
   * has type U. Return true if valid, false otherwise.
   */
  template<typename T, typename U>
  struct has_minus
    : substitution_succeeded<typename get_minus_result<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x * y where x has type T and
   * y has type U. If the operation is not supported for the tyeps T and U,
   * the result type is substitution_failure.
   */
  template<typename T, typename U>
  struct get_multiply_result
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x * y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Determine the validity of the expression x * y where x has type T and y
   * has type U. Return true if valid, false otherwise.
   */
  template<typename T, typename U>
  struct has_multiply
    : substitution_succeeded<typename get_multiply_result<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x / y where x has type T and
   * y has type U. If the operation is not supported for the tyeps T and U,
   * the result type is substitution_failure.
   */
  template<typename T, typename U>
  struct get_divide_result
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x / y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Determine the validity of the expression x / y where x has type T and y
   * has type U. Return true if valid, false otherwise.
   */

  template<typename T, typename U>
  struct has_divide
    : substitution_succeeded<typename get_divide_result<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x % y where x has type T and
   * y has type U. If the operation is not supported for the tyeps T and U,
   * the result type is substitution_failure.
   */
  template<typename T, typename U>
  struct get_modulus_result
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x % y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Determine the validity of the expression x % y where x has type T and y
   * has type U. Return true if valid, false otherwise.
   */
  template<typename T, typename U>
  struct has_modulus
    : substitution_succeeded<typename get_modulus_result<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression +x where x has type T If the 
   * operation is not supported for the tyeps T and U, the result type is 
   * substitution_failure.
   */
  template<typename T>
  struct get_unary_plus_result
  {
  private:
    template<typename X>
    static auto check(X const& x) -> decltype(+x);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>())) type;
  };

  /**
   * Determine the validity of the expression +x where x has type T. Return 
   * true if valid, false  otherwise.
   */
  template<typename T>
  struct has_unary_plus
    : substitution_succeeded<typename get_unary_plus_result<T>::type>
  { };

  /**
   * Deduce the result type of the expression -x where x has type T If the 
   * operation is not supported for the tyeps T and U, the result type is 
   * substitution_failure.
   */
  template<typename T>
  struct get_unary_minus_result
  {
  private:
    template<typename X>
    static auto check(X const& x) -> decltype(-x);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>())) type;
  };

  /**
   * Determine the validity of the expression -x where x has type T. Return 
   * true if valid, false otherwise.
   */
  template<typename T>
  struct has_unary_minus
    : substitution_succeeded<typename get_unary_minus_result<T>::type>
  { };
  //@}

  /**
   * @ingroup traits
   * @name Bitwise Operators
   */
  //@{
  /**
   * Deduce the result type of the expression x & y where x has type T and
   * y has type U. If the operation is not supported for the tyeps T and U,
   * the result type is substitution_failure.
   */
  template<typename T, typename U>
  struct get_bit_and_result
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x & y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Determine the validity of the expression x & y where x has type T and y
   * has type U. Return true if valid, false otherwise.
   */
  template<typename T, typename U>
  struct has_bit_and
    : substitution_succeeded<typename get_bit_and_result<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x % y where x has type T and
   * y has type U. If the operation is not supported for the tyeps T and U,
   * the result type is substitution_failure.
   */
  template<typename T, typename U>
  struct get_bit_or_result
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x | y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Determine the validity of the expression x | y where x has type T and y
   * has type U. Return true if valid, false otherwise.
   */
  template<typename T, typename U>
  struct has_bit_or
    : substitution_succeeded<typename get_bit_or_result<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x ^ y where x has type T and
   * y has type U. If the operation is not supported for the tyeps T and U,
   * the result type is substitution_failure.
   */
  template<typename T, typename U>
  struct get_bit_xor_result
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x ^ y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Determine the validity of the expression x ^ y where x has type T and y
   * has type U. Return true if valid, false otherwise.
   */
  template<typename T, typename U>
  struct has_bit_xor
    : substitution_succeeded<typename get_bit_xor_result<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x << y where x has type T and
   * y has type U. If the operation is not supported for the tyeps T and U,
   * the result type is substitution_failure.
   */
  template<typename T, typename U>
  struct get_left_shift_result
  {
  private:
    template<typename X, typename Y>
    static auto check(X&& x, Y&& y) -> decltype(x << y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Determine the validity of the expression x << y where x has type T and
   * y has type U. Return true if valid, false otherwise.
   */
  template<typename T, typename U>
  struct has_left_shift
    : substitution_succeeded<typename get_left_shift_result<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x >> y where x has type T and
   * y has type U. If the operation is not supported for the tyeps T and U,
   * the result type is substitution_failure.
   */
  template<typename T, typename U>
  struct get_right_shift_result
  {
  private:
    template<typename X, typename Y>
    static auto check(X&& x, Y&& y) -> decltype(x >> y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Determine the validity of the expression x >> y where x has type T and y
   * has type U. Return true if valid, false otherwise.
   */
  template<typename T, typename U>
  struct has_right_shift
    : substitution_succeeded<typename get_right_shift_result<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression ~x where x has type T If the 
   * operation is not supported for the tyeps T and U, the result type is 
   * substitution_failure.
   */
  template<typename T>
  struct get_complement_result
  {
  private:
    template<typename X>
    static auto check(X const& x) -> decltype(~x);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>())) type;
  };

  /**
   * Determine the validity of the expression ~x where x has type T. Return 
   * true if valid, false otherwise.
   */
  template<typename T>
  struct has_complement
    : substitution_succeeded<typename get_complement_result<T>::type>
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
   * Deduce the result type of the expression x == y where x has type T and y 
   * has type U. If no such operation is supported, the result type is 
   * substitution_failure.
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
   * Deduce the result type of the expression x != y where x has type T and y 
   * has type U. If no such operation is supported, the result type is 
   * substitution_failure.
   */
  template<typename T, typename U>
  struct get_not_equal_result
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x != y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Determine the validity of the expression x != y where x has type T and y
   * has type U. Return true if valid, false otherwise.
   */
  template<typename T, typename U>
  struct has_not_equal
    : substitution_succeeded<typename get_not_equal_result<T, U>::type>
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
   * Deduce the result type of the expression x > y where x has type T and y 
   * has type U. If no such operation is supported, the result type is 
   * substitution_failure.
   */
  template<typename T, typename U>
  struct get_greater_result
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x > y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Determine the validity of the expression x > y where x has type T and y
   * has type U. Return true if valid, false otherwise.
   */
  template<typename T, typename U>
  struct has_greater
    : substitution_succeeded<typename get_greater_result<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x <= y where x has type T and y 
   * has type U. If no such operation is supported, the result type is 
   * substitution_failure.
   */
  template<typename T, typename U>
  struct get_less_equal_result
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x <= y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Determine the validity of the expression x <= y where x has type T and y
   * has type U. Return true if valid, false otherwise.
   */
  template<typename T, typename U>
  struct has_less_equal
    : substitution_succeeded<typename get_less_equal_result<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x >= y where x has type T and y 
   * has type U. If no such operation is supported, the result type is 
   * substitution_failure.
   */
  template<typename T, typename U>
  struct get_greater_equal_result
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(x >= y);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Determine the validity of the expression x >= y where x has type T and y
   * has type U. Return true if valid, false otherwise.
   */
  template<typename T, typename U>
  struct has_greater_equal
    : substitution_succeeded<typename get_greater_equal_result<T, U>::type>
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
   * Deduce the result type of the expression x = y where x has type T and
   * y has type U. If the operation is not supported for the tyeps T and U,
   * the result type is substitution_failure.
   */
  template<typename T, typename U>
  struct get_assign_result
  {
  private:
    template<typename X, typename Y>
    static auto check(X& x, Y const& y) -> decltype(x = y);

    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T&>(), std::declval<U>())) type;
  };

  /**
   * Determine the validity of the expression x = y where x has type T and y
   * has type U. Return true if valid, false otherwise.
   */
  template<typename T, typename U>
  struct has_assign
    : substitution_succeeded<typename get_assign_result<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x += y where x has type T and
   * y has type U. If the operation is not supported for the tyeps T and U,
   * the result type is substitution_failure.
   */
  template<typename T, typename U>
  struct get_plus_assign_result
  {
  private:
    template<typename X, typename Y>
    static auto check(X& x, Y const& y) -> decltype(x += y);

    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T&>(), std::declval<U>())) type;
  };

  /**
   * Determine the validity of the expression x = y where x has type T and y
   * has type U. Return true if valid, false otherwise.
   */
  template<typename T, typename U>
  struct has_plus_assign
    : substitution_succeeded<typename get_plus_assign_result<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x -= y where x has type T and
   * y has type U. If the operation is not supported for the tyeps T and U,
   * the result type is substitution_failure.
   */
  template<typename T, typename U>
  struct get_minus_assign_result
  {
  private:
    template<typename X, typename Y>
    static auto check(X& x, Y const& y) -> decltype(x -= y);

    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T&>(), std::declval<U>())) type;
  };

  /**
   * Determine the validity of the expression x -= y where x has type T and y
   * has type U. Return true if valid, false otherwise.
   */
  template<typename T, typename U>
  struct has_minus_assign
    : substitution_succeeded<typename get_minus_assign_result<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x *= y where x has type T and
   * y has type U. If the operation is not supported for the tyeps T and U,
   * the result type is substitution_failure.
   */
  template<typename T, typename U>
  struct get_multiply_assign_result
  {
  private:
    template<typename X, typename Y>
    static auto check(X& x, Y const& y) -> decltype(x *= y);

    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T&>(), std::declval<U>())) type;
  };

  /**
   * Determine the validity of the expression x *= y where x has type T and y
   * has type U. Return true if valid, false otherwise.
   */
  template<typename T, typename U>
  struct has_multiply_assign
    : substitution_succeeded<typename get_multiply_assign_result<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x /= y where x has type T and
   * y has type U. If the operation is not supported for the tyeps T and U,
   * the result type is substitution_failure.
   */
  template<typename T, typename U>
  struct get_divide_assign_result
  {
  private:
    template<typename X, typename Y>
    static auto check(X& x, Y const& y) -> decltype(x /= y);

    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T&>(), std::declval<U>())) type;
  };

  /**
   * Determine the validity of the expression x /= y where x has type T and y
   * has type U. Return true if valid, false otherwise.
   */
  template<typename T, typename U>
  struct has_divide_assign
    : substitution_succeeded<typename get_divide_assign_result<T, U>::type>
  { };

  /**
   * Deduce the result type of the expression x %= y where x has type T and
   * y has type U. If the operation is not supported for the tyeps T and U,
   * the result type is substitution_failure.
   */
  template<typename T, typename U>
  struct get_modulus_assign_result
  {
  private:
    template<typename X, typename Y>
    static auto check(X& x, Y const& y) -> decltype(x %= y);

    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T&>(), std::declval<U>())) type;
  };

  /**
   * Determine the validity of the expression x %= y where x has type T and y
   * has type U. Return true if valid, false otherwise.
   */
  template<typename T, typename U>
  struct has_modulus_assign
    : substitution_succeeded<typename get_modulus_assign_result<T, U>::type>
  { };
  
  // FIXME: Missing traits for binary compound assignment operators

  /**
   * Deduce the result type of the expression ++x where x has type T. If no
   * such operation is supported, the result type is substitution_failure.
   */
  template<typename T>
  struct get_pre_increment_result
  {
  private:
    template<typename X>
    static auto check(X& x) -> decltype(++x);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T&>())) type;
  };

  /**
   * Determine the validity of the expression ++x where x has type T. Return
   * true if valid, false otherwise.
   */
  template<typename T>
  struct has_pre_increment
    : substitution_succeeded<typename get_pre_increment_result<T>::type>
  { };

  /**
   * Deduce the result type of the expression x++ where x has type T. If no
   * such operation is supported, the result type is substitution_failure.
   */
  template<typename T>
  struct get_post_increment_result
  {
  private:
    template<typename X>
    static auto check(X& x) -> decltype(x++);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T&>())) type;
  };

  /**
   * Determine the validity of the expression x++ where x has type T. Return
   * true if valid, false otherwise.
   */
  template<typename T>
  struct has_post_increment
    : substitution_succeeded<typename get_post_increment_result<T>::type>
  { };

  /**
   * Deduce the result type of the expression --x where x has type T. If no
   * such operation is supported, the result type is substitution_failure.
   */
  template<typename T>
  struct get_pre_decrement_result
  {
  private:
    template<typename X>
    static auto check(X& x) -> decltype(--x);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T&>())) type;
  };

  /**
   * Determine the validity of the expression --x where x has type T. Return
   * true if valid, false otherwise.
   */
  template<typename T>
  struct has_pre_decrement
    : substitution_succeeded<typename get_pre_decrement_result<T>::type>
  { };

  /**
   * Deduce the result type of the expression x-- where x has type T. If no
   * such operation is supported, the result type is substitution_failure.
   */
  template<typename T>
  struct get_post_decrement_result
  {
  private:
    template<typename X>
    static auto check(X& x) -> decltype(x--);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T&>())) type;
  };

  /**
   * Determine the validity of the expression x-- where x has type T. Return
   * true if valid, false otherwise.
   */
  template<typename T>
  struct has_post_decrement
    : substitution_succeeded<typename get_post_decrement_result<T>::type>
  { };
  //@}

  /**
   * @ingroup traits
   * @name Miscellaneous Operators
   */
  //@{
  /**
   * get the result type of the expression *x where x has type T. If no
   * such operation is supported, the result type is substitution_failure.
   */
  template<typename T>
  struct get_dereference_result
  {
  private:
    template<typename X>
    static auto check(X& x) -> decltype(*x);
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T&>())) type;
  };

  /**
   * Determine the validity of the expression *x where x has type T. Return
   * true if valid, false otherwise.
   */
  template<typename T>
  struct has_dereference
    : substitution_succeeded<typename get_dereference_result<T>::type>
  { };
  //@}



  /**
   * @internal
   * @ingroup traits
   *
   * Return the common type of the sequence of arguments. This is defined in
   * terms of a conditionally enabled conditional operator. The implementation
   * is fragile.
   */
  //@{
  template<typename... Args> struct find_conditional_result;

  template<typename T>
  struct find_conditional_result<T>
  {
    typedef T type;
  };

  template<typename T, typename U>
  struct find_conditional_result<T, U>
  {
  private:
    template<typename X, typename Y>
    static auto test(X x, Y y) -> decltype(true ? x : y);
    
    static substitution_failure test(...);
  public:
    typedef decltype(test(std::declval<T>(), std::declval<U>())) type;
  };

  template<typename T, typename... Args>
  struct find_conditional_result<T, Args...>
  {
  private:
    typedef typename front_type<Args...>::type Head;
    typedef typename find_conditional_result<T, Head>::type X;
    typedef typename find_conditional_result<Args...>::type Y;
  public:
    typedef typename find_conditional_result<X, Y>::type type;
  };

  /**
   * @ingroup traits
   *
   * Deduce the esult of the use of the conditional operator. The resulting 
   * type is not a reference type.
   */
  template<typename... Args>
  struct get_conditional_result
  {
    typedef typename std::remove_reference<
      typename find_conditional_result<Args...>::type
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
        typename get_conditional_result<Args...>::type
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
  
  //@}

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
  struct get_construct_result;

  // Check for default constructors
  template<typename T>
  struct get_construct_result<T>
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
  struct get_construct_result<T, Arg>
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
    : substitution_succeeded<typename get_construct_result<T>::type>
  { };

  // Check for nullary constructors
  template<typename T, typename Arg>
  struct is_constructible<T, Arg>
    : substitution_succeeded<typename get_construct_result<T, Arg>::type>
  { };
  //@}


} // namespace origin

#endif
