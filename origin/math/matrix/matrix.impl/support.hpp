// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MATH_MATRIX_HPP
#  error Do not include this file directly. Include matrix/matrix.hpp.
#endif

// NOTE: Facilities in this header could be moved into more general libraries.

namespace matrix_impl
{

  // ------------------------------------------------------------------------ //
  //                          Insert Flattened
  //
  // Insert the elements of a initializer list nesting into a vector such that
  // each subsequent set of "leaf" values are copied into a contiguous memory.

  // TODO: This algorithm could be generalized to flatten an arbitrary
  // initializer list structure.

  // For iterators over the leaf nodes, insert elements into the back of the
  // vector. We generally assume that the vector has sufficient capacity for
  // all such insertions, but insert guarantees that it will resize if needed.
  template <typename T, typename Vec>
    inline void 
    insert_flattened(const T* first, const T* last, Vec& vec)
    {
      vec.insert(vec.end(), first, last);
    }

  // For iterators into nested initializer lists, recursively intiailize each
  // sub-initializeer.
  template <typename T, typename Vec>
    inline void 
    insert_flattened(const std::initializer_list<T>* first,
                    const std::initializer_list<T>* last,
                    Vec& vec)
    {
      while (first != last) {
        insert_flattened(first->begin(), first->end(), vec);
        ++first;
      }
    }

  // Copy the elements from the initializer list nesting into contiguous
  // elements in the vector.
  template <typename T, typename Vec>
    inline void 
    insert_flattened(const std::initializer_list<T>& list, Vec& vec)
    {
      insert_flattened(list.begin(), list.end(), vec);
    }


  // ------------------------------------------------------------------------ //
  //                              Algorithms

  // Compute the a variant of partial product of the in array, storing the
  // results in the out array. This implementation stores the multiplicative
  // identinity in the first element
  //
  // The resulting vector is used to compute indexes in column-major order.
  template <typename Array>
    inline std::size_t
    forward_partial_product(const Array& in, Array& out)
    {
      using T = Value_type<Array>;
      std::multiplies<T> mul;
      auto f = in.begin();
      auto l = --in.end();
      auto o = out.begin();
      *o++ = T(1);
      std::partial_sum(f, l, o, mul);
      return *(--o) * *l;
    }

  // Compute the partial product, in reverse order, of the in array, storing
  // the results in the out array.
  template <typename Array>
    inline std::size_t
    reverse_partial_product(const Array& in, Array& out)
    {
      using T = Value_type<Array>;
      std::multiplies<T> mul;
      auto f = in.rbegin();
      auto l = --in.rend();
      auto o = out.rbegin();
      *o++ = T(1);
      o = std::partial_sum(f, l, o, mul);
      return *(--o) * *l;
    }


  // ------------------------------------------------------------------------ //
  //                          Derive Extents


  // Used in derive_extents, returns true if the array is not jagged. That is,
  // all sub-initializers of list must have the same size.
  template <typename List>
    inline bool
    check_non_jagged(const List& list)
    {
      auto i = list.begin();
      for (auto j = i + 1; j != list.end(); ++j) {
        if (i->size() != j->size())
          return false;
      }
      return true;
    }


  // Derive extents implementation, matches when N == 1.
  template <std::size_t N, typename I, typename List>
    inline Requires<(N == 1), void>
    derive_extents(I& first, I last, const List& list)
    {
      assert(first != last);
      *first++ = list.size();
    }

  // Derive extents implementation, matches when N > 1.
  template <std::size_t N, typename I, typename List>
    inline Requires<(N > 1), void>
    derive_extents(I& first, I last, const List& list)
    {
      assert(check_non_jagged(list));
      assert(first != last);
      *first++ = list.size();
      derive_extents<N - 1>(first, last, *list.begin());
    }


  template <std::size_t N, typename List>
    inline std::array<std::size_t, N>
    derive_extents(const List& list)
    {
      std::array<std::size_t, N> a;
      auto f = a.begin();
      auto l = a.end();
      derive_extents<N>(f, l, list);
      assert(f == l);

      return a;
    }


