// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONCEPTS_TRAITS_HPP
#define ORIGIN_CONCEPTS_TRAITS_HPP

#include <origin/concepts/impl.hpp>
#include <origin/concepts/fwd.hpp>

namespace origin
{
  /**
   * @defgroup concepts_type_traits Type Traits
   * @ingroup concepts
   *
   * Type traits are constraints that evaluate properties of types or sets
   * of types, especially from the perspective of the programming language.
   * These traits are often implemented in terms of an underlying standard
   * type trait.
   */

  /**
   * @ingroup concepts_type_traits
   *
   * The Same trait is valid for a sequence of types that all have (exactly)
   * the same type.
   */
  template<typename... Args>
    struct tSame
    {
      tSame()
      {
        static_assert(value, "Types are not the same");
      }

      typedef std::tuple<are_same<Args...>> requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  /**
   * @ingroup concepts_type_traits
   *
   * The Different trait is valid for two types that are not the Same. This
   * is equivalent to !Same<T, U>::value.
   */
  template<typename T, typename U>
    struct tDifferent
    {
      tDifferent()
      {
        static_assert(value, "Types are not different");
      }

      typedef std::tuple<is_different<T, U>> requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  /**
   * @ingroup concepts_type_traits
   *
   * The Common trait is valid if the type arguments share a common type. A
   * single type is its own common type. Two types have a common type, they can
   * be used as result values of a conditional expression (?:). An arbitrary
   * sequence of types share a common type if they can all be used as result
   * types of a recursivrely constructed conditional expression.
   */
  template<typename... Args>
    struct tCommon
    {
      tCommon()
      {
        static_assert(value, "Arguments do not share a common type");
      }

      typedef std::tuple<has_common_type<Args...>> requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  /**
   * @ingroup concepts_type_traits
   *
   * The Convertible trait is valid for a type T that can be converted to a
   * type U.
   */
  template<typename T, typename U>
    struct tConvertible
    {
      tConvertible()
      {
        static_assert(value, "Cannot convert types");
      }

      typedef std::tuple<std::is_convertible<T, U>> requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  /**
   * @ingroup concepts_type_traits
   *
   * The Derived trait is true for a type T that is derived from a type U.
   */
  template<typename T, typename U>
    struct tDerived
    {
      tDerived()
      {
        static_assert(value, "Type is not derived");
      }

      typedef std::tuple<std::is_base_of<T, U>> requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  /**
   * @ingroup concepts_type_traits
   *
   * The Int trait is true for all built-in integral types.
   */
  template<typename T>
    struct tInt
    {
      tInt()
      {
        static_assert(value, "Type is not a standard integral type");
      }

      typedef std::tuple<std::is_integral<T>> requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  /**
   * @ingroup concepts_type_traits
   *
   * The Signed_int trait is true for all built-in signed integral types.
   */
  template<typename T>
    struct tSigned_Int
    {
      tSigned_Int()
      {
        static_assert(value, "Type is not standard signed integral type");
      }

      typedef std::tuple<std::is_signed<T>> requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  /**
   * @ingroup concepts_type_traits
   *
   * The Unsigned_int trait is true for all built-in unsigned integral types.
   */
  template<typename T>
    struct tUnsigned_Int
    {
      tUnsigned_Int()
      {
        static_assert(value, "Type is not standard unsigned integral type");
      }

      typedef std::tuple<std::is_unsigned<T>> requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  /**
   * @defgroup concepts_interface_traits Interface Traits
   * @ingroup concepts
   *
   * Interface traits are constraints that evaluate an aspect of a type's
   * interface. These are often implemented in terms of use patterns.
   */

  /**
   * @ingroup concepts_interface_traits
   *
   * The Destructible constraint whether a type T can be destroyed.
   */
  template<typename T>
    struct tDestructible
    {
      tDestructible()
      {
        auto p = constraints;
      }

      static void constraints(T& x)
      {
        x.T::~T();
      }

      // FIXME: Use is_destructible when it becomes available. For now, it's
      // probably okay to assume that all types are destructible. I could write
      // a type trait just to be sure.
      typedef std::true_type type;
      static constexpr bool value = type::value;
    };

