// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONCEPTS_REGULAR_HPP
#define ORIGIN_CONCEPTS_REGULAR_HPP

#include <functional>

#include <origin/concepts/traits.hpp>
#include <origin/concepts/algebraic.hpp>

namespace origin
{
  /**
   * @defgroup concepts_regular
   * @ingroup concepts
   *
   * Concepts in this group address fundamental notions of regularity: a
   * fundamental set of operations supporting the value-oriented programming
   * model of most generic libraries.
   */

  /**
   * @ingroup concepts_axioms
   *
   * The Inequality axiom semantically connects the meaning of operator == to
   * operator !=.
   */
  template<typename T>
  bool aInequality(T x, T y)
  {
    return (x != y) == !(x == y);
  }

  /**
   * @ingroup concepts_regular
   *
   * The cComparable concept is valid for types that can be compared for
   * equivalence using the operators == and !=. Note that != is not explicitly
   * required, but is always is the set of valid expressions for Equal types.
   * Operator == must model an Equivalence_relation, and operator != must be
   * the negation of ==.
   */
  template<typename T>
  struct cComparable
  {
    cComparable()
    {
      auto p = constraints;
    }

    static void constraints(T x, T y)
    {
      tEqual<T, T>{};
      cEquivalence_Relation<std::equal_to<T>, T, T>{};
      aInequality(x, y);
    }

    typedef std::tuple<
      tEqual<T, T>,
      cEquivalence_Relation<std::equal_to<T>, T, T>
    > requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts_axiom
   *
   * Copy equivalence requires that a copy of an object compare equal to
   * the original.
   */
  template<typename T>
  bool aCopy_Equivalence(T x)
  {
    return T{x} == x;
  }

  /**
   * @ingroup concepts_regular
   *
   * The Copyable concept is valid for Comparable types that can be copy
   * constructed. Copyable type exhibit the Copy_equivalence property: that
   * a copy of an object is equivalent to the original.
   */
  template<typename T>
  struct cCopyable
    : cComparable<T>
  {
    cCopyable()
    {
      auto p = constraints;
    }

    static void constraints(T x)
    {
      tCopyable<T>{};
      aCopy_Equivalence(x);
    }

    // FIXME: Use std::has_copy_constructor and std::has_copy_assign when they
    // become available.
    typedef std::tuple<
      cComparable<T>, tCopyable<T>
    > requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts_regular
   *
   * The regular concept is valid for Copyable types that can be default
   * constructed.
   */
  template<typename T>
  struct cRegular
    : cCopyable<T>
  {
    cRegular()
    {
      tDefault<T>{};
    }

    typedef std::tuple<
      cCopyable<T>, tDefault<T>
    > requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concpepts_axioms
   *
   * The Greater axiom semantically connects the meaning of operator > to
   * operator <.
   */
  template<typename T>
  bool aGreater(T x, T y)
  { return (x > y) == (y < x); }

  /**
   * @ingroup concpepts_axioms
   *
   * The Less_Equal axiom semantically connects the meaning of operator <= to
   * operator <.
   */
  template<typename T>
  bool aLess_Equal(T x, T y)
  { return (x <= y) == !(y < x); }

  /**
   * @ingroup concpepts_axioms
   *
   * The Greater_Equal axiom semantically connects the meaning of operator >=
   * to operator <.
   */
  template<typename T>
  bool aGreater_Equal(T x, T y)
  { return (x >= y) == !(x < y); }

  /**
   * @ingroup concepts_regular
   *
   * The Ordered concept is valid for Regular types that can be orderd using
   * the relational operators <, >, <=, and >=. Note that the operators >,
   * <=, and >= are not explicitly required, but implicitly derived for any
   * type satisfying the Less requirements. The operator < is required to
   * model a Strict_Weak_Order. The meaning of these operators are defined by
   * the axioms Greater, Less_Equal, and Greater_Equal.
   */
  template<typename T>
  struct cOrdered
    : cRegular<T>
  {
    cOrdered()
    {
      auto p = constraints;
    }

    static void constraints(T x, T y)
    {
      tLess<T>{};
      cStrict_Weak_Order<std::less<T>, T, T>{};

      aGreater(x, y);
      aLess_Equal(x, y);
      aGreater_Equal(x, y);
    }

    typedef std::tuple<
      cRegular<T>, tLess<T>
    > requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

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
  struct cBoolean
  {
    cBoolean()
    {
    }

    static void constraints(T x, T y)
    {
      tLogical_And<T>{};
      tLogical_Or<T>{};
      tLogical_Not<T>{};
    }

    // FIXME: Axioms! It's generally going to be the case that the Boolean
    // concept also defines a Boolean algebra. However, we can't reliably
    // enforce these constraints

    typedef std::tuple<
      tLogical_And<T>,
      tLogical_Or<T>,
      tLogical_Not<T>
    > requirements;
    typedef typename requires_all<requirements>::type type;
    static constexpr bool value = type::value;
  };

  template<> struct cBoolean<bool> : std::true_type { };

} // namespace origin

#endif
