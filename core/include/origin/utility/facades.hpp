// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_UTILITY_FACADES_HPP
#define ORIGIN_UTILITY_FACADES_HPP

#include <origin/utility/meta.hpp>

namespace origin
{
  /**
   * The implicit_bool_facade implements the safe bool idiom for derived
   * classes, which allows objects of the derived type to be implicitly cast
   * to boolean values. The deriving class must define a member function named
   * 'valid' that takes no arguments and returns true or false.
   *
   * @todo Is this really a good name for this class. Historically, this was
   * called safe_bool_facade. I renamed it to implicit_bool because it
   * implements an implicit cast to something that can be used in a boolean
   * context.
   */
  template<typename Derived>
  struct implicit_bool_facade
  {
  private:
    typedef bool (Derived::*unspecified_bool_type)() const;
  public:
    operator unspecified_bool_type() const
    {
      return static_cast<Derived const*>(this)->valid()
        ? &Derived::valid
        : nullptr;
    }
  };

  /**
   * The dereference facade provides an implementation of the dereference
   * operator for the given type. Note that the reference type must be
   * given explicitly since there is no simple way to deduce it without
   * requiring the derived type to be complete (which is somewhat paradoxical
   * given the use of the curiously recurring template pattern).
   *
   * @tparam Derived The derived type inheriting the facade.
   * @tparam Reference The type returned by the dereference operator.
   */
  template<typename Derived, typename Reference>
  class dereference_facade
  {
  public:
    Reference operator*() const
    { return static_cast<Derived const*>(this)->dereference(); }
  };


  /**
   * The increment facade provides implementations of then pre- and pos-
   * decrement operators for the Derived type.
   *
   * @tparam Derived The derived type inheriting the facade. This type must be
   * Copyable.
   */
  template<typename Derived>
  class increment_facade
  {
  public:
    Derived& operator++()
    {
      static_cast<Derived*>(this)->increment();
      return *static_cast<Derived*>(this);
    }

    Derived operator++(int)
    {
      Derived tmp(*static_cast<Derived*>(this));
      static_cast<Derived*>(this)->increment();
      return tmp;
    }
  };

  /**
   * The decrement facade provides implements of then pre- and post-
   * decrement operators for the Derived type.
   *
   * @tparam Derived The derived type inheriting the facade. This type must be
   * Copyable.
   */
  template<typename Derived>
  class decrement_facade
  {
  public:
    Derived& operator--()
    {
      static_cast<Derived*>(this)->increment();
      return *static_cast<Derived*>(this);
    }

    Derived operator--(int)
    {
      Derived tmp(*static_cast<Derived*>(this));
      static_cast<Derived*>(this)->decrement();
      return tmp;
    }
  };

  // NOTE: I'm currently using the term "protocol" to describe something very
  // similar to a concept.

