// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONCEPTS_RELATION_HPP
#define ORIGIN_CONCEPTS_RELATION_HPP

#include <origin/logic.hpp>
#include <origin/relation.hpp>
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
  bool aIrrefexive(Rel r, T x)
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
   * @ingroup concepts_relation
   *
   * The Relation concept is valid for binary Predicates called over a pair
   * of homogenous arguments; the arguments must share a common type.
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
   * The cEquivalence_Relation concept is valid for Relations that satisfy
   * the semantic requirements of an equivalence relation: reflexivity,
   * symmetry, and transitivity.
   */
  template<typename Rel, typename T, typename U>
  struct cEquivalence_Relation
    : cRelation<Rel, T, U>
  {
    cEquivalence_Relation()
    {
      auto p = constraints;
    }

    static void constraints(T x, T y, T z)
    {
      aReflexive(x);
      aSymmetric(x, y);
      aTransitive(x, y, z);
    }
  };

  /**
   * @ingroup concepts_function
   *
   * The cStrict_Weak_Order concept is valid for Relations that satisfy the
   * semantic requirements of a strict weak order: irreflexivity (strictness),
   * asymmetry, and transitivty. Furthermore, indifference (the case that
   * !r(x, y) and !r(y, x)) defines an equivalence relation.
   */
  template<typename Rel, typename T, typename U>
  struct cStrict_Weak_Order
    : cRelation<Rel, T, U>
  {
    cStrict_Weak_Order()
    {
      auto p = constraints;
    }

    static void constraints(T x, T y, T z)
    {
      aIrreflexive(x);
      aAsymmetric(x, y);
      aTransitive(x, y, z);
      cEquivalence_Relation<indifferent_to<T>, T, T>{};
    }
  };

} // namespace origin

#endif
