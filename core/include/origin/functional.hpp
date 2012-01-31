// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
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
  // NOTE: The standard relation operators (o_equal, o_less, etc.) vary from
  // the standard operators in that a) they are not templates, b) they are
  // defined on heterogeneous types, and c) they are constrained by their
  // corresponding concepts.
  
  // The equal function object defines the relation a == b.
  struct o_equal
  {
    template<typename T, typename U>
      bool operator()(T&& a, U&& b) const
      {
        static_assert(Equality_comparable<T, U>(), "");
        return std::forward<T>(a) == std::forward<T>(b);
      }
  };
  
  // The type eq is an alias for o_equal.
  using eq = o_equal;
  
  
  // The less function object defines the relation a < b.
  struct o_less
  {
    template<typename T, typename U>
      bool operator()(T&& a, U&& b) const
      {
        static_assert(Totally_ordered<T, U>(), "");
        return std::forward<T>(a) < std::forward<T>(b);
      }
  };
  
  // The type lt is an alias for strict_less.
  using lt = o_less;
  
  
  
  // The less equal function object defines the relation a <= b, but only if
  // a and b are totally ordered.
  struct o_less_equal
  {
    template<typename T, typename U>
      bool operator()(T&& a, U&& b) const
      {
        static_assert(Totally_ordered<T, U>(), "");
        return std::forward<T>(a) <= std::forward<T>(b);
      }
  };
  
  // The type leq is an alias for weak_less.
  using leq = o_less_equal;

  
  
  
  // If r is a Relation, then the complement of r(a, b) is !r(a, b).
  template<typename R>
    struct complement
    {
      complement(R r) : r(r) { };
      
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
    struct converse
    {
      converse(R r) : r(r) { };
      
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

    
    
  // FIXME: Move the hash module into a separate header and borrow some of
  // Boost's facilities. Hashing is a really fundamental service. It probably
  // shouldn't be brought in with the functional header. Maybe something like
  // <origin/hashing.hpp>?
  //
  // FIXME: Specialize hash_value for other standard types.
  //
  // FIXME: Develop a Hashable concept.
  
  // Hash value (scalars)
  template<typename T>
    inline Requires<Scalar<T>(), std::size_t> hash_value(T x)
    {
      std::hash<T> h;
      return h(x);
    }
    
  // Hash value (member)
  template<typename T>
    inline auto hash_value(const T& x) -> decltype(x.hash())
    {
       return x.hash();
    }

    

  // Origin hashing
  // This class can be used to wrap a value type for use with the std::hash
  // function object. It redirects the usual hashing lookup to the Origin
  // hashing facility. It is used as:
  //
  //    hash<origin_hashing<T>> h;  // h will call origin::hash_value.
  //
  // See the corresponding specialization below.
  template<typename T>
    struct origin_hashing
    {
      using value_type = T;
    };
    
} // namespace origin


namespace std
{
  // Specialize the standard hash function so that it will use the origin
  // hashing facility.
  template<typename T>
    struct hash<origin::origin_hashing<T>>
    {
      std::size_t operator()(const T& x) const
      {
        using origin::hash_value;
        return hash_value(x);
      }
    };
}

#endif
