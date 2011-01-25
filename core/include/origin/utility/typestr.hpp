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
  /**
   * @ingroup util
   * This curious class is a type wrapper that is used to to fully preserve
   * typenames as they are printed. It has no other function.
   */
  template<typename> struct ___ { };

  /**
   * @ingroup util
   * Return a string that describes the type of the given template parameter.
   * The type name depends on the results of the typeid operator.
   *
   * @todo Rewrite this so that demangle will dynamically allocate the memory.
   *
   * @todo Implement this for MSVC. I think MSVC simply returns the type name.
   */
  template <typename T>
  std::string typestr()
  {
#if defined(__GNUC__)
    std::size_t const BUFSIZE = 8192;
    std::size_t n = BUFSIZE;
    char buf[BUFSIZE];
    abi::__cxa_demangle(typeid(___<T>).name(), buf, &n, 0);
    return std::string(buf, ::strlen(buf));
#endif
  }

  /**
   * @ingroup util
   * Return a string that describes the type of the given parameter. The type
   * name depends on the results of the typeid operator.
   */
  template <typename T>
  inline std::string typestr(T const&)
  { return typestr<T>(); }

} // namespace origin

#endif