  /**
   * @ingroup concepts_interface_traits
   *
   * The Constructible constraint evaluates whether a type T can be constructed
   * over a sequence of type arguments.
   */
  template<typename T, typename... Args>
    struct tConstructible
    {
      tConstructible()
      {
        auto p = constraints;
      }

      static void constraints(Args&&... args)
      {
        T{std::forward<Args>(args)...};
      }

      typedef std::tuple<is_constructible<T, Args...>> requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };
    
  // Don't evaluate constructibility on lvalue reference types.
  template<typename T, typename... Args>
    struct tConstructible<T&, Args...> : tConstructible<T, Args...> 
    { };

  // Don't evaluate constructibility on rvalue reference types.
  template<typename T, typename... Args>
    struct tConstructible<T&&, Args...> : tConstructible<T, Args...> 
    { };

  /**
   * @ingroup concepts_interface_traits
   * The Assignable constraint evaluates whether the type T (the assignee) can
   * be  assigned to an object of type U (the assigned). The result of the
   * expression must be (convertible to) a reference to the assignee type. 
   * 
   * Note that if the assigned type is an rvalue reference, the trait is 
   * interpreted as a requirement for move assignment.
   */
  template<typename T, typename U>
    struct tAssignable
    {
      tAssignable()
      {
        auto p = constraints;
      }

      static void constraints(T& x, U y)
      {
        tConvertible<decltype(x = y), T&>{};
      }

      typedef std::tuple<
        has_assign<T, U>,
        tConvertible<typename get_assign_result<T, U>::type, T&>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  // The specialization restates a requirement on move assignment in order
  // to ensure that the move is propagated correctly.
  template<typename T, typename U>
    struct tAssignable<T, U&&>
    {
      tAssignable()
      {
        auto p = constraints;
      }

      static void constraints(T& x, U y)
      {
        tConvertible<decltype(x = std::move(y)), T&>{};
      }

      typedef std::tuple<
        has_assign<T, U&&>,
        tConvertible<typename get_assign_result<T, U&&>::type, T&>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  /**
   * @ingroup concepts_interface_traits
   * The Equal constraint evaluates the existence of the binary == operator for
   * the type parameters T and U. The result of the operation must be 
   * convertible to bool, based on common usage scenarios. 
   */
  template<typename T, typename U>
    struct tEqual
    {
      typedef typename get_equal_result<T, U>::type result_type;
      
      tEqual()
      {
        auto p = constraints;
      }

      static void constraints(T x, U y)
      {
        tConvertible<decltype(x == y), bool>{};
      }

      typedef std::tuple<
        has_equal<T, U>, tConvertible<result_type, bool>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
  };

  // Derived inequality
  template<typename T, typename U>
    auto operator!=(T const& x, U const& y)
      -> typename std::enable_if<tEqual<T, U>::value, decltype(!(x == y))>::type
    {
      return !(x == y);
    }

  /**
   * @ingroup concepts_interface_traits
   * The Less constraint evaluates the existence of the binary < operator for
   * the type parameters T and U. The result of the operation must be 
   * convertible to bool, based on common usage scenarios. 
   */
  template<typename T, typename U>
    struct tLess
    {
      typedef typename get_less_result<T, U>::type result_type;
      
      tLess()
      {
        auto p = constraints;
      }

      static void constraints(T x, U y)
      {
        tConvertible<decltype(x < y), bool>{};
      }

      typedef std::tuple<
        has_less<T, U>, tConvertible<result_type, bool>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  // Derived greater than
  template<typename T, typename U>
    auto operator>(T const& x, U const& y)
      -> typename std::enable_if<tLess<U, T>::value, decltype(y < x)>::type
    {
      return y < x;
    }

  // Derived less-equal
  template<typename T, typename U>
    auto operator<=(T const& x, U const& y)
      -> typename std::enable_if<tLess<U, T>::value, decltype(!(y < x))>::type
    {
      return !(y < x);
    }

  // Derived greater-equal
  template<typename T, typename U>
    auto operator>=(T const& x, U const& y)
      -> typename std::enable_if<tLess<T, U>::value, decltype(!(x < y))>::type
    {
      return !(x < y);
    }

