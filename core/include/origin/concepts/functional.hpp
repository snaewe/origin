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

// FIXME: Should be called functional.hpp?

namespace origin
{
  template<typename T> struct Regular;

  // FIXME: Axiomatic concepts are not validated during concept checking. They
  // are trivially true unless they are explicitly required (e.g., multipass).
  // It would probably be a good idea to define several "levels" of
  // verification, that explain why something is or is not validated.
  
  // FIXME: All of these constraints implicitly require Callable, Relation,
  // Predicate, and Operations concepts. Implement these.
  
  template<typename F, typename... Args> struct Callable;
  
  template<typename Op, typename T> struct Relation;
  
  template<typename Op, typename T> struct Operation;

  /**
   * @ingroup concepts
   * A relation is reflexive if, for all objects x in the domain of the
   * operation, op(x, x) == true.
   */
  template<typename Op, typename T>
  struct Reflexive_Relation
    : std::true_type
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
   * An relation is symmetic if, for all objects x and y in the domain
   * of the operation, op(x, y) implies op(y, x).
   */
  template<typename Op, typename T>
  struct Symmetric_Relation
    : std::true_type
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
   * A binary operation is symmetic if, for all objects x, y, and z in the
   * domaion of the operation, op(x, y) and op(y, z) implies op(x, z).
   */
  template<typename Op, typename T>
  struct Transitive_Relation
    : std::true_type
  {
    Transitive_Relation()
    { auto p = constraints; }

    static void constraints(Op op, T x, T y, T z)
    { transitivity(op, x, y, z); }

    static void transitivity(Op op, T x, T y, T z)
    { if(op(x, y) && op(y, z)) assert(( op(x, z) )); }
  };
  
  // FIXME: The following implicitly require Equal<T>, and possibly Regular<T>.
  // I'm not entirely sure how, or if these should be so constrained since it
  // may induce recursive constraints.

  template<typename Op, typename T>
  struct Associative_Operation
  {
    static void associativity(Op op, T x, T y, T z)
    { assert(( op(x, op(y, z)) == op(op(x, y), z) )); }
  };
  
  template<typename Op, typename T>
  struct Commutative_Operation
  {
    static void commutativity(Op op, T x, T y)
    { assert(( op(x, y) == op(y, x) )); }
  };
  
  template<typename Op1, typename Op2, typename T>
  struct Absorptive_Property
  {
    static void absorb(Op1 op1, Op2 op2, T x, T y)
    { assert(( op1(a, op2(a, b)) == a )); }
  };
  
  template<typename Op1, typename Op2, typename T>
  struct Distributive_Property
  {
    static void distribute(Op1, Op2, T x, T y, T z)
    { assert(( op1(x, op2(y, z)) == op2(op1(x, y), op1(y, z)) )); }
  };
  
