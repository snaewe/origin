// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONCEPTS_RELATION_HPP
#define ORIGIN_CONCEPTS_RELATION_HPP

#include <origin/logic.hpp>
#include <origin/concepts/function.hpp>

namespace origin
{
  /**
   * @defgroup concepts_relation
   * @ingroup concepts
   *
   * The relation concepts express requirements on the notions of equivalence
   * and order.
   */

  template<typename Rel, typename T>
    bool aReflexive(Rel r, T x)
    {
      return r(x, x);
    }

  template<typename Rel, typename T>
  bool aIrreflexive(Rel r, T x)
    {
      return !r(x, x);
    }

  template<typename Rel, typename T>
  bool aSymmetric(Rel r, T x, T y)
    {
      return implies(r(x, y), r(y, x));
    }

  template<typename Rel, typename T>
    bool aAsymmetric(Rel r, T x, T y)
    {
      return implies(r(x, y), !r(y, x));
    }

  template<typename Rel, typename T>
    bool aTransitive(Rel r, T x, T y, T z)
    {
      return implies(r(x, y) && r(y, z), r(x, y));
    }

  template<typename Rel, typename T>
    bool aTotal(Rel r, T x, T y)
    {
      return r(x, y) || r(y, x);
    }

  // NOTE: We can't define antisymmetry or trichotomy without a separate
  // notion of equivalence on T.
  
  /**
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
   * A relation r is trichotomous if, for the arguments x and y, exactly on
   * of the following be true.
   *  - r(x, y)
   *  - r(y, x)
   *  - x == y
   */
  template<typename Rel, typename T>
    bool aTrichotomy(Rel r, T x, T y) 
    {
      return implies(r(x, y), !r(y, x) && x != y) 
          || implies(r(y, x), !r(y, x) && x != y) 
          || implies(x == y, !r(x, y) && !r(y, x));
    }

  /**
   * @ingroup concepts_relation
   *
   * The Relation concept is valid for binary Predicates called over a pair
   * of homogenous, regular arguments; the arguments must share a common type.
   * 
   * Note that generalized n-ary relations are not addressed by this concept,
   * as they are more relevant to libaries implementing aspects of relational
   * data structures and algorithms.
   */
  template<typename Rel, typename T, typename U>
    struct cRelation
      : cPredicate<Rel, T, U>
    {
      cRelation()
      {
        tCommon<T, U>{};
      }
    };

  /**
   * @ingroup concepts_function
   *
   * An Equivalence Relation is a semantic refinement of a Relation that is
   * Reflexive, Symmetric, and Transitive.
   */
  template<typename Rel, typename T>
    struct aEquivalence_Relation
    {
      aEquivalence_Relation()
      {
        auto p = constraints;
      }

      static void constraints(Rel r, T x, T y, T z)
      {
        cRelation<Rel, T, T>{};
        aReflexive(r, x);
        aSymmetric(r, x, y);
        aTransitive(r, x, y, z);
      }
    };
  
  /**
   * @ingroup concepts_function
   * 
   * A Strict Partial Order is a semantic refinement of a Relation that is
   * Irreflexive, Asymmetric, and Transtive. IEEE 754 floating point numbers 
   * are partially ordered by the built in operator< since comparing against
   * NaN returns false for every case.
   */
  template<typename Rel, typename T>
    struct aStrict_Partial_Order
    {
      aStrict_Partial_Order()
      {
        auto p = constraints;
      }
      
      static void constraints(Rel r, T x, T y, T z)
      {
        cRelation<Rel, T, T>{};
        aIrreflexive(r, x);
        aAsymmetric(r, x, y);
        aTransitive(r, x, y, z);
      }
    };

  /**
   * @ingroup concepts_function
   *
   * A Strict Weak Order is a semantic refinement of a Strict Partial Order 
   * that requires incomaprability, the case where neither !r(x, y) nor
   * !r(y, x) is true, to be transitive.
   */
  template<typename Rel, typename T>
    struct aStrict_Weak_Order
      : aStrict_Partial_Order<Rel, T>
    {

      aStrict_Weak_Order()
      {
        auto p = constraints;
      }
      
      static void constraints(Rel r, T x, T y, T z)
      {
        aTransitive(incomparable<T, Rel>{r}, x, y, z); 
      }
    };

  /**
   * @ingroup concepts_function
   *
   * A Strict Total Order is a semantic refinement of a Strict Weak Order 
   * in which incomaprability is the same as equality, or equivalently that 
   * the relation is trichotomous. That is, for any arguments x and y, exactly
   * one of the following is true: r(x, y), r(y, x), or x == y.
   */
  template<typename Rel, typename T>
    struct aStrict_Total_Order
      : aStrict_Weak_Order<Rel, T>
    {

      aStrict_Total_Order()
      {
        auto p = constraints;
      }
      
      static void constraints(Rel r, T x, T y, T z)
      {
        // Note that transitvity of this operation is required by the strict
        // weak order refinement.
        aTrichotomy(r, x, y); 
      }
    };

} // namespace origin

#endif
