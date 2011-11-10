// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_TRAITS_HPP
#define ORIGIN_TRAITS_HPP

#include <type_traits>

#include <origin/utility.hpp>

namespace origin
{
  // SFINAE support
    
  // The substitution failure type represents the result of failed name lookup
  // and is used to support queries about the existence or state of expressions
  // that might fail.
  struct subst_failure { };

  // Returns true if the the given type indicates a failed substitution.
  template<typename T>
    struct subst_failed: std::false_type
    { };

  template<>
    struct subst_failed<subst_failure> : std::true_type
    { };

  // The Subst_failed predicate returns true when the given type indicates
  // a substitution failure.
  template<typename T>
    constexpr bool Subst_failed()
    {
      return subst_failed<T>::value;
    }

    
  // Returns true if the given type indicates a successful substitution. This
  // is the case whenever T is not subst_failure.
  template<typename T>
    struct subst_succeeded : std::true_type
    { };

  template<>
    struct subst_succeeded<subst_failure> : std::false_type
    { };


  // The Subst_succeeded predicate returns true when its given type argument
  // does not indicate a substitution failure.
  template<typename T>
    constexpr bool Subst_succeeded()
    {
      return subst_succeeded<T>::value;
    }
    
    
  // Adaptors for standard traits
  
  // An alias for the std::enable_if psuedo-trait. Like Boost, we allow the
  // type argument to default to void. This is useful when writing Requires<>
  // expressions as function or template arguments.
  template<bool B, typename T = void>
    using Requires = typename std::enable_if<B, T>::type;

    
  // An alias for the std;:conditional type trait.
  template<bool B, typename T, typename F>
    using If = typename std::conditional<B, T, F>::type;
  
    
  // Return true if T is void, and false otherwise.
  template<typename T>
    constexpr bool Void()
    {
      return std::is_void<T>::value;
    }
    
    
  // Integral types

  template<typename T>
    constexpr bool Integral()
    {
      return std::is_integral<T>::value;
    }
    
  template<typename T>
    constexpr bool Signed()
    {
      return std::is_signed<T>::value;
    }
    
  template<typename T>
    constexpr bool Unsigned()
    {
      return std::is_unsigned<T>::value;
    }
    
  // The As_signed alias is a signed integral type with the same width as T.
  template<typename T>
    using Make_signed = typename std::make_signed<T>::type;

  // The As_unsigned alias is an unsigned integral type with the same width
  // as T.
  template<typename T>
    using Make_unsigned = typename std::make_unsigned<T>::type;
    
  
  template<typename T>
    constexpr bool Floating_point()
    {
      return std::is_floating_point<T>::value;
    }

  // Arrays
  
  // Return true if T is an array type fo the form T[n].
  template<typename T>
    constexpr bool Array()
    {
      return std::is_array<T>::value;
    }

  // Return the number of dimensions of the array type T. If T is not an
  // Array, it has 0 dimensions.
  template<typename T>
    constexpr unsigned Rank()
    {
      return std::rank<T>::value;
    }
    
  // Return the extent of the array type T...
  template<typename T, unsigned I = 0>
    constexpr std::size_t Extent()
    {
      return std::extent<T, I>::value;
    }
  
  template<typename T>
    using Remove_extent = typename std::remove_extent<T>::type;
    
  template<typename T>
    using Remove_all_extents = typename std::remove_all_extents<T>::type;
  

  // Pointers
    
  template<typename T>
    constexpr bool Pointer()
    {
      return std::is_pointer<T>::value;
    }
    
  template<typename T>
    using Add_pointer = typename std::add_pointer<T>::type;
    
  template<typename T>
    using Remove_pointer = typename std::remove_pointer<T>::type;
    
  
  // References
    
  template<typename T>
    constexpr bool Lvalue_reference()
    {
      return std::is_lvalue_reference<T>::value;
    }
    
  template<typename T>
    constexpr bool Rvalue_reference()
    {
      return std::is_rvalue_reference<T>::value;
    }
    
