// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MATRIX_COMMON_HPP
#define ORIGIN_MATRIX_COMMON_HPP

#include <cassert>
#include <memory>
#include <algorithm>
#include <iosfwd>

#include <origin/iterator.hpp>

namespace origin
{
  // The matrix base implements some basic facilities for matrix 
  // specializations. In particular, the exception safe allocation and 
  // deallocation of data.
  //
  // FIXME: Actually use the allocator.
  template<typename T, typename Alloc>
    class matrix_base
    {
    public:
      typedef Alloc          allocator_type; 
      typedef T              value_type;
      typedef T&             reference;
      typedef T const&       const_reference;
      typedef T*             pointer;
      typedef T const*       const_pointer;
      typedef std::size_t    size_type;
      typedef std::ptrdiff_t difference_type;

      matrix_base()
        : first(nullptr), last(first)
      { }
      
      matrix_base(size_type n)
        : first(n > 0 ? new value_type[n] : nullptr), last(first + n)
      { }
      
      // CopyConstructible
      matrix_base(matrix_base const& x)
        : first(new value_type[x.size()]), last(first + x.size())
      { 
        std::copy(x.first, x.last, first); 
      }
      
      // Move Constructible
      matrix_base(matrix_base&& x)
        : first(x.first), last(x.last)
      { 
        x.first = x.last = nullptr; 
      }
      
      // Destructible
      ~matrix_base() { delete [] first; }
      
      // Return the number of objects owned by the matrix.
      size_type size() const { return last - first; }
      
      // Operations
      void swap(matrix_base& x)
      {
        std::swap(first, x.first);
        std::swap(last, x.last);
      }

      // These are actually public, just for convenience.
      value_type *first;  // Points to the first data element (or nullptr)
      value_type *last;  // Points past the last element (or equal to first)
    };

  // The matrix class...
  //
  // There is no general implementation of the matrix class. Implementations
  // for 1, 2 and 3 dimensions are implemented as specializations.
  template<typename T, std::size_t N, typename Alloc = std::allocator<T>> 
    class matrix;

  // Functions that elements of a matrix, for use with m.apply()
  // FIXME: These should probably be in functional, and we need to make sure
  // they don't overlap with anything in std (I don't think that they do).
  template<typename T>
    struct assign
    {
      T& operator()(T& a, T const& b) const { return a = b; }
    };

  template<typename T>
    struct plus_assign
    {
      T& operator()(T& a, T const& b) const { return a += b; }
    };

  template<typename T>
    struct minus_assign
    {
      T& operator()(T& a, T const& b) const { return a -= b; }
    };

  template<typename T>
    struct multiplies_assign 
    {
      T& operator()(T& a, T const& b) const { return a *= b; }
    };

  template<typename T>
    struct divides_assign
    {
      T& operator()(T& a, T const& b) const { return a /= b; }
    };

  template<typename T>
    struct modulus_assign
    {
      T& operator()(T& a, T const& b) const { return a %= b; }
    };
}

#include <origin/math/matrix1.hpp>
#include <origin/math/matrix2.hpp>

#endif
