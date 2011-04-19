// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONCEPTS_TRAITS_HPP
#define ORIGIN_CONCEPTS_TRAITS_HPP

#include <origin/concepts/impl.hpp>
#include <origin/concepts/fwd.hpp>

namespace origin
{
  /**
   * @defgroup concepts_type_traits Type Traits
   * @ingroup concepts
   *
   * Type traits are constraints that evaluate properties of types or sets
   * of types, especially from the perspective of the programming language.
   * These traits are often implemented in terms of an underlying standard
   * type trait.
   */

  /**
   * @ingroup concepts_type_traits
   *
   * The Same trait is valid for a sequence of types that all have (exactly)
   * the same type.
   */
  template<typename... Args>
  struct tSame
  {
    tSame()
    {
      static_assert(value, "Types are not the same");
    }

    typedef std::tuple<are_same<Args...>> requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts_type_traits
   *
   * The Different trait is valid for two types that are not the Same. This
   * is equivalent to !Same<T, U>::value.
   */
  template<typename T, typename U>
  struct tDifferent
  {
    tDifferent()
    {
      static_assert(value, "Types are not different");
    }

    typedef std::tuple<is_different<T, U>> requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts_type_traits
   *
   * The Common trait is valid if the type arguments share a common type. A
   * single type is its own common type. Two types have a common type, they can
   * be used as result values of a conditional expression (?:). An arbitrary
   * sequence of types share a common type if they can all be used as result
   * types of a recursivrely constructed conditional expression.
   */
  template<typename... Args>
  struct tCommon
  {
    tCommon()
    {
      static_assert(value, "Arguments do not share a common type");
    }

    typedef std::tuple<has_common_type<Args...>> requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts_type_traits
   *
   * The Convertible trait is valid for a type T that can be converted to a
   * type U.
   */
  template<typename T, typename U>
  struct tConvertible
  {
    tConvertible()
    {
      static_assert(value, "Cannot convert types");
    }

    typedef std::tuple<std::is_convertible<T, U>> requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts_type_traits
   *
   * The Derived trait is true for a type T that is derived from a type U.
   */
  template<typename T, typename U>
  struct tDerived
  {
    tDerived()
    {
      static_assert(value, "Type is not derived");
    }

    typedef std::tuple<std::is_base_of<T, U>> requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts_type_traits
   *
   * The Int trait is true for all built-in integral types.
   */
  template<typename T>
  struct tInt
  {
    tInt()
    {
      static_assert(value, "Type is not a standard integral type");
    }

    typedef std::tuple<std::is_integral<T>> requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts_type_traits
   *
   * The Signed_int trait is true for all built-in signed integral types.
   */
  template<typename T>
  struct tSigned_Int
  {
    tSigned_Int()
    {
      static_assert(value, "Type is not standard signed integral type");
    }

    typedef std::tuple<std::is_signed<T>> requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts_type_traits
   *
   * The Unsigned_int trait is true for all built-in unsigned integral types.
   */
  template<typename T>
  struct tUnsigned_Int
  {
    tUnsigned_Int()
    {
      static_assert(value, "Type is not standard unsigned integral type");
    }

    typedef std::tuple<std::is_unsigned<T>> requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  /**
   * @defgroup concepts_interface_traits Interface Traits
   * @ingroup concepts
   *
   * Interface traits are constraints that evaluate an aspect of a type's
   * interface. These are often implemented in terms of use patterns.
   */

  /**
   * @ingroup concepts_interface_traits
   *
   * The Destructible trait is valid for a type T that can be destroyed.
   */
  template<typename T>
  struct tDestructible
  {
    tDestructible()
    {
      auto p = constraints;
    }

    static void constraints(T& x)
    {
      x.T::~T();
    }