  template<typename T>
    constexpr bool Reference()
    {
      return std::is_reference<T>::value;
    }
    
  template<typename T>
    using Add_lvalue_reference = typename std::add_lvalue_reference<T>::type;
    
  template<typename T>
    using Add_rvalue_reference = typename std::add_rvalue_reference<T>::type;
    
  template<typename T>
    using Remove_reference = typename std::remove_reference<T>::type;

    
  // Qualifiers
  template<typename T>
    constexpr bool Const()
    {
      return std::is_const<T>::value;
    }
    
  template<typename T>
    constexpr bool Volatile()
    {
      return std::is_volatile<T>::value;
    }
    
  template<typename T>
    using Add_const = typename std::add_const<T>::type;
    
  template<typename T>
    using Add_volatile = typename std::add_volatile<T>::type;
    
  template<typename T>
    using Add_cv = typename std::add_cv<T>::type;
    
  template<typename T>
    using Remove_const = typename std::remove_const<T>::type;
    
  template<typename T>
    using Remove_volatile = typename std::remove_volatile<T>::type;
    
  template<typename T>
    using Remove_cv = typename std::remove_cv<T>::type;
    

  // Classes and unions
    
  template<typename T>
    constexpr bool Class()
    {
      return std::is_class<T>::value;
    };
    
  template<typename T>
    constexpr bool Union()
    {
      return std::is_union<T>::value;
    }
    
  template<typename T>
    constexpr bool Empty()
    {
      return std::is_empty<T>::value;
    }
    
  template<typename T>
    constexpr bool Polymorphic()
    {
      return std::is_polymorphic<T>::value;
    }
    
  template<typename T>
    constexpr bool Abstract()
    {
      return std::is_abstract<T>::value;
    }
 

  // Class destruction and construction.
  
  template<typename T>
    constexpr bool Destructible()
    {
      return std::is_destructible<T>::value;
    }
    
//   template<typename T>
//     constexpr bool Trivially_destructible()
//     {
//       return std::is_trivially_destructible<T>::value;
//     }
    
//   template<typename T>
//     constexpr bool Nothrow_destructible()
//     {
//       return std::is_nothrow_destructible<T>::value;
//     }
    
  template<typename T>
    constexpr bool Has_virtual_destructor()
    {
      return std::has_virtual_destructor<T>::value;
    }
    

  template<typename T, typename... Args>
    constexpr bool Constructible()
    {
      return std::is_constructible<T, Args...>::value;
    }
    
//   template<typename T, typename... Args>
//     constexpr bool Trivially_constructible()
//     {
//       return std::is_trivially_constructible<T, Args...>::value;
//     }
    
  template<typename T, typename... Args>
    constexpr bool Nothrow_constructible()
    {
      return std::is_nothrow_constructible<T, Args...>::vlaue;
    }
    
    
  template<typename T>
    constexpr bool Default_constructible()
    {
      return std::is_default_constructible<T>::value;
    }
    
//   template<typename T>
//     constexpr bool Trivially_default_constructible()
//     {
//       return std::is_trivially_default_constructible<T>::value;
//     }
    
  template<typename T>
    constexpr bool Nothrow_default_constructible()
    {
      return std::is_nothrow_default_constructible<T>::value;
    }
    
    
  template<typename T>
    constexpr bool Copy_constructible()
    {
      return std::is_copy_constructible<T>::value;
    }
    
//   template<typename T>
//     constexpr bool Trivially_copy_constructible()
//     {
//       return std::is_trivially_copy_constructible<T>::value;
//     }

  template<typename T>
    constexpr bool Nothrow_copy_constructible()
    {
      return std::is_nothrow_copy_constructible<T>::value;
    }

    
  template<typename T>
    constexpr bool Move_constructible()
    {
      return std::is_move_constructible<T>::value;
    }

//   template<typename T>
//     constexpr bool Trivially_move_constructible()
//     {
//       return std::is_trivially_move_constructible<T>::value;
//     }

