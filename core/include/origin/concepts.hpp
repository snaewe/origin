// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONCEPTS_HPP
#define ORIGIN_CONCEPTS_HPP

#include <functional>
#include <initializer_list>

#include <origin/traits.hpp>

namespace origin
{
  // Declarations
  template<typename T, typename U = T> constexpr bool Equality_comparable();
  template<typename T, typename U = T> constexpr bool Totally_ordered();
  
  
  // The Axiom class is the base class of all axioms specifications. It derives
  // from bool_constant<true>, making axiom objects convertible to bool.
  struct axiom_type
  {
    explicit operator bool() const
    {
      return true;
    }
  };
  
  
  
  // Properties of relations
    
  // A relation is reflexive if, for all a, r(a, a) is true.
  template<typename R>
    struct reflexive_property : axiom_type
    {
      reflexive_property(R r) : r{r} { }
      
      template<typename T>
        bool operator()(T a) const { return r(a, a); }
      
      R r;
    };
    
  template<typename R>
    reflexive_property<R> reflexive(R r) { return r; }



  // A relation is irreflexive if, for all a, r(a, a) is false.
  template<typename R>
    struct irreflexive_property : axiom_type
    {
      irreflexive_property(R r) : r{r} { }
      
      template<typename T>
        bool operator()(R r, T a, T b) const { return !r(a, a); }
      
      R r;
    };
    
  template<typename R>
    irreflexive_property<R> irreflexive(R r) { return r; }



  // A relation is symmetric if, for all a and b, r(a, b) => r(b, a).
  template<typename R>
    struct symmetric_property : axiom_type
    {
      symmetric_property(R r) : r{r} { }
    
      template<typename T>
        bool operator()(R r, T a, T b) const 
        { 
          return r(a, b) ? r(b, a) : true; 
        }
      
      R r;
    };

  template<typename R>
    symmetric_property<R> symmetric(R r) { return r; }


    
  // A relation is asymmetric if, for all a and b, r(a, b) => !r(b, a)
  template<typename R>
    struct asymmetric_property : axiom_type
    {
      asymmetric_property(R r) : r{r} { }
    
      template<typename T>
        bool operator()(T a, T b) const
        {
          return r(a, b) ? !r(b, a) : true;
        }
      
      R r;
    };
  
  template<typename R>
    asymmetric_property<R> asymmetric(R r) { return r; }
    
  
  
  // A relation is antisymmetric if, for all a and b, 
  // r(a, b) && r(b, a) => a == b.
  template<typename R>
    struct antisymmetric_property : axiom_type
    {
      antisymmetric_property(R r) : r{r} { }
      
      template<typename T>
        bool operator()(T a, T b) const
        {
          static_assert(Equality_comparable<T>(), "");
          
          return r(a, b) && r(b, a) ? a == b : true;
        }
      
      R r;
    };
    
  template<typename R>
    antisymmetric_property<R> antisymmetric(R r) { return r; }



  // A relation is transitive if, for all a, b, and c, 
  // r(a, b) && r(b, c) => r(a, c)
  template<typename R>
    struct transitive_property : axiom_type
    {
      transitive_property(R r) : r{r} { }
      
      template<typename T>
        bool operator()(T a, T b, T c) const
        {
          return r(a, b) && r(b, c) ? r(a, c) : true;
        }
      
      R r;
    };
    
  template<typename R>
    transitive_property<R> transitive(R r)
    {
      return r;
    }
    
  
  
  // A relation is trichotomous if, for all a and b, exactly one of the 
  // following are true: a < b, b < a, or a == b.
  template<typename R>
    struct trichotomous_property : axiom_type
    {
      trichotomous_property(R r) : r{r} { }
      
      template<typename T>
        bool operator()(T a, T b) const
        {
          static_assert(Equality_comparable<T>(), "");

          if(r(a, b))
            return !(r(b, a) || a == b);
          else if(r(b, a))
            return a != b;
          else
            return a == b;
        }
      
      R r;
    };
    
  template<typename R>
    trichotomous_property<R> trichotomous(R r) { return r; }
    
    
    
  // An equivalence relation is reflexive, symmetric, and transitive.
  template<typename R>
    struct equivalence_relation_properties : axiom_type
    {
      equivalence_relation_properties(R r)
        : reflexive{r}, symmetric{r}, transitive{r}
      { }

      reflexive_property<R> reflexive;
      symmetric_property<R> symmetric;
      transitive_property<R> transitive;
    };
    