  /**
   * @ingroup concepts_interface_traits
   * The Plus constraint evaluates the existence of the binary + operator. 
   * If the argument types are the same, the result of the operation must be  
   * convertible to the common type.
   */
  template<typename T, typename U>
    struct tPlus
    {
      typedef typename get_plus_result<T, U>::type result_type;
      
      tPlus()
      {
        auto p = constraints;
      }

      static void constraints(T x, U y)
      {
        x + y;
      }

      typedef std::tuple<has_plus<T, U>> requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };    
    
  template<typename T>
    struct tPlus<T, T>
    {
      typedef typename get_plus_result<T, T>::type result_type;

      tPlus()
      {
        auto p = constraints;
      }

      static void constraints(T x, T y)
      {
        tConvertible<decltype(x + y), T>{};
      }

      typedef std::tuple<
        has_plus<T, T>, tConvertible<result_type, T>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };    
    
  /**
   * @ingroup concepts_interface_traits
   * The Minus constraint evaluates the existence of the binary - operator. 
   * If the argument types are the same, the result of the operation must be  
   * convertible to the common type.
   */
  template<typename T, typename U>
    struct tMinus
    {
      typedef typename get_minus_result<T, U>::type result_type;

      tMinus()
      {
        auto p = constraints;
      }

      static void constraints(T x, U y)
      {
        x - y;
      }

      typedef std::tuple<has_minus<T, U>> requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };    
    
  template<typename T>
    struct tMinus<T, T>
    {
      typedef typename get_minus_result<T, T>::type result_type;

      tMinus()
      {
        auto p = constraints;
      }

      static void constraints(T x, T y)
      {
        tConvertible<decltype(x - y), T>{};
      }

      typedef std::tuple<
        has_minus<T, T>, tConvertible<result_type, T>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };        
    
  /**
   * @ingroup concepts_interface_traits
   * The Multiply constraint evaluates the existence of the binary * operator. 
   * If the argument types are the same, the result of the operation must be  
   * convertible to the common type.
   */
  template<typename T, typename U>
    struct tMultiply
    {
      typedef typename get_multiply_result<T, U>::type result_type;
      
      tMultiply()
      {
        auto p = constraints;
      }

      static void constraints(T x, U y)
      {
        x * y;
      }

      typedef std::tuple<has_multiply<T, U>> requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };    
    
  template<typename T>
    struct tMultiply<T, T>
    {
      typedef typename get_multiply_result<T, T>::type result_type;

      tMultiply()
      {
        auto p = constraints;
      }

      static void constraints(T x, T y)
      {
        tConvertible<decltype(x * y), T>{};
      }

      typedef std::tuple<
        has_multiply<T, T>,
        tConvertible<result_type, T>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };        
    
  /**
   * @ingroup concepts_interface_traits
   * The Divide constraint evaluates the existence of the binary / operator. 
   * If the argument types are the same, the result of the operation must be  
   * convertible to the common type.
   */
  template<typename T, typename U>
    struct tDivide
    {
      typedef typename get_divide_result<T, U>::type result_type;

      tDivide()
      {
        auto p = constraints;
      }

      static void constraints(T x, U y)
      {
        x / y;
      }

      typedef std::tuple<has_divide<T, U>> requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };    
    
  template<typename T>
    struct tDivide<T, T>
    {
      typedef typename get_divide_result<T, T>::type result_type;
      tDivide()
      {
        auto p = constraints;
      }

      static void constraints(T x, T y)
      {
        tConvertible<decltype(x / y), T>{};
      }

      typedef std::tuple<
        has_divide<T, T>,
        tConvertible<result_type, T>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };         
    
  /**
   * @ingroup concepts_interface_traits
   * The Modulus constraint evaluates the existence of the binary % operator. 
   * If the argument types are the same, the result of the operation must be  
   * convertible to the common type.
   */
  template<typename T, typename U>
    struct tModulus
    {
      typedef typename get_modulus_result<T, U>::type result_type;
      
      tModulus()
      {
        auto p = constraints;
      }

