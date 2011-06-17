// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ORDINAL_MAP_HPP
#define ORIGIN_ORDINAL_MAP_HPP

#include <type_traits>
#include <limits>

#include <origin/utility/meta.hpp>

namespace origin
{
  /**
   * @internal
   * Wrap make_unsigned so that the conditionally enabled ord() for signed
   * types won't generate compiler errors when evaluating make_signed for
   * non-integral types.
   */
  //@{
  // For integral types, just use make_unsigned...
  template<typename T>
    struct make_unsigned_integral_
    {
      typedef typename std::make_unsigned<T>::type type;
    };
   
  // Unless it's boolean, in which case, just return bool (it's already 
  // unsigned).
  template<>
    struct make_unsigned_integral_<bool>
    {
      typedef bool type;
    };
  
  // Only try to call make_unsigned for integral types. If used with classes,
  // return substittuion failure.
  template<typename T, bool = std::is_integral<T>::value>
    struct safe_make_unsigned_
    {
      typedef typename make_unsigned_integral_<T>::type type;
    };
  
  template<typename T>
    struct safe_make_unsigned_<T, false>
    { 
      typedef substitution_failure type;
    };
  //@}
  
  /**
   * The ord() function returns the ordinal for a value of an Ordinal type.
   * 
   * @tparam Ord  An Ordinal type
   * 
   * @param x   A value of type T
   */
  //@{
  // For unsigned integers, ord is just an identity function.
  template<typename Ord>
    inline typename std::enable_if<std::is_unsigned<Ord>::value, Ord>::type
    ord(Ord x)
    {
      return x;
    }
    
  // For signed integers, the first ordinal (0) is the minimum value encoded
  // by the type (numeric_limits<Ord>::min. Values increase from there.
  template<typename Ord>
    inline typename std::enable_if<
      std::is_signed<Ord>::value, typename safe_make_unsigned_<Ord>::type
    >::type
    ord(Ord x)
    {
      typedef typename std::make_unsigned<Ord>::type Result;
      return x + Result{std::numeric_limits<Ord>::max()} + 1u;
    }
  //@}
  
  /**
   * The from_ord operation is the inverse of the ord() operation. Given an
   * ordinal, prototype Ordinal object, the from_ord operation returns the
   * Ordinal object identified by the given ordinal.
   * 
   * @tparam Num  An ordinal number type
   * @tparam Ord  An Ordinal type
   * 
   * @param x   An ordinal of type T
   * @param y   An unused prototype object of type U
   */
  template<typename Num, typename Ord>
    Ord from_ord(Num x, Ord const& y);
  
  /**
   * The ordinal_number_type trait returns the associated ordinal number type 
   * for an Ordinal type. This is deduced as the result of the ord() operation.
   */
  template<typename Ord>
    struct ordinal_number_type
    {
      typedef decltype(ord(std::declval<Ord>())) type;
    };
  
  /**
   * The is_ordinal_number type trait is true for all types that represent 
   * ordinal numbers. Currently this is limited to unsigned integral types.
   */
  template<typename Num>
    struct is_ordinal_number : std::is_unsigned<Num>
    { };

  // FIXME: Can we generalize the notion of an Ordinal_Number to include more
  // abstract concepts such as ordinals of infinite sequences? Probably, but
  // that would be future work, and it's not clear whether or not there's any
  // great need for it.
  /**
   * The ordinal cast function converts an ordinal number into an object of
   * the specified Ordinal type.
   * 
   * @tparam Ord  Explicitly specified, the resulting Ordinal type
   * @tparam Num  The ordinal number type
   * 
   * @param x   An ordinal number
   */
  //@{
  // For integral ordinal casts, the result type and the input type must be
  // the same size, otherwise we have to address widening and narrowing.
  
  // For ordinal cast to unsigned, just return the value.
  template<typename Ord, typename Num>
    inline typename std::enable_if<
      is_ordinal_number<Num>::value && std::is_unsigned<Ord>::value, Ord
    >::type
    ordinal_cast(Num x) 
    {
      static_assert(sizeof(Ord) == sizeof(Num), 
                    "cannot ordinal_cast types of different size");
      return static_cast<Ord>(x);
    }
  
  // For ordinal cast to signed, subtract half of the values.
  template<typename Ord, typename Num>
    inline typename std::enable_if<
      is_ordinal_number<Num>::value && std::is_signed<Ord>::value, Ord
    >::type
    ordinal_cast(Num x) 
    {
      static_assert(sizeof(Ord) == sizeof(Num), 
                    "cannot ordinal_cast types of different size");
      return Ord{x + std::numeric_limits<Ord>::min()};
    }

  // For enumerated types, we'll simply assume that the user knows what their
  // doing and has properly defined their enum and ordinal mapping. Note that
  // we don't actually check the sizeof the ordinal value type. It's probably
  // going to be size_t and that may be larger than sizeof(Ord).
  template<typename Ord, typename Num>
    inline typename std::enable_if<
      is_ordinal_number<Num>::value && std::is_enum<Ord>::value, Ord
    >::type
    ordinal_cast(Num x)
    {
      return from_ord(x, Ord{});
    }
  
  // For user-defined classs types, the ordinal type must be constructible
  // over the ordinal value type.
  // FIXME: This may be a serious design flaw. If the Ord type already uses
  // a numeric constructor for some other purpose, then we would be trying to
  // reuse it for our own goals, and this could easily lead to program errors.
  // Maybe it would be better to simply require from_ord for all non-integral
  // types.
  template<typename Ord, typename Num>
    inline typename std::enable_if<
      is_ordinal_number<Num>::value && std::is_class<Ord>::value,  Ord
    >::type
    ordinal_cast(Num x)
    {
      // tConstructible<Ord, Num>
      return Ord{x};
    }
  //@}

  /**
   * Ordinal identity requires that every value of an Ordinal type has an
   * ordinal value, and that the initial value can be uniquely identified from 
   * that ordinal.
   */
  template<typename Ord>
    bool aOrdinal_Identity(Ord x)
    {
      return ordinal_cast<Ord>(ord(x)) == x;
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

