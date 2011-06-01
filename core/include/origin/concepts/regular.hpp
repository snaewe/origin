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
   *  @ingroup concepts_axioms
   * Equality states that, for any values x and y, if x == y, then, for any
   * Predicate p, p(x) == p(y). That is, equal values will yield equal results
   * for any (reasonable) predicate.
   */
  template<typename T, typename Pred>
    bool aEquality(T x, T y, Pred p) 
    {
      return implies(x == y, p(x) == p(y));
    }  
  
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
        cBoolean<decltype(x == y)>{};
        cBoolean<decltype(x != y)>{};
        // aEquivalence_Relation<std::equal_to<T>, T>
        // aEquality<T, Pred>
        // aInequality<T>
      }

      typedef std::tuple<
        tEqual<T, T>
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
    struct cOrdered : cComparable<T>
    {
      cOrdered()
      {
        auto p = constraints;
      }

      static void constraints(T x, T y)
      {
        tLess<T>{};
        cBoolean<decltype(x < y)>{};
        cBoolean<decltype(x > y)>{};
        cBoolean<decltype(x <= y)>{};
        cBoolean<decltype(x >= y)>{};
        // aStrict_Total_Order<std::less<T>, T>
        // aGreater<T>
        // aLess_Equal<T>
        // aGreater_Equal<T>
      }

      typedef std::tuple<
        cComparable<T>, tLess<T>
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
   * Copy equality requires that a copy of an object compare equal to
   * the original.
   */
  template<typename T>
    bool aCopy_Assign_Equality(T x, T& y)
    {
      return (y = x) == x;
    }

  /**
   * The "can destroy" predicate evaluates the post-condition of a move
   * operation. The moved from object is in a valid, but unspecified state, 
   * which effectively means that the only reasonable operations is 
   * destruction.
   */
  template<typename T>
    bool can_destroy(T& x)
    { return true; }
    
  /**
   * After moving, the object x can be destroyed. Any other operations on
   * x may result in undefined behavior. Note that if T is assignable, then
   * x may also be assigned a new value.
   */
  template<typename T>
    bool aMove_Effect(T x, T y) 
    {
      return implies(x == y, T{std::move(x)} == y && can_destroy(x));
    }
    
  /**
   * After moving, the object x can be destroyed. Any other operations on
   * x may result in undefined behavior. Note that if T is assignable, then
   * x may also be assigned a new value.
   */
  template<typename T>
    bool aMove_Assign_Effect(T x, T y, T& z) 
    {
      return implies(x == y, (z = std::move(x)) == y && can_destroy(x));
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
        // aCopy_Equality<T>
      }

      // FIXME: Use std::has_copy_constructor and std::has_copy_assign when they
      // become available.
      typedef std::tuple<
        cComparable<T>, 
        tConstructible<T>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  
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
        // aMove_Effect<T>
      }

      typedef std::tuple<
        tConstructible<T, T&&>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };  

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
        // aCopy_Assign_Equality<T>
        // aMove_Assign_Effect<T>
      }

      typedef std::tuple<
        cCopyable<T>, cMoveable<T>,
        tConstructible<T>, tAssignable<T, T const&>, tAssignable<T, T&&>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };


  /**
   * @ingroup concepts
   * A Boolean type is conceptually a Regular, Ordered type that participates 
   * in Boolean expressions: it  must be convertible to and constructible over 
   * bool types, and support the operations &&, ||, and !.
   *
   * Note that Boolean types do not specifically refine Regular, although it
   * does require exactly the same constraints. The specification is decoupled
   * in order to prevent cyclic definitions. Rather than requiring the results
   * of operations to model the Boolean concept, they must be convertible to 
   * the type argument, T. In other words, operations on a Boolean type may not
   * transform the type. 
   */
  template<typename T>
    struct cBoolean
    {
      cBoolean()
      {
        auto p = constraints;
      }

      static void constraints(T x, T y, T z)
      {
        tDestructible<T>{};
        tConstructible<T>{};

        // Copy and Move
        tConstructible<T, T const&>{};
        tConstructible<T, T&&>{};
        tAssignable<T, T const&>{};
        tAssignable<T, T&&>{};
        
        // Equality and Order.
        tEqual<T>{};
        tConvertible<decltype(x == y), T>{};
        tLess<T>{};
        tConvertible<decltype(x < y), T>{};

        // Logical operations
        tLogical_And<T>{};
        tLogical_Or<T>{};
        tLogical_Not<T>{};

        // aEquivalence_Relation<std::equal_to<T>, T>
        // aEquality<T, Pred>
        // aInequality<T>

        // aStrict_Total_Order<std::less<T>, T>
        // aGreater<T>
        // aLess_Equal<T>
        // aGreater_Equal<T>

        // cCopy_Equality<T>
        // cMove_Effect<T>
        // cCopy_Assign_Equality<T>
        // cMove_Assign_Effect<T>
        
        // aBoolean_Algebra<std::logical_and<T>, 
        //                 std::logical_or<T>, 
        //                 std::logical_not<T>,
        //                 truth<T>, falsity<T>, T>;
      }

      typedef std::tuple<
        tDestructible<T>,
        tConstructible<T>,
        tConstructible<T, T const&>,
        tConstructible<T, T&&>,
        tAssignable<T, T const&>,
        tAssignable<T, T&&>,
        tEqual<T>,
        tConvertible<typename tEqual<T>::result_type, T>,
        tLess<T>,
        tConvertible<typename tLess<T>::result_type, T>,
        tLogical_And<T>,
        tLogical_Or<T>,
        tLogical_Not<T>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };
    
  /**
   * @ingroup concepts
   * An Integral type a type that is typified by integers. That is, it is an
   * arithmetic type (supporting the arithmetic operators) and modulus. That
   * is, subtraction is the additive inverse, and division is the multiplicative
   * inverse. The modulus operation returns the remainder of division. The 
   * number 1 is the additive identity and the number 0 is the multiplicative 
   * identity. Integral types do not have a 0 divisor. Note that Integral types 
   * can be constructed over integral values, supporting initialization of
   * the identity values.
   * 
   * The semantics of Integral types are formally defined by the Euclidean 
   * Domain axiom.
   */
  template<typename T>
    struct cIntegral
      : cRegular<T>
    {
      cIntegral()
      {
        auto p = constraints;
      }

      static void constraints(T x, T y)
      {
        tConstructible<T, long long>{};
        
        tPlus<T>{};
        tMinus<T>{};
        tMultiply<T>{};
        tDivide<T>{};
        tModulus<T>{};

        // FIXME: Implement Euclidean Domain (and all the the other algebraic
        // concepts contributing to that definition).
      }

      typedef std::tuple<
        cRegular<T>,
        tConstructible<T, long, long>,
        tPlus<T>,
        tMinus<T>,
        tMultiply<T>,
        tDivide<T>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };
    
} // namespace origin

#endif