  // FIXME: Probably not the right name for this.
  template<typename Bin_Op, typename Un_Op, typename T>
  struct Compliment_Lattice
  {
    static void compliment(Bin_Op bin, Un_Op un, T x, T comp)
    { assert(( bin(x, un(x)) == comp )); }
  };
  
} // namespace origin

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
      Boolean<decltype(x == y)>{};
      Boolean<decltype(x != y)>{};
      Boolean<decltype(y == x)>{};
      Boolean<decltype(y != x)>{};

      // FIXME: Is there anything else that I'm missing here?
    }

    // FIXME: Make this an axiom class.
    static void symmetry(T x, U y)
    {
      if(x == y) assert(( y == x ));
      if(y == x) assert(( x == y ));
    }

    typedef std::tuple<
      Boolean<typename deduce_equal<T, U>::type>,
      Boolean<typename deduce_not_equal<T, U>::type>,
      Boolean<typename deduce_equal<U, T>::type>,
      Boolean<typename deduce_not_equal<U, T>::type>
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
    /**
     * The equal identity concept requires that an object is equivalent to
     * itself. This is defined internally since it's definition is incomplete
     * without first evaluating whether or T is Equal.
     */
    struct Equal_Identity
      : std::true_type
    {
      Equal_Identity()
      { auto p = constraints; }

      static void constraints(T x, T y)
      { identity(x, y); }

      static void identity(T x, T y)
      { if(&x == &y) assert(( x == y )); }
    };

    // Encapsulate the operator.
    typedef std::equal_to<T> Eq;
    typedef std::not_equal_to<T> Neq;

    Equal()
    { auto p = constraints; }

    static void constraints(T x, T y)
    {
      Boolean<decltype(x == y)>{};
      Boolean<decltype(x != y)>{};

      Equal_Identity{};

      // FIXME: Say something about Eq and Neq?

      Reflexive_Relation<Eq, T>{};
      Symmetric_Relation<Eq, T>{};
      Transitive_Relation<Eq, T>{};
    }

    // NOTE: We can avoid writing has_ + deduce_ since dedce_ will return a
    // type that is not convertible to bool. This one test happens to cover
    // both cases.
    typedef std::tuple<
      std::is_convertible<typename deduce_equal<T, T>::type, bool>,
      std::is_convertible<typename deduce_not_equal<T, T>::type, bool>,
      Equal_Identity,
      Reflexive_Relation<Eq, T>,
      Symmetric_Relation<Eq, T>,
      Transitive_Relation<Eq, T>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  // FIXME:
  // Explicitly provide models for builtin types. Hopefully, this will
  // cut down compilation times.

  /**
   * The ordered concept defines two different comparisons. When instantiated
   * over one type (or the same type), it determines if that type is ordered,
   * that it can be compared using the relational ordering operators. When
   * instantiated over two types,
   */
  template<typename T, typename U>
  struct Ordered
  { };

  template<typename T>
  struct Ordered<T, T>
  {
    // Encapsulate the operator.
    typedef std::less<T> Lt;
    typedef std::greater<T> Gt;
    typedef std::less_equal<T> Leq;
    typedef std::Greater_equal<T> Geq;

    Ordered()
    { auto p = constraints; }

    static void constraints(T x, T y)
    {
      Boolean<decltype(x < y)>{};
      Boolean<decltype(x > y)>{};
      Boolean<decltype(x <= y)>{};
      Boolean<decltype(x >= y)>{};
    }
  };

  // FIXME: Provide built-in models.


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
      Equal<T>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  // FIXME: Should this type actually encompass the requirements of a
  // Boolean_Algebra? Probably not. This merely wants a type that can behave
  // like a bool in a bare number of cases. It would be interesting to know
  // if this sufficiently describes the syntax required by a boolean algebra.
  /**
   * A boolean type is a type that can be evaluated in boolean contexts and
   * participate in logical expresssions. Note that this does not constrain
   * the set of values defined by the boolean type, this concept is purely
   * syntactic.
   */
  template<typename T>
  struct Boolean
  {
    Boolean()
    { auto p = constraints; }

    static void constraints(T x, T y)
    {
      Regular<T>{};

      if(x) ;

      // NOTE: I can't recursively constrain these exressions as Boolean
      // types (infinite recursion!). I shouldn't constrain them as T since
      // T might not actually be Boolean-like (it could be an int*). The only
      // reasonable choice is to require these to be convertible to bool.
      //
      // FIXME: There's an ICE calling bool{..}.
      bool(!x);
      bool(x && y);
      bool(x || y);

      // FIXME: Could also apply convertability requirements to these functions
      // to further separate the constraints on the expression from the result
      // type.
      auto notf = [](T x) { return !x; }
      auto andf = [](T x, T y) { return x && y; }
      auto orf = [](T x, T y) { return x || y; }
      
      typedef decltype(notf) Not;
      typedef decltype(andf) And;
      typedef decltype(orf) Or;
      
      Associative_Operation<Or, T>{};
      Commutative_Operation<Or, T>{};
      Absorptive_Property<Or, And, T>{};
      Distributive_Property<Or, And, T>{};
      
      // FIXME: I can't write this yet.
      // Compliment_Lattice<Or, And, T>
    }

    typedef std::tuple<
      Regular<T>,
      std::is_convertible<T, bool>,
      std::is_convertible<typename deduce_logical_not<T>::type, bool>,
      std::is_convertible<typename deduce_logical_and<T, T>::type, bool>,
      std::is_convertible<typename deduce_logical_or<T, T>::type, bool>
    > requirements;
    typedef concept_check<requirements> type;
    static constexpr bool value = type::value;
  };

  // Make sure that we have some built in models of Boolean types. Note that
  // if we don't have this, we get recursive template instantiation.
  // FIXME: Are there any other known models? char, short, int?
  template<> struct Boolean<bool> : std::true_type { };

} // namespace origin

#endif
