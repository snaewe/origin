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
  // As bool (function)
  // The as bool function object evaluates its argument as a boolean value,
  // returning true or false.
  template<typename T = default_t>
    struct as_bool
    {
      static_assert(Convertible<T, bool>(), "");
      
      bool operator()(const T& x) const { return x; }
    };
    
  // The default specialization is polymorphic.
  template<>
    struct as_bool<default_t>
    {
      template<typename T>
        bool operator()(const T& x)
        {
          static_assert(Convertible<T, bool>(), "");
          return x;
        }
    };

  
  // NOTE: The standard relation operators (o_equal, o_less, etc.) vary from
  // the standard operators in that a) they are not templates, b) they are
  // defined on heterogeneous types, and c) they are constrained by their
  // corresponding concepts.
  
  
  // Equal (relation)
  // The equal function object names the relation a == b.
  template<typename T = default_t, typename U = T>
    struct o_equal_to
    {
      static_assert(Equality_comparable<T, U>(), "");
      
      bool operator()(const T& a, const U& b) const { return a == b; }
    };

  // The default specialization is polymorphic.
  template<>
    struct o_equal_to<default_t, default_t>
    {
      template<typename T, typename U>
        bool operator()(const T& a, const U& b) const
        {
          static_assert(Equality_comparable<T, U>(), "");
          return a == b;
        }
    };
  
  // The type eq is an alias for o_equal.
  using eq = o_equal_to<>;
  
  
  
  // Less (relation)
  // The less function object names the relation a < b.
  template<typename T = default_t, typename U = T>
    struct o_less
    {
        static_assert(Totally_ordered<T, U>(), "");
        
        bool operator()(const T& a, const U& b) const { return a < b; }
    };

  // The default specialization is polymorphic.
  template<>
    struct o_less<default_t, default_t>
    {
      template<typename T, typename U>
        bool operator()(T&& a, U&& b) const
        {
          static_assert(Totally_ordered<T, U>(), "");
          return std::forward<T>(a) < std::forward<T>(b);
        }
    };
  
  // The type lt is an alias for o_less<>.
  using lt = o_less<>;
  
  
  
  // TODO: Write more relations and add operators.
  

  
  // Complement (relation)
  // For a and b, the complement of r(a, b)  is !r(a, b).
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
  
  
  
  // Converse (relation)
  // For a and b, the converse of r(a, b) is r(b, a).
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

    

  // Complement of converse (relation)
  // For a and b, the complement of the converse of r(a, b) is !r(b, a).
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

    

  // Symmetric complement (relation)
  // For a and b, the symmetric complement of r is !r(a, b) && !r(b, a).
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
