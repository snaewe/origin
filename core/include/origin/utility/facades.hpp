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
    template<typename U>
    static auto check(U const& x) -> decltype(x.equal(x));

    static substitution_failure check(...);

    typedef decltype(check(std::declval<T>())) type;
  };

  /**
   * Returns true if an object of type T supports the syntax `x.equal(x).`
   */
  template<typename T>
  struct has_member_equal
    : is_different<typename deduce_member_equal<T>::type, substitution_failure>
  { };

  /**
   * Provide operator== for all types T that model the ... concept.
   */
  template<typename T>
  typename std::enable_if<
    has_member_equal<T>::value, typename deduce_member_equal<T>::type
  >::type
  operator==(T const& x, T const& y)
  { return x.equal(y); }

  /**
   * Provide operator!= for all types T that model the ... concept.
   */
  template<typename T>
  typename std::enable_if<
    has_member_equal<T>::value, typename deduce_member_equal<T>::type
  >::type
  operator!=(T const& x, T const& y)
  { return !x.equal(y); }
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
    template<typename U>
    static auto check(U const& x) -> decltype(x.less(x));

    static substitution_failure check(...);

    typedef decltype(check(std::declval<T>())) type;
  };

  /**
   * Returns true if an object of type T supports the syntax `x.equal(x).`
   */
  template<typename T>
  struct has_member_less
    : is_different<typename deduce_member_less<T>::type, substitution_failure>
  { };

  /**
   * Provide operator< for all types modeling the ... concept.
   */
  template<typename T>
  typename std::enable_if<
    has_member_less<T>::value, typename deduce_member_less<T>::type
  >::type
  operator<(T const& x, T const& y)
  { return x.less(y); }

  /**
   * Provide operator> for all types modeling the ... concept.
   */
  template<typename T>
  typename std::enable_if<
    has_member_less<T>::value, typename deduce_member_less<T>::type
  >::type
  operator>(T const& x, T const& y)
  { return y.less(x); }

  /**
   * Provide operator>= for all types modeling the ... concept.
   */
  template<typename T>
  typename std::enable_if<
    has_member_less<T>::value, typename deduce_member_less<T>::type
  >::type
  operator>=(T const& x, T const& y)
  { return !y.less(x); }

  /**
   * Provide operator<= for all types modeling the ... concept.
   */
  template<typename T>
  typename std::enable_if<
    has_member_less<T>::value, typename deduce_member_less<T>::type
  >::type
  operator<=(T const& x, T const& y)
  { return !x.less(y); }
  //@}

} // namespace origin


#endif

