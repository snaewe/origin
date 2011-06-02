// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONCEPTS_REGULAR_HPP
#define ORIGIN_CONCEPTS_REGULAR_HPP

#include <origin/functional.hpp>
#include <origin/concepts/constraints.hpp>

namespace origin
{
  /**
   * @defgroup concepts
   */
  
  /**
   * @defgroup axioms
   * @ingroup concepts
   * Axioms define the semantic, invariant properties of concepts.
   */
  
  /**
   * @defgroup algebra
   * @ingroup concepts
   * Algebraic axioms define semantic connections between operations and
   * values that are needed to define coherent domains (types).
   */
  
  /**
   * @defgroup func_concepts
   * @ingroup concepts
   *
   * Function concepts are constraints that define semantics functions,
   * function objects, and lambda expressions.
   */

  // Function Concepts
  template<typename F, typename... Args> struct cFunction;
  template<typename F, typename... Args> struct cOperation;
  template<typename F, typename... Args> struct cPredicate;
  template<typename F, typename T, typename U> struct cRelation;
  
  // Regular Concepts
  template<typename T> struct cComparable;
  template<typename T> struct cCopyable;
  template<typename T> struct cMoveable;
  template<typename T> struct cRegular;
  template<typename T> struct cOrdered;

  // Type Concepts
  template<typename T> struct cBoolean;
  template<typename T> struct Integral;
  
  /**
   * @ingroup axioms
   * Application equivalence requires that a function applied to equivalent
   * arguments produce equivalent results.
   */
  template<typename F, typename... Args>
    bool aApplication_Equality(F f, std::tuple<Args...> x, std::tuple<Args...> y)
    {
      return implies(x == y, tuple_invoke(f, x) == tuple_invoke(f, y));
    }

  /**
   * @ingroup func_concepts
   *
   * The Function concept is valid for Procedures that return a value and
   * satisfy the Application_Equality requirements.
   */
  template<typename Func, typename... Args>
    struct cFunction
    {
      typedef typename std::result_of<Func(Args...)>::type result_type;
      
      cFunction()
      {
        auto p = constriants;
      }

      static void constriants(Func func,
                              std::tuple<Args...> x,
                              std::tuple<Args...> y)
      {
        tProcedure<Func, Args...>{};
        tDifferent<result_type, void>{};
        // aApplication_Equality<Func, Args...>
      }

      typedef std::tuple<
        tProcedure<Func, Args...>, tDifferent<result_type, void>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  /**
   * @ingroup func_concepts
   *
   * The Operation concept is valid for Functions that are called over a
   * homogenous sequence of arguments; the sequence of arguments must share
   * a Common type.
   */
  template<typename Op, typename... Args>
    struct cOperation : cFunction<Op, Args...>
    {
      typedef typename cFunction<Op, Args...>::result_type result_type;
      typedef typename tCommon<Args...>::result_type domain_type;

      cOperation()
      {
        auto p = constraints;
      }
      
      static void constraints()
      {
        tCommon<Args...>{};
        tConvertible<domain_type, result_type>{};
      }

      typedef std::tuple<
        cFunction<Op, Args...>,
        tCommon<Args...>,
        tConvertible<domain_type, result_type>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  /**
   * @ingroup func_concepts
   *
   * A Predicate is a Function whose result type is Boolean.
   */
  template<typename Pred, typename... Args>
    struct cPredicate : cFunction<Pred, Args...>
    {
      typedef typename cFunction<Pred, Args...>::result_type result_type;

      cPredicate()
      {
        auto p = constraints;
      }

      static void constraints(Pred p,
                              std::tuple<Args...> x,
                              std::tuple<Args...> y)
      {
        cBoolean<result_type>{};
      }

