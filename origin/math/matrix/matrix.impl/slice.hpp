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
//                                   Slice
//
// A slice describes a sequence of elements in some dimension (or row) of a 
// matrix. It is a triple comprised of a starting index, a number of elements, 
// and the stride between subsequent elements.
//
// The special members slice::none and slice::all represent the selection of
// no or all elements in a particular dimension.
struct slice
{
  slice()
    : start(-1), length(-1), stride(1)
  { }

  explicit slice(std::size_t s)
    : start(s), length(-1), stride(1)
  { }

  slice(std::size_t s, std::size_t l, std::size_t n = 1)
    : start(s), length(l), stride(n)
  { }

  static slice all;

  std::size_t start;
  std::size_t length;
  std::size_t stride;
};

template <typename C, typename T>
  std::basic_ostream<C, T>&
  operator<<(std::basic_ostream<C, T>& os, const slice& s)
  {
    return os << '(' << s.start 
              << ' ' << s.length
              << ' ' << s.stride << ')';
  }



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
    //    s({dims...}) // An initializer list with N arguments
    //
    // This computation does not check that each index is within the bound
    // specified by its correspdonding extent.

    template <typename... Args>
      Requires<matrix_impl::Requesting_element<Args...>(), std::size_t>
      operator()(Args... args) const;

    template <typename... Args>
      Requires<matrix_impl::Requesting_slice<Args...>(), matrix_slice<N>>
      operator()(const Args&... args) const;


    // Return the offset
    template <typename R>
      std::size_t offset(R&& range) const;

    // Compute the slice.
    template <typename... Args>
      void get_slice(matrix_slice<N>& out, const Args&...) const;


  private:
    std::size_t init();
    
    std::size_t do_slice(matrix_slice<N>&) const;

    template <typename T, typename... Args>
      std::size_t do_slice(matrix_slice<N>&, const T&, const Args&...) const;

    template <std::size_t D>
      std::size_t do_slice_dim(matrix_slice<N>&, slice) const;

    template <std::size_t D>
      std::size_t do_slice_dim(matrix_slice<N>&, std::size_t) const;

  public:
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
      size = init();
    }

template <std::size_t N>
  matrix_slice<N>::matrix_slice(std::size_t s, 
                                std::initializer_list<std::size_t> exts)
    : start(s)
  {
    assert(exts.size() == N);
    std::copy(exts.begin(), exts.end(), extents);
    size = init();
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
  template <typename... Args>
    inline Requires<matrix_impl::Requesting_element<Args...>(), std::size_t>
    matrix_slice<N>::operator()(Args... args) const
    {
      static_assert(sizeof...(Args) == N, "");
      std::size_t indexes[N] {std::size_t(args)...};
      return offset(indexes);
    }

template <std::size_t N>
  template <typename... Args>
    inline Requires<matrix_impl::Requesting_slice<Args...>(), matrix_slice<N>>
    matrix_slice<N>::operator()(const Args&... args) const
    {
      matrix_slice<N> d;
      get_slice(d, args...);
      return d;
    }

template <std::size_t N>
  template <typename R>
    inline std::size_t
    matrix_slice<N>::offset(R&& range) const
    {
      using std::begin;
      constexpr std::size_t zero = 0;
      return start + std::inner_product(strides, strides+N, begin(range), zero);
    }

// Compute a slice for the specified arguments.
template <std::size_t N>
  template <typename... Args>
    inline void
    matrix_slice<N>::get_slice(matrix_slice<N>& out, const Args&... args) const
    {
      static_assert(matrix_impl::Requesting_slice<Args...>(), "");
      out.start = do_slice(out, args...);
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
  inline void 
  slice_dimension(std::size_t n, const matrix_slice<N>& in, matrix_slice<N-1>& out)
  {
    out.size = in.size / in.extents[M];       // Scale the size of the matrix
    out.start = in.start + n * in.strides[M]; // Compute the starting offset

    // Copy into out.extents all those from in except those in the Mth dim.
    std::copy_n(in.extents+M+1, N-M-1, std::copy_n(in.extents, M, out.extents));

    // Copy into out.strides all those from in except those in the Mth dim.
    std::copy_n(in.strides+M+1, N-M-1, std::copy_n(in.strides, M, out.strides));
  }


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
    slice_dimension<M>(n, s, r);
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



// Initialize the stride vector so that it computes offsets in row-major order.
// This is effectively the partial product of extents, computed in reverse,
// with 1 being the stride in the innermost dimension.
template <std::size_t N>
  inline std::size_t
  matrix_slice<N>::init()
  {
    strides[N - 1] = 1;
    for (std::size_t i = N - 1; i != 0; --i) {
      strides[i - 1] = strides[i] * extents[i];
    }
    return extents[0] * strides[0];
  }


// Compute the extents and strides for the specified slice in the dim dimension.
// Note that dim is an integral_constant specifying the dimension in which the
// slice is computed.
template <std::size_t N>
  template <std::size_t D>
  inline std::size_t
  matrix_slice<N>::do_slice_dim(matrix_slice<N>& out, slice s) const
  {
    // If the starting point is past the extent, we're requesting the
    // entire slice.
    if (s.start >= extents[D])
      s.start = 0;

    // If the lenght is large or the slice requests more elements than are
    // available, make it stop at the right extent.
    if (s.length > extents[D] || s.start + s.length > extents[D])
      s.length = extents[D] - s.start;

    // If the stride over-runs the edge of the matrix, re-compute the length
    // so that we stop after the right number of increments. This is:
    //
    //    l = ceil(d/s)
    //
    // where d is the distance from the start to the extent, and s is the
    // stride. 
    if (s.start + s.length * s.stride > extents[D])
      s.length = ((extents[D] - s.start) + s.stride - 1) / s.stride;

    // Compute the extents and stride in this dimension.
    out.extents[D] = s.length;
    out.strides[D] = strides[D] * s.stride;
    return s.start * strides[D];
  }

// Slicing a single column is the same as a 1-count slice at the current
// dimension. Here, dim is an integral_constant specifying the dimension
// being sliced.
template <std::size_t N>
  template <std::size_t D>
    inline std::size_t
    matrix_slice<N>::do_slice_dim(matrix_slice<N>& out, std::size_t n) const
    {
      return do_slice_dim<D>(out, slice(n, 1, 1));
    }

template <std::size_t N>
  inline std::size_t
  matrix_slice<N>::do_slice(matrix_slice<N>& out) const { return 0; }

// Translate the slice arguments is {s, args...} into the output slice, and
// return the offset of the first element of the matrix. Note that the returned
// element is effectively computed as the dot product of the starting offsets of
// each slice argument with the strides of this slice.
//
// All of the heavy lifting is done in do_slice_dim.
//
// TODO: It is possible to reduce the dimensionality of the resulting slice when
// we encounter slice::none arguments or plain index. An index requests only a
// single element, so we can effectively compute drop the corresponding
// dimension.
template <std::size_t N>
  template <typename T, typename... Args>
    inline std::size_t
    matrix_slice<N>::do_slice(matrix_slice<N>& out, const T& s, const Args&... args) const
    {
      constexpr std::size_t D = N - sizeof...(Args) - 1;
      std::size_t m = do_slice_dim<D>(out, s);
      std::size_t n = do_slice(out, args...);
      return m + n;
    }


// -------------------------------------------------------------------------- //
//                              Equality Comparison
//
// Two strides compare equal when they describe the same sequence of offsets.
// Use same_extents to determine if two slices have the same bounds or shape.

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

