// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MATH_MATRIX_HPP
#  error Do not include this file directly. Include matrix/matrix.hpp.
#endif


// -------------------------------------------------------------------------- //
//                                Matrix Slice
//
// A matrix slice specifies the N-dimensional matrix properties of a contiguous
// region of memory. The slice is primarily described by 3 parameters:
// A sequence of extents, a sequence of strides, and a starting offset. An
// extent describes an array bound in a particular dimension. A stride defines
// the distance between sub-matrices in a particular dimension. This class
// also maintains the total number of elements in the slice, which is just the
// product of dimensions.
template <std::size_t N>
  struct matrix_slice
  {
    static constexpr std::size_t order = N;

    matrix_slice() = default;

    // Copy semantics
    matrix_slice(const matrix_slice&) = default;
    matrix_slice& operator=(const matrix_slice&) = default;


    // Create a slice from a range of extents with the a starting offset, s.
    // This requires that size(range) == N.
    template <typename R>
      matrix_slice(std::size_t s, R&& range);

    // Create a slice with a starting offset s, and the extents, exts.
    matrix_slice(std::size_t s, std::initializer_list<std::size_t> exts);

    // Create a slice with a starting offset s, a sequence of extents (exts),
    // and a sequence of strides (strs).
    matrix_slice(std::size_t s, 
                 std::initializer_list<std::size_t> exts,
                 std::initializer_list<std::size_t> strs);

    // Offset
    //
    // Compute the offset of an element that corresponds to a sequence of
    // indexes. The indexes can be given using the following syntaxes:
    //
    //    s(dims...)   // N index arguments
    //    s(range)     // A range containing N arguments
    //    s({dims...}) // An initializer list with N arguments
    //
    // This computation does not check that each index is within the bound
    // specified by its correspdonding extent.

    template <typename... Dims, 
              typename = Requires<All(Convertible<Dims, std::size_t>()...)>>
      std::size_t operator()(Dims... dims) const;

    template <typename R, typename = Requires<Range<R>()>>
      std::size_t operator()(R&& range) const;

    template <typename T>
      std::size_t operator()(std::initializer_list<T> list) const;


    std::size_t size;
    std::size_t start;
    std::size_t extents[N];
    std::size_t strides[N];
  };


template <std::size_t N>
  template <typename R>
    matrix_slice<N>::matrix_slice(std::size_t s, R&& range)
      : start(s)
    {
      using std::begin;
      using std::end;
      std::copy(begin(range), end(range), extents);
      size = matrix_impl::init_row_major(extents, strides);
    }

template <std::size_t N>
  matrix_slice<N>::matrix_slice(std::size_t s, 
                                std::initializer_list<std::size_t> exts)
    : start(s)
  {
    assert(exts.size() == N);
    std::copy(exts.begin(), exts.end(), extents);
    size = matrix_impl::init_row_major(extents, strides);
  }

template <std::size_t N>
  matrix_slice<N>::matrix_slice(std::size_t s, 
                                std::initializer_list<std::size_t> exts,
                                std::initializer_list<std::size_t> strs)
    : start(s)
  {
    assert(exts.size() == N);
    assert(strs.size() == N);
    std::copy(exts.begin(), exts.end(), extents);
    std::copy(strs.begin(), strs.end(), strides);

    // Compute the size of the slice.
    std::multiplies<std::size_t> mul;
    constexpr std::size_t zero = 0;
    size = std::accumulate(extents, extents + N, zero, mul);
  }


template <std::size_t N>
  template <typename... Dims, typename X>
  inline std::size_t
    matrix_slice<N>::operator()(Dims... dims) const
    {
      static_assert(sizeof...(Dims) == N, "");
      std::size_t indexes[N] {std::size_t(dims)...};
      return operator()(indexes);
    }

template <std::size_t N>
  template <typename R, typename X>
    inline std::size_t
    matrix_slice<N>::operator()(R&& range) const
    {
      using std::begin;
      using std::end;
      constexpr std::size_t zero = 0;
      return start + std::inner_product(strides, strides + N, begin(range), zero);
    }

