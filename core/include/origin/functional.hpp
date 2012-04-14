// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_FUNCTIONAL_HPP
#define ORIGIN_FUNCTIONAL_HPP

#include <functional>

#include <origin/utility.hpp>
#include <origin/traits.hpp>
#include <origin/concepts_fwd.hpp>

namespace origin
{
  // Function adaptors
  // The set of function adaptors usable in std + origin are:
  //
  //    nullary_function
  //    unary_function
  //    binary_function
  //    ternary_function
  
  
  // Nullary function (function adaptor)
  // The nullary function adaptor explicitly specifies the result type of a
  // function object.
  template <typename Result>
    struct nullary_function
    {
      using result_type = Result;
    };
  
    
    
  // Ternary function (function adaptor)
  template <typename Arg1, typename Arg2, typename Arg3, typename Result>
    struct ternary_function
    {
      using first_argument_type = Arg1;
      using second_argument_type = Arg2;
      using third_argument_type = Arg3;
      using result_type = Result;
    };
  
    
    
  // Result type (trait)
  // The result type of a type is an associated type named result_type.

  // Safely deduce the argument of the callable type F.
  template <typename F>
    struct get_result_type
    {
    private:
      template <typename X>
        static typename X::result_type check(const X&);
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<F>()));
    };

  // An alias for the result type of a unary function.
  template <typename F>
    using Result_type = typename get_result_type<F>::type;
    
  // Returns true if F takes a single result.
  template <typename F>
    constexpr bool Has_result_type()
    {
      return Subst_succeeded<Result_type<F>>();
    }    


  // Argument type (trait)
  // Deduce the argument type of a unary callable type.
  //
  // Note that this deduction will fail for polymorphic function objects. In 
  // Origin, polymorphic function objects will result have the result type
  // unspecified_t, so they will partially work.
  //
  // FIXME: Adapt this type trait for function pointers, member function
  // pointers, and lambda expressions.
    
  // Safely deduce the argument of the callable type F.
  template <typename F>
    struct get_argument_type
    {
    private:
      template <typename X>
        static typename X::argument_type check(const X&);
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<F>));
    };

  // An alias for the argument type of a unary function.
  template <typename F>
    using Argument_type = typename get_argument_type<F>::type;
    
  // Returns true if F takes a single argument.
  template <typename F>
    constexpr bool Has_argument_type()
    {
      return Subst_succeeded<Argument_type<F>>();
    }
  

  // First argument type (trait)
  //
  // FIXME: Adapt this trait for the full set of callable types.
  
  // Safely deduce the first argument type of the callable type F.
  template <typename F>
    struct get_first_argument_type
    {
    private:
      template <typename X>
        static typename X::first_argument_type check(const X&);
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<F>));
    };

  // An alias for the argument type of a unary function.
  template <typename F>
    using First_argument_type = typename get_first_argument_type<F>::type;
    
  // Returns true if F takes a first argument (among 2 or more).
  template <typename F>
    constexpr bool Has_first_argument()
    {
      return Subst_succeeded<First_argument_type<F>>();
    }
    
    
  
  // Second argument type (trait)
  //
  // FIXME: Adapt this trait for the full set of callable type.
  
  // Safely deduce the second argument type of the callable type F.
  template <typename F>
    struct get_second_argument_type
    {
    private:
      template <typename X>
        static typename X::second_argument_type check(const X&);
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<F>));
    };

  // An alias for the argument type of a unary function.
  template <typename F>
    using Second_argument_type = typename get_second_argument_type<F>::type;
    
  // Returns true if F takes a second argument.
  template <typename F>
    constexpr bool Has_second_argument()
    {
      return Subst_succeeded<Second_argument_type<F>>();
    }

    

  // Third argument type (trait)
  // FIXME: Adapt this trait for the full set of callable type.
  
  // Safely deduce the third argument type of the given function type.
  template <typename F>
    struct get_third_argument_type
    {
    private:
      template <typename X>
        static typename X::third_argument_type check(const X&);
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<F>));
    };

  // An alias for the argument type of a unary function.
  template <typename F>
    using Third_argument_type = typename get_third_argument_type<F>::type;
    
  // Returns true if F takes a third argument.
  template <typename F>
    constexpr bool Has_third_argument()
    {
      return Subst_succeeded<Third_argument_type<F>>();
    }
  
  
  
  // Unary function (concept)
  // Returns true if F is a unary function.
  //
  // Note that this concept may yield false negatives for polymorphic function
  // objects.
  template <typename F>
    constexpr bool Unary_function() 
    { 
      return Subst_succeeded<Argument_type<F>>(); 
    }

    
    
  // Binary function (concept)
  // Returns true if F is a binary function.
  //
  // Note that this concept may yield false negatives for polymorphic function
  // objects.
  template <typename F>
    constexpr bool Binary_function()
    {
      return Has_second_argument<F>() && !Has_third_argument<F>();
    }
    

    
  // Ternary function (concept)
  // Returns trie if F is a ternary function.
  //
  // Note that this concept may yield false negatives for polymorphic function
  // objects.
  template <typename F>
    constexpr bool Ternary_function()
    {
      return Has_third_argument<F>();
    }
  


  // Identity (function)
  // The identity function object returns its argument as is.
  struct identity
  {
    template <typename T>
      const T& operator()(const T& x) const { return x; }
  };



  // Constant function (function)
  // A constant function is a nullary function that returns the same value.
  template <typename T>
    struct constant_function
    {
      T value;

      constant_function(const T& value) : value(value) { }

      const T& operator()() const { return value; }
    };

  // Create a constant function from the given value.
  template <typename T>
    inline constant_function<T> make_constant_function(const T& value) 
    {
      return {value};
    }



  // To bool (function)
  // The to bool function object evaluates its argument as a boolean value by
  // explicitly converting it to bool.
  struct to_bool_function
  {
    template <typename T>
      bool operator()(const T& x) const
      {
        static_assert(Boolean<T>(), "");
        return x;
      }
  };

  // An alias for the to_bool_function type.
  using To_bool = to_bool_function;

  // Returns a function that converts values to bool.
  inline to_bool_function make_to_bool() { return {}; }



  // Equality (relation)
  // The equal function object denotes the relation a == b.
  struct equal_relation
  {
    template <typename T, typename U>
      auto operator()(const T& a, const U& b) const
        -> Requires<Equality_comparable<T, U>(), bool>
      {
        static_assert(Equality_comparable<T, U>(), "");
        return a == b;
      }
  };
  


  // Value equality (predicate)
  // Denotes the expression value == x for a given value.
  template <typename T>
    struct equal_value_predicate
    {
      T value;

      equal_value_predicate(const T& value) : value(value) { }

      template <typename U>
        auto operator()(const U& x) const
          -> Requires<Equality_comparable<T, U>(), bool>
        {
          static_assert(Equality_comparable<T, U>(), "");
          return value == x;
        }
    };



  // Equal to (constructor)
  // The neq function creates an operation that compares for distinct values. 
  // All constructors return predicate functions. The supported syntaxes are:
  // 
  //    eq()     // A relation evaluating a == b, for all a and b
  //    eq(x)    // A predicate evaluating x == a for all a.
  //    eq(x, y) // A constant predicate evaluating x == y  
  inline equal_relation eq() { return {}; }

  template <typename T>
    inline equal_value_predicate<T> eq(const T& x) { return {x}; }

  template <typename T, typename U>
    inline auto eq(const T& a, const U& b) 
      -> Requires<Equality_comparable<T, U>(), constant_function<bool>>
    { 
      return make_constant_function(a == b);
    }



  // Equal to (alias)
  // An alias to the type of the equality reltaion.
  using Equal_to = equal_relation;

  // Equal to value (alias)
  // An alias to the type of the value equality predicate.
  template <typename T>
    using Equal_to_value = equal_value_predicate<T>;


  // Distinction from (relation)
  // The not equal function denotes the expression a != b.
  struct not_equal_relation
  {
    template <typename T, typename U>
      auto operator()(const T& a, const U& b) const
        -> Requires<Equality_comparable<T, U>(), bool>
      {
        return a != b;
      }
  };



  // Distinct from value (predicate)
  template <typename T>
    struct not_equal_value_predicate
    {
      T value;

      not_equal_value_predicate(const T& value) : value(value) { }

      template <typename U>
        auto operator()(const U& x) const 
          -> Requires<Equality_comparable<T, U>(), bool>
        {
          return value != x;
        }
    };



  // Distinct from (constructor)
  // The neq function creates an operation that compares for distinct values. 
  // All constructors return predicate functions. The supported syntaxes are:
  // 
  //    neq()     // A relation evaluating a != b, for all a and b
  //    neq(x)    // A predicate evaluating x != a for all a.
  //    neq(x, y) // A constant predicate evaluating x != y
  inline not_equal_relation neq() { return {}; }

  template <typename T>
    inline not_equal_value_predicate<T> neq(const T& x) { return {x}; }

  template <typename T, typename U>
    inline auto  neq(const T& a, const U& b)
      -> Requires<Equality_comparable<T, U>(), constant_function<bool>>
    {
      return make_constant_function(a != b);
    }



  // Distinct from (alias)
  // An alias to the type of the distinction relation.
  using Distinct_from = not_equal_relation;

  // Distinct from value (alias)
  // An alias to the type of the value distinction predicate
  template <typename T>
    using Distinct_from_value = not_equal_value_predicate<T>;



  // Less than (relation)
  // The not equal function denotes the expression a < b.
  struct less_relation
  {
    template <typename T, typename U>
      auto operator()(const T& a, const U& b) const
        -> Requires<Weakly_ordered<T, U>(), bool>
      {
        return a < b;
      }
  };



  // Less than value (predicate)
  template <typename T>
    struct less_value_predicate
    {
      const T& value;

      less_value_predicate(const T& value) : value(value) { }

      template <typename U>
        auto operator()(const U& x) const
          -> Requires<Weakly_ordered<T, U>(), bool>
        {
          return value < x;
        }
    };

  
  
  // Less than (constructor)
  // The lt function creates an operation that compares for distinct values. 
  // All constructors return predicate functions. The supported syntaxes are:
  // 
  //    lt()     // A relation evaluating a < b, for all a and b
  //    lt(x)    // A predicate evaluating x < a for all a.
  //    lt(x, y) // A constant predicate evaluating x < y
  inline less_relation lt() { return {}; }

  template <typename T>
    inline less_value_predicate<T> lt(const T& x) { return {x}; }

  template <typename T, typename U>
    inline auto lt(const T& a, const U& b)
      -> Requires<Weakly_ordered<T, U>(), constant_function<bool>>
    {
      return make_constant_function(a < b);
    }



  // Less than (alias)
  using Less_than = less_relation;

  // Less than value (alias)
  template <typename T>
    using Less_than_value = less_value_predicate<T>;



  // Greater than (relation)
  struct greater_relation
  {
    template <typename T, typename U>
      auto operator()(const T& a, const U& b) const
        -> Requires<Weakly_ordered<T, U>(), bool>
      {
        return a > b;
      }
  };



  // Greater than value (predicate)
  template <typename T>
    struct greater_value_predicate
    {
      T value;

      template <typename U>
        auto operator()(const U& x) const
          -> Requires<Weakly_ordered<T, U>(), bool>
        {
          return value > x;
        }
    };
  
    
    
  // Greater than (constructor)
  // The gt function creates an operation that compares for distinct values. 
  // All constructors return predicate functions. The supported syntaxes are:
  // 
  //    gt()     // A relation evaluating a > b, for all a and b
  //    gt(x)    // A predicate evaluating x > a for all a.
  //    gt(x, y) // A constant predicate evaluating x > y
  inline greater_relation gt() { return {}; }

  template <typename T>
    inline greater_value_predicate<T> gt(const T& x) { return {x}; }

  template <typename T, typename U>
    inline auto gt(const T& a, const U& b)
      -> Requires<Weakly_ordered<T, U>(), constant_function<bool>>
    {
      return make_constant_function(a > b);
    }



  // Greater than (alias)
  using Greater_than = greater_relation;

  // Greater than value (alias)
  template <typename T>
    using Greater_than_value = greater_value_predicate<T>;



  // Less than or equal to (relation)
  struct less_equal_relation
  {
    template <typename T, typename U>
      auto operator()(const T& a, const U& b) const
        -> Requires<Weakly_ordered<T, U>(), bool>
      {
        return a <= b;
      }
  };



  // Less than or equal to value (predicate)
  template <typename T>
    struct less_equal_value_predicate
    {
      T value;

      less_equal_value_predicate(const T& value) : value(value) { }

      template <typename U>
        auto operator()(const U& x) const
          -> Requires<Weakly_ordered<T, U>(), bool>
        {
          static_assert(Weakly_ordered<T, U>(), "");
          return value <= x;
        }
    };



  // Less than or equal to (constructor)
  inline less_equal_relation lte() { return {}; }

  template <typename T>
    inline less_equal_value_predicate<T> lte(const T& x) { return {x}; }

  template <typename T, typename U>
    inline auto lte(const T& a, const U& b)
      -> Requires<Weakly_ordered<T, U>(), constant_function<bool>>
    {
      static_assert(Weakly_ordered<T, U>(), "");
      return make_constant_function(a <= b);
    }



  // Less than or equal to (alias)
  using Less_equal_to = less_equal_relation;

  // Less than or equal to value (alias)
  template <typename T>
    using Less_equal_to_value = less_equal_value_predicate<T>;



  // Greater than or equal to (relation)
  struct greater_equal_relation
  {
    template <typename T, typename U>
      auto operator()(const T& a, const U& b) const
        -> Requires<Weakly_ordered<T, U>(), bool>
      {
        return a >= b;
      }
  };


  // Greater than or equal to value (predicate)
  template <typename T>
    struct greater_equal_value_predicate
    {
      T value;

      greater_equal_value_predicate(const T& value) : value(value) { }

      template <typename U>
        auto operator()(const U& x) const
          -> Requires<Weakly_ordered<T, U>(), bool>
        {
          return value >= x;
        }
    };



  // Greater than or equal to (constructor)
  inline greater_equal_relation gte() { return {}; }

  template <typename T>
    inline greater_equal_value_predicate<T> gte(const T& x) { return {x}; }

  template <typename T, typename U>
    inline auto gte(const T& a, const U& b)
      -> Requires<Weakly_ordered<T, U>(), constant_function<bool>> 
    {
      return make_constant_function(a <= b);
    }
  
  

  // Greater than or equal to (alias)
  using Greater_equal_to = greater_equal_relation;

  // Greater than or equal to (alias)
  template <typename T>
    using Greater_equal_to_value = greater_equal_value_predicate<T>;



  // Negation (predicate)
  // The negation of a predicate p(args...) is !p(args...)
  template <typename P>
    struct negate_predicate
    {
      P pred;

      negate_predicate(P p = {}) : pred(p) { }

      template <typename... Args>
        bool operator()(Args&&... args) const
        {
          return !pred(std::forward<Args>(args)...);
        }
    };

  // Return the negation of the given predicate.
  //
  // NOTE: Do not confuse this with the numeric operation neg(x), which 
  // returns the negation of a numeric value.
  //
  // FIXME: This should be called negate(), but that name is taken by a class
  // in <functional>
  template <typename P>
    negate_predicate<P> negation(P p) { return {p}; }


  
  // Complement (relation)
  // For a and b, the complement of r(a, b)  is !r(a, b).
  template <typename R>
    struct complement_relation
    {
      complement_relation(R r = {}) : r(r) { }
      
      template <typename T, typename U>
        bool operator()(T const& a, U const& b) const
        {
          static_assert(Relation<R, T, U>(), "");
          return !r(a, b);
        }

      R r;
    };

  // An alias for for the complement relation.
  template <typename R>
    using Complement_of = complement_relation<R>;

  // Return the complement of the relation r.
  template <typename R>
    inline complement_relation<R> complement(R r) { return {r}; }
  

  
  // Converse (relation)
  // For a and b, the converse of r(a, b) is r(b, a).
  template <typename R>
    struct converse_relation
    {
      converse_relation(R r = {}) : r(r) { };
      
      template <typename T, typename U>
        bool operator()(T const& a, U const& b) const
        {
          static_assert(Relation<R, T, U>(), "");
          return r(b, a);
        }

      R r;
    };

  // An alias for the converse relation.
  template <typename R>
    using Converse_of = converse_relation<R>;

  // Returns the converse of the relation r.
  template <typename R>
    inline converse_relation<R> converse(R r) { return {r}; }
    


  // Complement of converse (relation)
  // For a and b, the complement of the converse of r(a, b) is !r(b, a).
  template <typename R>
    struct complement_of_converse_relation
    {
      complement_of_converse_relation(R r = {}) : r(r) { };
      
      template <typename T, typename U>
        bool operator()(T const& a, U const& b) const
        {
          static_assert(Relation<R, T, U>(), "");
          return !r(b, a);
        }

      R r;
    };

  // An alias for the complement of converse relation
  template <typename R>
    using Complement_of_converse_of = complement_of_converse_relation<R>;

  // Returns the complement of the converse of the relation r.
  template <typename R>
    inline complement_of_converse_relation<R> complement_of_converse(R r) 
    { 
      return {r}; 
    }



  // Symmetric complement (relation)
  // For a and b, the symmetric complement of r is !r(a, b) && !r(b, a).
  template <typename R>
    struct symmetric_complement_relation
    {
      symmetric_complement_relation(R r = {}) : r(r) { };
      
      template <typename T, typename U>
        bool operator()(T const& a, U const& b) const
        {
          static_assert(Relation<R, T, U>(), "");
          return !r(a, b) && !r(b, a);
        }
        
      R r;
    };

  // An alias for the symmetric complement relation.
  template <typename R>
    using Symmetric_complement_of = symmetric_complement_relation<R>;

  // Returns the symmetric complement of the relation r.
  template <typename R>
    inline symmetric_complement_relation<R> symmetric_complement(R r)
    {
      return {r};
    }

    

  // Common numeric predicates
  // These abstractions are useful for testing numeric values with relation
  // to 0.
  //
  // TODO: It may be better to write these in terms of abtracted generic
  // algorithsm. I know for a fact that there are more efficient ways to
  // determine positivity/negativity of integers than comparing two values
  // (i.e., test the high-order bit!).
  //
  // FIXME: Follow the naming from above and rename all of these function
  // objects. Also, they should name expressions; the function object should
  // not be parameterized over their type.


  template <typename T>
    struct is_zero_pred
    {
      bool operator()(const T& x) const { return x == T {0}; }
    };

  template <typename T>
    struct non_zero_pred
    {
      bool operator()(const T& x) const { return x != T {0}; }
    };


  template <typename T>
    struct negative_pred
    {
      bool operator()(const T& x) const { return x < T {0}; }
    };

  template <typename T>
    struct non_negative_pred
    {
      bool operator()(const T& x) const { return x >= T {0}; }
    };

  template <typename T>
    struct positive_pred
    {
      bool operator()(const T& x) const { return x > T {1}; }
    };

  template <typename T>
    struct non_positive_pred
    {
      bool operator()(const T& x) const { return x <= T {0}; }
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
  template <typename T>
    inline Requires<Scalar<T>(), std::size_t> hash_value(T x)
    {
      std::hash<T> h;
      return h(x);
    }
    
  // Hash value (member)
  template <typename T>
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
  template <typename T>
    struct origin_hashing
    {
      using value_type = T;
    };
    
} // namespace origin


namespace std
{
  // Specialize the standard hash function so that it will use the origin
  // hashing facility.
  template <typename T>
    struct hash<origin::origin_hashing<T>>
    {
      std::size_t operator()(const T& x) const
      {
        using origin::hash_value;
        return hash_value(x);
      }
    };
}

#include <origin/concepts.hpp>

#endif