  // Compute an array of strides for for the given extents such that indexing
  // will compute a row-major ordering. The total number of elements in the
  // slice is returned.
  template <std::size_t N>
    inline std::size_t
    init_row_major(const std::size_t (&extents)[N], std::size_t (&strides)[N])
    {
      strides[N - 1] = 1;
      for (std::size_t i = N - 1; i != 0; --i) {
        strides[i - 1] = strides[i] * extents[i];
      }
      return extents[0] * strides[0];
    }



  // Returns true if each element in range is within the bounds of the
  // corresponding extent.
  template <std::size_t N, typename... Dims>
    inline bool
    check_bounds(const matrix_slice<N>& slice, Dims... dims)
    {
      std::size_t indexes[N] {std::size_t(dims)...};
      std::less<std::size_t> lt;
      return std::equal(indexes, indexes + N, slice.extents, lt);
      return check_bounds(slice, indexes);
    }


// -------------------------------------------------------------------------- //
//                              Slicing
//
// The follwoing algorithms support common slicing operations.

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
  slice_dim(std::size_t n, const matrix_slice<N>& in, matrix_slice<N-1>& out)
  {
    // static_assert(M < N, "");

    out.size = in.size / in.extents[M];       // Scale the size of the matrix
    out.start = in.start + n * in.strides[M]; // Compute the starting offset

    // Copy into out.extents all those from in except those in the Mth dim.
    auto e = std::copy_n(in.extents, M, out.extents);
    std::copy_n(in.extents + M + 1, N - M - 1, e);

    // Copy into out.strides all those from in except those in the Mth dim.
    auto s = std::copy_n(in.strides, M, out.strides);
    std::copy_n(in.strides + M + 1, N - M - 1, s);
  }



  // Mores slicing.

  template <std::size_t D, std::size_t N>
    inline std::size_t
    do_slice_dim(const matrix_slice<N>& os, matrix_slice<N>& ns, slice s)
    {
      constexpr std::size_t I = N - D;
      
      // If the starting point is past the extent, we're requesting the
      // entire slice.
      if (s.start >= os.extents[I])
        s.start = 0;

      // If the lenght is large or the slice requests more elements than are
      // available, make it stop at the right extent.
      if (s.length > os.extents[I] || s.start + s.length > os.extents[I])
        s.length = os.extents[I] - s.start;

      // If the stride over-runs the edge of the matrix, re-compute the length
      // so that we stop after the right number of increments. This is:
      //
      //    l = ceil(d/s)
      //
      // where d is the distance from the start to the extent, and s is the
      // stride. 
      if (s.start + s.length * s.stride > os.extents[I])
        s.length = ((os.extents[I] - s.start) + s.stride - 1) / s.stride;

      // Compute the extents and stride in this dimension.
      ns.extents[I] = s.length;
      ns.strides[I] = os.strides[I] * s.stride;
      return s.start * os.strides[I];
    }

  // Slicing a single column is the same as a 1-count slice at the current
  // dimension.
  template <std::size_t D, std::size_t N>
    inline std::size_t
    do_slice_dim(const matrix_slice<N>& os, matrix_slice<N>& ns, std::size_t n) 
    {
      return do_slice_dim<D>(os, ns, slice(n, 1, 1));
    }


  template <std::size_t N>
    inline std::size_t
    do_slice(const matrix_slice<N>& os, matrix_slice<N>& ns) 
    {
      return 0;
    }

  // Translate the slice arguments is {s, args...} into a new slice descriptor
  // (ns), and return the offset of the first element of the matrix. Note that
  // the returned element is effectively computed as the dot product of the
  // starting offsets of each slice argument with the strides of the original
  // slice (os).
  //
  // All of the heavy lifting is done in do_slice_dim.
  //
  // Extensions: It is possible to reduce the dimensionality of the resulting
  // slice when we encounter slice::none arguments or plain index. An index
  // requests only a single element, so we can effectively compute drop the
  // corresponding dimension.
  template <std::size_t N, typename T, typename... Args>
    inline std::size_t
    do_slice(const matrix_slice<N>& os, matrix_slice<N>& ns, const T& s, const Args&... args)
    {
      std::size_t m = do_slice_dim<sizeof...(Args) + 1>(os, ns, s);
      std::size_t n = do_slice(os, ns, args...);
      return m + n;
    }


} // namespace matrix_impl

