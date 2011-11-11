// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONTANIER_HPP
#define ORIGIN_CONTAINER_HPP

// This file contains common declarations for all containers. It does not
// include any specific containers.

#include <origin/iterator.hpp>

namespace origin
{
  // Begin iterator
  // Ranges and containers have begin and end operations that return iterators.

  // Safely get the type returned by std::begin(x).
  template<typename T>
    struct begin_result
    {
    private:
      template<typename X>
        static auto check(X&& x) -> decltype(std::begin(x));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };
    
  // An alias to the result of the expression empty(x).
  template<typename T>
    using Begin_result = typename begin_result<T>::type;
    
  // Return true if empty(t) is a valid expression.
  template<typename T>
    bool constexpr Has_begin()
    {
      return Subst_succeeded<Begin_result<T>>();
    }

  // Safely get the type returned by std::end(x).
  template<typename T>
    struct end_result
    {
    private:
      template<typename X>
        static auto check(X&& x) -> decltype(std::end(x));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };
    
  // An alias to the result of the expression empty(x).
  template<typename T>
    using End_result = typename end_result<T>::type;
    
  // Return true if empty(t) is a valid expression.
  template<typename T>
    bool constexpr Has_end()
    {
      return Subst_succeeded<End_result<T>>();
    }


  // Ranges
  // A range is simply a class that exposes a pair of iterators called begin()
  // and end(). It is, in some senses, a very lightweight container.
  
  // Ranges
  // A range is a type that provides a pair of iterators that is accessed
  // through the begin(x) and end(x) operations.
  //
  // FIXME: Should ranges have initialization requirements? If so, I imagine
  // that they're basically the same as lambda closure types.
  template<typename R>
    struct Range_concept
    {
      static constexpr bool check()
      {
        return Has_begin<R>() 
            && Iterator<Begin_result<R>>()
            && Has_end<R>()
            && Iterator<End_result<R>>()
            && Same<Begin_result<R>, End_result<R>>();
      }
      
      // FIXME Write semantics.
    };
  
  // Return true if R is a range.
  template<typename R>
    constexpr bool Range()
    {
      return Range_concept<R>::check();
    }
    
  // An alias to the iterator type of a range. This is the same as the
  // result of the begin operation on the same type. Note that "const R" may
  // yield a different type than an unqualifed "R".
  template<typename R>
    using Iterator_type = Begin_result<R>;

    
    
  // Size
  // The following type traits and type predicates establish the notion of
  // sized type, some type for which size(x) is valid query. Size is defined
  // in 3 ways:
  //    - For user-defined containers, it is x.size().
  //    - For ranges, it is distance(begin(x), end(x))
  //    - For statically sized arrays of type T[N], it is N.

  // Safely get the type returned by the member function x.size().
  template<typename T>
    struct member_size_result
    {
    private:
      template<typename X>
        static auto check(X const& x) -> decltype(x.size());
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };
    
  // An alias to the result of the expression empty(x).
  template<typename T>
    using Member_size_result = typename member_size_result<T>::type;
    
  // Return true if empty(t) is a valid expression.
  template<typename T>
    bool constexpr Has_member_size()
    {
      return Subst_succeeded<Member_size_result<T>>();
    }
    
  // Return the size of x. The meaning of size is dependent on the type of
  // T, and the operation generally returns an unsigned integral type.
  //
  // NOTE: Some containers may have constexpr size (e.g., array).
  template<typename T>
    constexpr auto size(T const& x) -> decltype(x.size())
    {
      return x.size();
    }

  // Specialize size for statically sized arrays.
  template<typename T, std::size_t N>
    constexpr std::size_t size(T(&)[N])
    {
      return N;
    }
    
  // Specialize for size for Ranges that are not containers.
  template<typename R>
    inline auto size(R const& r)
      -> Requires<Range<R>() && !Has_member_size<R>(), Make_unsigned<Distance_type<R>>>
    {
      return std::distance(std::begin(r), std::end(r));
    }
  
