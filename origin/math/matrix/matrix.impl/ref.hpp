// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MATH_MATRIX_HPP
#  error Do not include this file directly. Include matrix/matrix.hpp.
#endif


// A matrix ref is a reference to a contiguous sub-matrix with a specified
// shape. The matrix ref does not own its elements.
//
// This is closely related to the notion of a slice or a submatrix except
// that all elements are contiguously allocated and define a complete matrix.
// Indexing into a matrix row does not require strides or spans to move from
// one element to the next.
//
// The matrix row class is a Matrix except that assignment writes through
// (not to) the matrix reference to the referenced data.
//
// Template parameters:
//    T -- The underlying value type of the matrix, possibly const. If T is
//         const, then the matrix reference becomes read only. All references
//         and pointers returned from a const matrix reference are also
//         const.
//    N -- The number of dimensions of this sub-matrix.
// 
// TODO: When matrix<T, N> gets an allocator, this class will need to get
// an allocator also.
template <typename T, std::size_t N>
  class basic_matrix_ref
  {
    static_assert(0 < N, "");

  public:
    static constexpr std::size_t order = N;

    using value_type          = Remove_const<T>;
    using extents_type        = std::array<std::size_t, N>;
    using iterator            = T*;
    using const_iterator      = const T*;
  private:
    using base_type = matrix_base<value_type, N>;
  public:

    // Default construction
    basic_matrix_ref() = delete;

    // Move semantics
    basic_matrix_ref(basic_matrix_ref&&) = default;
    basic_matrix_ref& operator=(basic_matrix_ref&&) = default;

    // Copy semantics
    basic_matrix_ref(const basic_matrix_ref&) = default;
    basic_matrix_ref& operator=(const basic_matrix_ref& x) = default;

    // Destruction
    ~basic_matrix_ref() = default;


    // Matrix binding
    //
    // A matrix reference can be "bound" to a matrix lvalue of the same type
    // and order. However, a matrix reference cannot be "bound" to a matrix
    // temporary. That could lead to leaks and/or dereferencing deleted
    // memory. 
    //
    // The Remove_const ensures that we don't try to instantiate a matrix
    // with a const value type.
    //
    // NOTE: If T is const, then binding to a non-const lvalue matrix is
    // not permitted.
    //
    // TODO: Allow references to an arbitrary slice of a matrix.
    //
    // TODO: Allow references to other matrix references.
    basic_matrix_ref(basic_matrix<value_type, N>& x);
    basic_matrix_ref& operator=(basic_matrix<value_type, N>& x);

    basic_matrix_ref(const basic_matrix<value_type, N>& x);
    basic_matrix_ref& operator=(const basic_matrix<value_type, N>& x);

    basic_matrix_ref(basic_matrix<value_type, N>&&) = delete;
    basic_matrix_ref& operator=(basic_matrix<value_type, N>&&) = delete;

    // This is not part of the public interface.
    basic_matrix_ref(const base_type& b, T* p);

    // Properties

    // Returns the shape of the matrix.

    // Return the array of extents describing the shape of the matrix.
    const extents_type& extents() const { return base.extents; }

    // Returns the extent of the matrix in the nth dimension.
    std::size_t extent(std::size_t n) const { return base.extents[n]; }

    // Returns the total number of elements contained in the matrix.
    std::size_t size() const { return base.size; }


    // Element access
    template <typename... Dims>
      T& operator()(Dims... dims);
    template <typename... Dims>
      const T& operator()(Dims... dims) const;


    // Row access
    matrix_ref<T, N - 1>       operator[](std::size_t n);
    matrix_ref<const T, N - 1> operator[](std::size_t n) const;

    // Data access
    T*       data()       { return ptr; }
    const T* data() const { return ptr; }

    // Scalar operations
    basic_matrix_ref& operator=(const value_type& value);
    basic_matrix_ref& operator+=(const value_type& value);
    basic_matrix_ref& operator-=(const value_type& value);
    basic_matrix_ref& operator*=(const value_type& value);
    basic_matrix_ref& operator/=(const value_type& value);
    basic_matrix_ref& operator%=(const value_type& value);

    // Matrix addition
    template <typename M>
      Requires<Matrix<M>(), basic_matrix_ref&> operator+=(const M& x);
    template <typename M>
      Requires<Matrix<M>(), basic_matrix_ref&> operator-=(const M& x);


    // Iterators
    iterator begin() { return ptr; }
    iterator end()   { return ptr + size(); }

    const_iterator begin() const { return ptr; }
    const_iterator end() const   { return ptr + size(); }

  private:

    base_type base; // Manage extents and indexing
    T* ptr;         // Points to the front of the matrix.
  };


// Directly initialize the matrix over a base and a pointer.
// This is used internally to construct rows.
template <typename T, std::size_t N>
  basic_matrix_ref<T, N>::basic_matrix_ref(const base_type& b, T* p)
    : base(b), ptr(p)
  { }