      typedef std::tuple<
        cFunction<Pred, Args...>, cBoolean<result_type>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  /**
   * @ingroup func_concepts
   *
   * The Relation concept is valid for binary Predicates called over a pair
   * of homogenous, regular arguments; the arguments must share a common type.
   * 
   * Note that generalized n-ary relations are not addressed by this concept,
   * as they are more relevant to libaries implementing aspects of relational
   * data structures and algorithms.
   */
  template<typename Rel, typename T, typename U>
    struct cRelation : cPredicate<Rel, T, U>
    {
      typedef typename cPredicate<Rel, T, U>::result_type result_type;
      typedef typename tCommon<T, U>::result_type domain_type;

      cRelation()
      {
        auto p = constraints;
      }
      
      static void constraints()
      {
        tCommon<T, U>{};
      }

      typedef std::tuple<
        cPredicate<Rel, T, U>, tCommon<T, U>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;    };
    
  /**
   * @ingroup axioms
   * 
   * A relation is Reflexive if, for any object x, r(x, x) is true. Examples
   * include equality, equivalence, and non-strict orders.
   */
  template<typename Rel, typename T>
    bool aReflexive(Rel r, T x)
    {
      return r(x, x);
    }

  /**
   * @ingroup axioms
   * 
   * A relation is Irreflexive if, for any object x, r(x, x) is false, or
   * equivalently that !r(x, x) is true. Examples include strict orders and
   * the strict subset relation.
   */
  template<typename Rel, typename T>
  bool aIrreflexive(Rel r, T x)
    {
      return !r(x, x);
    }

  /**
   * @ingroup axioms
   * 
   * A relation is symmetric if, for any objects x and y, r(x, y) implies
   * r(y, x). Examples include equality, equivalence, and non-strict orders.
   */
  template<typename Rel, typename T>
  bool aSymmetric(Rel r, T x, T y)
    {
      return implies(r(x, y), r(y, x));
    }

  /**
   * @ingroup axioms
   * 
   * A relation is asymmetric if, for any objects x and y, r(x, y) implies
   * !r(y, x). Examples include strict orders (if x < y, it cannot be the
   * case that y < x).
   */
  template<typename Rel, typename T>
    bool aAsymmetric(Rel r, T x, T y)
    {
      return implies(r(x, y), !r(y, x));
    }

  /**
   * @ingroup axioms
   * 
   * A relation is transitive if...
   */
  template<typename Rel, typename T>
    bool aTransitive(Rel r, T x, T y, T z)
    {
      return implies(r(x, y) && r(y, z), r(x, y));
    }

  /**
   * @ingroup axioms
   * 
   * A relation is total if, for any objects x and y, it is the case that 
   * either r(x, y) is true or r(y, x) is true. Counterexamples include
   * partial equivalences and partial orders.
   */
  template<typename Rel, typename T>
    bool aTotal(Rel r, T x, T y)
    {
      return r(x, y) || r(y, x);
    }

  /**
   * @ingroup axioms
   * 
   * A relation r is antisymmetric if, for any arguments x and y, r(x, y) 
   * and r(y, x) are both true, then x == y. The operator <= on real numbers is 
   * antisymmetric, as is the subset operator for sets.
   */
  template<typename Rel, typename T>
    bool aAntisymmetry(Rel r, T x, T y)
    {
      return implies(r(x, y) && r(y, x), x == y);
    }
  
  /**
   * @ingroup axioms
   * 
   * A relation r is trichotomous if, for the arguments x and y, exactly on
   * of the following be true: r(x, y), r(y, x), or x == y. 
   */
  template<typename Rel, typename T>
    bool aTrichotomy(Rel r, T x, T y) 
    {
      return implies(r(x, y), !r(y, x) && x != y) 
          || implies(r(y, x), !r(y, x) && x != y) 
          || implies(x == y, !r(x, y) && !r(y, x));
    }

  /**
   * @ingroup axioms
   *
   * An Equivalence Relation is a semantic refinement of a Relation that is
   * Reflexive, Symmetric, and Transitive.
   */
  template<typename Rel, typename T>
    bool aEquivalence_Relation(Rel r, T x, T y, T z)
    {
      return aReflexive(r, x)
          && aSymmetric(r, x, y)
          && aTransitive(r, x, y, z);
    }
  
