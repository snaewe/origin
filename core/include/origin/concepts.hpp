// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONCEPTS_HPP
#define ORIGIN_CONCEPTS_HPP

#include <initializer_list>

#include <origin/traits.hpp>
#include <origin/functional.hpp>

namespace origin
{
  // Equality comparable (concept)
  // The equality comparable concept defines the syntax and semantics of
  // comparing for value equality.
  //
  // Note that when parameterized over two type arguments, the requirements of
  // the concept are different.
  template <typename T, typename U>
    struct Equality_comparable_concept
    {
      static constexpr bool check()
      {
        return Common<T, U>() 
            && Equality_comparable<T>()
            && Equality_comparable<U>()
            && Equality_comparable<Common_type<T, U>>()
            && Has_equal<T, U>()     && Boolean<Equal_result<T, U>>()
            && Has_equal<U, T>()     && Boolean<Equal_result<U, T>>()
            && Has_not_equal<T, U>() && Boolean<Equal_result<T, U>>()
            && Has_not_equal<U, T>() && Boolean<Equal_result<T, U>>();
        }
    };
  
  template <typename T>
    struct Equality_comparable_concept<T, T>
    {
      static constexpr bool check()
      {
        return Has_equal<T>()     && Boolean<Equal_result<T>>()
            && Has_not_equal<T>() && Boolean<Not_equal_result<T>>();
      }
    };
    
  // Returns true if the values of T (and U) can be compared for value equality.
  template <typename T, typename U>
    constexpr bool Equality_comparable()
    {
      return Equality_comparable_concept<T, U>::check();
    }



  // Weakly ordered (concept)
  // A type is weakly ordered if it defines the standard relational operators
  // <, >, <=, and >= with the usual meaning. In particular < must be a strict
  // weak order.
  //
  // The concept is extended for heterogeneous types. In order for two types
  // to be weak-order comparable, they must both be weakly ordered and
  // symmetrically comparable. (i.e., if a < b is valid, b < a must also be
  // valid).
  template <typename T, typename U>
    struct Weakly_ordered_concept
    {
      static constexpr bool check()
      {
        return Common<T, U>()
            && Totally_ordered<T>()
            && Totally_ordered<T>()
            && Totally_ordered<Common_type<T, U>>()
            && Has_less<T, U>()          && Boolean<Less_result<T, U>>()
            && Has_less<U, T>()          && Boolean<Less_result<U, T>>()
            && Has_greater<T, U>()       && Boolean<Greater_result<T, U>>()
            && Has_greater<U, T>()       && Boolean<Greater_result<U, T>>()
            && Has_less_equal<T, U>()    && Boolean<Less_equal_result<T, U>>()
            && Has_less_equal<U, T>()    && Boolean<Less_equal_result<U, T>>()
            && Has_greater_equal<T, U>() && Boolean<Greater_equal_result<T, U>>()
            && Has_greater_equal<U, T>() && Boolean<Greater_equal_result<U, T>>();
      }
    };

  // Specialization for the unary type.
  template <typename T>
    struct Weakly_ordered_concept<T, T>
    {
      static constexpr bool check()
      {
        return Has_less<T>()          && Boolean<Less_result<T>>()
            && Has_greater<T>()       && Boolean<Greater_result<T>>()
            && Has_less_equal<T>()    && Boolean<Less_equal_result<T>>()
            && Has_greater_equal<T>() && Boolean<Greater_equal_result<T>>();
      }
    };

  // Returns true if T (and U) are weakly ordered.
  template <typename T, typename U>
    constexpr bool Weakly_ordered()
    {
      return Weakly_ordered_concept<T, U>::check();
    }
  


  // Totally ordered (concept)
  // A type T is totally ordered if it is equality comparable and weakly
  // ordered such that the equivalence of incomparable values is the same as
  // equality.
  template <typename T, typename U>
    constexpr bool Totally_ordered()
    {
      return Equality_comparable<T, U>() && Weakly_ordered<T, U>();
    }
    


  // Movable (concept)
  // Return true if T is movable. A movable type is both destructible,
  // move constructible and mvoe assignable. Note that movable types may not be
  // copyable.
  template <typename T>
    constexpr bool Movable()
    {
      return Destructible<T>() && Move_constructible<T>() && Move_assignable<T>();
    };
    
  
  // FIXME: The Copyable, Semiregular, and Regular concepts need to be
  // Re-aligned. Copyable should be the new Semiregular. Semiregular should
  // be the new Regular (copyable and equality comparable). Regular should be
  // semiregular and totally ordered.
  
  
    
  // Movable (concept).
  // Return true if T is copyable. A copyable type is both copy constructible
  // and assignable. Note that copyable types are also inherently Movable.
  template <typename T>
    constexpr bool Copyable()
    {
      return Movable<T>() && Copy_constructible<T>() && Copy_assignable<T>();
    }
    
    
  
