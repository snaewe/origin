// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_FUNCTIONAL_HPP
#define ORIGIN_FUNCTIONAL_HPP

#include <functional>

#include <origin/concepts.hpp>

namespace origin
{
  // If r is a Relation, then the complement of r(a, b) is !r(a, b).
  template<typename R>
    struct complement_relation
    {
      complement_relation(R r) : r(r) { };
      
      template<typename T, typename U>
        bool operator()(T const& a, U const& b) const
        {
          static_assert(Relation<R, T, U>(), "");
          return !r(a, b);
        }

      R r;
    };
  
  // If r is a Relation, the the converse of r(a, b) is r(b, a).
  template<typename R>
    struct converse_relation
    {
      converse_relation(R r) : r(r) { };
      
      template<typename T, typename U>
        bool operator()(T const& a, U const& b) const
        {
          static_assert(Relation<R, T, U>(), "");
          return r(b, a);
        }

      R r;
    };

  // If r is a Relation, then the complement of the converse of r(a, b) is
  // !r(b, a).
  template<typename R>
    struct complement_of_converse_relation
    {
      complement_of_converse_relation(R r) : r(r) { };
      
      template<typename T, typename U>
        bool operator()(T const& a, U const& b) const
        {
          static_assert(Relation<R, T, U>(), "");
          return !r(b, a);
        }

      R r;
    };

  // If r is a Relation, the symmetric complement of r is true if and
  // only if !r(a, b) and !r(b, a) is true. In the case of orderings, this
  // is the case that neither a < b nor b < a.
  template<typename R>
    struct symmetric_complement
    {
      symmetric_complement(R r) : r(r) { };
      
      template<typename T, typename U>
        bool operator()(T const& a, U const& b) const
        {
          static_assert(Relation<R, T, U>(), "");
          return !r(a, b) && !r(b, a);
        }
        
      R r;
    };
    
} // namespace origin

#endif
