// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONCEPTS_REGULAR_HPP
#define ORIGIN_CONCEPTS_REGULAR_HPP

#include <functional>

#include <origin/concepts/functional.hpp>

namespace origin
{
  template<typename T, typename U = T> struct Equal;
  template<typename T, typename U = T> struct Ordered;

  template<typename T> struct Regular;
  template<typename T> struct Boolean;

  /**
   * @ingroup concepts
   * When instantiated over two different types, the concept determines whether
   * or not the two types can be compared using the relational equality
   * operators (== and !=). This further requires that the the equality operator
   * be a symmetric relation.
   */
  template<typename T, typename U>
  struct Equal
  {
    Equal()
    { auto p = constraints; }

    static void constraints(T x, U y)
    {
      x == y; x != y;
      y == x; y != x;
    }

    // FIXME: Axioms!

    typedef std::tuple<
      has_equal<T, U>, has_not_equal<T, U>,
      has_equal<U, T>, has_not_equal<U, T>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  /**
   * When instantiated over a single type (or when both types are the same),
   * the concept determines if the given type can be used with the relational
   * equality operators (== and !=) and that it defines an equivalence relation
   * over the equality operator (==).
   */
  template <typename T>
  struct Equal<T, T>
  {
    Equal()
    { auto p = constraints; }

    static void constraints(T x, T y, T z)
    {
      x == y;
      x != y;
    }

    struct axioms {
      //  FIXME: Requires Boolean<T>
      static auto Equal_Identity(T x, T y)
        -> decltype(implies(&x == &y, x == y))
      { return implies(&x == &y, x == y); }

      static auto Reflexive(T x)
        -> decltype(x == x)
      { return x == x; }

      //  FIXME: Requires Boolean<T>
      static auto Symmetric(T x, T y)
        -> decltype(implies(x == y, y == x))
      { return implies(x == y, y == x); }

      //  FIXME: Requires Boolean<T>
      static auto Transitive(T x, T y, T z)
        -> decltype(implies(x == y && y == z, x == z))
      { return implies(x == y && y == z, x == z); }
    };

    typedef std::tuple<
      has_equal<T, T>, has_not_equal<T, T>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  /**
   * The ordered concept defines two different comparisons. When instantiated
   * over one type (or the same type), it determines if that type is ordered,
   * that it can be compared using the relational ordering operators. When
   * instantiated over two types,
   */
  template<typename T, typename U>
  struct Ordered
  {
    Ordered()
    { auto p = constraints; }

    static void constraints(T x, T y)
    {
      x < y; x > y; x <= y; y <= x;
      y < x; y > x; x <= y; x >= y;
    }

    // FIXME: Axioms!

    typedef std::tuple<
      has_less<T, U>, has_greater<T, U>,
      has_less_equal<T, U>, has_greater_equal<T, U>,
      has_less<U, T>, has_greater<U, T>,
      has_less_equal<U, T>, has_greater_equal<U, T>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  template<typename T>
  struct Ordered<T, T>
  {
    Ordered()
    { auto p = constraints; }

    static void constraints(T x, T y)
    {
      x < y;
      x > y;
      x <= y;
      y <= x;
    }

    struct axioms
    {
      static auto irreflexive(T x)
        -> decltype(!(x < x))
      { return !(x < x); }

      // FIXME: Requires Boolean<T>
      static auto antisymmetric(T x, T y)
        -> decltype(implies(x < y, !(y < x)))
      { return implies(x < y, !(y < x)); }

      //  FIXME: Requires Boolean<T>
      static auto transitive(T x, T y, T z)
        -> decltype(implies(x < y && y < z, x == z))
      { return implies(x < y && y < z, x == z); }
    };

    typedef std::tuple<
      has_less<T, T>,
      has_greater<T, T>,
      has_less_equal<T, T>,
      has_greater_equal<T, T>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };


  /**
   * A default type can be default constructed.
   */
  template<typename T>
  struct Default
  {
    Default()
    { auto p = constraints; }

    static void constraints()
    {
      T{};
      T x;
    }

    // FIXME: Use std::has_default_constructor when it becomes available.
    typedef std::tuple<
      is_constructible<T>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  /**
   * A type is copyable if it can be copy constucted or copy assigned.
   */
  template<typename T>
  struct Copyable
    : Equal<T>
  {
    Copyable()
    { auto p = constraints; }

    static void constraints(T x)
    {
      T y{x};
      x = y;
    }

    struct axioms
    {
      static auto copy_equivalence(T x, T y)
        -> decltype(x == y)
      { x = y; return x == y; }
    };

    // FIXME: Use std::has_copy_constructor and std::has_copy_assign when they
    // become available.
    typedef std::tuple<
      is_constructible<T, T const&>,
      has_assign<T, T const&>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts
   * A type is moveable if it can be constructed over or assigned to an rvalue
   * reference. This is typically done by calling std::move to generate an
   * rvalue reference.
   *
   * Note that, as a concept, moveability is unrelated to copyability even
   * though all copyable types will generally satisfy the move syntax unless
   * they explicitly delete those operations.
   */
  template<typename T>
  struct Moveable
  {
    Moveable()
    { auto p = constraints; }

    static void constraints(T x)
    {
      T y{std::move(x)};
      x = std::move(y);
    }

    // FIXME: Use std::has_move_constructor and std::has_move_assign when they
    // become available.
    typedef std::tuple<
      is_constructible<T, T&&>,
      has_assign<T, T&&>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts
   * A regular type is one that can be default constructed and copied, which
   * also implies equality comparison.
   */
  template<typename T>
  struct Regular
    : Default<T>, Copyable<T>
  {
    Regular()
    { auto p = constraints; }

    static void constraints()
    { }

    typedef std::tuple<
      Default<T>, Copyable<T>, Equal<T>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  // FIXME: It's probably worthwhile to consider a refinement of Boolean, say
  // Bool that does require evaluation in a Boolean context.

  /**
   * @ingroup concepts
   * A boolean type is one that can participate in Boolean expressions.
   * Specifically, a Boolean type must be negatable, logically and'd and
   * logically or'd. This concept requires two further binary operators:
   * implies and iff.
   *
   * Note that the Boolean concept does not require the type to be convertible
   * to bool. This allows a type to behave as a boolean, but not to require
   * evaluation in boolean contexts (e.g., if conditions).
   */
  template<typename T>
  struct Boolean
  {
    Boolean()
    { auto p = constraints; }

    static void constraints(T x, T y)
    {
      !x;
      x && y;
      x || y;
    }

    // FIXME: Axioms! It's generally going to be the case that the Boolean
    // concept also defines a Boolean algebra. However, we can't reliably
    // enforce these constraints

    typedef std::tuple<
      has_logical_not<T>,
      has_logical_and<T, T>,
      has_logical_or<T, T>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  /**
   * The Bool concept is a refinement of Boolean that adds the requirement
   * that the type T and the result of all logical operations are implicitly
   * convertible to bool. This allows models to be evaluated in a boolean
   * context.
   */
  template<typename T>
  struct Bool
    : Boolean<T>
  {
    Bool()
    { auto p = constraints; }

    static void constraints(T x, T y)
    {
      if(x);
      if(!x);
      if(x && y);
      if(x || y);
    }

    typedef std::tuple<
      Boolean<T>,
      Convertible<T, bool>,
      Convertible<typename deduce_logical_not<T>::type, bool>,
      Convertible<typename deduce_logical_and<T, T>::type, bool>,
      Convertible<typename deduce_logical_or<T, T>::type, bool>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

} // namespace origin

#endif
