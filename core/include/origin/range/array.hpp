// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_ARRAY_HPP
#define ORIGIN_RANGE_ARRAY_HPP

namespace origin
{
  // Wraps a C array with static bounds and guarantees that it will behave like
  // an array.
  template<typename T, std::size_t N>
    struct array_range
    {
      array_range(T(&a)[N])
        : array(a)
      { }
      
      T* begin() const { return array; }
      T* end()   const { return array + N; }

      T(&array)[N];
    };

  // Return a wrapper around an array that makes it behave like a range.
  // This can be used to disambiguate overloads for functions that take arrays
  // as both ranges and pointers (through decay).
  template<typename T, std::size_t N>
    inline array_range<T, N> arr(T(&a)[N])
    {
      return {a};
    }

  // A constant version of the function above.
  template<typename T, std::size_t N>
    inline array_range<T const, N> arr(T const(&a)[N])
    {
      return {a};
    }

} // namespace origin

#endif
