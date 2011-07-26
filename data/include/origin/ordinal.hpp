// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ORDINAL_HPP
#define ORIGIN_ORDINAL_HPP

#include <type_traits>
#include <limits>

#include <origin/utility/meta.hpp>

namespace origin
{
  // FIXME: Should ord() return a type-specific ordinal number type or just 
  // size_t (like hash). Returning only size_t makes this conceptually much,
  // much simpler to write.

  template<typename T> struct ordinal_traits;
  template<typename T> struct is_ordinal;
  template<typename T> struct ordinal_number_type;
  
  /**
   * @internal
   * Wrap make_unsigned so that the conditionally enabled ord() for signed
   * types won't generate compiler errors when evaluating make_signed for
   * non-integral types.
   */
  //@{
  template<typename T>
    struct make_unsigned_integral
    {
      typedef typename std::make_unsigned<T>::type type;
    };
   
  // Unless it's boolean Then, just return bool since it's already unsigned.
  template<>
    struct make_unsigned_integral<bool>
    {
      typedef bool type;
    };
  
  // Only try to call make_unsigned for integral types. If used with classes,
  // return substitution failure.
  template<typename T, bool = std::is_integral<T>::value>
    struct safe_make_unsigned
    {
      typedef typename make_unsigned_integral<T>::type type;
    };
  
  template<typename T>
    struct safe_make_unsigned<T, false>
    { 
      typedef substitution_failure type;
    };
  //@}

  // For unsigned integral types, the associated ordinal number type is the
  // same as T and chr() is an identity function.
  template<typename T, bool Unsigned = true>
    struct integral_ordinal_traits
    {
      static T chr(T n)
      {
        return n;
      }
    };
    
  // For signed integral types, the associated ordinal number type is the
  // unsigned representation of T. The chr() function returns the unsigned
  template<typename T>
    struct integral_ordinal_traits<T, false>
    {
      typedef typename std::make_unsigned<T>::type Number;
      
      static T chr(Number n)
      {
        return Number{std::numeric_limits<T>::max()} - n + 1;
      }
    };

  // Check if T defines a member ord() function. This returns the result type
  // of the member_ord function.
  template<typename T>
    struct check_member_ord
    {
    private:
      template<typename X>
        static auto check(X x) -> decltype(x.ord());
      static substitution_failure check(...);
    public:
      typedef decltype(check(std::declval<T>())) type;
    };
  
  // Return true x.ord() is a valid expression for on object x of type T.
  template<typename T>
    struct has_member_ord
      : substitution_succeeded<typename check_member_ord<T>::type>
    { };

  // Check if T defines a static chr() member taking an argument of Ord type.
  template<typename T, typename Ord>
    struct check_member_chr
    {
    private:
      template<typename X, typename Y>
        static auto check(X, Y y) -> decltype(X::chr(y));
      static substitution_failure check(...);
    public:
      typedef decltype(check(std::declval<T>(), std::declval<Ord>())) type;
    };
    
  // return true if x.chr(y) is a valid expression for an object x of type T
  // and an ordinal number y.
  template<typename T, typename Ord>
    struct has_member_chr
      : substitution_succeeded<typename check_member_chr<T, Ord>::type>
    { };
    

  /*
  // Return true if T has an ord() function. Note that there is a default model
  // of ord() for all classes with member ords. The overload emulates the
  // member lookup rule.
  template<typename T>
    struct check_ord
    {
    private:
      template<typename X>
        static auto check(X x) -> decltype(ord(x));
      static substitution_failure check(...);
    public:
      typedef decltype(check(std::declval<T>())) type;
    };
  
  // Return true if ord(x) is a valid expression for an object x with type T.
  template<typename T>
    struct has_ord
      : substitution_succeeded<typename check_ord<T>::type>
    { };
  */
    
  // Return true if T supports the ordinal interface. That is, ord() is defined
  // as either a member or not and there is a member chr() function.
  //
  // FIXME: There seems to be a limitation where I can't mix and match a free
  // function ord() with a member chr(). At least I can't write this check in
  // terms of a free ord() since it would otherwise be recursive.
  template<typename T>
    struct has_ordinal_interface
      : bool_constant<
          has_member_ord<T>::value && 
          has_member_chr<T, typename check_member_ord<T>::type>::value
        >
    { };
    
  // If T supports the member interface for ordinal functions, then the
  // default traits class will use the member chr to translate from ordinal
  // numbers to objects. There is a corresponding overload of ord() below.
  template<typename T, bool Member = false>
    struct member_ordinal_traits;
    
