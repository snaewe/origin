// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONCEPTS_ALGEBRA_HPP
#define ORIGIN_CONCEPTS_ALGEBRA_HPP

#include <origin/concepts/functional.hpp>

namespace origin
{
  /**
   * @ingroup algebraic_concepts
   * A boolean algebra...
   */
  template<typename T,
           typename And,
           typename Or,
           typename Not,
           typename True,
           typename False>
  struct Boolean_Algebra
    : Operation<And, T, T>
    , Operation<Or, T, T>
    , Operation<Not, T>
    , Constant<True, T>
    , Constant<False, T>
  {
    Boolean_Algebra()
    { auto p = constraints; }

    static void constraints()
    {
      Associative_Operation<And, T>{};
      Associative_Operation<Or, T>{};
      Commutative_Operation<And, T>{};
      Commutative_Operation<Or, T>{};
      Distributive_Property<And, Or, T>{};
      Distributive_Property<Or, And, T>{};
      Absorption_Law<And, Or, T>{};
      Absorption_Law<Or, And, T>{};
      Complement_Law<And, Not, False, T>{};
      Complement_Law<Or, Not, True, T>{};
    }
  };

} // namespace origin

#endif