  // Semiregular (concept)
  // A semiregular type approximates regular data types in that they can
  // construct objects, be copied and moved, and destroyed, but they may not
  // be equality comparable. However, semiregular types are required to
  // implement the most basic form of equality: copy-equality. That is, a copy
  // must be equal to its original. However, that equality may not be true
  // value equality.
  //
  // Semiregular types are not required to be default constructible. That
  // requirement invalidates a number of useful adaptor types (e.g., iterator 
  // and range adaptors).
  template <typename T>
    constexpr bool Semiregular()
    {
      return Copyable<T>();
    }
    


  // Regular (concept)
  // A regular type is default constructible, copyable, and equality
  // comparable.
  // FIXME: Isn't this also Default_constructible? I thinks so.
  template <typename T>
    constexpr bool Regular()
    {
      return Default_constructible<T>()
          && Copyable<T>()
          && Equality_comparable<T>();
    }
    
    
    
  // TODO: The standard includes a specialized version of the Regular concepts
  // called Trivial concepts. We should have Trivially_copyable and Trivial
  // as specializations of Copyable and Regular.
    
    

  // Function concepts
  // The following concept classes, predicates, and aliases implement 
  // facilities for checking function types.
  
    
    
  // Function (concept)
  // A function type is one that can be called with a sequence of arguments,
  // and producing some (possibly void) result. In general, functions are
  // not required to be equality preserving.
  //
  // The result type of a function can be accessed using the Result_of type
  // trait (e.g., Result_of<F(Args...)>).
  template <typename F, typename... Args>
    constexpr bool Function()
    {
      return Has_call<F, Args...>();
    }
    


  // Regular Function (concept)
  // A regular function is a Function that is also equality preserving. This 
  // is a purely semantic refinement of Function, so the two are statically 
  // synonymous.
  //
  // FIXME: Should it be the 
  template <typename F, typename... Args>
    constexpr bool Regular_function()
    {
      return Function<F, Args...>() && !Same<Result_of<F(Args...)>, void>();
    }



  // Predicates
  // A predicate is a regular function whose result type is convertible to
  // bool.
  template <typename P, typename... Args>
    constexpr bool Predicate()
    {
      return Function<P, Args...>() && Convertible<Result_of<P(Args...)>, bool>();
    }
    
    
    
  // Relations
  // A Relation is a binary Predicate with a homogenous domain (i.e., the
  // argument types are the same). This can be generalized to different types
  // that share a common type.

  template <typename R, typename T, typename U = T>
    struct Relation_concept
    {
      static constexpr bool check()
      {
        return Common<T, U>()
            && Relation<R, Common_type<T, U>>()
            && Relation<R, T>()
            && Relation<R, U>()
            && Predicate<R, T, U>()
            && Predicate<R, U, T>();
      }
    };
    
  template <typename R, typename T>
    struct Relation_concept<R, T, T>
    {
      static constexpr bool check()
      {
        return Predicate<R, T, T>();
      }
    };
    
  // Returns true if R is a Relation on T x U.
  template <typename R, typename T, typename U>
    constexpr bool Relation()
    {
      return Relation_concept<R, T, U>::check();
    }



  // Relation properties
  // The followwing predicates are used to write preconditions in generic
  // algorithms or other data structures. Note that because these properties
  // cannot be evaluated at runtime, they simply return true.
  //
  // FIXME: Can these actually evaluate properties of functions? Probably
  // not -- at least not without some kind of improved runtime support.

  // Evaluates whether r is an equivalence relation. Always returns true.
  template <typename R>
    constexpr bool is_equivalence_relation(R r) { return true; }

  // Determines whether r is a strict partial order. Always returns true.
  template <typename R>
    constexpr bool is_strict_partial_order(R r) { return true; }

  // Determines whether r is a strict weak order. Always returns true.
  template <typename R>
    constexpr bool is_strict_weak_order(R r) { return true; }

  // Determines whether r is a total order. Always returns true.
  template <typename R>
    constexpr bool is_total_order(R r) { return true; }


    
  // Concepts for common associated types

  
  // Value type.
  //
  // A value type is the type of a value contained or referred to by another
  // object. For example, the value type of a container is the type of objects
  // it contains. The value type of an iterator is the type of object it
  // refers to.
  //
  // Note that value types are never references or cv-qualified.
  //
  // User-defined types model this concept by providing a nested value_type
  // declaration (i.e., T::value_type must be a valid type name). The concept
  // is implicitly satisfied for pointer and array types.
  
  // Safely deduce the associated value type, returning the associated value
  // type if it exists, or subst_failure if it doesn't.
  //
  // NOTE: This *must* be specialized for externally adapted types (e.g.,
  // pointers). We handle pointer types internally as a special case.
  template <typename T>
    struct get_value_type
    {
    private:
      template <typename X> static typename X::value_type check(X&&);
      template <typename X> static X check(X*);
      template <typename X> static X check(X const*);
      template <typename X, std::size_t N> X check(X(&)[N]);
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };

  // The Value_type alias associates a value type with another type, usually
  // the type of a sub-object or contained object.
  template <typename T>
    using Value_type = typename get_value_type<T>::type;
  