  template<typename R>
    equivalence_relation_properties<R> equivalence_relation(R r)
    {
      return r;
    }
    
    
    
  // A strict weak ordering is irreflexive, asymmetric, and transitive.
  template<typename R>
    struct strict_weak_ordering_properties : axiom_type
    {
      strict_weak_ordering_properties(R r)
        : irreflexive{r}, asymmetric{r}, transitive{r}
      { }
    
      irreflexive_property<R> irreflexive;
      asymmetric_property<R> asymmetric;
      transitive_property<R> transitive;
    };
    
  template<typename R>
    strict_weak_ordering_properties<R> strict_weak_ordering(R r)
    {
      return r;
    }
    
    
    
  // A strict total ordering is antisymmetric, transitive, and trichotomous.
  template<typename R>
    struct strict_total_ordering_properties : axiom_type
    {
      strict_total_ordering_properties(R r)
        : antisymmetric{r}, transitive{r}, trichotomous{r}
      { }
    
      antisymmetric_property<R> antisymmetric;
      transitive_property<R> transitive;
      trichotomous_property<R> trichotomous;
    };

  template<typename R>
    strict_total_ordering_properties<R> strict_total_ordering(R r)
    {
      return r;
    }
    
    
    
  // The Equality_comparable concept defines the syntax and semantics of
  // comparing for value equality.
  //
  // When given two types, the requirements are different.
  //
  // FIXME: Refactor requirements so Common<> won't generate lookup errors.
  template<typename T, typename U = T>
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
      
