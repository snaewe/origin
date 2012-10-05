// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MATH_MATRIX_HPP
#  error Do not include this file directly. Include matrix/matrix.hpp.
#endif


// A submatrix is a reference to memory in a matrix specified by a slice. A
// submatrix does not own its elements.
//
//
// Template parameters:
//    T -- The underlying value type of the matrix, possibly const.
//    N -- The order of the matrix (number of extents).
//
template <typename T, std::size_t N>
  class submatrix
  {
  public:
    static constexpr std::size_t order = N;

    using value_type          = Remove_const<T>;
    using iterator            = slice_iterator<T, N>;
    using const_iterator      = slice_iterator<const T, N>;

    // Default construction
    submatrix() = delete;

    // Move semantics
    submatrix(submatrix&&) = default;
    submatrix& operator=(submatrix&&) = default;

    // Copy semantics
    submatrix(const submatrix&) = default;
    submatrix& operator=(const submatrix& x) = default;

    // Slice initialization
    //
    // Initialize the submatrix over the slice, s, starting at the element
    // pointed to by p.
    submatrix(const matrix_slice<N>& s, T* p);

    // Destruction
    ~submatrix() = default;


    // Properties

    // Returns the shape of the matrix.

    // Return the array of extents describing the shape of the matrix.
    const matrix_slice<N>& descriptor() const { return slice; }

    // Returns the extent of the matrix in the nth dimension.
    std::size_t extent(std::size_t n) const { return slice.extents[n]; }

    // Returns the total number of elements contained in the matrix.
    std::size_t size() const { return slice.size; }


    // Element access
    template <typename... Dims>
      T& operator()(Dims... dims);

    template <typename... Dims>
      const T& operator()(Dims... dims) const;


    // Row
    //
    // Return a submatrix referring to the nth row. This is equivalent to
    // calling m.row(n).
    submatrix<T, N-1>       operator[](std::size_t n)       { return row(n); }
    submatrix<const T, N-1> operator[](std::size_t n) const { return row(n); }

    // Row
    //
    // Return a submatrix referring to the nth row.
    submatrix<T, N-1>       row(std::size_t n);
    submatrix<const T, N-1> row(std::size_t n) const;

    // Column
    //
    // Return a submatrix referring to the nth column.
    submatrix<T, N-1>       col(std::size_t n);
    submatrix<const T, N-1> col(std::size_t n) const;


    // Data access
    T*       data()       { return ptr; }
    const T* data() const { return ptr; }


    // Apply
    template <typename F>
      submatrix& apply(F f);

    template <typename M, typename F>
      submatrix& apply(const M& m, F f);


    // Scalar arithmetic
    submatrix& operator=(const value_type& x);
    submatrix& operator+=(const value_type& x);
    submatrix& operator-=(const value_type& x);
    submatrix& operator*=(const value_type& x);
    submatrix& operator/=(const value_type& x);
    submatrix& operator%=(const value_type& x);

    // Matrix arithmetic
    template <typename M>
      submatrix& operator+=(const M& m);
    
    template <typename M>
      submatrix& operator-=(const M& m);


    // Iterators
    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

  private:
    matrix_slice<N> slice; // Descirbes the submatrix
    T* ptr;                // Points to the first element of a matrix
  };


template <typename T, std::size_t N>
  inline
  submatrix<T, N>::submatrix(const matrix_slice<N>& s, T* p)
    : slice(s), ptr(p)
  { }

template <typename T, std::size_t N>
  template <typename... Dims>
    inline T&
    submatrix<T, N>::operator()(Dims... dims)
    {
      assert(matrix_impl::check_bounds(slice, dims...));
      return *(ptr + slice(dims...));
    }

template <typename T, std::size_t N>
  template <typename... Dims>
    inline const T&
    submatrix<T, N>::operator()(Dims... dims) const
    {
      assert(matrix_impl::check_bounds(slice, dims...));
      return *(ptr + slice(dims...));
    }


template <typename T, std::size_t N>
  inline submatrix<T, N-1>
  submatrix<T, N>::row(std::size_t n)
  {
    assert(n < extent(0));
    matrix_slice<N-1> row;
    matrix_impl::slice_dim<0>(n, slice, row);
    return {data(), row};
  }