  /**
   * @name Equality Protocol
   * The equality protocol defines the sets of function required for a type
   * to be a model of the Equality_Comparable concept.
   */
  //@{
  /**
   * Return the result type of the member equal function or indicate a
   * substitution failure.
   */
  template<typename T>
  struct deduce_member_equal
  {
  private:
    template<typename U>
    static auto check(U const& x) -> decltype(x.equal(x));
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>())) type;
  };

  /**
   * Returns true if an object of type T supports the syntax `x.equal(x).`
   */
  template<typename T>
  struct has_member_equal
    : is_different<
        typename deduce_member_equal<T>::type, substitution_failure
      >
  { };

  /**
   * Safely return the result of the expression T::equal(x, y) where x is an
   * object of type T and y is an object of type U.
   */
  template<typename T, typename U>
  struct deduce_static_member_equal
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(X::equal(x, y));
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Return true if the type T supports the syntax T::equal(x, y) where x is
   * an object of type T and y is an object of type U.
   */
  template<typename T, typename U>
  struct has_static_member_equal
    : is_different<
        typename deduce_static_member_equal<T, U>::type, substitution_failure
      >
  { };

  /**
   * Provide the syntax x == y for all objects x and y of type T where T
   * supports the syntax x.equal(y).
   */
  template<typename T>
  typename std::enable_if<
    has_member_equal<T>::value, typename deduce_member_equal<T>::type
  >::type
  operator==(T const& x, T const& y)
  { return x.equal(y); }

  /**
   * Provide the syntax x != y for all objects x and y of type T where T
   * supports the syntax x.equal(y). The expression is equivalent to
   * !x.equal(y).
   */
  template<typename T>
  typename std::enable_if<
    has_member_equal<T>::value, typename deduce_member_equal<T>::type
  >::type
  operator!=(T const& x, T const& y)
  { return !x.equal(y); }

  /**
   * Provide the syntax x == y for all objects x of type T where T supports
   * the syntax T::equal(x, y).
   */
  template<typename T, typename U>
  typename std::enable_if<
    has_static_member_equal<T, U>::value,
    typename deduce_static_member_equal<T, U>::type
  >::type
  operator==(T const& x, U const& y)
  { return T::equal(x, y); }

  /**
   * Provide the syntax x == y for all objects y of type U where U supports
   * the syntax U::equal(y, x).
   */
  template<typename T, typename U>
  typename std::enable_if<
    has_static_member_equal<U, T>::value,
    typename deduce_static_member_equal<U, T>::type
  >::type
  operator==(T const& x, U const& y)
  { return U::equal(y, x); }

  /**
   * Provide the syntax x != y for all objects x of type T where T supports
   * the syntax T::equal(x, y). This is equivalent to !T::equal(x, y).
   */
  template<typename T, typename U>
  typename std::enable_if<
    has_static_member_equal<T, U>::value,
    typename deduce_static_member_equal<T, U>::type
  >::type
  operator!=(T const& x, U const& y)
  { return !T::equal(x, y); }

  /**
   * Provide the syntax for x != y for all objects y of type U where U supports
   * the syntax T::equal(y, x). This is equivalent to !U::equal(y, x).
   */
  template<typename T, typename U>
  typename std::enable_if<
    has_static_member_equal<U, T>::value,
    typename deduce_static_member_equal<U, T>::type
  >::type
  operator!=(T const& x, U const& y)
  { return !U::equal(y, x); }
  //@}

  /**
   * @name Ordered Protocol
   * The ordered protocol describes a set of member functions that can be
   * defined in order to model the Ordered concept.
   */
  //@{
  /**
   * Return the result type of the member less function or indicate a
   * substitution failure.
   */
  template<typename T>
  struct deduce_member_less
  {
  private:
    template<typename U>
    static auto check(U const& x) -> decltype(x.less(x));
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>())) type;
  };

  /**
   * Returns true if an object of type T supports the syntax `x.equal(x).`
   */
  template<typename T>
  struct has_member_less
    : is_different<typename deduce_member_less<T>::type, substitution_failure>
  { };

  // NOTE: Because operator< is an asymmetric operation, we actually require
  // two different protocols. One where the primary type is on the left and
  // one where it is on the right.
  /**
   * Deduce the result type of the expression T::equal(x, y) where x is of type
   * T and y is not.
   */
  template<typename T, typename U>
  struct deduce_left_static_member_less
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(X::less(x, y));
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<T>(), std::declval<U>())) type;
  };

  /**
   * Return true if type T supports the syntax T::equal(x, y) where x is of
   * type T and y is not.
   */
  template<typename T, typename U>
  struct has_left_static_member_less
    : is_different<
      typename deduce_left_static_member_less<T, U>::type, substitution_failure
    >
  { };

  /**
   * Deduce the result type of the expression T::less(y, x) where x is of type
   * T and y is not.
   */
  template<typename T, typename U>
  struct deduce_right_static_member_less
  {
  private:
    template<typename X, typename Y>
    static auto check(X const& x, Y const& y) -> decltype(Y::less(x, y));
    static substitution_failure check(...);
  public:
    typedef decltype(check(std::declval<U>(), std::declval<T>())) type;
  };

  /**
   * Return true if T supports the syntax T::less(y, x) where x is of type T
   * and y is not.
   */
  template<typename T, typename U>
  struct has_right_static_member_less
    : is_different<
      typename deduce_right_static_member_less<T, U>::type, substitution_failure
    >
  { };

  // FIXME: What's the right way to write "..." Is it a concept being modeled,
  // or is it a protocol being implemented? Is there really a difference.
  /**
   * Provide the syntax x < y for all objects x and y of type T where...
   */
  template<typename T>
  typename std::enable_if<
    has_member_less<T>::value, typename deduce_member_less<T>::type
  >::type
  operator<(T const& x, T const& y)
  { return x.less(y); }

  /**
   * Provide the syntax x > y for all objects x and y of type T where...
   */
  template<typename T>
  typename std::enable_if<
    has_member_less<T>::value, typename deduce_member_less<T>::type
  >::type
  operator>(T const& x, T const& y)
  { return y.less(x); }

  /**
   * Provide the syntax x <= y for all objects x and y of type T where...
   */
  template<typename T>
  typename std::enable_if<
    has_member_less<T>::value, typename deduce_member_less<T>::type
  >::type
  operator>=(T const& x, T const& y)
  { return !y.less(x); }

  /**
   * Provide the syntax x >= y for all objects x and y of type T where...
   */
  template<typename T>
  typename std::enable_if<
    has_member_less<T>::value, typename deduce_member_less<T>::type
  >::type
  operator<=(T const& x, T const& y)
  { return !x.less(y); }

  /**
   * Provide the syntax x < y for objects x of type T where...
   */
  template<typename T, typename U>
  typename std::enable_if<
    has_left_static_member_less<T, U>::value,
    typename deduce_left_static_member_less<T, U>::type
  >::type
  operator<(T const& x, U const& y)
  { return T::less(x, y); }

  /**
   * Provide the syntax x < y for objects y of type U where...
   */
  template<typename T, typename U>
  typename std::enable_if<
    has_right_static_member_less<U, T>::value,
    typename deduce_right_static_member_less<U, T>::type
  >::type
  operator<(T const& x, U const& y)
  { return U::less(x, y); }
  //@}

} // namespace origin


#endif

