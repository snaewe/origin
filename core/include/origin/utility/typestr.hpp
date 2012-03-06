// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_UTILITY_TYPESTR_HPP
#define ORIGIN_UTILITY_TYPESTR_HPP

#include <string>
#include <cstring>
#include <typeinfo>

#if defined(__GNUC__)
#include <cxxabi.h>
#endif

namespace origin
{
  template <typename... Args> struct typestr_dispatch;
  template <typename T> struct type_to_string;
  template <typename... Args> struct typelist_to_string;
  
  

  // Typestr  
  // Return a textual representation of the explicitly specified type name.
  // Note that if multiple arguments are given, the result is printed in
  // initializer list format, "{T1, T2, ...}"
  template <typename... Args>
    inline std::string typestr()
    {
      return std::move(typestr_dispatch<Args...>{}());
    }
    
  // Return a textual representation of the type name of the given argument.
  //
  // Note that the printed strings will reflect the type deduction rules for
  // perfect forwarding; lvalues arguments will be result in references, and
  // rvalues in unqualified value types.
  template <typename... Args>
    inline std::string typestr(Args&&...)
    {
      return std::move(typestr<Args...>());
    }
    
  // The typestr dispatcher is used to format type sequences. If multiple types
  // are given, the dispatcher will generate the string "{T1, T2, ...}". If a 
  // single type is given, the result is just the name of the type.
  template <typename... Args>
    struct typestr_dispatch
    {
      std::string operator()() const
      {
        return "{" + std::move(typelist_to_string<Args...>{}()) + "}";
      }
    };
    
  template <typename T>
    struct typestr_dispatch<T>
    {
      std::string operator()() const
      {
        return std::move(type_to_string<T>{}());
      }
    };

    
  // Type to str.
  // The type_to_string class is a function object used to generate the name 
  // of a type. It is specialized for cv qualifiers and references so that 
  // they are preserved in the generated string.
  template <typename T>
    struct type_to_string
    {
      static constexpr std::size_t max_buffer = 8192;

      std::string operator()() const
      {
  #if defined(__GNUC__)
        char buf[max_buffer];
        std::size_t n = max_buffer;
        abi::__cxa_demangle(typeid(T).name(), buf, &n, 0); 
        return std::move(std::string(buf, ::strlen(buf)));
  #endif
      }
    };
    
  // Generate type names for const-qualified types.
  template <typename T>
    struct type_to_string<T const>
    {
      std::string operator()() const
      {
        return std::move(typestr<T>() + " const");
      }
    };
  
  // Generate type names for volatile-qualified types.
  template <typename T>
    struct type_to_string<T volatile>
    {
      std::string operator()() const
      {
        return std::move(typestr<T>() + " volatile");
      }
    };

  // Generate type names for cv-qualified types.
  template <typename T>
    struct type_to_string<T const volatile>
    {
      std::string operator()() const
      {
        return std::move(typestr<T>() + " const volatile");
      }
    };
    
  // Generate type names for lvalue-reference types.
  template <typename T>
    struct type_to_string<T&>
    {
      std::string operator()() const
      {
        return std::move(typestr<T>() + "&");
      }
    };

  // Generate type names for rvalue-reference types.
  template <typename T>
    struct type_to_string<T&&>
    {
      std::string operator()() const
      {
        return std::move(typestr<T>() + "&&");
      }
    };
    
    
    
  // Type list to string
  // Compute an intercalated list of type names (i.e., T1, T2, ..., Tn). This 
  // function (object) is intended to be used by typestr.
  template <typename T, typename... Args>
    struct typelist_to_string<T, Args...>
    {
      std::string operator()() const
      {
        return typestr<T>() + ", " + typelist_to_string<Args...>{}();
      }
    };

  // Specialization for a single type.
  template <typename T>
    struct typelist_to_string<T>
    {
      std::string operator()() const { return typestr<T>(); }
    };

  // Specialization for an empty type list
  template<>
    struct typelist_to_string<>
    {
      std::string operator()() const { return std::string{}; }
    };

} // namespace origin

#endif
