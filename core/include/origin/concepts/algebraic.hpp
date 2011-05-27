// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONCEPTS_ALGEBRAIC_HPP
#define ORIGIN_CONCEPTS_ALGEBRAIC_HPP

#include <origin/concepts/impl.hpp>
#include <origin/concepts/fwd.hpp>

namespace origin
{
  /**
   * @defgroup concepts_algebraic
   *
   * Algebraic concepts are constraints that define semantic relationships
   * between types, operations, and relations.
   */

  /**
   * @ingroup concepts_axioms
   *
   * The Associative axiom requires an operation to be associative: the order
   * of evaluation does not affect the result.
   */
  template<typename Op, typename T>
    bool aAssociative(Op op, T x, T y, T z)
    {
      return op(x, op(y, z)) == op(op(x, y), z);
    }

  /**
   * @ingroup concepts_axioms
   *
   * The Commutative axiom requires an operation to be commutatative: the
   * order of operands does not affect the result.
   */
  template<typename Op, typename T>
    bool aCommutative(Op op, T x, T y)
    {
      return op(x, y) == op(y, x);
    }

  /**
   * @ingroup concepts_axim
   *
   * The Absorption axiom links two binary operations. An operation op1 absorbs
   * op2 iff, for any objects x and y having type T, op1(x, op2(x, y)) == x.
   */
  template<typename Op1, typename Op2, typename T>
    bool aAbsorption(Op1 op1, Op2 op2, T x, T y)
    {
      return op1(x, op2(x, y)) == x;
    }

  /**
   * @ingroup concepts_axiom
   *
   * The Distributive axiom connects two binary operations. An operation op1
   * distributes over op2 iff, for any objects x, y, and z having type T,
   * op1(x, op2(y, z)) == op2(op1(x, y), op1(x, z))
   */
  template<typename Op1, typename Op2, typename T>
    bool aDistributive(Op1 op1, Op2 op2, T x, T y, T z)
    {
      return op1(x, op2(y, z)) == op2(op1(x, y), op1(x, z));
    }

  /**
   * @ingroup concepts_axiom
   *
   * The Complement axiom connects a binary operation with a unary operation
   * (complement) and a value in the domain.
   */
  template<typename Op, typename Comp, typename Value, typename T>
    bool aComplement(Op op, Comp comp, Value val, T x)
    {
      return op(x, comp(x)) == val;
    }


  /**
   * @ingroup concepts_algebraic
   *
   * The Boolean_algebra concept describes the semantic requirements of a
   * boolean algebra defined over a Regular type T, two binary Operations
   * (called "and" and "or"), a unary Operation (called "not"), and two
   * constant Functions (called "true" and "false").
   */
  template<typename T,
           typename And,
           typename Or,
           typename Not,
           typename True,
           typename False>
    struct aBoolean_Algebra
    {
      aBoolean_Algebra()
      {
        auto p = constraints;
      }

      static void constraints(And a, Or o, Not n, True t, False f, T x, T y, T z)
      {
        cOperation<And, T, T>{};
        cOperation<Or, T, T>{};
        cOperation<Not, T>{};

        aAssociative(a, x, y, z);     // x and (y and z) == (x and y) and z
        aAssociative(o, x, y, z);     // x or (y or z) == (x or y) or z

        aCommutative(a, x, y);        // x and y == y and x
        aCommutative(o, x, y);        // x or y == y or x

        aAbsorption(a, o, x, y);      // x and (x or y) == x
        aAbsoprtion(o, a, x, y);      // x or (x and y) == x

        aDistributive(a, o, x, y, z); // x and (y or z) == (x and y) or (y and z)
        aDistributuve(o, a, x, y, z); // x or (y and z) == (x or y) and (y or z)

        aComplement(a, n, f, x);      // x and not x == f
        aComplement(o, n, t, x);      // x or not x == t
      }
    };

} // namespace origin

#endif