  template<typename T>
    constexpr bool Nothrow_move_constructible()
    {
      return std::is_nothrow_move_constructible<T>::value;
    }
    
    
  // Assignment
  
  template<typename T, typename U>
    constexpr bool Assignable()
    {
      return std::is_assignable<T, U>::value;
    }
    
//   template<typename T, typename U>
//     constexpr bool Trivially_assignable()
//     {
//       return std::is_trivially_assignable<T>::value;
//     }
    
  template<typename T, typename U>
    constexpr bool Nothrow_move_assignable()
    {
      return std::is_nothrow_move_assignable<T>::value;
    }
    
    
  template<typename T>
    constexpr bool Copy_assignable()
    {
      return std::is_copy_assignable<T>::value;
    }
    
//   template<typename T>
//     constexpr bool Trivially_copy_assignable()
//     {
//       return std::is_trivially_copy_assignable<T>::value;
//     }
    
  template<typename T>
    constexpr bool Nothrow_copy_assignable()
    {
      return std::is_nothrow_copy_assignable<T>::value;
    }

    
  template<typename T>
    constexpr bool Move_assignable()
    {
      return std::is_move_assignable<T>::value;
    }
    
//   template<typename T>
//     constexpr bool Trivially_move_assignable()
//     {
//       return std::is_trivially_move_assignable<T>::value;
//     }
    
  template<typename T>
    constexpr bool Nothrow_move_assignable()
    {
      return std::is_nothrow_move_assignable<T>::value;
    }
    
    
  // Enums
  
  template<typename T>
    constexpr bool Enum()
    {
      return std::is_enum<T>::value;
    }
    
  template<typename T>
    using Underlying_type = typename std::underlying_type<T>::type;
 
    
  // Functions
    
  // Returns true if T is a function.
  template<typename T>
    constexpr bool Function()
    {
      return std::is_function<T>();
    }
    
  // An alias for the result of the function type F. Note that F must have
  // the form G(Args...) where G is a callable type and Args... is a sequence
  // of arguments.
  template<typename F>
    using Result_of = typename std::result_of<F>::type;
    
 
  // Common Type
  //
  // We only define common type in terms of two types. It can be generalized
  // to any number of types if we find the need later.
  
