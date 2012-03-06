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
  

  
  // As bool (function)
  // The as bool function object evaluates its argument as a boolean value,
  // returning true or false.
  template <typename T = default_t>
    struct as_bool : std::unary_function<T, bool>
    {
      static_assert(Boolean<T>(), "");
      
      bool operator()(const T& x) const { return x; }
    };
    
  // The default specialization is polymorphic.
  template <>
    struct as_bool<default_t> : std::unary_function<unspecified_t, bool>
    {
      template <typename T>
        bool operator()(const T& x)
        {
          static_assert(Boolean<T>(), "");
          return x;
        }
    };

  
  // NOTE: The standard relation operators (o_equal, o_less, etc.) vary from
  // the standard operators in that a) they are not templates, b) they are
  // defined on heterogeneous types, and c) they are constrained by their
  // corresponding concepts.
    
    
  // TODO: Make sure that all relations and operations use the adaptors above.
  
  
  // Equal (relation)
  // The equal function object denotes the relation a == b.
  template <typename T = default_t, typename U = T>
    struct o_equal_to
    {
      static_assert(Equality_comparable<T, U>(), "");
      
      bool operator()(const T& a, const U& b) const { return a == b; }
    };

  // The default specialization is polymorphic.
  template <>
    struct o_equal_to<default_t, default_t>
    {
      template <typename T, typename U>
        bool operator()(const T& a, const U& b) const
        {
          static_assert(Equality_comparable<T, U>(), "");
          return a == b;
        }
    };
  
  // The type eq is an alias for o_equal.
  using eq = o_equal_to<>;
  

  
  // Not equal (relation)
  // The not equal function denotes the expression a != b.
  template <typename T = default_t, typename U = T>
    struct o_not_equal_to
    {
      static_assert(Equality_comparable<T, U>(), "");
      
      bool operator()(const T& a, const U& b) const { return a != b; }
    };
  
  template <>
    struct o_not_equal_to<default_t, default_t>
    {
      template <typename T, typename U>
        bool operator()(const T& a, const U& b) const
        {
          static_assert(Equality_comparable<T, U>(), "");
          return a != b;
        }
    };

  // The type neq denotes the not equal relation.
  using neq = o_not_equal_to<>;
  
  
  
  // Less (relation)
  // The less function object names the relation a < b.
  template <typename T = default_t, typename U = T>
    struct o_less
    {
      static_assert(Totally_ordered<T, U>(), "");
      
      bool operator()(const T& a, const U& b) const { return a < b; }
    };

  // The default specialization is polymorphic.
  template <>
    struct o_less<default_t, default_t>
    {
      template <typename T, typename U>
        bool operator()(const T a, const U b) const
        {
          static_assert(Totally_ordered<T, U>(), "");
          return a < b;
        }
    };
  
  // The type lt is an alias for o_less<>.
  using lt = o_less<>;
  
  
  
  // Greater (relation)
  template <typename T = default_t, typename U = T>
    struct o_greater
    {
      static_assert(Totally_ordered<T, U>(), "");
      
      bool operator()(const T& a, const U& b) const { return a > b; }
    };
    
  template <>
    struct o_greater<default_t, default_t>
    {
      template <typename T, typename U>
        bool operator()(const T& a, const U& b) const
        {
          static_assert(Totally_ordered<T, U>(), "");
          return a > b;
        }
    };
  
  using gt = o_greater<>;
    
    

  // Less than or equal to (relation)
  template <typename T = default_t, typename U = T>
    struct o_less_equal
    {
      static_assert(Totally_ordered<T, U>(), "");
      
      bool operator()(const T& a, const U& b) const { return a <= b; }
    };
    
  template <>
    struct o_less_equal<default_t, default_t>
    {
      template <typename T, typename U>
        bool operator()(const T& a, const U& b) const
        {
          static_assert(Totally_ordered<T, U>(), "");
          return a <= b;
        }
    };
  
  using leq = o_less_equal<>;


  
  // Greater than or equal to (relation)
  template <typename T = default_t, typename U = T>
    struct o_greater_equal
    {
      static_assert(Totally_ordered<T, U>(), "");
      
      bool operator()(const T& a, const U& b) const { return a >= b; }
    };
    
  template <>
    struct o_greater_equal<default_t, default_t>
    {
      template <typename T, typename U>
        bool operator()(const T& a, const U& b) const
        {
          static_assert(Totally_ordered<T, U>(), "");
          return a >= b;
        }
    };
  
  using geq = o_greater_equal<>;
  
  
  
  // TODO: Write more relations and add operators.
  

  
  // Complement (relation)
  // For a and b, the complement of r(a, b)  is !r(a, b).
  template <typename R>
    struct complement
    {
      complement(R r = {}) : r(r) { };
      
      template <typename T, typename U>
        bool operator()(T const& a, U const& b) const
        {
          static_assert(Relation<R, T, U>(), "");
          return !r(a, b);
        }

      R r;
    };
  
  
  
  // Converse (relation)
  // For a and b, the converse of r(a, b) is r(b, a).
  template <typename R>
    struct converse
    {
      converse(R r = {}) : r(r) { };
      
      template <typename T, typename U>
        bool operator()(T const& a, U const& b) const
        {
          static_assert(Relation<R, T, U>(), "");
          return r(b, a);
        }

      R r;
    };

    

  // Complement of converse (relation)
  // For a and b, the complement of the converse of r(a, b) is !r(b, a).
  template <typename R>
    struct complement_of_converse
    {
      complement_of_converse(R r = {}) : r(r) { };
      
      template <typename T, typename U>
        bool operator()(T const& a, U const& b) const
        {
          static_assert(Relation<R, T, U>(), "");
          return !r(b, a);
        }

      R r;
    };

    

  // Symmetric complement (relation)
  // For a and b, the symmetric complement of r is !r(a, b) && !r(b, a).
  template <typename R>
    struct symmetric_complement
    {
      symmetric_complement(R r = {}) : r(r) { };
      
      template <typename T, typename U>
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
