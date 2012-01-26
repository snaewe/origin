// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MEMORY_HPP
#define ORIGIN_MEMORY_HPP

#include <algorithm>

#include <origin/concepts.hpp>

namespace origin
{
  // Forward declarations
  template<typename I, typename O> O o_copy(I, I, O);
  template<typename I, typename O> O o_copy_n(I, Distance_type<I>, O);

  
  
  // A temporary buffer is like a unique pointer to a dynamically sized array
  // except that it is specifically designed to work with the standard library's
  // temporary buffer facilities.
  template<typename T>
    class temporary_buffer
    {
    public:
      temporary_buffer() = delete;
      
      // Non-copyable
      temporary_buffer(const temporary_buffer&) = delete;
      temporary_buffer& operator=(const temporary_buffer&) = delete;
      
      // FIXME: Should this be non-movable?
      
      explicit temporary_buffer(std::ptrdiff_t n)
        : len{n}, buf{std::get_temporary_buffer<T>(n)}
      { }
      
      // Allocate a temporary buffer, and if enough memory is returned, copy
      // [first, last) elements into it. Note that no copy is performed if the 
      // memory is not allocated.
      template<typename I>
        temporary_buffer(I first, I last)
          : len{o_distance(first, last)}, buf{std::get_temporary_buffer<T>(len)}
        {
          if(len == buf.second)
            o_copy(first, last, buf.first);
        }

      // Allocate a temporary buffer, and if enough memory is returned, copy
      // [first, first + n) elements into it. Note that no copy is performed
      // if the memory is not allocated.
      template<typename I>
        temporary_buffer(I first, std::ptrdiff_t n)
          : len{n}, buf{std::get_temporary_buffer<T>(n)}
        {
          if(len == buf.second)
            o_copy_n(first, n, buf.first);
        }

      ~temporary_buffer() { std::return_temporary_buffer(buf.first); }
      
      // Boolean
      // Returns true if the requested length was allocated.
      explicit operator bool() const { return len == buf.second; }

      // Subscript
      T&       operator[](std::size_t n)       { return buf.first[n]; }
      const T& operator[](std::size_t n) const { return buf.first[n]; }
      
      // Range
      T* begin() { return buf.first; }
      T* end()   { return buf.first + buf.second; }
      
      const T* begin() const { return buf.first; }
      const T* end() const   { return buf.first + buf.second; }
     
    private:
      std::ptrdiff_t len;                // Requested buffer size
      std::pair<T*, std::ptrdiff_t> buf; // Allocated buffer and length
    };

} // namespace origin

#endif