    // FIXME: Use is_destructible when it becomes available. For now, it's
    // probably okay to assume that all types are destructible. I could write
    // a type trait just to be sure.
    typedef std::true_type type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts_interface_traits
   *
   * The Constructible trait is valid for a type T that can be constructed over
   * the given sequence of arguments.
   */
  template<typename T, typename... Args>
  struct tConstructible
    : tDestructible<T>
  {
    tConstructible()
    {
      auto p = constraints;
    }

    static void constraints(Args&&... args)
    {
      T{std::forward<Args>(args)...};
    }

    typedef std::tuple<is_constructible<T, Args...>> requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts_interface_traits
   *
   * The Default trait is valid for types that can be default constructed.
   */
  template<typename T>
  struct tDefault
  {
    tDefault()
    {
      static_assert(
        std::is_constructible<T>::value,
        "Type is not Default constructible"
      );
    }

    // FIXME: Use std::has_default_constructor when it becomes available.
    typedef std::tuple<std::is_constructible<T>> requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts_interface_traits
   *
   * The Copyable trait is valid for a type T that supports copy construction.
   * Note that the trait not associate semantics with copyability.
   *
   * See the constraint class cCopyable for a semantic version of Copyable.
   */
  template<typename T>
  struct tCopyable
  {
    tCopyable()
    {
      static_assert(
        is_constructible<T, T const&>::value,
        "Type is not Copy constructible"
      );
    }

    // FIXME: Use std::has_copy_constructor when it becomes available.
    typedef std::tuple<std::is_constructible<T, T const&>> requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts_interface_traits
   *
   * The Moveable trait is valid for a type T that supports move construction.
   */
  template<typename T>
  struct tMoveable
  {
    tMoveable()
    {
      static_assert(
        is_constructible<T, T&&>::value,
        "Type is not Move constructible"
      );
    }

    // FIXME: Use std::has_move_constructor when it becomes available.
    typedef std::tuple<std::is_constructible<T, T&&>> requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts_interface_traits
   *
   * The Assignable trait is valid for a T (the assignee) that supports
   * assignment to an object type U (the assigned). The result of the
   * expression must be convertible to the assignee type. Note that if the
   * assigned type is an rvalue reference, the trait is interpreted as a
   * requirement for move assignment.
   */
  template<typename T, typename U>
  struct tAssignable
  {
    tAssignable()
    {
      auto p = constraints;
    }

    static void constraints(T& x, U y)
    {
      tConvertible<decltype(x = y), T&>{};
    }

    typedef std::tuple<
      has_assign<T, U>,
      tConvertible<typename get_assign_result<T, U>::type, T&>
    > requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  // The specialization restates a requirement on move assignment.
  template<typename T, typename U>
  struct tAssignable<T, U&&>
  {
    tAssignable()
    {
      auto p = constraints;
    }

    static void constraints(T& x, U y)
    {
      tConvertible<decltype(x = std::move(y)), T&>{};
    }

    typedef std::tuple<
      has_assign<T, U&&>,
      tConvertible<typename get_assign_result<T, U&&>::type, T&>
    > requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts_interface_traits
   *
   * The Equal concept is valid for types types that can be compared using
   * the operator ==. The result of equality operations are required to be
   * Boolean. Operator != is derived.
   */
  template<typename T, typename U>
  struct tEqual
  {
    tEqual()
    {
      auto p = constraints;
    }

    static void constraints(T x, T y)
    {
      cBoolean<decltype(x == y)>{};
      cBoolean<decltype(y == x)>{};
    }

    typedef std::tuple<
      has_equal<T, U>,
      cBoolean<typename get_equal_result<T, U>::type>,
      has_equal<U, T>,
      cBoolean<typename get_equal_result<U, T>::type>
    > requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  // Derived inequality
  template<typename T, typename U>
  auto operator!=(T const& x, U const& y)
    -> typename std::enable_if<tEqual<T, U>::value, decltype(!(x == y))>::type
  {
    return !(x == y);
  }

  /**
   * @ingroup concepts_interface_traits
   *
   * The Less concept is valid for types types that can be compared using
   * the operator <. The result of equality operations are required to be
   * Boolean. The operators >, <= and >= are derived.
   */
  template<typename T, typename U>
  struct tLess
  {
    tLess()
    {
      auto p = constraints;
    }

    static void constraints(T x, T y)
    {
      cBoolean<decltype(x < y)>{};
      cBoolean<decltype(y < x)>{};
    }

    typedef std::tuple<
      has_less<T, U>,
      cBoolean<typename get_less_result<T, U>::type>,
      has_equal<U, T>,
      cBoolean<typename get_less_result<U, T>::type>
    > requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  // Derived greater than
  template<typename T, typename U>
  auto operator>(T const& x, U const& y)
    -> typename std::enable_if<tLess<T, U>::value, decltype(y < x)>::type
  {
    return y < x;
  }

  // Derived less-equal
  template<typename T, typename U>
  auto operator<=(T const& x, U const& y)
    -> typename std::enable_if<tLess<T, U>::value, decltype(!(y < x))>::type
  {
    return !(y < x);
  }

  // Derived greater-equal
  template<typename T, typename U>
  auto operator>=(T const& x, U const& y)
    -> typename std::enable_if<tLess<T, U>::value, decltype(!(x < y))>::type
  {
    return !(x < y);
  }

  /**
   * @ingroup concepts_interface_traits
   *
   * The Logical_And trait is valid for types that can be logically conjoined
   * using the operator &&.
   */
  template<typename T, typename U>
  struct tLogical_And
  {
    tLogical_And()
    {
      auto p = constraints;
    }

    static void constraints(T x, T y)
    {
      tConvertible<decltype(x && y), bool>{};
      tConvertible<decltype(y && x), bool>{};
    }

    typedef std::tuple<
      has_logical_and<T, U>,
      tConvertible<typename get_logical_and_result<T, U>::type, bool>,
      has_logical_and<U, T>,
      tConvertible<typename get_logical_and_result<U, T>::type, bool>
    > requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts_interface_traits
   *
   * The Logical_Or trait is valid for types that can be logically disjoined
   * using the operator ||.
   */
  template<typename T, typename U>
  struct tLogical_Or
  {
    tLogical_Or()
    {
      auto p = constraints;
    }

    static void constraints(T x, T y)
    {
      tConvertible<decltype(x || y), bool>{};
      tConvertible<decltype(y || x), bool>{};
    }

    typedef std::tuple<
      has_logical_or<T, U>,
      tConvertible<typename get_logical_or_result<T, U>::type, bool>,
      has_logical_or<U, T>,
      tConvertible<typename get_logical_or_result<U, T>::type, bool>
    > requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts_interface_traits
   *
   * The Logical_Not trait is valid for any type that can be logically inverted
   * using the operator !.
   */
  template<typename T>
  struct tLogical_Not
  {
    tLogical_Not()
    {
      auto p = constraints;
    }

    static void constraints(T x)
    {
      tConvertible<decltype(!x), bool>{};
    }

    typedef std::tuple<
      has_logical_not<T>,
      tConvertible<typename get_logical_not_result<T>::type, bool>
    > requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

} // namespace origin

#endif