// Reference initialization to non-const matrices
template <typename T, std::size_t N>
  basic_matrix_ref<T, N>::basic_matrix_ref(basic_matrix<value_type, N>& x) 
    : base(x.base), ptr(x.data())
  { }

// Reference assignment to non-const matrices
template <typename T, std::size_t N>
  inline basic_matrix_ref<T, N>&
  basic_matrix_ref<T, N>::operator=(basic_matrix<value_type, N>& x) 
  {
    base = x.base;
    ptr = x.data();
    return *this;
  }

// Reference initializatino to const matrices
template <typename T, std::size_t N>
  basic_matrix_ref<T, N>::basic_matrix_ref(const basic_matrix<value_type, N>& x) 
    : base(x.base), ptr(x.data())
  { }

// Reference assignment to cont matrices
template <typename T, std::size_t N>
  inline basic_matrix_ref<T, N>&
  basic_matrix_ref<T, N>::operator=(const basic_matrix<value_type, N>& x) 
  {
    base_type::operator=(x);
    ptr = x.data();
    return *this;
  }

// FIXME: Write type requirements.
template <typename T, std::size_t N>
  template <typename... Dims>
    inline T&
    basic_matrix_ref<T, N>::operator()(Dims... dims)
    {
      return *(ptr + base(dims...));
    }

template <typename T, std::size_t N>
  template <typename... Dims>
    inline const T&
    basic_matrix_ref<T, N>::operator()(Dims... dims) const
    {
      return *(ptr + base(dims...));
    }

// Row indexing
template <typename T, std::size_t N>
  inline matrix_ref<T, N - 1>
  basic_matrix_ref<T, N>::operator[](std::size_t n)
  {
    return matrix_impl::row(base, ptr, n);
  }

template <typename T, std::size_t N>
  inline matrix_ref<const T, N - 1>
  basic_matrix_ref<T, N>::operator[](std::size_t n) const
  {
    return matrix_impl::row(base, ptr, n);
  }

// Scalar assignment
template <typename T, std::size_t N>
  inline basic_matrix_ref<T, N>& 
  basic_matrix_ref<T, N>::operator=(const value_type& value)  
  {
    base.apply_scalar(ptr, value, matrix_impl::assign<value_type>{});
    return *this;
  }
    
// Scalar addition.
template <typename T, std::size_t N>
  inline basic_matrix_ref<T, N>& 
  basic_matrix_ref<T, N>::operator+=(const value_type& value) 
  { 
    base.apply_scalar(ptr, value, matrix_impl::plus_assign<value_type>{});
    return *this;
  }

// Scalar subtraction
template <typename T, std::size_t N>
  inline basic_matrix_ref<T, N>& 
  basic_matrix_ref<T, N>::operator-=(value_type const& value) 
  { 
    base.apply_scalar(ptr, value, matrix_impl::minus_assign<value_type>{});
    return *this;
  }

// Scalar multiplication
template <typename T, std::size_t N>
  inline basic_matrix_ref<T, N>& 
  basic_matrix_ref<T, N>::operator*=(value_type const& value) 
  { 
    base.apply_scalar(ptr, value, matrix_impl::multiplies_assign<value_type>{});
    return *this;
  }

// Scalar division
template <typename T, std::size_t N>
  inline basic_matrix_ref<T, N>& 
  basic_matrix_ref<T, N>::operator/=(value_type const& value) 
  { 
    base.apply_scalar(ptr, value, matrix_impl::divides_assign<value_type>{});
    return *this;
  }

// Scalar modulus
// This operation is only defined when T is a model of Euclidean domain.
template <typename T, std::size_t N>
  inline basic_matrix_ref<T, N>& 
  basic_matrix_ref<T, N>::operator%=(value_type const& value) 
  { 
    base.apply_scalar(ptr, value, matrix_impl::assign<value_type>{});
    return *this;
  }

// Matrix addition
template <typename T, std::size_t N>
  template <typename M>
    inline Requires<Matrix<M>(), basic_matrix_ref<T, N>&>
    basic_matrix_ref<T, N>::operator+=(const M& x)
    {
      static_assert(order == x.order, "");
      assert(extents() == x.extents());
      this->apply_matrix(ptr, x.data(), matrix_impl::plus_assign<value_type>{});
      return *this;
    }

// Matrix subtraction
template <typename T, std::size_t N>
  template <typename M>
    inline Requires<Matrix<M>(), basic_matrix_ref<T, N>&>
    basic_matrix_ref<T, N>::operator-=(const M& x)
    {
      static_assert(order == x.order, "");
      assert(extents() == x.extents());
      this->apply_matrix(ptr, x.data(), matrix_impl::minus_assign<value_type>{});
      return *this;
    }
