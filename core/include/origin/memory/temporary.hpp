// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MEMORY_TEMPORARY_HPP
#define ORIGIN_MEMORY_TEMPORARY_HPP

#include <utility>

namespace origin
{
  template <typename I> 
    constexpr bool Forward_iterator();


  // FIXME: The acutal allocation should be performed in the binary library.

  // TODO: What are my options with this class? Can I do anyhting interesting
  // with it? Allocate a buffer once and leave it in memory until something
  // larger is asked for?

  template <typename T>
    inline std::pair<T*, std::ptrdiff_t> get_temporary_buffer(std::ptrdiff_t n) noexcept
    {
      std::pair<T*, std::ptrdiff_t> result;
      try {
        result.first = new T[n];
        result.second = n;
      } catch(...) {
        result.first = nullptr;
        result.second = 0;
      }
      return result;
    }

  template <typename T>
    inline void return_temporary_buffer(T* p) noexcept
    {
      delete [] p;
    }



  // Temporary buffer
  // A temporary buffer is like a unique pointer to a dynamically sized array
  // except that it is specifically designed to work with the specialized
  // temporary buffer routines above. 
  //
  // This class is s
  template <typename T>
    class temporary_buffer
    {
      std::pair<T*, std::ptrdiff_t> buf;

    public:
      temporary_buffer() = delete;
      
      // FIXME: What are the move semantics of this class? It should probably
      // be movable.

      // Non-copyable
      temporary_buffer(const temporary_buffer&) = delete;
      temporary_buffer& operator=(const temporary_buffer&) = delete;
      
      
      explicit temporary_buffer(std::ptrdiff_t n)
        : buf { get_temporary_buffer<T>(n) }
      { }
      
      // Allocate a temporary buffer, and if enough memory is returned, copy
      // [first, last) elements into it. Note that no copy is performed if the 
      // memory is not allocated.
      template <typename I>
        temporary_buffer(I first, I last)
          : buf {get_temporary_buffer<T>(distance(first, last))}
        { 
          static_assert(Forward_iterator<I>(), "");
          if (!empty())
            copy(first, last, buf.first);
        }

      // Allocate a temporary buffer, and if enough memory is returned, copy
      // [first, first + n) elements into it. Note that no copy is performed
      // if the memory is not allocated.
      template <typename I>
        temporary_buffer(I first, std::ptrdiff_t n)
          : buf {get_temporary_buffer<T>(n)}
        {
          if (!empty())
            copy_n(first, n, buf.first);
        }

      ~temporary_buffer() { return_temporary_buffer(buf.first); }
      

      // Returns true if the allocated buffer is empty (i.e., insufficient
      // memory was allocated).
      bool empty() const { return buf.second == 0; }

      // Returns the number of objects allocated to the temporary buffer.
      std::size_t size() const { return buf.second; }

      // Returns true if the requested length was allocated.
      explicit operator bool() const { return !empty(); }


      // Subscript
      T&       operator[](std::size_t n)       { return buf.first[n]; }
      const T& operator[](std::size_t n) const { return buf.first[n]; }
      

      // Iterators
      T* begin() { return buf.first; }
      T* end()   { return buf.first + buf.second; }
      
      const T* begin() const { return buf.first; }
      const T* end() const   { return buf.first + buf.second; }
    };



} // namespace origin

#endif
