// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_UTILITY_META_HPP
#define ORIGIN_UTILITY_META_HPP

#include <type_traits>

namespace origin
{
  // The bool constant is an alias for the type integral_constant<bool, X>.
  // This type is provided purely for convenience.
  template <bool B>
    using bool_constant = std::integral_constant<bool, B>;

  
  // Traits and metafunctions on type sequences
    
  // Return the first type in the given sequence of arguments.
  template <typename... Args> struct front_type;

  template <typename T, typename... Args>
    struct front_type<T, Args...>
    { 
      using type = T; 
    };
    
  // Given a non-empty sequence of types, return the first type.
  template <typename T, typename... Args>
    using Front_type = typename front_type<T, Args...>::type;


  
  // Return the last type in the given sequence of types.
  template <typename... Args> struct back_type;

  template <typename T>
    struct back_type<T>
    { 
      using type = T;
    };

  template <typename T, typename... Args>
    struct back_type<T, Args...> : back_type<Args...>
    { };

  // Given a non-empty sequence of types, return the type at the back of
  // the sequence.
  template <typename T, typename... Args>
    using Back_type = typename back_type<T, Args...>::type;
    


  // Returns true if all of the types are the same.
  template <typename... Args> struct are_same;

  // For a single type, this is trivially true.
  template <typename T> 
    struct are_same<T> : std::true_type 
    { };

  // Recursively apply are_same (is_same) to T and Args...
  // FIXME: Does && properly short-circuit the instantiation, or do I need to
  // use std::conditional to make sure thta it's done correctly. How do you
  // test this?
  template <typename T, typename... Args>
    struct are_same<T, Args...>
      : bool_constant<
          std::is_same<T, typename front_type<Args...>::type>::value &&
          are_same<Args...>::value
        >
    { };


  // The Same predicate returns true if all of its type arguments are the same
  // type.
  template <typename... Args>
    constexpr bool Same()
    {
      return are_same<Args...>::value;
    }
    


  // Returns true if the types T and U are not the same. This is an alias for
  // !<is_same<T, U>::value, and is primarily provided fro convenience.
  template <typename T, typename U>
    struct is_different : bool_constant<!std::is_same<T, U>::value>
    { };

    
  // The Different predicate returns true if the type arguments T and U are
  // not the same. This is an alias for !Same<T, U>() and is primarily provided
  // for convenience.
  template <typename T, typename U>
    constexpr bool Different()
    {
      return is_different<T, U>::value;
    }
    
} // namespace origin

#endif