template <std::size_t N>
  template <typename T>
    inline std::size_t
    matrix_slice<N>::operator()(std::initializer_list<T> list) const
    {
      constexpr std::size_t zero = 0;
      return start + std::inner_product(strides, strides + N, list.begin(), zero);
    }


// -------------------------------------------------------------------------- //
//                              Equality Comparison
//
// Two strides compare equal when they describe the same sequence of offsets.

template <std::size_t N>
  inline bool
  operator==(const matrix_slice<N>& a, const matrix_slice<N>& b)
  {
    return a.start == b.start
        && std::equal(a.extents, a.extents + N, b.extents)
        && std::equal(a.strides, a.strides + N, b.strides);
  }

template <std::size_t N>
  inline bool
  operator!=(const matrix_slice<N>& a, const matrix_slice<N>& b)
  {
    return !(a == b);
  }


// -------------------------------------------------------------------------- //
//                              Streaming
//
// Primarily for debugging purposes, print a slice descriptor as a triple:
// start, extents, and strides.
template <typename C, typename T, std::size_t N>
  std::basic_ostream<C, T>&
  operator<<(std::basic_ostream<C, T>& os, const matrix_slice<N>& s)
  {
    os << '[' << s.start << ',' << '[';
    for (auto i = 0; i < N - 1; ++i)
      os << s.extents[i] << ',';
    os << s.extents[N-1] << ']' << ',' << '[';
    for (auto i = 0; i < N - 1; ++i)
      os << s.strides[i] << ',';
    os << s.strides[N-1] << ']' << ']';
    return os;
  }



// -------------------------------------------------------------------------- //
//                              Same Extents
//
// The same_extents function returns true when two slices describe matrices
// with the same order and extents. The starting offset and strides do not
// factor into the comparison.
//
// An overload is provided for Matrix types. It compares the descriptors of its
// matrix arguments.
template <std::size_t N>
  inline bool
  same_extents(const matrix_slice<N>& a, const matrix_slice<N>& b)
  {
    return a.order == b.order
        && std::equal(a.extents, a.extents + N, b.extents);
  }

template <typename M1, typename M2>
  inline bool
  same_extents(const M1& a, const M2& b)
  {
    return same_extents(a.descriptor(), b.descriptor());
  }


// -------------------------------------------------------------------------- //
//                              Slicing
//
// The follwoing algorithms support common slicing operations. 
//
// NOTE: These operations are primarily provided for convenience. They may
// incur the cost of an extra copy of the resulting slice, which is exactly
// 2*N+2 copies of std::size_t values. Internally, the matrix and submatrix
// classes avoid the extra copy initialization by using the slice_dim
// implementation.

// Compute an (N-1)D slice from an N-D slice. This is done by copying all
// extents and strides into the smaller-dimensional slice by excluding the
// Mth dimension. Note that:
//    If M == 0, this is called a row slice.
//    If M == 1, this is called a column slice.
//    If M == 2, this is a slice of the "z" plane.
//
// TODO: For fun, make a version that slices over a sequence of dimensions.
template <std::size_t M, std::size_t N>
  inline matrix_slice<N-1>
  slice_dimension(const matrix_slice<N>& s, std::size_t n)
  {
    matrix_slice<N-1> r;
    matrix_impl::slice_dim<M>(n, s, r);
    return r;
  }

// Compute the nth row slice of the given matrix.
template <std::size_t N>
  inline matrix_slice<N-1>
  slice_row(const matrix_slice<N>& s, std::size_t n)
  {
    return slice_dimension<0>(s, n);
  }

// Compute the nth column slie of the given matrix.
template <std::size_t N>
  inline matrix_slice<N-1>
  slice_col(const matrix_slice<N>& s, std::size_t n)
  {
    return slice_dimension<1>(s, n);
  }