  /**
   * @ingroup axioms
   *
   * A Strict Partial Order is a semantic refinement of a Relation that is
   * Irreflexive, Asymmetric, and Transtive. IEEE 754 floating point numbers 
   * are partially ordered by the built in operator< since comparing against
   * NaN returns false for every case.
   */
  template<typename Rel, typename T>
    bool aStrict_Partial_Order(Rel r, T x, T y, T z) 
    {
      return aIrreflexive(r, x)
          && aAsymmetric(r, x, y)
          && aTransitive(r, x, y, z);
    }

  /**
   * @ingroup axioms
   *
   * A Strict Weak Order is a semantic refinement of a Strict Partial Order 
   * that requires incomaprability, the case where neither !r(x, y) nor
   * !r(y, x) is true, to be transitive.
   */
  template<typename Rel, typename T>
    bool aStrict_Weak_Order(Rel r, T x, T y, T z)
    {
      return aTransitive(incomparable_to<T, Rel>{r}, x, y, z); 
    }

  /**
   * @ingroup axioms
   *
   * A Strict Total Order is a semantic refinement of a Strict Weak Order 
   * in which incomaprability is the same as equality, or equivalently that 
   * the relation is trichotomous. That is, for any arguments x and y, exactly
   * one of the following is true: r(x, y), r(y, x), or x == y.
   */
  template<typename Rel, typename T>
    bool aStrict_Total_Order(Rel r, T x, T y, T z)
    {
      // Note that transitvity of this operation is required by the strict
      // weak order refinement.
      return aTrichotomy(r, x, y); 
    }
    
  /**
   * @ingroup axioms
   *
   * An Operation is associtive if the order of evaluation does not affect the 
   * result.
   */
  template<typename Op, typename T>
    bool aAssociative(Op op, T x, T y, T z)
    {
      return op(x, op(y, z)) == op(op(x, y), z);
    }

  /**
   * @ingroup axioms
   *
   * An Operation is commutative if ordering of operands does not affect the 
   * result.
   */
  template<typename Op, typename T>
    bool aCommutative(Op op, T x, T y)
    {
      return op(x, y) == op(y, x);
    }

  /**
   * @ingroup axioms
   *
   * The absoprtion law (or identity) connects a apir of binary Operations such
   * that, for Operations op1 and op2 and objects x and  y:
   * op1(x, op2(x, y)) == x.
   */
  template<typename Op1, typename Op2, typename T>
    bool aAbsorption(Op1 op1, Op2 op2, T x, T y)
    {
      return op1(x, op2(x, y)) == x;
    }

  /**
   * @ingroup axioms
   *
   * The distributive property connects two binary Operations such that, for
   * Operations op1 and op2 and objects x and y, 
   * op1(x, op2(y, z)) == op2(op1(x, y), op1(x, z))
   */
  template<typename Op1, typename Op2, typename T>
    bool aDistributive(Op1 op1, Op2 op2, T x, T y, T z)
    {
      return op1(x, op2(y, z)) == op2(op1(x, y), op1(x, z));
    }

  /**
   * @ingroup axioms
   *
   * The complement identity connects a binary Operation with a unary Operation
   * (the complement) and a value (a constant Function) in the domain such that 
   * op(x, comp(x)) == v.
   */
  template<typename Op, typename Comp, typename Value, typename T>
    bool aComplement(Op op, Comp comp, Value val, T x)
    {
      return op(x, comp(x)) == val();
    }