template <typename T, std::size_t N>
  inline submatrix<const T, N-1>
  submatrix<T, N>::row(std::size_t n) const
  {
    assert(n < extent(0));
    matrix_slice<N-1> row;
    matrix_impl::slice_dim<0>(n, slice, row);
    return {row, ptr};
  }


template <typename T, std::size_t N>
  inline submatrix<T, N-1>
  submatrix<T, N>::col(std::size_t n)
  {
    assert(n < extent(1));
    matrix_slice<N-1> col;
    matrix_impl::slice_dim<1>(n, slice, col);
    return {data(), col};
  }

template <typename T, std::size_t N>
  inline submatrix<const T, N-1>
  submatrix<T, N>::col(std::size_t n) const
  {
    assert(n < extent(1));
    matrix_slice<N-1> col;
    matrix_impl::slice_dim<1>(n, slice, col);
    return {data(), col};
  }


template <typename T, std::size_t N>
  template <typename F>
    inline submatrix<T, N>&
    submatrix<T, N>::apply(F f)
    {
      for (auto i = begin(); i != end(); ++i)
        f(*i);
      return *this;
    }

template <typename T, std::size_t N>
  template <typename M, typename F>
    inline submatrix<T, N>&
    submatrix<T, N>::apply(const M& m, F f)
    {
      assert(same_extents(slice, m.descriptor()));
      auto i = begin();
      auto j = m.begin();
      while (i != end()) {
        f(*i, *j);
        ++i;
        ++j;
      }
      return *this;
    }


// Scalar assignment
template <typename T, std::size_t N>
  inline submatrix<T, N>& 
  submatrix<T, N>::operator=(const value_type& value)  
  {
    return apply([&](T& x) { x = value; });
  }
    
// Scalar addition.
template <typename T, std::size_t N>
  inline submatrix<T, N>& 
  submatrix<T, N>::operator+=(const value_type& value) 
  { 
    return apply([&](T& x) { x += value; });
  }

// Scalar subtraction
template <typename T, std::size_t N>
  inline submatrix<T, N>& 
  submatrix<T, N>::operator-=(value_type const& value) 
  { 
    return apply([&](T& x) { x -= value; });
  }

// Scalar multiplication
template <typename T, std::size_t N>
  inline submatrix<T, N>& 
  submatrix<T, N>::operator*=(value_type const& value) 
  { 
    return apply([&](T& x) { x *= value; });
  }

// Scalar division
template <typename T, std::size_t N>
  inline submatrix<T, N>& 
  submatrix<T, N>::operator/=(value_type const& value) 
  { 
    return apply([&](T& x) { x /= value; });
  }

// Scalar modulus
// This operation is only defined when T is a model of Euclidean domain.
template <typename T, std::size_t N>
  inline submatrix<T, N>& 
  submatrix<T, N>::operator%=(value_type const& value) 
  { 
    return apply([&](T& x) { x %= value; });
  }

// Matrix addition
template <typename T, std::size_t N>
  template <typename M>
    inline submatrix<T, N>&
    submatrix<T, N>::operator+=(const M& m)
    {
      using U = Value_type<M>;
      return apply(m, [&](T& t, const U& u) { t += u; });
    }

// Matrix subtraction
template <typename T, std::size_t N>
  template <typename M>
    inline submatrix<T, N>&
    submatrix<T, N>::operator-=(const M& m)
    {
      using U = Value_type<M>;
      return apply(m, [&](T& t, const U& u) { t -= u; });
    }


template <typename T, std::size_t N>
  inline auto
  submatrix<T, N>::begin() -> iterator
  {
    return {slice, ptr};
  }

template <typename T, std::size_t N>
  inline auto
  submatrix<T, N>::end() -> iterator
  {
    return {slice, ptr, true};
  }



  // ------------------------------------------------------------------------ //
  //                        Zero-Dimension Submatrix
  //
  // The type submatrix<T, 0> is not really a matrix. It contains a pointer
  // to an element in a submatrix.

  template <typename T>
    class submatrix<T, 0>
    {
    public:
      submatrix(const matrix_slice<0>& s, T* p) : ptr(p + s.start) { }

      // Modifying operators
      submatrix& operator=(const T& x) { *ptr = x; return *this; }

      T& operator()()       { return *ptr; }
      T& operator()() const { return *ptr; }

      operator T&() { return *ptr; }

    private:
      T* ptr;
    };
