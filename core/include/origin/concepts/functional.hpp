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
   * @ingroup func_concepts
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
  template<typename F>
  struct Constant
    : Callable<F>
  {
    Constant()
    { auto p = constraints; }

    static void constraints()
    { }

    // FIXME: This probably isn't the most accurate way of stating that f()
    // denotes a constant, unchanging value.
    // FIXME: This implicitly requires that result_of<F()> is implicitly
    // convertible to bool.
    static void constant(F f)
    { assert(( f() == f() )); }

    typedef std::tuple<
      Callable<F>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup func_concepts
   * A relation is a binary function whose result type is convertible to bool.
   *
   * Note that the relation concept enforces a type constraint on an
   * intermediate type (i.e., the result of the operation). Constraining a
   * template to require a Relation may result in the exclusion of viable
   * models.
   *
   * Note that binary relations are typically homogenous; there are a lot of
   * nice properties of such functions (e.g., symmetry, reflexivity, etc.).
   * However, in the most general sense, we can't strictly enforce this
   */
  template<typename Op, typename T, typename U>
  struct Relation
    : Callable<Op, T, U>
  {
    Relation()
    { auto p = constraints; }

    static void constraints(Op op, T x, U y)
    { bool result = op(x, y); }

    typedef std::tuple<
      Callable<Op, T, U>,
      std::is_convertible<typename deduce_callable<Op, T, U>::type, bool>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  // NOTE: All of the "property" concepts for relations and operations are
  // specified in terms of homogenous relations. If the relation is not
  // homogenous, then it seems unlikely that we can realistically evaluate
  // any semantic requirements.
  //
  // This is particularly true for expression templates. Consider an expression
  // op(x, y) where x and y are expression template types denoting different,
  // yet ulitmiately equivalent subtrees (e.g., b and !!b). We could evaluate
  // whether or not op satisfies the reflexive property, but since the types
  // of x and y vary, we would have to develop additional notions of
  // equivalence. Is lit<b> the same as not<not<lit<b>>>?

  /**
   * @ingroup concepts
   * A relation is reflexive if, for all objects x in the domain of the
   * operation, op(x, x) == true.
   */
  template<typename Op, typename T>
  struct Reflexive_Relation
    : Relation<Op, T, T>
  {
    Reflexive_Relation()
    { auto p = constraints; }

    static void constraints()
    { }

    // FIXME: Only valid if result_of<Op(T, U)> is convertible to bool.
    static void reflexivity(Op op, T x)
    { assert(( op(x, x) )); }

    typedef std::tuple<
      Relation<Op, T, T>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
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
    : Relation<Op, T, T>
  {
    Symmetric_Relation()
    { auto p = constraints; }

    static void constraints()
    { }

    static void symmetry(Op op, T x, T y)
    { if(op(x, y)) assert(( op(y, x) )); }

    typedef std::tuple<
      Relation<Op, T, T>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts
   * @ingroup unchecked_concepts
   * A binary operation is symmetic if, for all objects x, y, and z in the
   * domaion of the operation, op(x, y) and op(y, z) implies op(x, z).
   */
  template<typename Op, typename T>
  struct Transitive_Relation
    : Relation<Op, T, T>
  {
    Transitive_Relation()
    { auto p = constraints; }

    static void constraints()
    { }

    static void transitivity(Op op, T x, T y, T z)
    { if(op(x, y) && op(y, z)) assert(( op(x, z) )); }

    typedef std::tuple<
      Relation<Op, T, T>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup func_concepts
   * An operation is a callable function whose argument and result types are
   * intended to be eqiuvalent.
   *
   * Note that this concept enforces a number of type constraints on both
   * argument and intermediate types. Using this concept to costrain a template
   * may exclude viable models if not weaker alternative is provided.
   */
  template<typename Op, typename... Args>
  struct Operation
    : Callable<Op, Args...>
  {
    Operation()
    { auto p = constraints; }

    static void constraints(Op op, Args&&... args)
    {
      Same<Args...>{};
      typedef typename front_type<Args...>::type Dom;
      typedef typename deduce_callable<Op, Args...>::type Codom;
      Same<Dom, Codom>{};
    }

    typedef std::tuple<
      Callable<Op, Args...>,
      Same<Args...>,
      Same<
        typename front_type<Args...>::type,
        typename deduce_callable<Op, Args...>::type
      >
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

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

    static void constraints()
    { }

    // FIXME: This is only valid if a) the result type if the result type of
    // op is equality comparable and if the result of that operation is
    // convertible to bol.
    static void associativity(Op op, T x, T y, T z)
    { assert(( op(x, op(y, z)) == op(op(x, y), z) )); }

    typedef std::tuple<
      Operation<Op, T, T>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
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

    static void constraints()
    { }

    // FIXME: This is only valid if a) the result type if the result type of
    // op is equality comparable and if the result of that operation is
    // convertible to bol.
    static void commutativity(Op op, T x, T y)
    { assert(( op(x, y) == op(y, x) )); }

    typedef std::tuple<
      Operation<Op, T, T>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts
   * @ingroup unchecked_concepts
   * Two binary operations Op1 and Op2 are connected by the distributive
   * property if the Op1 distributes over Op2.
   */
  template<typename Op1, typename Op2, typename T>
  struct Distributive_Property
  {
    Distributive_Property()
    { auto p = constraints; }

    static void constraints()
    {
      Operation<Op1, T, T>{};
      Operation<Op2, T, T>{};
    }

    static void distribute(Op1 op1, Op2 op2, T x, T y, T z)
    { assert(( op1(x, op2(y, z)) == op2(op1(x, y), op1(y, z)) )); }

    typedef std::tuple<
      Operation<Op1, T, T>,
      Operation<Op2, T, T>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  /**
   * @ingroup concepts
   * @ingroup unchecked_concepts
   * Two binary operations Op1 and Op2 are connected by the absoption law if
   * Op1 absorbs Op2.
   */
  template<typename Op1, typename Op2, typename T>
  struct Absorption_Law
  {
    Absorption_Law()
    { auto p = constraints; }

    static void constraints()
    {
      Operation<Op1, T, T>{};
      Operation<Op2, T, T>{};
    }

    static void absorb(Op1 op1, Op2 op2, T x, T y)
    { assert(( op1(x, op2(x, y)) == x )); }

    typedef std::tuple<
      Operation<Op1, T, T>,
      Operation<Op2, T, T>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  // FIXME: Would it be reasonable to further constrain the intermediate types
  // in this concept (i.e., the result types of Inv and Elem)?
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
  {
    Complement_Law()
    { auto p = constraints; }

    static void constraints(Op op, Inv inv, Elem elem, T x)
    {
      Operation<Op, T, T>{};
      Operation<Inv, T>{};
      Constant<Elem>{};
    }

    // FIXME: Actually evaluatin this axiom requires a number of semantic
    // requirements. Specifically, the result type of Inv must be T, and the
    // result type of Op must be equality comparable with the result type of
    // Elem, and the result of that comparison must be implicitly convertible
    // to bool.
    static void compliment(Op op, Inv inv, Elem elem, T x)
    { assert(( op(x, inv(x)) == elem() )); }

    typedef std::tuple<
      Operation<Op, T, T>,
      Operation<Inv, T>,
      Constant<Elem>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;

  };

} // namespace origin

#endif