  /**
   * @ingroup algebra
   *
   * The Boolean_algebra concept describes the semantic requirements of a
   * boolean algebra defined over a Regular type T, two binary Operations
   * (called "and" and "or"), a unary Operation (called "not"), and two
   * constant Functions (called "true" and "false").
   */
  template<typename And,
           typename Or,
           typename Not,
           typename True,
           typename False,
           typename T>
    bool aBoolean_Algebra(And a, Or o, Not n, True t, False f, T x, T y, T z)
    {
      return aAssociative(a, x, y, z)     // x and (y and z) == (x and y) and z
          && aAssociative(o, x, y, z)     // x or (y or z) == (x or y) or z
          && aCommutative(a, x, y)        // x and y == y and x
          && aCommutative(o, x, y)        // x or y == y or x
          && aAbsorption(a, o, x, y)      // x and (x or y) == x
          && aAbsoprtion(o, a, x, y)      // x or (x and y) == x
          && aDistributive(a, o, x, y, z) // x and (y or z) == (x and y) or (y and z)
          && aDistributuve(o, a, x, y, z) // x or (y and z) == (x or y) and (y or z)
          && aComplement(a, n, f, x)      // x and not x == f
          && aComplement(o, n, t, x);     // x or not x == t
    }

  /**
   * @defgroup regular_concepts
   * @ingroup concepts
   *
   * Concepts in this group address fundamental notions of regularity: a
   * fundamental set of operations supporting the value-oriented programming
   * model of most generic libraries.
   */

  /**
   *  @ingroup axioms
   * 
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
   * @ingroup axioms
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
   * @ingroup regular_concepts
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
   * @ingroup axioms
   * 
   * The Greater axiom semantically connects the meaning of operator > to
   * operator <.
   */
  template<typename T>
    bool aGreater(T x, T y)
    { 
      return (x > y) == (y < x); 
    }

  /**
   * @ingroup axioms
   * 
   * The Less_Equal axiom semantically connects the meaning of operator <= to
   * operator <.
   */
  template<typename T>
    bool aLess_Equal(T x, T y)
    {
      return (x <= y) == !(y < x); 
    }

  /**
   * @ingroup axioms
   * 
   * The Greater_Equal axiom semantically connects the meaning of operator >=
   * to operator <.
   */
  template<typename T>
    bool aGreater_Equal(T x, T y)
    { 
      return (x >= y) == !(x < y); 
    }

  /**
   * @ingroup regular_concepts
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
   * @ingroup axioms
   * 
   * Copy equivalence requires that a copy of an object compare equal to
   * the original.
   */
  template<typename T>
    bool aCopy_Equality(T x)
    {
      return T{x} == x;
    }

  /**
   * @ingroup axioms
   * 
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
   * @ingroup axioms
   * 
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
   * @ingroup axioms
   * 
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
   * @ingroup regular_concepts
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
   * @ingroup regular_concepts
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
   * @ingroup regular_concepts
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
   * @ingroup type_concepts
   * 
   * A Boolean type is conceptually a Regular, Ordered type that participates 
   * in Boolean expressions: it  must be convertible to and constructible over 
   * bool types, and support the operations &&, ||, and !.
   *
   * Note that Boolean types do not specifically refine Regular, although it
   * does require exactly the same constraints and axioms. The specification is 
   * decoupled in order to prevent cyclic definitions. Rather than requiring 
   * the results of operations to model the Boolean concept, they must be 
   * convertible to  the type argument, T. In other words, operations on a 
   * Boolean type may not transform the type. 
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
   * @ingroup type_concepts
   * 
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
        tConvertible<decltype(x + y), T>{};
        tMinus<T>{};
        tConvertible<decltype(x - y), T>{};
        tMultiply<T>{};
        tConvertible<decltype(x * y), T>{};
        tDivide<T>{};
        tConvertible<decltype(x / y), T>{};
        tModulus<T>{};
        tConvertible<decltype(x % y), T>{};

        // FIXME: Implement Euclidean Domain (and all the the other algebraic
        // concepts contributing to that definition).
      }

      typedef std::tuple<
        cRegular<T>,
        tConstructible<T, long, long>,
        tPlus<T>,
        tConvertible<typename tPlus<T>::result_type, T>,
        tMinus<T>,
        tConvertible<typename tMinus<T>::result_type, T>,
        tMultiply<T>,
        tConvertible<typename tMultiply<T>::result_type, T>,
        tDivide<T>,
        tConvertible<typename tDivide<T>::result_type, T>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };
    
} // namespace origin

#endif