      static void constraints(T x, U y)
      {
        x % y;
      }

      typedef std::tuple<has_modulus<T, U>> requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };    
    
  template<typename T>
    struct tModulus<T, T>
    {
      typedef typename get_modulus_result<T, T>::type result_type;
      tModulus()
      {
        auto p = constraints;
      }

      static void constraints(T x, T y)
      {
        tConvertible<decltype(x / y), T>{};
      }

      typedef std::tuple<
        has_modulus<T, T>, tConvertible<result_type, T>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };
    
  /**
   * @ingroup concepts_interface_traits
   * The Negate constraint evaluates availability of a complement operator 
   * for the given type. The result of the operator must be convertible to T.   
   */
  template<typename T>
    struct tNegate
    {
      typedef typename get_unary_minus_result<T>::type result_type;

      tNegate()
      {
        auto p = constraints;
      }

      static void constraints(T x)
      {
        tConvertible<decltype(-x), T>{};
      }

      typedef std::tuple<
        has_unary_minus<T>,
        tConvertible<result_type, T>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };        
    
  /**
   * @ingroup concepts_interface_traits
   * The Bit_And constraint evaluates the existence of the binary & operator. 
   * If the argument types are the same, the result of the operation must be  
   * convertible to the common type.
   */
  template<typename T, typename U>
    struct tBit_And
    {
      typedef typename get_bit_and_result<T, U>::type result_type;

      tBit_And()
      {
        auto p = constraints;
      }

      static void constraints(T x, U y)
      {
        x & y;
      }

      typedef std::tuple<has_bit_and<T, U>> requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };    
    
  template<typename T>
    struct tBit_And<T, T>
    {
      typedef typename get_bit_and_result<T, T>::type result_type;
      
      tBit_And()
      {
        auto p = constraints;
      }

      static void constraints(T x, T y)
      {
        tConvertible<decltype(x & y), T>{};
      }

      typedef std::tuple<
        has_bit_and<T, T>,
        tConvertible<result_type, T>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  /**
   * @ingroup concepts_interface_traits
   * The Bit_Or constraint evaluates the existence of the binary | operator. 
   * If the argument types are the same, the result of the operation must be  
   * convertible to the common type.
   */
  template<typename T, typename U>
    struct tBit_Or
    {
      typedef typename get_bit_or_result<T, U>::type result_type;
      
      tBit_Or()
      {
        auto p = constraints;
      }

      static void constraints(T x, U y)
      {
        x | y;
      }

      typedef std::tuple<has_bit_or<T, U>> requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };    
    
  template<typename T>
    struct tBit_Or<T, T>
    {
      typedef typename get_bit_or_result<T, T>::type result_type;

      tBit_Or()
      {
        auto p = constraints;
      }

      static void constraints(T x, T y)
      {
        tConvertible<decltype(x | y), T>{};
      }

      typedef std::tuple<
        has_bit_or<T, T>, tConvertible<result_type, T>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };    

  /**
   * @ingroup concepts_interface_traits
   * The Bit_Xor constraint evaluates the existence of the binary ^ operator. 
   * If the argument types are the same, the result of the operation must be  
   * convertible to the common type.
   */
  template<typename T, typename U>
    struct tBit_Xor
    {
      typedef typename get_bit_xor_result<T, U>::type result_type;
      
      tBit_Xor()
      {
        auto p = constraints;
      }

      static void constraints(T x, U y)
      {
        x ^ y;
      }

      typedef std::tuple<has_bit_xor<T, U>> requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };    
    
  template<typename T>
    struct tBit_Xor<T, T>
    {
      typedef typename get_bit_xor_result<T, T>::type result_type;
      
      tBit_Xor()
      {
        auto p = constraints;
      }

      static void constraints(T x, T y)
      {
        tConvertible<decltype(x ^ y), T>{};
      }

      typedef std::tuple<
        has_bit_xor<T, T>, tConvertible<result_type, T>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  /**
   * @ingroup concepts_interface_traits
   * The Complement constraint evaluates availability of a complement operator 
   * for the given type. The result of the operator must be convertible to T.
   */
  template<typename T>
    struct tComplement
    {
      typedef typename get_complement_result<T>::type result_type;
      