  // Safely get the type associated with the result of the size() query.
  // Note that size() is expected to be constant.
  template<typename T>
    struct size_result
    {
    private:
      template<typename X>
        static auto check(X const& x) -> decltype(size(x));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };
    
  // Return T's associated size type, if defined.
  template<typename T>
    using Size_type = typename size_result<T>::type;
    
  // Return true if size(t) is a valid expression.
  template<typename T>
    bool constexpr Has_size()
    {
      return Subst_succeeded<Size_type<T>>();
    }

    
    
  // Empty
  // A type may support empty queries as in empty(x). Like size(), empty is
  // defined in 3 ways:
  //    - For containers, it is x.empty().
  //    - For ranges, it is begin(x) == end(x)
  //    - For statically sized arrays of type T[N], it is N == 0.
  
  // Safely get the type returned by the member function x.empty().
  template<typename T>
    struct member_empty_result
    {
    private:
      template<typename X>
        static auto check(X const& x) -> decltype(x.empty());
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };
    
  // An alias to the result of the expression empty(x).
  template<typename T>
    using Member_empty_result = typename member_empty_result<T>::type;
    
  // Return true if empty(t) is a valid expression.
  template<typename T>
    bool constexpr Has_member_empty()
    {
      return Subst_succeeded<Member_empty_result<T>>();
    }
  
  // Return true if x is empty. The meaning of empty is dependent on the type
  // of T.
  //
  // NOTE: Some containers may have constexpr empty (e.g., arrays).
  template<typename T>
    constexpr auto empty(T const& x) -> decltype(x.empty())
    {
      return x.empty();
    }

  // Specialization for statically sized arrays.
  template<typename T, std::size_t N>
    constexpr bool empty(T(&a)[N])
    {
      return N == 0;
    }

  // Specialization for ranges, which don't have a member empty.
  template<typename R>
    inline Requires<!Has_member_empty<R>(), bool> empty(R const& r)
    {
      return std::begin(r) == std::end(r);
    }

  // Safely get the type associated with the result of the empty() query. Note
  // that empty is expected to be constant.
  template<typename T>
    struct empty_result
    {
    private:
      template<typename X>
        static auto check(X const& x) -> decltype(empty(x));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };
    
  // An alias to the result of the expression empty(x).
  template<typename T>
    using Empty_result = typename empty_result<T>::type;
    
  // Return true if empty(t) is a valid expression.
  template<typename T>
    bool constexpr Has_empty()
    {
      return Subst_succeeded<Empty_result<T>>();
    }

    
  // Concepts
  
  // Containers
  // A container is a collection of sub-objects.
  
  // There are two types for containers that are not readily derived from the
  // interface. The other types, e.g., Size_type, Iterator_type, etc are 
  // derived from the interface and don't need to be specified.
  //
  // NOTE: This is not strictly true for containers. The value type can be
  // deduced by decaying decltype(*begin(c)) and the distance type can be
  // deduced as decltype(distance(begin(c), end(c))). The reason that we can
  // get the value type from begin() is that it must be a Forward_iterator.
  //
  // FIXME: Consider automatically deriving the value type and distance type
  // for the container.
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
        return Unsigned<Size_type<C>>()

            // Size_type<C> == { size(c) };
            && Has_size<C>()
            
            // bool { empty(c) }
            && Has_empty<C>()
            && Convertible<Empty_result<C>, bool>();
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
             Regular<C>() 
          && Range<C>() 
          && Range<C const>() 
          && Has_container_types<C>(), 
          C
        >::check();
      }
      
      static bool test()
      {
        // FIXME: Semantics
        return true;
      }
    };
  
  // Return true if C is a container.
  template<typename C>
    constexpr bool Container()
    {
      return Container_concept<C>::check();
    }

    
} // namespace origin

#endif
