// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONTAINER_HPP
#define ORIGIN_CONTAINER_HPP

// This file contains common declarations for all containers. It does not
// include any specific containers.

#include <origin/range.hpp>

namespace origin
{
  // Swap
  // Safely get the result type of the expression swap(a, b).
  template<typename T>
    struct swap_result
    {
    private:
      template<typename X>
        static auto check(X& x, X& y) -> decltype(swap(x, y));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<T>()));
    };
  
  // An alias for the result of the swap(a, b) expression. This is always
  // void, unless swap is not defined.
  template<typename T>
    using Swap_result = typename swap_result<T>::type;

  // Returns true if swap(a, b) is a valid expression.
  template<typename T>
    constexpr bool Has_swap()
    {
      return Subst_succeeded<Empty_result<T>>();
    }
  
    
    
  // Containers
  // A container is a collection of sub-objects. The container concept is
  // defined generally in terms of types that can be used as containers. This
  // essentially means that C is a container if it has the following type names
  // and valid expressions:
  //
  //    - Value_type<C>     // The type of sub-object
  //    - Distance_type<C>  // Distance between sub-object positions
  //    - begin(c)          // An iterator to the first sub-object
  //    - end(c)            // An iterator past the last sub-object
  //    - size(c)           // The number of sub-objects
  //    - empty(c)          // True if size(c) == 0
  //    - swap(a, b)        // Conainers can bee swapped
  //
  // Note that the results of begin and end must be Forward_iterators. The
  // following template aliases are valid for Containers:
  //
  //    - Iterator_type<C>
  //    - Iterator_type<C const>
  //    - Size_type<C>
  //
  // Note that we are not explicitly checking for member functions. This is
  // because container is very general and there are some built-in types that
  // can be made to model the concept: arrays, in particular. More advanced
  // container types require member functions.
  //
  // FIXME: Initialization requirements? I don't think there are any.
  

  // Returns true if C has all the requisite associated types. Note that this
  // only checks the non-derivable associated types. The size type and iterator
  // type are trivially deduced from required expressions.
  template<typename C>
    constexpr bool Has_container_types()
    {
      return Has_value_type<C>() && Has_distance_type<C>();
    }

  // A helper class for checking syntactic requirements.
  template<bool Prereqs, typename C>
    struct Container_requirements
    {
      static constexpr bool check() { return false; }
    };
    
  template<typename C>
    struct Container_requirements<true, C>
    {
      static constexpr bool check()
      {
        return Forward_iterator<Iterator_type<C>>()

            // Size_type<C> == { size(a) }
            && Has_size<C>() && Unsigned<Size_type<C>>()
            
            // bool { empty(a) }
            && Has_empty<C>() && Boolean<Empty_result<C>>()
            
            // swap(a, b)
            && Has_swap<C>();
      }
    };
    
  // The specification of the Container concept.
  //
  // NOTE: A container type is both a Range and a const Range. That is, it
  // provides both regular and const iterators.
  template<typename C>
    struct Container_concept
    {
      static constexpr bool check()
      {
        return Container_requirements<
          Range<C>() && Has_container_types<C>(), C
        >::check();
      }
      
      static bool test()
      {
        // FIXME: Write semantics
        return true;
      }
    };
  
  // Return true if C is a container.
  template<typename C>
    constexpr bool Container()
    {
      return Container_concept<C>::check();
    }

  template<typename C>
    using Container_reference = Dereference_result<Iterator_type<C>>;
    
  template<typename C>
    using Container_pointer = Address_result<Iterator_type<C>>;
    
    

  // Associative containers
  // An associative container supports efficient search.
  //
  // The associated container concept requires the member function c.find(k) 
  // instead of the more general expression find(c, k). Not every searchable
  // container is associative; we require find() to have sub-linear complexity
  // (i.e., O(log n) for search trees and O(1) for hash tables).
  //
  // FIXME: This is completely unfinished.
  


    
  // A helper class for checking syntactic requirements
  template<bool Prereqs, typename C>
    struct Associative_container_requirements
    {
      static constexpr bool check() { return false; }
    };

  template<typename C>
    struct Associative_container_requirements<true, C>
    {
      static constexpr bool check()
      {
        return Has_member_find<C, Value_type<C>>()
            && Forward_iterator<Member_find_result<C, Value_type<C>>>();
      }
    };

  // Specification of the associative container concept.
  template<typename C>
    struct Associative_container_concept
    {
      static constexpr bool check()
      {
        return Associative_container_requirements<Container<C>(), C>::check();
      }
    };

  // Return true if C is an associative container.
  template<typename C>
    constexpr bool Associative_container()
    {
      return Associative_container_concept<C>::check();
    }
    
} // namespace origin

#endif