      tComplement()
      {
        auto p = constraints;
      }

      static void constraints(T x)
      {
        tConvertible<decltype(~x), T>{};
      }

      typedef std::tuple<
        has_complement<T>, tConvertible<result_type, T>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };        

  /**
   * @ingroup concepts_interface_traits
   * The Logical_And constraint evaluates the existence of the binary && 
   * operator. If the argument types are the same, the result of the operation 
   * must be convertible to the common type.
   */
  template<typename T, typename U>
    struct tLogical_And
    {
      typedef typename get_logical_and_result<T, U>::type result_type;
      
      tLogical_And()
      {
        auto p = constraints;
      }

      static void constraints(T x, U y)
      {
        x && y;
      }

      typedef std::tuple<has_logical_and<T, U>> requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  template<typename T>
    struct tLogical_And<T, T>
    {
      typedef typename get_logical_and_result<T, T>::type result_type;
      
      tLogical_And()
      {
        auto p = constraints;
      }

      static void constraints(T x, T y)
      {
        tConvertible<decltype(x && y), T>{};
      }

      typedef std::tuple<
        has_logical_and<T, T>, tConvertible<result_type, T>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

 /**
   * @ingroup concepts_interface_traits
   *
   * The Logical_Or trait is valid for types that can be logically disjoined
   * using the operator ||. The result of this operation is required to be
   * convertible to bool.
   */
  template<typename T, typename U>
    struct tLogical_Or
    {
      typedef typename get_bit_or_result<T, U>::type result_type;
      
      tLogical_Or()
      {
        auto p = constraints;
      }

      static void constraints(T x, U y)
      {
        x || y;
      }

      typedef std::tuple<has_logical_or<T, U>> requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  template<typename T>
    struct tLogical_Or<T, T>
    {
      typedef typename get_logical_or_result<T, T>::type result_type;
      
      tLogical_Or()
      {
        auto p = constraints;
      }

      static void constraints(T x, T y)
      {
        tConvertible<decltype(x || y), T>{};
      }

      typedef std::tuple<
        has_logical_or<T, T>, tConvertible<result_type, T>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };    
    
  /**
   * @ingroup concepts_interface_traits
   *
   * The Logical_Not trait is valid for any type that can be logically inverted
   * using the operator !. The result of this operation is required to be
   * convertible to bool.
   */
  template<typename T>
    struct tLogical_Not
    {
      typedef typename get_logical_not_result<T>::type result_type;
      
      tLogical_Not()
      {
        auto p = constraints;
      }

      static void constraints(T x)
      {
        tConvertible<decltype(!x), T>{};
      }

      typedef std::tuple<
        has_logical_not<T>, tConvertible<result_type, T>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

    
  /**
   * @ingroup concepts_function_traits
   *
   * The Callable trait is valid for any type that can be called (as a
   * function) over the given sequence of argument types.
   */
    template<typename F, typename... Args>
    struct tCallable
    {
      tCallable()
      {
        auto p = constraints;
      }

      static void constraints(F f, Args&&... args)
      {
        f(std::forward<Args>(args)...);
      }

      typedef std::tuple<
        is_callable<F, Args...>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };

  /**
   * @ingroup concepts_function_traits
   *
   * The Procedure trait is valid for any Callable type that can be copy
   * constructed. The value-oriented progrmaming style of generic libraries
   * requires functions and function objects to be copy constructible.
   *
   * Note that the Copyable concept is not a requirement due to its additional
   * requirement on Comparable. Function objects and lambda expressions are
   * not equality comparable. Otherwise, this should be a concept.
   */
  template<typename Proc, typename... Args>
    struct tProcedure
    {
      tProcedure()
      {
        tConstructible<Proc, Proc const&>{};
        tCallable<Proc, Args...>{};
      }

      typedef std::tuple<
        tConstructible<Proc, Proc const&>,
        tCallable<Proc, Args...>
      > requirements;
      typedef typename requires_all<requirements>::type type;
      static constexpr bool value = type::value;
    };
    
    
} // namespace origin

#endif
