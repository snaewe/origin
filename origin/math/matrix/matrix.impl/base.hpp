// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MATH_MATRIX_HPP
#  error Do not include this file directly. Include matrix/matrix.hpp.
#endif

namespace matrix_impl
{
  // Convenience aliases
  template <std::size_t N>
    using Size_array = std::array<std::size_t, N>;

  // Declarations
  template <typename Array>
    void forward_partial_product(const Array& in, const Array& out);

  template <typename Array>
    void reverse_partial_product(const Array& in, const Array& out);

  template <std::size_t N, typename List>
    void derive_extents(Size_array<N>& extents, const List& list);


  // ------------------------------------------------------------------------ //
  //                             Matrix Base
  //
  // Provides support for features common to both matrices and matrix
  // references (with 0 < N). 

  // Note that matrix initialization is always in row-major order. This means
  // that sizes[0] will contain the computed size of the matrix immediately
  // after value or dimensional initialization.
  //
  // Implementation Notes:
  // The sizes array is cached by the matrix base to provide efficient
  // computation of element indexes in row-major and column-major order.
  template <typename T, std::size_t N>
    struct matrix_base
    {
      static_assert(0 < N, "");

      static std::size_t constexpr order = N;

      // Default construction
      matrix_base() = default;

      // Move semantics
      matrix_base(matrix_base&&) = default;
      matrix_base& operator=(matrix_base&&) = default;

      // Copy semantics
      matrix_base(const matrix_base&) = default;
      matrix_base& operator=(const matrix_base&) = default;

      // Destruction
      ~matrix_base() = default;

      // Dimension initialization
      template <typename... Dims>
        matrix_base(Dims... dims);

      // Value initialization
      matrix_base(Matrix_initializer<T, N> list);

      // Index computation
      template <typename... Dims>
        std::size_t index(Dims... dims) const;

      // Return a matrix base describing the inner row.
      matrix_base<T, N - 1> row() const;

      template <typename Act>
        void apply_scalar(T* self, const T& value, Act act);

      template <typename Act>
        void apply_matrix(T* self, const T* data, Act act);

      // Base swap
      void swap(matrix_base& base);

      Size_array<N> extents; // The dimensions of the matrix
      Size_array<N> sizes;   // The partial product of dimensions
      std::size_t size;      // The total size of the matrix
    };


  template <typename T, std::size_t N>
    template <typename... Dims>
      inline
      matrix_base<T, N>::matrix_base(Dims... dims)
        // : extents{dims...}
      {
        static_assert(N == sizeof...(Dims), "");

        // TODO: GCC is complaing about the member-initializer above. That
        // should be completely legal, but somehow GCC is getting confused.
        // There are no narrowing problems, either. The error is:
        //
        //    array must be initialized with a brace-enclosed initializer
        //
        // Clearly, extents is being bing initialized correctly. As a work
        // around, we have this:
        Size_array<N> tmp { dims... };
        copy(tmp, extents);

        // Compute the row major ordering vector.
        size = reverse_partial_product(extents, sizes);
      }

  template <typename T, std::size_t N>
    inline
    matrix_base<T, N>::matrix_base(Matrix_initializer<T, N> list)
    {
      // Derive the extents from nested list.
      derive_extents(extents, list);

      // Compute the row major ordering vector.
      size = reverse_partial_product(extents, sizes);
    }

  template <typename T, std::size_t N>
    template <typename... Dims>
      inline std::size_t
      matrix_base<T, N>::index(Dims... dims) const
      {
        static_assert(sizeof...(Dims) == N, "");

        // Copy arguments into a vector.
        std::size_t args[N] { std::size_t(dims)... };

        // The index is the inner product of given indexes and sizes.
        return std::inner_product(args, args + N, sizes.begin(), std::size_t(0));
      }

  template <typename T, std::size_t N>
    inline matrix_base<T, N - 1>
    matrix_base<T, N>::row() const
    {
      static_assert(N != 0, "");

      // We can construct the the lower-dimension base by simply chopping off
      // the first element of the array.
      matrix_base<T, N - 1> res;
      std::copy_n(extents.begin() + 1, N, res.extents.begin());
      std::copy_n(sizes.begin() + 1, N, res.sizes.begin());
      res.size = sizes[0];
      return res;
    }


  template <typename T, std::size_t N>
    template <typename Act>
    inline void
    matrix_base<T, N>::apply_scalar(T* self, const T& value, Act act)
    {
      apply(self, self + size, value, act);
    }

  template <typename T, std::size_t N>
    template <typename Act>
    inline void
    matrix_base<T, N>::apply_matrix(T* self, const T* data, Act act)
    {
      apply_each(self, self + size, data, act);
    }

  template <typename T, std::size_t N>
    inline void
    matrix_base<T, N>::swap(matrix_base& x)
    {
      using std::swap;
      std::swap(extents, x.extents);
      std::swap(sizes, x.sizes);
      std::swap(size, x.size);
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

  // Returns the matrix shape corresponding to a sequence of nested initialize
  // lists. Here, List is sequence of nested initializer lists. Note that the
  // nested list must be non-jagged; all initializer lists at the same depth
  // must have the same length.
  template <std::size_t N, typename List>
    inline void
    derive_extents(Size_array<N>& extents, const List& list)
    {
      auto first = extents.begin();
      auto last = extents.end();
      derive_extents<N>(first, last, list);
      assert(first == last);
    }


  // Returns a reference to the nth row in a matrix.
  //
  // The function takes a base object (whose order must be 1 greater than N),
  // a pointer p to the start of a matrix or matrix reference descirbed by
  // base, and n, the row to return.

  // When N > 0, we are returning a matrix reference of N dimensions.
  template <typename T, typename Base>
    inline Requires<(Base::order > 1), matrix_ref<T, Base::order - 1>>
    row(const Base& b, T* p, std::size_t n)
    {
      return {b.row(), p + n * b.extents[1]};
    }

  // When N == 0, we are returning an element reference, possibly const
  // qualified.
  template <typename T, typename Base>
    inline Requires<(Base::order == 1), matrix_ref<T, 0>>
    row(const Base& b, T* p, std::size_t n)
    {
      return *(p + n);
    }

} // namespace matrix_impl