  // Yields the common type of the T and U.
  //
  // NOTE: This supercedes the std implementation of common type, which is
  // inherently unsafe. Unlike this version, it can't be reliably tested
  // without causing errors. We provide supplemental specializations of
  // common type to ensure interoperability with the std::common_type.
  template<typename T, typename U>
    struct common_type
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& a, Y&& b) -> decltype(true ? a : b);

      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };
    
  // FIXME: Add specializations for std::duration.


  // The alias yields the common type of T and U if it exists, and 
  // subst_failure if it does not. Note that the common type of T and U may
  // not be T or U.
  template<typename T, typename U>
    using Common_type = typename common_type<T, U>::type;


  // The Common predicate is true if T and U share a common type.
  template<typename T, typename U>
    constexpr bool Common()
    {
      return Subst_succeeded<Common_type<T, U>>();
    }

    
  // Return true if T is convertible to U.
  template<typename T, typename U>
    constexpr bool Convertible()
    {
      return std::is_convertible<T, U>::value;
    }
    
  // True if T is derived from U.
  template<typename T, typename U>
    constexpr bool Derived()
    {
      return std::is_base_of<U, T>::value;
    }

    
  // Relational operator traits
  
  // Safely deduce the result type of the expression t == u.
  template<typename T, typename U>
    struct equal_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x == y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // An alias for the result of t == u.
  template<typename T, typename U = T>
    using Equal_result = typename equal_result<T, U>::type;
    
  // Return true if the expression t == u is valid.
  template<typename T, typename U = T>
    constexpr bool Has_equal()
    {
      return Subst_succeeded<Equal_result<T, U>>();
    }
    

  // Safely deduce the result type of the expression t != u.
  template<typename T, typename U>
    struct not_equal_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y const&& y) -> decltype(x != y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // An alias for the result of t != u.
  template<typename T, typename U = T>
    using Not_equal_result = typename not_equal_result<T, U>::type;
    
  // Return true if the expression t != u is valid.
  template<typename T, typename U = T>
    constexpr bool Has_not_equal()
    {
      return Subst_succeeded<Not_equal_result<T, U>>();
    }

    
  // Safely deduce the result type of the expression t < u.
  template<typename T, typename U>
    struct less_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x < y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // An alias for the result of t < u.
  template<typename T, typename U = T>
    using Less_result = typename less_result<T, U>::type;
    
  // Return true if the expression t < u is valid.
  template<typename T, typename U = T>
    constexpr bool Has_less()
    {
      return Subst_succeeded<Less_result<T, U>>();
    }


  // Safely deduce the result type of the expression t > u.
  template<typename T, typename U>
    struct greater_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x > y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // An alias for the result of t > u.
  template<typename T, typename U = T>
    using Greater_result = typename greater_result<T, U>::type;
    
  // Return true if the expression t > u is valid.
  template<typename T, typename U = T>
    constexpr bool Has_greater()
    {
      return Subst_succeeded<Greater_result<T, U>>();
    }

    
  // Safely deduce the result type of the expression t <= u.
  template<typename T, typename U>
    struct less_equal_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x <= y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // An alias for the result of t <= u.
  template<typename T, typename U = T>
    using Less_equal_result = typename less_equal_result<T, U>::type;
    
  // Return true if the expression t <= u is valid.
  template<typename T, typename U = T>
    constexpr bool Has_less_equal()
    {
      return Subst_succeeded<Less_equal_result<T, U>>();
    }
    
    
  // Safely deduce the result type of the expression t >= u.
  template<typename T, typename U>
    struct greater_equal_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x >= y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // An alias for the result of t >= u.
  template<typename T, typename U = T>
    using Greater_equal_result = typename greater_equal_result<T, U>::type;
    
  // Return true if the expression t >= u is valid.
  template<typename T, typename U = T>
    constexpr bool Has_greater_equal()
    {
      return Subst_succeeded<Greater_equal_result<T, U>>();
    }    
    
    
  // Arithmetic operator traits

  // Safely deduce the result type of the expression t + u.
  template<typename T, typename U>
    struct plus_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x + y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // An alias for the result of t + u.
  template<typename T, typename U = T>
    using Plus_result = typename plus_result<T, U>::type;
    
  // Return true if the expression t + u is valid.
  template<typename T, typename U = T>
    constexpr bool Has_plus()
    {
      return Subst_succeeded<Plus_result<T, U>>();
    }

    
  // Safely deduce the result type of the expression t - u.
  template<typename T, typename U>
    struct minus_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x - y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // An alias for the result of t - u.
  template<typename T, typename U = T>
    using Minus_result = typename minus_result<T, U>::type;
    
  // Return true if the expression t - u is valid.
  template<typename T, typename U = T>
    constexpr bool Has_minus()
    {
      return Subst_succeeded<Minus_result<T, U>>();
    }
  

  // Safely deduce the result type of the expression t * u.
  template<typename T, typename U>
    struct multiplies_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x * y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // An alias for the result of t * u.
  template<typename T, typename U = T>
    using Multiplies_result = typename multiplies_result<T, U>::type;
    
  // Return true if the expression t * u is valid.
  template<typename T, typename U = T>
    constexpr bool Has_multiplies()
    {
      return Subst_succeeded<Multiplies_result<T, U>>();
    }

    
  // Safely deduce the result type of the expression t / u.
  template<typename T, typename U>
    struct divides_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x / y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // An alias for the result of t / u.
  template<typename T, typename U = T>
    using Divides_result = typename divides_result<T, U>::type;
    
  // Return true if the expression t / u is valid.
  template<typename T, typename U = T>
    constexpr bool Has_divides()
    {
      return Subst_succeeded<Divides_result<T, U>>();
    }

    
  // Safely deduce the result type of the expression t % u.
  template<typename T, typename U>
    struct modulus_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x % y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // An alias for the result of t % u.
  template<typename T, typename U = T>
    using Modulus_result = typename modulus_result<T, U>::type;
    
  // Return true if the expression t % u is valid.
  template<typename T, typename U = T>
    constexpr bool Has_modulus()
    {
      return Subst_succeeded<Modulus_result<T, U>>();
    }
    

  // Safely deduce the result of +t.
  template<typename T>
    struct unary_plus_result
    {
    private:
      template<typename X>
        static auto check(X&& a) -> decltype(+a);
      
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };

  // An alias for the result of +t.
  template<typename T>
    using Unary_plus_result = typename unary_plus_result<T>::type;
    
  // Return true if the expression +t is valid.
  template<typename T>
    constexpr bool Has_unary_plus()
    {
      return Subst_succeeded<Unary_plus_result<T>>();
    }

  
  // Safely deduce the result of -t.
  template<typename T>
    struct unary_minus_result
    {
    private:
      template<typename X>
        static auto check(X&& a) -> decltype(-a);
      
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };

  // An alias for the result of -t.
  template<typename T>
    using Unary_minus_result = typename unary_minus_result<T>::type;
    
  // Return true if the expression -t is valid.
  template<typename T>
    constexpr bool Has_unary_minus()
    {
      return Subst_succeeded<Unary_plus_result<T>>();
    }
    
    
  
  // Safely deduce the result of ++t;
  template<typename T>
    struct pre_increment_result
    {
    private:
      template<typename X>
        static auto check(X& x) -> decltype(++x);
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T&>()));
    };
    
  // An alias for the result of ++t.
  template<typename T>
    using Pre_increment_result = typename pre_increment_result<T>::type;

  // Return true if the expression ++t is valid.
  template<typename T>
    constexpr bool Has_pre_increment()
    {
      return Subst_succeeded<Pre_increment_result<T>>();
    }



  // Safely deduce the result of t++;
  template<typename T>
    struct post_increment_result
    {
    private:
      template<typename X>
        static auto check(X& x) -> decltype(x++);
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T&>()));
    };
    
  // An alias for the result of t++.
  template<typename T>
    using Post_increment_result = typename post_increment_result<T>::type;

  // Return true if the expostssion t++ is valid.
  template<typename T>
    constexpr bool Has_post_increment()
    {
      return Subst_succeeded<Post_increment_result<T>>();
    }

    
    
  // Safely deduce the result of --t;
  template<typename T>
    struct pre_decrement_result
    {
    private:
      template<typename X>
        static auto check(X& x) -> decltype(--x);
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T&>()));
    };
    
  // An alias for the result of --t.
  template<typename T>
    using Pre_decrement_result = typename pre_decrement_result<T>::type;

  // Return true if the expression --t is valid.
  template<typename T>
    constexpr bool Has_pre_decrement()
    {
      return Subst_succeeded<Pre_decrement_result<T>>();
    }



  // Safely deduce the result of t--;
  template<typename T>
    struct post_decrement_result
    {
    private:
      template<typename X>
        static auto check(X& x) -> decltype(x--);
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T&>()));
    };
    
  // An alias for the result of t--.
  template<typename T>
    using Post_decrement_result = typename post_decrement_result<T>::type;

  // Return true if the expostssion t-- is valid.
  template<typename T>
    constexpr bool Has_post_decrement()
    {
      return Subst_succeeded<Post_decrement_result<T>>();
    }
    
    
    
    // Logical operators
  
  // Safely deduce the result type of the expression t && u.
  template<typename T, typename U>
    struct and_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x && y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // An alias for the result of t && u.
  template<typename T, typename U = T>
    using And_result = typename and_result<T, U>::type;
    
  // Return true if the expression t && u is valid.
  template<typename T, typename U = T>
    constexpr bool Has_and()
    {
      return Subst_succeeded<And_result<T, U>>();
    }
    
    
  // Safely deduce the result type of the expression t || u.
  template<typename T, typename U>
    struct or_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x || y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // An alias for the result of t || u.
  template<typename T, typename U = T>
    using Or_result = typename or_result<T, U>::type;
    
  // Return true if the expression t || u is valid.
  template<typename T, typename U = T>
    constexpr bool Has_or()
    {
      return Subst_succeeded<Or_result<T, U>>();
    }
    
    
   // Safely deduce the result of !t.
  template<typename T>
    struct not_result
    {
    private:
      template<typename X>
        static auto check(X&& a) -> decltype(!a);
      
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };

  // An alias for the result of !t.
  template<typename T>
    using Not_result = typename not_result<T>::type;
    
  // Return true if the expression !t is valid.
  template<typename T>
    constexpr bool Has_not()
    {
      return Subst_succeeded<Not_result<T>>();
    }   

    
  // Binary operations
  
  // Safely deduce the result type of the expression t & u.
  template<typename T, typename U>
    struct bit_and_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x & y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // An alias for the result of t & u.
  template<typename T, typename U = T>
    using Bit_and_result = typename bit_and_result<T, U>::type;
    
  // Return true if the expression t & u is valid.
  template<typename T, typename U = T>
    constexpr bool Has_bit_and()
    {
      return Subst_succeeded<Bit_and_result<T, U>>();
    }
    
  // Safely deduce the result type of the expression t | u.
  template<typename T, typename U>
    struct bit_or_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x | y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // An alias for bit_or the result of t | u.
  template<typename T, typename U = T>
    using Bit_or_result = typename bit_or_result<T, U>::type;
    
  // Return true if the expression t | u is valid.
  template<typename T, typename U = T>
    constexpr bool Has_bit_or()
    {
      return Subst_succeeded<Bit_or_result<T, U>>();
    }
    
    
  // Safely deduce the result type of the expression t ^ u.
  template<typename T, typename U>
    struct bit_xor_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x ^ y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // An alias fbit_xor the result of t ^ u.
  template<typename T, typename U = T>
    using Bit_xor_result = typename bit_xor_result<T, U>::type;
    
  // Return true if the expression t ^ u is valid.
  template<typename T, typename U = T>
    constexpr bool Has_bit_xor()
    {
      return Subst_succeeded<Bit_xor_result<T, U>>();
    }
    
    
  // Safely deduce the result type of the expression t << u.
  template<typename T, typename U>
    struct left_shift_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x << y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // An alias fleft_shift the result of t << u.
  template<typename T, typename U = T>
    using Left_shift_result = typename left_shift_result<T, U>::type;
    
  // Return true if the expression t << u is valid.
  template<typename T, typename U = T>
    constexpr bool Has_left_shift()
    {
      return Subst_succeeded<Left_shift_result<T, U>>();
    }
    
    
  // Safely deduce the result type of the expression t >> u.
  template<typename T, typename U>
    struct right_shift_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x >> y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // An alias fright_shift the result of t >> u.
  template<typename T, typename U = T>
    using Right_shift_result = typename right_shift_result<T, U>::type;
    
  // Return true if the expression t >> u is valid.
  template<typename T, typename U = T>
    constexpr bool Has_right_shift()
    {
      return Subst_succeeded<Right_shift_result<T, U>>();
    }
    
    
   // Safely deduce the result of ~t.
  template<typename T>
    struct complement_result
    {
    private:
      template<typename X>
        static auto check(X&& a) -> decltype(~a);
      
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };

  // An alias for the result of ~t.
  template<typename T>
    using Complement_result = typename complement_result<T>::type;
    
  // Return true if the expression ~t is valid.
  template<typename T>
    constexpr bool Has_complement()
    {
      return Subst_succeeded<Complement_result<T>>();
    }   
  

  // Address, dereference, subscript, and function call

   // Safely deduce the result of &t.
  template<typename T>
    struct address_result
    {
    private:
      template<typename X>
        static auto check(X&& a) -> decltype(&a);
      
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };

  // An alias for the result of &t.
  template<typename T>
    using Address_result = typename address_result<T>::type;
    
  // Return true if the expression &t is valid.
  template<typename T>
    constexpr bool Has_address()
    {
      return Subst_succeeded<Address_result<T>>();
    }   
    
    
   // Safely deduce the result of *t.
  template<typename T>
    struct dereference_result
    {
    private:
      template<typename X>
        static auto check(X&& a) -> decltype(*a);
      
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>()));
    };

  // An alias for the result of *t.
  template<typename T>
    using Dereference_result = typename dereference_result<T>::type;
    
  // Return true if the expression *t is valid.
  template<typename T>
    constexpr bool Has_dereference()
    {
      return Subst_succeeded<Dereference_result<T>>();
    }
    

  // Safely deduce the result type of the expression t[u].
  template<typename T, typename U>
    struct subscript_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x[y]);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T>(), std::declval<U>()));
    };

  // An alias fsubscript the result of t[u].
  template<typename T, typename U>
    using Subscript_result = typename subscript_result<T, U>::type;
    
  // Return true if the expression t[u] is valid.
  template<typename T, typename U>
    constexpr bool Has_subscript()
    {
      return Subst_succeeded<Subscript_result<T, U>>();
    }
    
    
  // Safely deduce the result type of the expression f(args...).
  //
  // NOTE: This is not simply the same as result_of. That can fail noisily,
  // whereas this does not.
  template<typename F, typename... Args>
    struct call_result
    {
    private:
      template<typename X, typename... Y>
        static auto check(X&& x, Y&&... y) -> decltype(x(std::forward<Y>(y)...));
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<F>(), std::declval<Args>()...));
    };

  // An alias fcall the result of t >> u.
  template<typename F, typename... Args>
    using Call_result = typename call_result<F, Args...>::type;
    
  // Return true if the expression t >> u is valid.
  template<typename F, typename... Args>
    constexpr bool Has_call()
    {
      return Subst_succeeded<Call_result<F, Args...>>();
    }
    

  // Compound assignment
    
  // Safely deduce the result type of the expression t += u.
  template<typename T, typename U>
    struct plus_assign_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X& x, Y&& y) -> decltype(x += y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T&>(), std::declval<U>()));
    };

  // An alias fplus_assign the result of t += u.
  template<typename T, typename U>
    using Plus_assign_result = typename plus_assign_result<T, U>::type;
    
  // Return true if the expression t += u is valid.
  template<typename T, typename U>
    constexpr bool Has_plus_assign()
    {
      return Subst_succeeded<Plus_assign_result<T, U>>();
    }    

    
  // Safely deduce the result type of the expression t -= u.
  template<typename T, typename U>
    struct minus_assign_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X& x, Y&& y) -> decltype(x -= y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T&>(), std::declval<U>()));
    };

  // An alias fminus_assign the result of t -= u.
  template<typename T, typename U>
    using Minus_assign_result = typename minus_assign_result<T, U>::type;
    
  // Return true if the expression t -= u is valid.
  template<typename T, typename U>
    constexpr bool Has_minus_assign()
    {
      return Subst_succeeded<Minus_assign_result<T, U>>();
    }    
    
    
  // Safely deduce the result type of the expression t *= u.
  template<typename T, typename U>
    struct multiplies_assign_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X& x, Y&& y) -> decltype(x *= y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T&>(), std::declval<U>()));
    };

  // An alias fmultiplies_assign the result of t *= u.
  template<typename T, typename U>
    using Multiplies_assign_result = typename multiplies_assign_result<T, U>::type;
    
  // Return true if the expression t *= u is valid.
  template<typename T, typename U>
    constexpr bool Has_multiplies_assign()
    {
      return Subst_succeeded<Multiplies_assign_result<T, U>>();
    }    
    
    
  // Safely deduce the result type of the expression t /= u.
  template<typename T, typename U>
    struct divides_assign_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X& x, Y&& y) -> decltype(x /= y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T&>(), std::declval<U>()));
    };

  // An alias fdivides_assign the result of t /= u.
  template<typename T, typename U>
    using Divides_assign_result = typename divides_assign_result<T, U>::type;
    
  // Return true if the expression t /= u is valid.
  template<typename T, typename U>
    constexpr bool Has_divides_assign()
    {
      return Subst_succeeded<Divides_assign_result<T, U>>();
    }    
    
    
  // Safely deduce the result type of the expression t %= u.
  template<typename T, typename U>
    struct modulus_assign_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X& x, Y&& y) -> decltype(x %= y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T&>(), std::declval<U>()));
    };

  // An alias fmodulus_assign the result of t %= u.
  template<typename T, typename U>
    using Modulus_assign_result = typename modulus_assign_result<T, U>::type;
    
  // Return true if the expression t %= u is valid.
  template<typename T, typename U>
    constexpr bool Has_modulus_assign()
    {
      return Subst_succeeded<Modulus_assign_result<T, U>>();
    }        
    

  // Safely deduce the result type of the expression t &= u.
  template<typename T, typename U>
    struct bit_and_assign_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X& x, Y&& y) -> decltype(x &= y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T&>(), std::declval<U>()));
    };

  // An alias fbit_and_assign the result of t &= u.
  template<typename T, typename U>
    using Bit_and_assign_result = typename bit_and_assign_result<T, U>::type;
    
  // Return true if the expression t &= u is valid.
  template<typename T, typename U>
    constexpr bool Has_bit_and_assign()
    {
      return Subst_succeeded<Bit_and_assign_result<T, U>>();
    }        

    
  // Safely deduce the result type of the expression t |= u.
  template<typename T, typename U>
    struct bit_or_assign_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X& x, Y&& y) -> decltype(x |= y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T&>(), std::declval<U>()));
    };

  // An alias fbit_or_assign the result of t |= u.
  template<typename T, typename U>
    using Bit_or_assign_result = typename bit_or_assign_result<T, U>::type;
    
  // Return true if the expression t |= u is valid.
  template<typename T, typename U>
    constexpr bool Has_bit_or_assign()
    {
      return Subst_succeeded<Bit_or_assign_result<T, U>>();
    }        

    
  // Safely deduce the result type of the expression t ^= u.
  template<typename T, typename U>
    struct bit_xor_assign_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X& x, Y&& y) -> decltype(x ^= y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T&>(), std::declval<U>()));
    };

  // An alias fbit_xor_assign the result of t ^= u.
  template<typename T, typename U>
    using Bit_xor_assign_result = typename bit_xor_assign_result<T, U>::type;
    
  // Return true if the expression t ^= u is valid.
  template<typename T, typename U>
    constexpr bool Has_bit_xor_assign()
    {
      return Subst_succeeded<Bit_xor_assign_result<T, U>>();
    }        

    
  // Safely deduce the result type of the expression t <<= u.
  template<typename T, typename U>
    struct left_shift_assign_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X& x, Y&& y) -> decltype(x <<= y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T&>(), std::declval<U>()));
    };

  // An alias fleft_shift_assign the result of t <<= u.
  template<typename T, typename U>
    using Left_shift_assign_result = typename left_shift_assign_result<T, U>::type;
    
  // Return true if the expression t <<= u is valid.
  template<typename T, typename U>
    constexpr bool Has_left_shift_assign()
    {
      return Subst_succeeded<Left_shift_assign_result<T, U>>();
    }        

    
  // Safely deduce the result type of the expression t >>= u.
  template<typename T, typename U>
    struct right_shift_assign_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X& x, Y&& y) -> decltype(x >>= y);
        
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<T&>(), std::declval<U>()));
    };

  // An alias fright_shift_assign the result of t >>= u.
  template<typename T, typename U>
    using Right_shift_assign_result = typename right_shift_assign_result<T, U>::type;
    
  // Return true if the expression t >>= u is valid.
  template<typename T, typename U>
    constexpr bool Has_right_shift_assign()
    {
      return Subst_succeeded<Right_shift_assign_result<T, U>>();
    }        

} // namespace origin

#endif
