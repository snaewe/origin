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
#include <origin/concepts/relation.hpp>
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
        aEquivalence_Relation<std::equal_to<T>, T>{};
        aInequality(x, y);
      }

      typedef std::tuple<
        tEqual<T, T>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  /**
   * Copy equivalence requires that a copy of an object compare equal to
   * the original.
   */
  template<typename T>
    bool aCopy_Equality(T x)
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
    {
      cCopyable()
      {
        auto p = constraints;
      }

      static void constraints(T x)
      {
        cComparable<T>{};
        tConstructible<T, T const&>{};
        aCopy_Equality(x);
      }

      // FIXME: Use std::has_copy_constructor and std::has_copy_assign when they
      // become available.
      typedef std::tuple<
        cComparable<T>, tConstructible<T>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  /**
   * After moving, the object x can be destroyed. Any other operations on
   * x may result in undefined behavior. Note that if T is assignable, then
   * x may also be assigned a new value.
   */
  template<typename T>
    bool Move_effect(T x) 
    {
      T y{std::move(x)};
      return true;
    }
  
  /**
   * @ingroup concepts_regular
   *
   * The Moveable concept is valid for types that can be move constructed.
   * After being moved, any operations other than destruction and assignment
   * may result in undefined behavior.
   */
  template<typename T>
    struct cMoveable
    {
      cMoveable()
      {
        auto p = constraints;
      }

      static void constraints(T x)
      {
        tConstructible<T, T&&>{};
        aMove_Effect(x);
      }

      typedef std::tuple<
        tConstructible<T, T&&>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };  
  
  /**
   * Moving an object transfers the state of a source object (x) into a target
   * object. After the move, the target is equal to a previous copy of x.
   */
  template<typename T>
    bool aMove_semantics(T x) 
    {
      T y{x};             // x == y
      T z{std::move(x)};  // z == y && x is destructible
      return z == y;
    }
  
  /**
   * @ingroup concepts_regular
   *
   * The regular concept is valid for Copyable types that can be default
   * constructed, copy assigned, and move assigned. In essence, a Regular type
   * is required to provide the set of operations make it behave as if it
   * were a built in type.
   */
  template<typename T>
    struct cRegular
    {
      cRegular(T x)
      {
        cCopyable<T>{};
        cMoveable<T>{};
        
        tConstructible<T>{};
        tAssignable<T, T const&>{};
        tAssignable<T, T&&>{};
      
        aMove_semantics(x);
      }

      typedef std::tuple<
        cCopyable<T>, cMoveable<T>,
        tConstructible<T>, tAssignable<T, T const&>, tAssignable<T, T&&>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  /**
   * The Greater axiom semantically connects the meaning of operator > to
   * operator <.
   */
  template<typename T>
    bool aGreater(T x, T y)
    { 
      return (x > y) == (y < x); 
    }

  /**
   * The Less_Equal axiom semantically connects the meaning of operator <= to
   * operator <.
   */
  template<typename T>
    bool aLess_Equal(T x, T y)
    {
      return (x <= y) == !(y < x); 
    }

  /**
   * The Greater_Equal axiom semantically connects the meaning of operator >=
   * to operator <.
   */
  template<typename T>
    bool aGreater_Equal(T x, T y)
    { 
      return (x >= y) == !(x < y); 
    }

  // FIXME: Should this be a total order or not?
  /**
   * @ingroup concepts_regular
   *
   * The Ordered concept is valid for Comparable types that can be orderd using
   * the relational operators <, >, <=, and >=. Note that the operators >,
   * <=, and >= are not explicitly required, but implicitly derived for any
   * type satisfying the Less requirements. The operator < is required to
   * satisfy the semantics of a Strict_Total_Order. The meaning of these 
   * operators are defined by the axioms Greater, Less_Equal, and Greater_Equal.
   */
  template<typename T>
    struct cOrdered
    {
      cOrdered()
      {
        auto p = constraints;
      }

      static void constraints(T x, T y)
      {
        cComparable<T>{};
        tLess<T>{};
        aStrict_Total_Order<std::less<T>, T>{};

        aGreater(x, y);
        aLess_Equal(x, y);
        aGreater_Equal(x, y);
      }

      typedef std::tuple<
        cComparable<T>, tLess<T>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  /**
   * @ingroup concepts
   * A Boolean type is a Regular type participates in Boolean expressions: it 
   * must be convertible to and constructible over bool types, and support the 
   * operations &&, ||, and !.
   *
   * Note that the Boolean concept does not require the type to be convertible
   * to bool. This allows a type to behave as a boolean, but not to require
   * evaluation in boolean contexts (e.g., if conditions).
   */
  template<typename T>
    struct cBoolean
      : cRegular<T>
    {
      cBoolean()
      {
        auto p = constraints;
      }

      static void constraints(T x, T y)
      {
        tLogical_And<T>{};
        tLogical_Or<T>{};
        tLogical_Not<T>{};
        
        aBoolean_Algebra<
          T, 
          std::logical_and<T>, std::logical_or<T>, std::logical_not<T>,
          truth<T>, falsity<T>
        >{};
      }

      typedef std::tuple<
        cRegular<T>,
        tLogical_And<T>,
        tLogical_Or<T>,
        tLogical_Not<T>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  // Prevent cyclic instantiations for (extremely) common argument types.
  template<> struct cBoolean<bool> : std::true_type { };
  template<> struct cBoolean<substitution_failure> : std::false_type{};

} // namespace origin

#endif
