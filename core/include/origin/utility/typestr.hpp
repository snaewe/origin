// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_TYPESTR_HPP
#define ORIGIN_TYPESTR_HPP

#include <string>
#include <cstring>
#include <typeinfo>

#if defined(__GNUC__)
#include <cxxabi.h>
#endif

namespace origin
{
  template<typename T> struct typerep;

  /**
   * Return a textual representation of the explicitly specified type name.
   */
  template<typename T>
    std::string typestr()
    {
      return std::move(typerep<T>{}());
    }
    
  /**
   * Return a textual representation of the type name of the given argument.
   */
  template<typename T>
    inline std::string typestr(T&&)
    {
      return std::move(typestr<T>());
    }

  /**
   * @internal
   * The typerep class is a function object used to generate the name of a type.
   * It is specialized for cv qualifiers and references so that that information
   * is preserved in the requested type name.
   */
  template<typename T>
    struct typerep
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
    
  template<typename T>
    struct typerep<T const>
    {
      std::string operator()() const
      {
        return std::move(typestr<T>() + " const");
      }
    };
    
  template<typename T>
    struct typerep<T volatile>
    {
      std::string operator()() const
      {
        return std::move(typestr<T>() + " volatile");
      }
    };

  template<typename T>
    struct typerep<T const volatile>
    {
      std::string operator()() const
      {
        return std::move(typestr<T>() + " const volatile");
      }
    };
    
  template<typename T>
    struct typerep<T&>
    {
      std::string operator()() const
      {
        return std::move(typestr<T>() + "&");
      }
    };

  template<typename T>
    struct typerep<T&&>
    {
      std::string operator()() const
      {
        return std::move(typestr<T>() + "&&");
      }
    };

} // namespace origin

#endif
