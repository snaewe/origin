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

  // Properties of relations
  //
  // FIXME: Document this correctly.
  //
  // This framework is used to support the assertion and testing of properties
  // of (binary) relations. Each of these properties is comprised of three
  // components: 
  //
  //    - a test function that evaluates the property over some arguments
  //    - a constructor function
  //    - a predicate that always returns true.
  //
  // The test functions do not evaluate the entire domain of a relation r; that
  // would be impossible. Instead, they evaluate the semantics of the relation
  // for a particular set of input values. This should allow users to do e.g.,
  // boundary value testing to help ensure that their function arguments 
  // actually do what they are supposed to.
  //
  // The predicate returns true so that no assertions fail. We can't actually
  // assert the property since it's quantified over the domain of the relation,
  // but we'd still like to document the requirement. It should be a queue for
  // programmers to verify function arguments.
  
  
  // FIXME: Clean this section up.

  template <typename Expr> struct common_type_equivalence_property;
  template <typename R> struct reflexive_property;
  template <typename R> struct irreflexive_property;
  template <typename R> struct symmetric_property;
  template <typename R> struct asymmetric_property;
  template <typename R> struct antisymmetric_property;
  template <typename R> struct transitive_property;
  template <typename R> struct trichotomous_property;
  template <typename R> struct equivalence_relation_spec;
  template <typename R> struct strict_partial_order_spec;
  template <typename R> struct strict_weak_order_spec;
  template <typename R> struct strict_total_order_spec;
  template <typename R, typename N> struct negation_property;

   

  // Evaluates whether r is an reflexive relation. Always returns true.
  template <typename R>
    constexpr bool is_reflexive_relation(R r) { return true; }


  // Evaluates whether r is an irreflexive relation. Always returns true.
  template <typename R>
    constexpr bool is_irreflexive_relation(R r) { return true; }


  // Evaluates whether r is a symmetric relation. Always returns true.
  template <typename R>
    constexpr bool is_symmetric_relation(R r) { return true; }

  // Returns true if r is an asymmetric relation., Always returns true.
  template <typename R>
    constexpr bool is_asymmetric_relation(R r) { return true; }
    

  // Evaluates whether r is an antisymmetric property. Always returns true.
  template <typename R>
    constexpr bool is_antisymmetric_property(R r) { return true; }


  // Evaluates whether r is a transitive relation. Always returns true.
  template <typename R>
    constexpr bool is_transitive_relation(R r) { return true; }
    

  // Evaluates whether r is a trichotomous relation. Always returns true.
  template <typename R>
    constexpr bool is_trichotomous_relation(R r) { return true; }
    

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

  // The specification of equality semantics.
  template <typename T, typename U = T> struct equality_comparable_semantics;


    
  // TODO: Add Weakly_ordered as a concept.
    

  // Totally ordered (concept)
  // The totally ordered concept defines the syntax and semantics of ordering
  // values.
  template <typename T, typename U>
    struct Totally_ordered_concept
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
      
      static bool test(T a, U b)
      {
        using C = Common_type<T, U>;
        return (a < b) == (C{a} < C{b})
            && (b < a) == (C{b} < C{a})
            && (a > b) == (C{a} > C{b})
            && (b > a) == (C{b} > C{a})
            && (a <= b) == (C{a} <= C{b})
            && (a <= b) == (C{b} <= C{a})
            && (a >= b) == (C{a} >= C{b})
            && (b >= a) == (C{b} >= C{a});
      }
    };
    
  template <typename T>
    struct Totally_ordered_concept<T, T>
    {
      static constexpr bool check()
      {
        return Has_less<T>()          && Boolean<Less_result<T>>()
            && Has_greater<T>()       && Boolean<Greater_result<T>>()
            && Has_less_equal<T>()    && Boolean<Less_equal_result<T>>()
            && Has_greater_equal<T>() && Boolean<Greater_equal_result<T>>();
      }
    };

  // Returns true if the values of T (and U) can be totally ordered using
  // the standard inequality operators.
  template <typename T, typename U>
    constexpr bool Totally_ordered()
    {
      return Totally_ordered_concept<T, U>::check();
    }
    
  // The specification of total ordering semantics.
  template <typename T, typename U = T> struct totally_ordered_semantics;
    
    
  // Movable (concept)
  // Return true if T is movable. A movable type is both destructible,
  // move constructible and mvoe assignable. Note that movable types may not be
  // copyable.
  template <typename T>
    constexpr bool Movable()
    {
      return Destructible<T>() && Move_constructible<T>() && Move_assignable<T>();
    };
    
  template <typename T> struct move_semantics;
  
  
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
  // Returns true if T is a Regular type. A Regular type is a Semiregular type 
  // that is also equality comparable.
  //
  // FIXME: Isn't this also Default_constructible? I thinks so.
  template <typename T>
    constexpr bool Regular()
    {
      return Semiregular<T>() && Equality_comparable<T>();
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
  template <typename F, typename... Args>
    constexpr bool Regular_function()
    {
      return Function<F, Args...>();
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

  // Specification of Relation syntax and semantics.
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
