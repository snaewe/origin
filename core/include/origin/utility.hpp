// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_UTILITY_HPP
#define ORIGIN_UTILITY_HPP

#include <origin/utility/meta.hpp>
#include <origin/utility/typestr.hpp>

#include <iosfwd>

namespace origin
{
  // Utility types:
  
  // The default_t type is a tag class used to indicate the selection of a
  // default value. This is only used to support class template specialization.
  struct default_t { };

  
  // The unspecified_t type is a tag class used to indicate that an argument
  // for a template parameter has not been specified.
  struct uspecified_t { };

  
  // The empty type is an empty, trivial type that is meant to be used as a
  // placeholder for unspecified types in containers or other data structures.
  struct empty_t { };

  // Make sure that empty is I/O-streamable.
  template<typename Char, typename Traits>
    inline std::basic_ostream<Char, Traits>& 
    operator<<(std::basic_ostream<Char, Traits>& os, empty_t)
    { 
      return os; 
    }

  template<typename Char, typename Traits>
    inline std::basic_istream<Char, Traits>& 
    operator>>(std::basic_istream<Char, Traits>& is, empty_t&)
    { 
      return is; 
    }
    
    
  template<typename T>
    constexpr bool lazy_false()
    {
      return false;
    }

} // namespace origin

#endif
