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
  // NOTE: Where are all of the semantic requirements? Where are the
  // restrictions on result types?
  //
  // It turns out that restricting result types or enforcing requirements do
  // not make sufficiently generic concepts. Consider the extreme case where
  // the argument types of a required operation are expression templates. It
  // is unlikely that the results will satisfy any type restrictions or semantic
  // requirements.

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

    // FIXME: There are some axioms here... I know it!

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
      x == y; x != y;
    }

    // FIXME: Only valid if convertible<decltype(x == y), bool>
    static void equal_identity(T x, T y)
    { if(&x == &y) assert(( x == y )); }

    static void reflexive(T x)
    { assert(( x == x )); }

    static void symmetric(T x, T y)
    { if(x == y) assert(( y == x )); }

    static void transitive(T x, T y, T z)
    { if(x == y && y == z) assert(( x == z )); }

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

    // FIXME: Axioms here!

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
      x < y; x > y; x <= y; y <= x;
    }

    static void irreflexive(T x)
    { assert(( !(x < x) )); }

    static void antisymmetric(T x)
    { if(x < y) assert(( !(y < x) )); }

    static void transitive(T x, T y, T z)
    { if(x == y && y == z) assert(( x == z )); }

    typedef std::tuple<
      has_less<T, T>, has_greater<T, T>,
      has_less_equal<T, T>, has_greater_equal<T, T>,
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

    typedef std::tuple<
    > type;
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

    // FIXME: Only valid if Equal_Bool<T>
    void copy_equivalence(T x)
    { T y{x}; assert((x == y)); }

    typedef std::tuple<
    > type;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  // FIXME: Implement me!
  /**
   * A regular type...
   */
  template<typename T>
  struct Regular
  {
    Regular()
    { auto p = constraints; }

    static void constraints()
    {
      Equal<T>{};
    }

    typedef std::tuple<
//       Equal<T>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  // The difference between a boolean type and a type (and operations) that
  // model a boolean algebra is the ability to evaluate a boolean type in a
  // boolean context (i.e., if(x)).
  /**
   * A boolean type is a type that can be evaluated in boolean contexts and
   * participate in logical expresssions.
   */
  template<typename T>
  struct Boolean
    : Regular<T>
  {
    Boolean()
    { auto p = constraints; }

    static void constraints(T x, T y)
    {
      if(x);
      !x;
      x && y;
      x || y;
    }

    typedef std::tuple<
      Regular<T>,
      std::is_convertible<T, bool>,
      has_logical_not<T>,
      has_logical_and<T, T>,
      has_logical_or<T, T>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  // Make sure that we have some built in models of Boolean types. Note that
  // if we don't have this, we get recursive template instantiation.
  template<> struct Boolean<bool> : std::true_type { };

} // namespace origin

#endif
