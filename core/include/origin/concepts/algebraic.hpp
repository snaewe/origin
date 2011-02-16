// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONCEPTS_ALGEBRA_HPP
#define ORIGIN_CONCEPTS_ALGEBRA_HPP

#include <origin/concepts/function.hpp>

// FIXME: Should be called algebraic.hpp?

namespace origin
{
  template<typename T, 
           typename And, 
           typename Or, 
           typename Not, 
           typename True, 
           typename False>
  struct Boolean_Algebra
  {
    Boolean_Algebra()
    { auto p = constraints; }
    
    static void constraints()
    {
      // FIXME: These are largely subsumed by the semantic requirements.
      Operation<And, T, T>{};
      Operation<Or, T, T>{};
      Operation<Not, T>{};
      
      // FIXME: These are generators, but they're constant generators. What
      // do you call a nullary function that always returns the same value?
      // Constant_Operation<True, T>{};
      // Constant_Operation<False, T>{};
      
      Associative_Operation<And, T>{};
      Associative_Operation<Or, T>{};
      Commutative_Operation<And, T>{};
      Commutative_Operation<Or, T>{};
      Absorptive_Property<And, Or, T>{};
      Absorptive_Property<Or, And, T>{};
      Distribute_Property<And, Or, T>{};
      Distribute_Property<Or, And, T>{};
      
      // FIXME: These names will change.
      Complement_Lattice<And, False>{};
      Complement_Lattice<Or, True>{};
    }
  };

} // namespace origin

#endif