      static bool test(T a, U b)
      {
        using C = Common_type<T, U>;
        return (a == b) == (C{a} == C{b})
            && (b == a) == (C{b} == C{a})
            && (a != b) == (C{a} != C{b})
            && (b != a) == (C{b} != C{a});
      }
    };
  
  template<typename T>
    struct Equality_comparable_concept<T, T>
    {
      static constexpr bool check()
      {
        return Has_equal<T>()     && Boolean<Equal_result<T>>()
            && Has_not_equal<T>() && Boolean<Not_equal_result<T>>();
      }


      // Semantics
      
      // Return the equivalence properties for testing
      static equivalence_relation_properties<std::equal_to<T>> equivalence()
      {
        return equivalence_relation(std::equal_to<T>{});
      }

      static bool test_not_equal(T a, T b) 
      {
        return (a != b) == !(a == b);
      }
    };
    
  // Returns true if the values of T (and U) can be compared for value equality.
  template<typename T, typename U>
    constexpr bool Equality_comparable()
    {
      return Equality_comparable_concept<T, U>::check();
    }

  
  
  // The Totally_ordered concept defines the syntax and semantics of ordering
  // values.
  // FIXME: Refactor requirements so Common<> won't generate lookup errors.
  template<typename T, typename U>
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
    
  template<typename T>
    struct Totally_ordered_concept<T, T>
    {
      static constexpr bool check()
      {
        return Has_less<T>()          && Boolean<Less_result<T>>()
            && Has_greater<T>()       && Boolean<Greater_result<T>>()
            && Has_less_equal<T>()    && Boolean<Less_equal_result<T>>()
            && Has_greater_equal<T>() && Boolean<Greater_equal_result<T>>();
      }

      // Semantics
      
      // Return the ordering properties for testing.
      static strict_total_ordering_properties<std::less<T>> ordering()
      {
        return srict_total_ordering(std::less<T>{});
      }
      
      static bool test_greater(T a, T b)
      {
        return a > b == b < a;
      }
      
      static bool test_less_equal(T a, T b)
      {
        return a <= b == !(b < a);
      }
      
      static bool test_greater_equal(T a, T b)
      {
        return a >= b == !(a < b);
      }
    };

  // Returns true if the values of T (and U) can be totally ordered using
  // the standard inequality operators.
  template<typename T, typename U>
    constexpr bool Totally_ordered()
    {
      return Totally_ordered_concept<T, U>::check();
    }
    
    
    
  // The Movable concept...
  template<typename T>
    struct Movable_concept
    {
      static constexpr bool check()
      {
        return Move_constructible<T>() && Move_assignable<T>();
      }
      
      static bool test()
      {
        // FIXME: Write semantics.
        return true;
      }
    };
    
  // Return true if T is movable.
  template<typename T>
    constexpr bool Movable()
    {
      return Movable_concept<T>::check();
    };
    
  
    
  // The Copyable concept
  template<typename T>
    struct Copyable_concept
    {
      static constexpr bool check()
      {
        return Copy_constructible<T>() && Copy_assignable<T>();
      }
      
      static bool test()
      {
        // FIXME: Write semantics.
        return false;
      }
    };
    
  // Return true if T is copyabe.
  template<typename T>
    constexpr bool Copyable()
    {
      return Copyable_concept<T>::check();
    }
    
    
  
  // Semiregular (concept)
  // A semiregular type approximates regular data types in that they can
  // construct objects, be copied and moved, and destroyed.
  //
  // Semiregular types are not required to be default constructible. That
  // requirement invalidates a number of useful adaptor types (e.g., iterator 
  // and range adaptors).
  
  // Specification of the semiregular concept.
  template<typename T>
    struct Semiregular_concept
    {
      static constexpr bool check()
      {
        return Destructible<T>()  // FIXME: Nothrow_destructible
            && Move_constructible<T>()
            && Move_assignable<T>()
            && Copy_constructible<T>()
            && Copy_assignable<T>();
      }
      
      static bool test()
      {
        // FIXME: Write semantics.
        return true;
      }
    };

  // Returns true if T is a Semiregular type.
  template<typename T>
    constexpr bool Semiregular()
    {
      return Semiregular_concept<T>::check();
    }
    
    
    
  // A Regular type is a Semiregular type that is also equality comparable.
  template<typename T>
    struct Regular_concept
    {
      static constexpr bool check()
      {
        return Semiregular<T>() && Equality_comparable<T>();
      }
      
      static bool test(T a, T b, T c)
      {
        return Semiregular_concept<T>::test(a)
            && Equality_comparable_concept<T>::test(a, b, c);
      }
    };

    
  // Returns true if T is a Regular type.
  template<typename T>
    constexpr bool Regular()
    {
      return Regular_concept<T>::check();
    }
    
    
    
  // Function concepts
  // The following concept classes, predicates, and aliases implement 
  // facilities for checking function types.
  
    
    
  // Function (concept)
  // A function type is one that can be called with a sequence of arguments,
  // and producing some (possibly void) result. In general, functions are
  // not required to be equality preserving.
  
  // A type F is a Function if it can be called with the given arguments.
  template<typename F, typename... Args>
    struct Function_concept
    {
      static constexpr bool check()
      {
        return Has_call<F, Args...>();
      }
    };
  
  template<typename F, typename... Args>
    constexpr bool Function()
    {
      return Function_concept<F, Args...>::check();
    }
    
  // The result type of a Function.
  template<typename F, typename... Args>
    using Result_type = Call_result<F, Args...>;
    


  // Regular Function (concept)
  // A regular function is a Function that is also equality preserving. This 
  // is a purely semantic refinement of Function, so the two are statically 
  // synonymous.
    
  template<typename F, typename... Args>
    struct Regular_function_concept
    {
      static constexpr bool check()
      {
        return Function<F, Args...>();
      }
      
      static bool test(F f, std::tuple<Args...> a, std::tuple<Args...> b)
      {
        // a == b ? f(a...) == f(b...) : true;
        return true;
      }
    };
    
  // Return true if F is a regular function.
  template<typename F, typename... Args>
    constexpr bool Regular_function()
    {
      return Regular_function_concept<F, Args...>::check();
    }

    
    
  // Predicates
  // A predicate is a regular function whose result type is convertible to
  // bool.

  // A helper class to check syntactic requirements.
  template<bool Prereqs, typename P, typename... Args>
    struct Predicate_requirements
    {
      static constexpr bool check() { return false; }
    };

  template<typename P, typename... Args>
    struct Predicate_requirements<true, P, Args...>
    {
      static constexpr bool check() 
      {
        return Convertible<Result_type<P, Args...>, bool>();
      }
    };

  // Specification of the Predicate concept.
  template<typename P, typename... Args>
    struct Predicate_concept
    {
      static constexpr bool check()
      {
        return Predicate_requirements<Function<P, Args...>(), P, Args...>::check();
      }
    };
    
  // Return true if P is a Predicate.
  template<typename P, typename... Args>
    constexpr bool Predicate()
    {
      return Predicate_concept<P, Args...>::check();
    }
    
    
    
  // Relations
  // A Relation is a binary Predicate with a homogenous domain (i.e., the
  // argument types are the same). This can be generalized to different types
  // that share a common type.
  
  // A helper class for checking syntactic requirements of generalized
  // Relations.
  template<bool Prereqs, typename R, typename T, typename U>
    struct Relation_requirements
    {
      static constexpr bool check() { return false; }
    };
    
    
  template<typename R, typename T, typename U>
    struct Relation_requirements<true, R, T, U>
    {
      static constexpr bool check()
      {
        return Predicate<R, T>()
            && Predicate<R, U>()
            && Predicate<Common_type<T, U>>()
            && Predicate<R, T, U>()
            && Predicate<R, U, T>();
      }
      
      static bool test(R r, T a, U b)
      {
        using C = Common_type<T, U>;
        return r(a, b) == r(C{a}, C{b})
            && r(b, a) == r(C{b}, C{a});
      }
    };
  
  
  // Specification of Relation syntax and semantics.
  template<typename R, typename T, typename U = T>
    struct Relation_concept
    {
      static constexpr bool check()
      {
        return Relation_requirements<Common<T, U>(), R, T, U>();
      }
    };
    
  template<typename R, typename T>
    struct Relation_concept<R, T, T>
    {
      static constexpr bool check()
      {
        return Predicate<R, T, T>();
      }
    };
    
  // Returns true if R is a Relation on T x U.
  template<typename R, typename T, typename U = T>
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
  template<typename T>
    struct get_value_type
    {
    private:
      template<typename X> static typename X::value_type check(X&&);
      template<typename X> static X check(X*);
      template<typename X> static X check(X const*);
      template<typename X, std::size_t N> X check(X(&)[N]);
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };

  // The Value_type alias associates a value type with another type, usually
  // the type of a sub-object or contained object.
  template<typename T>
    using Value_type = typename get_value_type<T>::type;
  
  // Return true if T has an associated value type.
  template<typename T>
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
  template<typename T>
    struct get_distance_type
    {
    private:
      template<typename X> 
        static auto check(const X&, Requires<!(Integral<X>() || Pointer<X>())>* = {})
          -> typename X::difference_type;
      
      template<typename X> 
        static auto check(const X&, Requires<Integral<X>() || Pointer<X>()>* = {})
          -> std::ptrdiff_t;
          
      template<typename X, std::size_t N>
        static std::ptrdiff_t check(const X(&)[N]);
        
      template<typename X>
        static std::ptrdiff_t check(std::initializer_list<X>);

      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };
  
  // An alias to the associated distance type, if supported.
  template<typename T>
    using Distance_type = typename get_distance_type<T>::type;
    
  // Returns true if T has an associated difference type.
  template<typename T>
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
  template<typename S, typename T>
    struct get_input_stream_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X& x, Y& y) -> decltype(x >> y);
      
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<S&>(), std::declval<T&>()));
    };

  // An alias for the result type of the expression, s >> x, for any Stream s.
  template<typename S, typename T>
    using Input_stream_result = typename get_input_stream_result<S, T>::type;

  // The Input_streamable concept.
  template<typename S, typename T>
    struct Input_streamable_concept
    {
      static constexpr bool check()
      {
        return Subst_succeeded<Input_stream_result<S, T>>();
      }
    };
    
  template<typename T>
    struct Input_streamable_concept<T, default_t>
    {
      static constexpr bool check()
      {
        return Subst_succeeded<Input_stream_result<std::istream, T>>();
      }
    };

  // Return true if values of type T can be read from an input stream.
  template<typename T, typename U = default_t>
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
  template<typename S, typename T>
    struct get_output_stream_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X& x, Y const& y) -> decltype(x << y);
      
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<S&>(), std::declval<T const&>()));
    };

  // An alias for the result type of the expression, s >> x, for any Stream s.
  template<typename S, typename T>
    using Output_stream_result = typename get_output_stream_result<S, T>::type;

  // The Input_streamable concept.
  template<typename S, typename T>
    struct Output_streamable_concept
    {
      static constexpr bool check()
      {
        return Subst_succeeded<Output_stream_result<S, T>>();
      }
    };
    
  template<typename T>
    struct Output_streamable_concept<T, default_t>
    {
      static constexpr bool check()
      {
        return Subst_succeeded<Output_stream_result<std::ostream, T>>();
      }
    };

  // Return true if values of type T can be read from an input stream.
  template<typename T, typename U = default_t>
    constexpr bool Output_streamable()
    {
      return Output_streamable_concept<T, U>::check();
    }



  // Return true if the values of T can be streamed: read from and written to
  // a stream.
  template<typename T, typename U = default_t>
    constexpr bool Streamable()
    {
      return Input_streamable<T, U>() && Output_streamable<T, U>();
    }


} // namespace origin

#endif