  template<typename T>
    struct member_ordinal_traits<T, true>
    {
      typedef typename check_member_ord<T>::type Number;
      
      static T chr(Number n)
      {
        return T::chr(n);
      }
    };
    
  // Derive a set of default ordinal traits based on whether the type is a
  // built-in integral type or a class type. 
  template<typename T, bool Int, bool Class>
    struct default_ordinal_traits;
    
  template<typename T, bool Class>
    struct default_ordinal_traits<T, true, Class>
      : integral_ordinal_traits<T, std::is_unsigned<T>::value>
    { };
    
  template<typename T, bool Int>
    struct default_ordinal_traits<T, Int, true>
      : member_ordinal_traits<T, has_ordinal_interface<T>::value>
    { };

  /**
   * @ingroup ordinal
   * 
   * Ordinal traits defines an association between an ordinal type and its
   * associated value type.
   */
  template<typename T>
    struct ordinal_traits 
      : default_ordinal_traits<
          T, std::is_integral<T>::value, std::is_class<T>::value
        >
    { };
    
  // Explicitly specialize ordinal traits for boolean types. We can be 
  // fairly general with boolean/ordinal conversions. Inf
  template<>
    struct ordinal_traits<bool>
    {
      static bool chr(unsigned int n)
      {
        return bool{n};
      }
    };

  /**
   * @ingroup ordinal
   * 
   * The ordinal_number_type trait returns the associated ordinal number type 
   * for an Ordinal type. This is deduced as the result of the ord() operation.
   */
  template<typename Ord>
    struct ordinal_number_type
    {
      typedef decltype(ord(std::declval<Ord>())) type;
    };

  /**
   * @ingroup ordinal
   * 
   * The ordinal cast function converts an ordinal number into an object of
   * the specified Ordinal type.
   * 
   * @tparam Ord  Explicitly specified, the resulting Ordinal type
   * @tparam Num  The ordinal number type
   * 
   * @param x   An ordinal number
   */
  template<typename T, typename Num>
    inline typename std::enable_if<is_ordinal<Num>::value, T>::type
    ordinal_cast(Num n) 
    {
      return ordinal_traits<T>::chr(n);
    }
  
  // Default models.

  // For unsigned integers, ord is just an identity function.
  template<typename T>
    inline typename std::enable_if<std::is_unsigned<T>::value, T>::type
    ord(T x)
    {
      return x;
    }
    
  // For signed integers, the first ordinal (0) is the minimum value encoded
  // by the type (numeric_limits<Ord>::min. Values increase from there.
  template<typename T>
    inline typename std::enable_if<
      std::is_signed<T>::value, typename safe_make_unsigned<T>::type
    >::type
    ord(T x)
    {
      typedef typename std::make_unsigned<T>::type Result;
      return x + Result{std::numeric_limits<T>::max()} + 1u;
    }
    
  // For class types with a member ord(), generate a delegating free
  // function.
  template<typename T>
    inline auto ord(T x)
      -> typename std::enable_if<has_member_ord<T>::value, decltype(x.ord())>::type
    {
      return x.ord();
    }
  //@}
  

  // NOTE: Defined after the default models so we don't get lookup errors with
  // ord(). Very annoying. Concepts needed.

  // Check if T is ordinal. Any type specializing ordinal_traits will be 
  // ordinal since it requires a definition of chr for ordinal's value type.
  template<typename T>
    struct check_ordinal
    {
    private:
      template<typename X, typename Y> 
        static auto check(X x, Y y) -> decltype(ordinal_traits<X>::chr(y));
      static substitution_failure check(...);
    public:
      typedef decltype(check(std::declval<T>(), ord(std::declval<T>()))) type;
    };
    
  /**
   * @ingroup ordinal
   * 
   * Return true if the type T is Ordinal. Ordinal types provide implement
   * the ord() and chr() functions.
   */
  template<typename T>
    struct is_ordinal
      : substitution_succeeded<typename check_ordinal<T>::type>
    { };


  /**
   * Ordinal identity requires that every value of an Ordinal type has an
   * ordinal value, and that the initial value can be uniquely identified from 
   * that ordinal.
   */
  template<typename T>
    bool aOrdinal_Identity(T x)
    {
      return ordinal_cast<T>(ord(x)) == x;
    }

  /**
   * Ordinal equality guarantees that equal values of Ordinal type have the
   * same ordinal value.
   */
  template<typename Ord>
    bool aOrdinal_Equality(Ord x, Ord y)
    {
      return x == y ? ord(x) == ord(y) : true;
    }
  
} // namespace origin

#endif