  // Return true if T has an associated value type.
  template <typename T>
    constexpr bool Has_value_type()
    {
      return Subst_succeeded<Value_type<T>>();
    }

    

  // Distance Type
  //
  // The distance type is an integral type that can express the distance
  // between two positions (e.g., iterators, pointers, and integral types
  // in general).
  //
  // This is called difference type in the standard library. We rename it
  // to be an accordance with the Palo Alto TR. Distance is also easier to
  // write than difference.
  //
  // User-defined types model this concepts by containing a nested member
  // called the difference_type (i.e., T::difference_type must be a valid
  // type name). The distance type is implicitly defined for integral types
  // pointers, and statically sized arrays.


  // Safely deduce the distance type.
  template <typename T>
    struct get_distance_type
    {
    private:
      template <typename X> 
        static auto check(const X&, Requires<!(Integral<X>() || Pointer<X>())>* = {})
          -> typename X::difference_type;
      
      template <typename X> 
        static auto check(const X&, Requires<Integral<X>() || Pointer<X>()>* = {})
          -> std::ptrdiff_t;
          
      template <typename X, std::size_t N>
        static std::ptrdiff_t check(const X(&)[N]);
        
      template <typename X>
        static std::ptrdiff_t check(std::initializer_list<X>);

      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };
  
  // An alias to the associated distance type, if supported.
  template <typename T>
    using Distance_type = typename get_distance_type<T>::type;
    
  // Returns true if T has an associated difference type.
  template <typename T>
    constexpr bool Has_distance_type()
    {
      return Subst_succeeded<Distance_type<T>>();
    }


  
  // Streaming concepts
  // There are 3 concepts describing streaming operations.
  // - Input_streamable<T>
  // - Output_streamable<T>
  // - Streamable<T>



  // Input streamable
  // Describes types that can be read from a stream using the input stream
  // operator (>>). There are two overloads of this concept:
  // - Input_streamable<T>
  // - Input_streamable<S, T>
  //
  // The first determines if T can be read from a stream derived from either 
  // istream or wistream. Note that the two are equivalent are equivalent if 
  // the >> overloads are defined in terms of basic_istream.
  //
  // The second determines if T can be read from the stream S.
  
  // Safely deduce the result type of an input streaming operator, s >> x, for
  // any Stream s.
  template <typename S, typename T>
    struct get_input_stream_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X& x, Y& y) -> decltype(x >> y);
      
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<S&>(), std::declval<T&>()));
    };

  // An alias for the result type of the expression, s >> x, for any Stream s.
  template <typename S, typename T>
    using Input_stream_result = typename get_input_stream_result<S, T>::type;

  // The Input_streamable concept.
  template <typename S, typename T>
    struct Input_streamable_concept
    {
      static constexpr bool check()
      {
        return Subst_succeeded<Input_stream_result<S, T>>();
      }
    };
    
  template <typename T>
    struct Input_streamable_concept<T, default_t>
    {
      static constexpr bool check()
      {
        return Subst_succeeded<Input_stream_result<std::istream, T>>();
      }
    };

  // Return true if values of type T can be read from an input stream.
  template <typename T, typename U = default_t>
    constexpr bool Input_streamable()
    {
      return Input_streamable_concept<T, U>::check();
    }



  // Output streamable
  // Describes types that can be written to a stream using output stream
  // operator (<<). There are two overloads of this concept:
  // - Output_streamable<T>
  // - Output_streamable<S, T>
  //
  // The first determines if T can be written to a stream derived from either 
  // ostream or wostream. Note that the two are equivalent are equivalent if 
  // the >> overloads are defined in terms of basic_ostream.
  //
  // The second determines if T can be written to the stream S.
  
  // Safely deduce the result type of an input streaming operator, s << x, for
  // any Stream s.
  template <typename S, typename T>
    struct get_output_stream_result
    {
    private:
      template <typename X, typename Y>
        static auto check(X& x, Y const& y) -> decltype(x << y);
      
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<S&>(), std::declval<T const&>()));
    };

  // An alias for the result type of the expression, s >> x, for any Stream s.
  template <typename S, typename T>
    using Output_stream_result = typename get_output_stream_result<S, T>::type;

  // The Input_streamable concept.
  template <typename S, typename T>
    struct Output_streamable_concept
    {
      static constexpr bool check()
      {
        return Subst_succeeded<Output_stream_result<S, T>>();
      }
    };
    
  template <typename T>
    struct Output_streamable_concept<T, default_t>
    {
      static constexpr bool check()
      {
        return Subst_succeeded<Output_stream_result<std::ostream, T>>();
      }
    };

  // Return true if values of type T can be read from an input stream.
  template <typename T, typename U = default_t>
    constexpr bool Output_streamable()
    {
      return Output_streamable_concept<T, U>::check();
    }



  // Return true if the values of T can be streamed: read from and written to
  // a stream.
  template <typename T, typename U = default_t>
    constexpr bool Streamable()
    {
      return Input_streamable<T, U>() && Output_streamable<T, U>();
    }


} // namespace origin

#endif
