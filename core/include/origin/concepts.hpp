// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONCEPTS_HPP
#define ORIGIN_CONCEPTS_HPP

#include <functional>

#include <origin/traits.hpp>

namespace origin
{
  // Declarations
  template<typename T, typename U = T> constexpr bool Equality_comparable();
  template<typename T, typename U = T> constexpr bool Totally_ordered();
  
  
  // Relational properties
  
  // For all a, r(a, a) is true.
  template<typename R, typename T>
    bool reflexive(R r, T a)
    {
      return r(a, a);
    }
    
  // For all a, r(a, a) is false.
  template<typename R, typename T>
    bool irreflexive(R r, T a, T b)
    {
      return !r(a, a);
    }
    
  // For all a and b, r(a, b) => r(b, a)
  template<typename R, typename T>
    bool symmetric(R r, T a, T b)
    {
      return r(a, b) ? r(b, a) : true;
    }

    
  // For all a and b, r(a, b) => !r(b, a)
  template<typename R, typename T>
    bool asymmetric(R r, T a, T b)
    {
      return r(a, b) ? !r(b, a) : true;
    }

  // For all a, b, and c, r(a, b) && r(b, c) => r(a, c)
  template<typename R, typename T>
    bool transitive(R r, T a, T b, T c)
    {
      return r(a, b) && r(b, c) ? r(a, c) : true;
    }
    
  // R is reflexive, symmetric, and transitive.
  template<typename R, typename T>
    bool equivalence_relation(R r, T a, T b, T c)
    {
      return reflexive(r, a) 
          && symmetric(r, a, b) 
          && transitive(r, a, b, c);
    }
    
  template<typename R, typename T>
    bool strict_weak_ordering(R r, T a, T b, T c)
    {
      return irreflexive(r, a)
          && asymmetric(r, a, b)
          && transitive(r, a, b, c);
    }
    
  template<typename R, typename T>
    bool strict_total_ordering(R r, T a, T b, T c)
    {
      static_assert(Equality_comparable<T>(), "");

      // FIXME: The totality condition needs to exclusive
      return strict_weak_ordering(r, a, b, c)
          && (r(a, b) || r(b, a) || a == b);  
    }


    
  // The Equality_comparable concept defines the syntax and semantics of
  // comparing for value equality.
  //
  // When given two types, the requirements are different.
  template<typename T, typename U = T>
    struct Equality_comparable_concept
    {
      static constexpr bool check()
      {
        return Common<T, U>() 
            && Equality_comparable<T>()
            && Equality_comparable<U>()
            && Equality_comparable<Common_type<T, U>>()
            && Has_equal<T, U>()     && Convertible<Equal_result<T, U>, bool>()
            && Has_equal<U, T>()     && Convertible<Equal_result<U, T>, bool>()
            && Has_not_equal<T, U>() && Convertible<Equal_result<T, U>, bool>()
            && Has_not_equal<U, T>() && Convertible<Equal_result<T, U>, bool>();
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
        return Has_equal<T>()     && Convertible<Equal_result<T>, bool>()
            && Has_not_equal<T>() && Convertible<Not_equal_result<T>, bool>();
      }

      static bool test(T a, T b, T c) 
      {
        std::equal_to<T> eq;
        return reflexive(eq, a)
            && symmetric(eq, a, b)
            && transtitive(eq, a, b, c)
            && (a != b) == !(a == b);
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
  template<typename T, typename U>
    struct Totally_ordered_concept
    {
      static constexpr bool check()
      {
        return Common<T, U>()
            && Totally_ordered<T>()
            && Totally_ordered<T>()
            && Totally_ordered<Common_type<T, U>>()
            && Has_less<T, U>()          && Convertible<Less_result<T, U>, bool>()
            && Has_less<U, T>()          && Convertible<Less_result<U, T>, bool>()
            && Has_greater<T, U>()       && Convertible<Greater_result<T, U>, bool>()
            && Has_greater<U, T>()       && Convertible<Greater_result<U, T>, bool>()
            && Has_less_equal<T, U>()    && Convertible<Less_equal_result<T, U>, bool>()
            && Has_less_equal<U, T>()    && Convertible<Less_equal_result<U, T>, bool>()
            && Has_greater_equal<T, U>() && Convertible<Greater_equal_result<T, U>, bool>()
            && Has_greater_equal<U, T>() && Convertible<Greater_equal_result<U, T>, bool>();
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
        return Has_less<T>()          && Convertible<Less_result<T>, bool>()
            && Has_greater<T>()       && Convertible<Greater_result<T>, bool>()
            && Has_less_equal<T>()    && Convertible<Less_equal_result<T>, bool>()
            && Has_greater_equal<T>() && Convertible<Greater_equal_result<T>, bool>();
      }
      
      static bool test(T a, T b, T c)
      {
        std::less<T> less;
        return strict_total_ordering(less, a, b, c)
            && (a > b) == (b < a)
            && (a <= b) == !(b < a)
            && (a >= b) == !(a < b);
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
    
    
    
  // A semiregular type approximates regular data types. It is copyable, 
  // movable, default constructible, and doesn't have any weird overloads
  // (e.g., comma, new, etc.).
  template<typename T>
    struct Semiregular_concept
    {
      static constexpr bool check()
      {
        return Destructible<T>()
            && Default_constructible<T>() // FIXME: Nothrow?
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
  
  
  // A type F is a Function if it can be called with the given arguments.
  template<typename F, typename... Args>
    struct Function_concept
    {
    };
  
  template<typename F, typename... Args>
    constexpr bool Function()
    {
      return Function_concept<F, Args...>::check();
    }
    


  // A type F is a Regular_function if it is a Function and is also preserves
  // equality.
  template<typename F, typename... Args>
    struct Regular_function_concept
    {
    };
    
  template<typename F, typename... Args>
    constexpr bool Regular_function()
    {
      return Regular_function_concept<F, Args...>::check();
    }
    
    
    
  // A Predicate (taking some arguments) is a Function that returns a
  // boolean value.
  template<typename P, typename... Args>
    struct Predicate_concept
    {
      
    };
    
  template<typename P, typename... Args>
    constexpr bool Predicate()
    {
      return Predicate_concept<P, Args...>::check();
    }
    
    
    
  // A Relation is a binary Predicate with a homogenous domain (i.e., the
  // argument types are the same). This can be generalized to different types
  // that share a common type.
  template<typename T, typename U = T>
    struct Relation_concept
    {
    };
    
  template<typename T>
    struct Relation_concept<T, T>
    {
    };
    
  template<typename T, typename U = T>
    constexpr bool Relation()
    {
      return Relation_concept<T, U>::check();
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
  // is implicitly satisfied for pointer types.
  
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
  // and pointers.
  //
  // FIXME: What about a nested distance_type?

  // Safely deduce the distance type.
  template<typename T>
    struct get_distance_type
    {
    private:
      template<typename X> 
        static auto check(X&&, Requires<!(Integral<X>() || Pointer<X>())>* = {})
          -> typename X::difference_type;
      
      template<typename X> 
        static auto check(X&&, Requires<Integral<X>() || Pointer<X>()>* = {})
          -> std::ptrdiff_t;

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

    
} // namespace origin

#endif
