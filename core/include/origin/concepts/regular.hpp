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
   * @name Equal
   * Concept checking classes and axioms for equality comparability.
   */
  //@{
  // FIXME: What do we REALLY know, or what can we really say about different
  // types compared with the equality comparison operator. For example, does
  // Equal<T, U> imply Equal<U, T>?
  /**
   * @class Equal<T, U>
   * @ingroup concepts
   * When instantiated over two different types, the concept determines whether
   * or not the two types can be compared using the relational equality
   * operators (== and !=).
   */
  template<typename T, typename U>
  struct Equal
  {
    Equal()
    { auto p = constraints; }

    static void constraints(T x, U y)
    {
      Boolean<decltype(x == y)>{};
      Boolean<decltype(y == x)>{};
    }

    typedef std::tuple<
      has_equal<T, U>, has_equal<U, T>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  /**
   * @fn operator!=(x, y)
   * Provide a default implementation of operator!= for all types T and U
   * such that Equal<T, U> is a valid model.
   */
  template<typename T, typename U>
  auto operator!=(T const& x, U const& y)
    -> typename std::enable_if<Equal<T, U>::value, decltype(!(x == y))>::type
  { return !(x == y); }

  /**
   * @class Equal<T>
   * @ingroup concepts
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

    static void constraints(T x, T y)
    { Boolean<decltype(x == y)>{}; }

    typedef std::tuple<
      has_equal<T, T>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  /**
   * @fn operator!=(x, y)
   * Provide a default implementation of operator!= for all types T such that
   * Equal<T> is true.
   */
  template<typename T>
  auto operator!=(T const& x, T const& y)
    -> typename std::enable_if<Equal<T>::value, decltype(!(x == y))>::type
  { return !(x == y); }

  /** @name Axioms */
  //@{
  // FIXME: This cannot be true for volatile T, but could be for const
  // volatile? Not sure...
  /**
   * Two objects that are in actuality the same object, are equivalent. Said
   * otherwise, an object is always the same as itself.
   */
  template<typename T>
  auto Equal_Identity(T const& x, T const& y)
    -> decltype(implies(std::addressof(x) == std::addressof(y), x == y))
  { return implies(std::addressof(x) == std::addressof(y), x == y); }

  /**
   * The equality comparison operator is reflexive.
   */
  template<typename T>
  auto Equal_Reflexive(T x)
    -> decltype(x == x)
  { return x == x; }

  /**
   * The equality comparison operatopr is symmetric.
   */
  template<typename T>
  auto Equal_Symmetric(T x, T y)
    -> decltype(implies(x == y, y == x))
  { return implies(x == y, y == x); }

  /**
   * The equality comparison operator is transitive.
   */
  template<typename T>
  auto Equal_Transitive(T x, T y, T z)
    -> decltype(implies(x == y && y == z, x == z))
  { return implies(x == y && y == z, x == z); }

  /**
   * Connect the equality operator to the inequality operator. If x == y, then
   * it is not the cast that x != y.
   */
  template<typename T>
  auto Equal_Not_Equal(T x, T y)
    -> decltype(implies(x == y, !!(x == y)))
  { return implies(x == y, !(x != y)); }
  //@}
  //@}

  /** @name Ordered */
  //@{
  /**
   * @class Ordered<T, U>
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
      Boolean<decltype(x < y)>{};
      Boolean<decltype(y < x)>{};
    }

    typedef std::tuple<
      has_less<T, U>, has_less<U, T>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  /**
   * @fn operator>(x, y)
   * Provide a default implementation of operator> for all types T and U
   * such that Ordered<T, U> is a valid model.
   */
  template<typename T, typename U>
  auto operator>(T const& x, U const& y)
    -> typename std::enable_if<Ordered<T, U>::value, decltype(y < x)>::type
  { return y < x; }

  /**
   * @fn operator<=(x, y)
   * Provide a default implementation of operator<= for all types T and U
   * such that Ordered<T, U> is a valid model.
   */
  template<typename T, typename U>
  auto operator<=(T const& x, U const& y)
    -> typename std::enable_if<Ordered<T, U>::value, decltype(!(x < y))>::type
  { return !(x < y); }

  /**
   * @fn operator>=(x, y)
   * Provide a default implementation of operator>= for all types T and U
   * such that Ordered<T, U> is a valid model.
   */
  template<typename T, typename U>
  auto operator<=(T const& x, U const& y)
    -> typename std::enable_if<Ordered<T, U>::value, decltype(!(y < x))>::type
  { return !(x < y); }

  /**
   * @class Ordered<T>
   */
  template<typename T>
  struct Ordered<T, T>
  {
    Ordered()
    { auto p = constraints; }

    static void constraints(T x, T y)
    { Boolean<decltype(x < y)>{}; }

    typedef std::tuple<
      has_less<T, T>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  /** @name Defaults */
  //@{
  /**
   * @fn operator>(x, y)
   * Provide a default implementation of operator> for all types T such that
   * that Ordered<T> is a valid model.
   */
  template<typename T>
  auto operator>(T const& x, T const& y)
    -> typename std::enable_if<Ordered<T>::value, decltype(y < x)>::type
  { return y < x; }

  /**
   * @fn operator<=(x, y)
   * Provide a default implementation of operator<= for all types T such that
   * Ordered<T, U> is a valid model.
   */
  template<typename T>
  auto operator<=(T const& x, T const& y)
    -> typename std::enable_if<Ordered<T>::value, decltype(!(x < y))>::type
  { return !(x < y); }

  /**
   * @fn operator>=(x, y)
   * Provide a default implementation of operator>= for all types T such that
   * Ordered<T, U> is a valid model.
   */
  template<typename T>
  auto operator<=(T const& x, T const& y)
    -> typename std::enable_if<Ordered<T>::value, decltype(!(y < x))>::type
  { return !(y < x); }
  //@}

  /** @name Axioms */
  //@{
  template<typename T>
  auto Ordered_Irreflexive(T x)
    -> decltype(!(x < x))
  { return !(x < x); }

  // FIXME: Requires Boolean<T>
  template<typename T>
  auto Ordered_Antisymmetric(T x, T y)
    -> decltype(implies(x < y, !(y < x)))
  { return implies(x < y, !(y < x)); }

  //  FIXME: Requires Boolean<T>
  template<typename T>
  auto Ordered_Transitive(T x, T y, T z)
    -> decltype(implies(x < y && y < z, x == z))
  { return implies(x < y && y < z, x == z); }
  //@}
  //@}


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
