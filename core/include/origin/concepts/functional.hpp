// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONCEPTS_FUNCTION_HPP
#define ORIGIN_CONCEPTS_FUNCTION_HPP

#include <functional>

#include <origin/concepts/impl.hpp>

namespace origin
{

  template<typename Op, typename T>
  struct is_boolean_relation
    : bool_constant<
        is_callable<Op, T, T>::value &&
        std::is_convertible<typename deduce_callable<Op, T, T>::type, bool>::value
      >
  { };

  // The following functions enforce axiomatic properties. We can only actually
  // evaluate these axioms if the subject operations are implicitly convertible
  // to bool. If not, we cannot validate the claim.
  //
  // Note that these are separate from the concept classes that might require
  // them.

  template<typename Op, typename T>
  typename std::enable_if<
    is_boolean_relation<Op, T>::value, void
  >::type
  reflexive_relation(Op op, T x)
  { assert(( op(x, x) )); }

  template<typename Op, typename T>
  typename std::enable_if<
    !is_boolean_relation<Op, T>::value, void
  >::type
  reflexive_relation(Op op, T x)
  { }


  template<typename Op, typename T>
  typename std::enable_if<
    is_boolean_relation<Op, T>::value, void
  >::type
  symmetric_relation(Op op, T x, T y)
  { if(op(x, y)) assert(( op(y, x) )); }

  template<typename Op, typename T>
  typename std::enable_if<
    !is_boolean_relation<Op, T>::value, void
  >::type
  symmetric_relation(Op op, T x, T y)
  { }


  template<typename Op, typename T>
  typename std::enable_if<
    is_boolean_relation<Op, T>::value, void
  >::type
  transitive_relation(Op op, T x, T y, T z)
  { if(op(x, y) && op(y, z)) assert(( op(x, z) )); }

  template<typename Op, typename T>
  typename std::enable_if<
    !is_boolean_relation<Op, T>::value, void
  >::type
  transitive_relation(Op op, T x, T y, T z)
  { }

  /**
   * @ingroup func_concepts
   * A type F is callable over a sequence of argument types Args... if the
   * expression f(args...) is valid where f is of type F and args... is of
   * type Args...
   *
   * Note that for types F and Args..., if Callable<F, Args...> is true, the
   * result_of<F(Args...)> will yield the result type of the expression.
   */
  template<typename F, typename... Args>
  struct Callable
  {
    Callable()
    { auto p = constraints; }

    static void constraints(F f, Args&&... args)
    { f(std::forward<Args>(args)...); }

    typedef std::tuple<
      is_callable<F, Args...>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  // FIXME: I think Symbol might also be a valid name for this concept. In fact,
  // it might be better
  /**
   * @ingroup concepts
   * A constant is a function taking no arguments (a nullary function) and
   * returning the same value. Examples include functions that return 0, 1,
   * pi, true, false, and the empty set.
   *
   * In contrast a generator is a nullary function whose return values may and
   * are generally expected to change.
   *
   * Note that the Constant concept is not related to the language notion of
   * const.
   */
  template<typename F, typename T>
  struct Constant
    : Callable<F>
  {
    Constant()
    { auto p = constraints; }

    static void constraints(F f)
    {
      T{f()};
      constant(f);
    }

    // FIXME: This probably isn't the most accurate way of stating that f()
    // denotes a constant, unchanging value.
    static void constant(F f)
    { assert(( f() == f() )); }
  };

  /**
   * @ingroup func_concepts
   * A relation is a homogenous binary function whose result type is
   * convertible to bool.
   */
  template<typename Op, typename T>
  struct Relation
  {
    Relation()
    { auto p = constraints; }

    static void constraints(Op op, T x, T y)
    {
      // FIXME: GCC was complaining about bool{op(x, y)}, saying that I can't
      // use op as a function. I beg to differ.
      bool result = op(x, y);
    }

    typedef std::tuple<
      Callable<Op, T, T>,
      std::is_convertible<typename deduce_callable<Op, T, T>::type, bool>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };


  /**
   * @ingroup concepts
   * @ingroup unchecked_concepts
   * A relation is reflexive if, for all objects x in the domain of the
   * operation, op(x, x) == true.
   */
  template<typename Op, typename T>
  struct Reflexive_Relation
    : Relation<Op, T>
  {
    Reflexive_Relation()
    { auto p = constraints; }

    static void constraints(Op op, T x)
    { reflexivity(op, x); }

    static void reflexivity(Op op, T x)
    { assert(( op(x, x) )); }
  };

  // NOTE: Symmetry can conceivably be applied to relations on objects of
  // different types.
  /**
   * @ingroup concepts
   * @ingroup unchecked_concepts
   * A relation is symmetric if, for all objects x and y in the domain
   * of the operation, op(x, y) implies op(y, x).
   */
  template<typename Op, typename T>
  struct Symmetric_Relation
    : Relation<Op, T>
  {
    Symmetric_Relation()
    { auto p = constraints; }

    static void constraints(Op op, T x, T y)
    { symmetry(op, x, y); }

    static void symmetry(Op op, T x, T y)
    { if(op(x, y)) assert(( op(y, x) )); }
  };

