// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONCEPTS_REGULAR_HPP
#define ORIGIN_CONCEPTS_REGULAR_HPP

#include <functional>

#include <origin/concepts/impl.hpp>
#include <origin/concepts/operator.hpp>

namespace origin
{
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