  /**
   * @ingroup concepts
   * @ingroup unchecked_concepts
   * A binary operation is symmetic if, for all objects x, y, and z in the
   * domaion of the operation, op(x, y) and op(y, z) implies op(x, z).
   */
  template<typename Op, typename T>
  struct Transitive_Relation
    : Relation<Op, T>
  {
    Transitive_Relation()
    { auto p = constraints; }

    static void constraints(Op op, T x, T y, T z)
    { transitivity(op, x, y, z); }

    static void transitivity(Op op, T x, T y, T z)
    { if(op(x, y) && op(y, z)) assert(( op(x, z) )); }
  };


  /**
   * @ingroup func_concepts
   * An operation is a homogenous function whose domain and codomain are the
   * same types. Note that the arity of an operation is determined by the
   * number of arguments that it can be called over.
   */
  template<typename Op, typename... Args>
  struct Operation
  {
    Operation()
    { auto p = constraints; }

    static void constraints(Op op, Args&&... args)
    {
      // This is the weakest possible constraint. The operation must be callable
      // over its argument types.
      Callable<Op, Args...>{};

      // These are very strict type requirements. I'm not sure if they're
      // strictly necessary. Well, they're necessary to be strict, but I'm
      // don't how limiting they'll be on implementations that are otherwise
      // viable.
      static_assert( are_same<Args...>::value,
                     "Argument types are not the same" );
      typedef typename head_type<Args...>::type Arg;
      typedef typename deduce_callable<Op, Args...>::type Result;
      static_assert( std::is_same<Arg, Result>::value,
                     "Result type does not match the argument types" );
    }

    typedef std::tuple<
      Callable<Op, Args...>,
      are_same<Args...>,
      std::is_same<
        typename head_type<Args...>::type,
        typename deduce_callable<Op, Args...>::type
      >
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  // FIXME: The following implicitly require Equal<T>, and possibly Regular<T>.
  // I'm not entirely sure how, or if these should be so constrained since it
  // may induce recursive constraints.

  /**
   * @ingroup concepts
   * @ingroup unchecked_concepts
   * An associative operation is a binary operation that exhibits the
   * associative property.
   */
  template<typename Op, typename T>
  struct Associative_Operation
    : Operation<Op, T, T>
  {
    Associative_Operation()
    { auto p = constraints; }

    static void constraints(Op op, T x, T y, T z)
    { associativity(op, x, y, z); }

    static void associativity(Op op, T x, T y, T z)
    { assert(( op(x, op(y, z)) == op(op(x, y), z) )); }
  };

  /**
   * @ingroup concepts
   * @ingroup unchecked_concepts
   * A commutative operation is a binary operation that exhibits the
   * commutative property.
   */
  template<typename Op, typename T>
  struct Commutative_Operation
    : Operation<Op, T, T>
  {
    Commutative_Operation()
    { auto p = constraints; }

    static void constraints(Op op, T x, T y)
    { commutativity(op, x, y); }

    static void commutativity(Op op, T x, T y)
    { assert(( op(x, y) == op(y, x) )); }
  };

  /**
   * @ingroup concepts
   * @ingroup unchecked_concepts
   * Two binary operations Op1 and Op2 are connected by the distributive
   * property if the Op1 distributes over Op2.
   */
  template<typename Op1, typename Op2, typename T>
  struct Distributive_Property
    : Operation<Op1, T, T>
    , Operation<Op2, T, T>
  {
    Distributive_Property()
    { auto p = constraints; }

    static void constraints(Op1 op1, Op2 op2, T x, T y, T z)
    { distribute(op1, op2, x, y, z); }

    static void distribute(Op1 op1, Op2 op2, T x, T y, T z)
    { assert(( op1(x, op2(y, z)) == op2(op1(x, y), op1(y, z)) )); }
  };

  /**
   * @ingroup concepts
   * @ingroup unchecked_concepts
   * Two binary operations Op1 and Op2 are connected by the absoption law if
   * Op1 absorbs Op2.
   */
  template<typename Op1, typename Op2, typename T>
  struct Absorption_Law
    : Operation<Op1, T, T>
    , Operation<Op2, T, T>
  {
    Absorption_Law()
    { auto p = constraints; }

    static void constraints(Op1 op1, Op2 op2, T x, T y)
    { absorb(op1, op2, x, y); }

    static void absorb(Op1 op1, Op2 op2, T x, T y)
    { assert(( op1(x, op2(x, y)) == x )); }
  };

  /**
   * @ingroup concepts
   * @ingroup unchecked_concepts
   * A binary operation Op and an unary operation Inv are connected by the
   * complement law if the applying Op to an object x and the inverse of x
   * results in a least or greatest element of T. Here, Inv denotes an
   * involution and Elem is a constant function.
   */
  template<typename Op, typename Inv, typename Elem, typename T>
  struct Complement_Law
    : Operation<Op, T, T>
    , Operation<Inv, T>
    , Constant<Elem, T>
  {
    Complement_Law()
    { auto p = constraints; }

    static void constraints(Op op, Inv inv, Elem elem, T x)
    { compliment(); }

    static void compliment(Op op, Inv inv, Elem elem, T x)
    { assert(( op(x, inv(x)) == elem() )); }
  };

  // FIXME: Do I need the ivolution law (!!x == x)

} // namespace origin

#endif
