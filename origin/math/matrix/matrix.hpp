// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MATH_MATRIX_HPP
#define ORIGIN_MATH_MATRIX_HPP

#include <cassert>

#include <algorithm>
#include <array>
#include <iostream>

#include <origin/type/concepts.hpp>
#include <origin/type/typestr.hpp>
#include <origin/sequence/algorithm.hpp>

namespace origin
{
  // Declarations
  template <typename T, std::size_t N> class basic_matrix;
  template <typename T, std::size_t N> class basic_matrix_ref;
  template <typename T, std::size_t N> class basic_matrix_slice;


#include "matrix.impl/traits.hpp" // Type traits


  // Returns true if T is a matrix.
  //
  // FIXME: A matrix is substantially more complex than this. Finish defining
  // and implementing the concept. There is an interesting design question
  // here. What are the minimum requirements of a Matrix? Surely we can't 
  // require all arithmetic operations.
  template <typename M>
    constexpr bool Matrix()
    {
      return matrix_impl::Has_order<M>();
    }


  // The matrix initializer is a sequence of nested initializer lists that
  // describes the initailization structure of an N dimensional matrix. For
  // example, an initializer for a 2D matrix of ints allows us to construct 
  // the following initializer list:
  //
  //    {{0, 1},
  //     {2, 3},
  //     {4, 5}}
  //
  // Assuming we have a 3x2 matrix.
  template <typename T, std::size_t N>
    using Matrix_initializer = typename matrix_impl::matrix_init<T, N>::type;


  // ------------------------------------------------------------------------ //
  // Matrix
  //
  // A matrix is an N-dimensional arrangement of elements of type T where 
  // 0 <= N.
  //
  // Note that when N == 0, the matrix type is the same as the element or
  // scalar type T. Zero-dimensional matrices do not have the usual properties
  // of a Matrix.
  template <typename T, std::size_t N>
    using matrix = If<(N == 0), T, basic_matrix<T, N>>;


  // ------------------------------------------------------------------------ //
  // Matrix Reference
  //
  // A matrix reference is a reference to an N-dimensional arrangement of
  // elements of type T where 0 <= N. The type T may be const qualified. A
  // matrix reference does not own its elements.
  //
  // Note that when N == 0, the matrix reference is a reference to the element
  // or sacalar type T. Zero-dimensional matrices do not have the usual
  // properties of a Matrix.
  template <typename T, std::size_t N>
    using matrix_ref = If<(N == 0), T&, basic_matrix_ref<T, N>>;


#include "matrix.impl/support.hpp" // Miscellaneous facilities
#include "matrix.impl/base.hpp"    // Includes matrix_base


  // ------------------------------------------------------------------------ //
  // Basic Matrix
  //
  // The basic matrix template specifies an implementaiton of an N-dimensional
  // matrix parameterized over some value type T where 0 < N. The class provides
  // the common arithmetic operators, profided they are supported by the
  // value type.
  //
  // TODO: Don't use vector<T> as the underlying store; just use a dynamically
  // allocated array. Certain operations will become faster (with fewer
  // assignments) and the class will take less space (2 pointers less). This
  // would also allow us to write an interesting version of reserve().
  template <typename T, std::size_t N>
    class basic_matrix
    {
    public:
      static constexpr std::size_t order = N;

      using value_type     = T;
      using extents_type   = std::array<std::size_t, N>;
      using iterator       = typename std::vector<T>::iterator;
      using const_iterator = typename std::vector<T>::const_iterator;


      // Default construction
      basic_matrix() = default;

      // Move semantics
      basic_matrix(basic_matrix&&) = default;
      basic_matrix& operator=(basic_matrix&&) = default;

      // Copy semantics
      basic_matrix(const basic_matrix&) = default;
      basic_matrix& operator=(const basic_matrix&) = default;

      // Destruction
      ~basic_matrix() = default;


      // Matrix reference iniitalization
      //
      // Initialize the matrix from the matrix reference by copying its
      // elements. Note that we can't move initialize from a matrix reference
      // since the reference does not own its elements.
      //
      // This conversion is enabled whenver the element type of U can be
      // converted to the value type T. This accommodates both conversion
      // and const differences.
      template <typename U>
        basic_matrix(const basic_matrix_ref<U, N>& x);
      
      template <typename U>
        basic_matrix& operator=(const basic_matrix_ref<U, N>& x);


      // Extent initialization
      //
      // Initialize the matrix with the dimensions given as a sequence of
      // arguments. All elements are default initialized.
      //
      // NOTE: There is no corresponding assignment operator for this
      // constructor. The resize() operation to change the dimensions of
      // the matrix.
      // 
      // TODO: Create overloads that allow the specification of a default
      // value as the last argument and one that allows the specification of
      // a default value and an allocator as the last pairs of arguments.
      template <typename... Dims>
        explicit
        basic_matrix(Dims... dims);


      // Direct initialization
      //
      // Initialize the matrix over a nesting of initializer lists. The number
      // of nestings is determined by the order of the matrix, N.
      basic_matrix(Matrix_initializer<T, N> init);
      basic_matrix& operator=(Matrix_initializer<T, N> init);

      // The "flat" initializer list is disabled, preventing a user from
      // writing m {a, b, c, ...} for an arbitrarily nested type. This may be
      // confused with extent initialization.
      template <typename U>
        basic_matrix(std::initializer_list<U> list) = delete;


      // Properties

      // Return the array of extents describing the shape of the matrix.
      const extents_type& extents() const { return base.extents; }

      // Returns the extent of the matrix in the nth dimension.
      std::size_t extent(std::size_t n) const { return base.extents[n]; }

      // Returns the total number of elements contained in the matrix.
      std::size_t size() const { return elems.size(); }


      // Element access
      //
      // Returns a reference to the element at the index given by the sequence
      // of indexes, Exts.
      template <typename... Exts>
        T& operator()(Exts... exts);
      template <typename... Exts>
        const T& operator()(Exts... exts) const;


      // Row access
      //
      // Returns a reference to the row at the nth index. When N == 1, the
      // resulting matrix_ref is simply a reference to the value type.
      matrix_ref<T, N - 1>       operator[](std::size_t n);
      matrix_ref<const T, N - 1> operator[](std::size_t n) const;


      // Data access
      //
      // Returns a pointer to the underlying data.
      T*       data()       { return elems.data(); }
      const T* data() const { return elems.data(); }


      // Scalar operations
      basic_matrix& operator=(const value_type& value);
      basic_matrix& operator+=(const value_type& value);
      basic_matrix& operator-=(const value_type& value);
      basic_matrix& operator*=(const value_type& value);
      basic_matrix& operator/=(const value_type& value);
      basic_matrix& operator%=(const value_type& value);

      // Matrix operations
      template <typename M>
        Requires<Matrix<M>(), basic_matrix&> operator+=(const M& x);
      template <typename M>
        Requires<Matrix<M>(), basic_matrix&> operator-=(const M& x);


      // Iterators
      //
      // The begin() and end() functions return iterators over the underlying
      // data in the matrix. It is not structured.
      //
      // TODO: Write iterators over rows and columns.
      iterator begin() { return elems.begin(); }
      iterator end()   { return elems.end(); }

      const_iterator begin() const { return elems.begin(); }
      const_iterator end() const   { return elems.end(); }


      // Mutators
      
      void swap(basic_matrix& x);
      void clear();

    private:
      using base_type = matrix_base<T, N>;

      base_type base;
      std::vector<T> elems;

      template <typename U, std::size_t M> friend class basic_matrix_ref;
    };


  template <typename T, std::size_t N>
    template <typename U>
    inline
    basic_matrix<T, N>::basic_matrix(const basic_matrix_ref<U, N>& x)
      : base(x.base), elems(x.begin(), x.end())
    {
      static_assert(Convertible<U, T>(), "");
    }

  template <typename T, std::size_t N>
    template <typename U>
    inline basic_matrix<T, N>&
    basic_matrix<T, N>::operator=(const basic_matrix_ref<U, N>& x)
    {
      base = x.base;
      elems.assign(x.begin(), x.end());
      return*this;
    }

  template <typename T, std::size_t N>
    template <typename... Dims>
      inline
      basic_matrix<T, N>::basic_matrix(Dims... dims)
        : base(std::size_t(dims)...), elems(base.size)
      { }

  template <typename T, std::size_t N>
    inline
    basic_matrix<T, N>::basic_matrix(Matrix_initializer<T, N> init)
      : base(init)
    {
      elems.reserve(base.size);
      matrix_impl::insert_flattened(init, elems);
      assert(elems.size() == base.size);
    }

  template <typename T, std::size_t N>
    inline basic_matrix<T, N>&
    basic_matrix<T, N>::operator=(Matrix_initializer<T, N> init)
    {
      basic_matrix tmp(init);
      swap(tmp);
      return *this;
    }

  template <typename T, std::size_t N>
    template <typename... Exts>
      inline T&
      basic_matrix<T, N>::operator()(Exts... exts)
      {
        return *(data() + base(exts...));
      }

  template <typename T, std::size_t N>
    template <typename... Exts>
      inline const T&
      basic_matrix<T, N>::operator()(Exts... exts) const
      {
        return *(data() + base(exts...));
      }

  template <typename T, std::size_t N>
    inline matrix_ref<T, N - 1>
    basic_matrix<T, N>::operator[](std::size_t n)
    {
      return matrix_impl::row(base, data(), n);
    }

  template <typename T, std::size_t N>
    inline matrix_ref<const T, N - 1>
    basic_matrix<T, N>::operator[](std::size_t n) const
    {
      return matrix_impl::row(base, data(), n);
    }

  // Scalar assignment
  template <typename T, std::size_t N>
    inline basic_matrix<T, N>& 
    basic_matrix<T, N>::operator=(const value_type& value)  
    { 
      base.apply_scalar(data(), value, matrix_impl::assign<T>{});
      return *this;
    }

  // Scalar addition
  template <typename T, std::size_t N>
    inline basic_matrix<T, N>& 
    basic_matrix<T, N>::operator+=(const value_type& value) 
    { 
      base.apply_scalar(data(), value, matrix_impl::plus_assign<T>{});
      return *this;
    }

  // Scalar subtraction      
  template <typename T, std::size_t N>
    inline basic_matrix<T, N>& 
    basic_matrix<T, N>::operator-=(value_type const& value) 
    {
      base.apply_scalar(data(), value, matrix_impl::minus_assign<T>{});
      return *this;
    }

  // Scalar multiplication
  template <typename T, std::size_t N>
    inline basic_matrix<T, N>& 
    basic_matrix<T, N>::operator*=(value_type const& value) 
    { 
      base.apply_scalar(data(), value, matrix_impl::multiplies_assign<T>{});
      return *this;
    }

  // Scalar division
  template <typename T, std::size_t N>
    inline basic_matrix<T, N>& 
    basic_matrix<T, N>::operator/=(value_type const& value) 
    { 
      base.apply_scalar(data(), value, matrix_impl::divides_assign<T>{});
      return *this;
    }
  
  // Scalar remainder    
  template <typename T, std::size_t N>
    inline basic_matrix<T, N>& 
    basic_matrix<T, N>::operator%=(value_type const& value) 
    { 
      base.apply_scalar(data(), value, matrix_impl::modulus_assign<T>{});
      return *this;
    }

  // NOTE: Matrix addition and subtraction require the arguments to have the
  // same order, dimensions, and size. It must be the case that if two matrices
  // have the same dimensions, then they have the same size.

  // Matrix addition
  template <typename T, std::size_t N>
    template <typename M>
      inline Requires<Matrix<M>(), basic_matrix<T, N>&>
      basic_matrix<T, N>::operator+=(const M& x)
      {
        static_assert(order == x.order, "");
        assert(extents() == x.extents());
        base.apply_matrix(data(), x.data(), matrix_impl::plus_assign<T>{});
        return *this;
      }

  // Matrix subtraction
  template <typename T, std::size_t N>
    template <typename M>
      inline Requires<Matrix<M>(), basic_matrix<T, N>&>
      basic_matrix<T, N>::operator-=(const M& x)
      {
        static_assert(order == x.order, "");
        assert(this->extents == x.extents);
        base.apply_matrix(data(), x.data(), matrix_impl::minus_assign<T>{});
        return *this;
      }

  template <typename T, std::size_t N>
    inline void
    basic_matrix<T, N>::swap(basic_matrix& x)
    {
      base.swap(x);
      elems.swap(x.elems);
    }

// Supporting classes
#include "matrix.impl/ref.hpp"   // Include matrix_ref
#include "matrix.impl/slice.hpp" // Include matrix_slice


  // ------------------------------------------------------------------------ //
  //                          Matrix Operations
  //
  // The following operations are defined for all Matrix types.
  //
  // TODO: Some of these algorithms should be moved into a more general
  // purpose linear algebra library.



  // Returns the number of rows in a matrix with rank > 1. The number of rows
  // is the same as the extent in the 1st dimension.
  template <typename M>
    inline auto rows(const M& m) -> decltype(m.extent(0))
    {
      static_assert(0 < M::order, "");
      return m.extent(0);
    }


  // Returns the number of columns in a matrix with rank > 1. This number of
  // columns is the same as the extent in the 2nd dimension.
  template <typename M>
    inline auto cols(const M& m) -> decltype(m.extent(1))
    {
      static_assert(1 < M::order, "");
      return m.extent(1);
    }


  // Equality comparable
  // Two matrices compare equal when they have the same shape and elements.
  template <typename M1, typename M2>
    inline Requires<Matrix<M1>() && Matrix<M2>(), bool> 
    operator==(const M1& a, const M2& b)
    { 
      assert(a.extents() == b.extents());
      return range_equal(a, b);
    }
  
  template <typename M1, typename M2>
    inline Requires<Matrix<M1>() && Matrix<M2>(), bool> 
    operator!=(const M1& a, const M2& b)
    { 
      return !(a == b);
    }
      


  // NOTE: This operation is kind of funny because it is heterogeneous in its
  // result type. If we try to concept check Matrix<R, R> (where R is a matrix
  // ref type), we would normally be asking for an operation a homogeneous
  // operator+(R,R)->R. That's not what we have.
  //
  // In order to check this concept, we have to weaken the result type. The
  // C++0x concepts required that the result be convertible to the argument
  // types. That doesn't work here because matrix is not convertible to matrix
  // ref. It's the other way around.
  //
  // The correct way to check this is to say that the result type must share
  // a common type with the domain type. That is, if U is the result type of the
  // expression t + t (with t having type T), then Common<T, U> must be true.
  //
  // NOTE: We may be able to generalize and differentiate the addition of 
  // matrices vs. matrix references in the the future. A fully general matrix
  // slice would have the same properties as matrix_ref w.r.t. to addition
  // and subtraction.

  
  // Matrix addition
  //
  // Adding two matrices with the same shape adds corresponding elements in
  // each operatand.

  template <typename T, std::size_t N>
    inline basic_matrix<T, N>
    operator+(const basic_matrix<T, N>& a, const basic_matrix<T, N>& b)
    {
      assert(a.extents() == b.extents());
      basic_matrix<T, N> result = a;
      return result += b;
    }

  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator+(const matrix_ref<T, N>& a, const matrix_ref<T, N>& b)
    {
      assert(a.extents == b.extents);
      matrix<T, N> result = a;
      return result += b;
    }

  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator+(const matrix<T, N>& a, const matrix_ref<T, N>& b)
    {
      assert(a.extents == b.extents);
      matrix<T, N> result = a;
      return result += b;
    }

  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator+(const matrix_ref<T, N>& a, const matrix<T, N>& b)
    {
      assert(a.extents == b.extents);
      matrix<T, N> result = a;
      return result += b;
    }


  //////////////////////////////////////////////////////////////////////////////
  // Matrix subtraction
  //
  // Subtracting one matrix from another with the same shape subtracts
  // corresponding elements in each operatand.
  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator-(const matrix<T, N>& a, const matrix<T, N>& b)
    {
      asssert(a.dim() == b.dim());
      matrix<T, N> result = a;
      return a -= b;
    }

  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator-(const matrix_ref<T, N>& a, const matrix_ref<T, N>& b)
    {
      assert(a.dim() == b.dim());
      matrix<T, N> result = a;
      return result -= b;
    }

  // Cross-type subtraction
  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator-(const matrix<T, N>& a, const matrix_ref<T, N>& b)
    {
      assert(a.dim() == b.dim());
      matrix<T, N> result = a;
      return result -= b;
    }

  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator-(const matrix_ref<T, N>& a, const matrix<T, N>& b)
    {
      assert(a.dim() == b.dim());
      matrix<T, N> result = a;
      return result -= b;
    }



  //////////////////////////////////////////////////////////////////////////////
  // Scalar addition
  //
  // Adding a scalar to a matrix adds that value to each element in the matrix.
  // The following operations are supported:
  //
  //    a + n
  //    n + a
  template <typename T, std::size_t N>
    inline matrix<T, N> 
    operator+(const matrix<T, N>& x, const T& n)
    {
      matrix<T, N> result = x;
      return result += n;
    }

  template <typename T, std::size_t N>
    inline matrix<T, N> 
    operator+(const T& n, const matrix<T, N>& x)
    {
      matrix<T, N> result = x;
      return result += n;
    }

  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator+(const matrix_ref<T, N>& x, const T& n)
    {
      matrix<T, N> result = x;
      return result += n;
    }

  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator+(const T& n, const matrix_ref<T, N>& x)
    {
      matrix<T, N> result = x;
      return result += n;
    }


  //////////////////////////////////////////////////////////////////////////////
  // Scalar subtraction
  //
  // A scalar can be subtracted from a matrix. It is equivalent to adding the
  // negation of the scalar. That is:
  //
  //    a - n <=> a + -n;
  //
  // It is not possible to subtract a matrix from a scalar.
  template <typename T, std::size_t N>
    inline matrix<T, N> 
    operator-(const matrix<T, N>& x, const T& n)
    {
      matrix<T, N> result = x;
      return result += n;
    }

  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator-(const matrix_ref<T, N>& x, const T& n)
    {
      matrix<T, N> result = x;
      return result -= n;
    }



  //////////////////////////////////////////////////////////////////////////////
  // Scalar multiplication
  //
  // Multiplying a matrix by a scalar multiplies each element by the scalar
  // value. The following operations are supported:
  //
  //    a * n
  //    n * a
  //
  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator*(const matrix<T, N>& x, const T& n)
    {
      matrix<T, N> result = x;
      return result *= n;
    }

  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator*(const T& n, const matrix<T, N>& x)
    {
      matrix<T, N> result = x;
      return result *= n;
    }

  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator*(const matrix_ref<T, N>& x, const T& n)
    {
      matrix<T, N> result = x;
      return result *= n;
    }

  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator*(const T& n, const matrix_ref<T, N>& x)
    {
      matrix<T, N> result = x;
      return result *= n;
    }


  //////////////////////////////////////////////////////////////////////////////
  // Scalar division
  //
  // A matrix can be divided by a scalar value. It is equivalent to multiplying
  // the matrix by the reciprocal of the scalar.
  //
  //    a / n <=> a * 1/n
  //
  // It is not possible to divide a scalar by a matrix.
  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator/(const matrix<T, N>& a, const T& n)
    {
      matrix<T, N> result = a;
      return result /= n;
    }

  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator/(const matrix_ref<T, N>& x, const T& n)
    {
      matrix<T, N> result = x;
      return result /= n;
    }



  //////////////////////////////////////////////////////////////////////////////
  // Scalar modulus
  //
  // Returns a matrix containing the remainder of each element divided by the
  // given scalar value.
  //
  // This operation is only available when T is an Integer type.
  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator%(const matrix<T, N>& a, const T& n)
    {
      matrix<T, N> result = a;
      return result %= n;
    }

  template <typename T, std::size_t N>
    inline matrix<T, N>
    operator%(const matrix_ref<T, N>& x, const T& n)
    {
      matrix<T, N> result = x;
      return result %= n;
    }


  // Declarations
  template <typename M1, typename M2, typename M3>
    void matrix_product(const M1&, const M2&, M3&);

  template <typename M1, typename M2, typename M3>
    void hadamard_product(const M1&, const M2&, M3&);



  //////////////////////////////////////////////////////////////////////////////
  // Matrix Multiplication
  //
  // Two 2D matrices a (m x p) and b (p x n) can be multiplied, resulting in a
  // matrix c (m x n). Note that the "inner" dimension of the operands must
  // be the same.
  template <typename T>
    inline matrix<T, 2>
    operator*(const matrix<T, 2>& a, const matrix<T, 2>& b) 
    {
      matrix<T, 2> result (a.dims());
      matrix_product(a, b, result);
      return result;
    }

  template <typename T>
    inline matrix<T, 2>
    operator*(const matrix_ref<T, 2>& a, const matrix_ref<T, 2>& b) 
    {
      matrix<T, 2> result (a.dims());
      matrix_product(a, b, result);
      return result;
    }

  // Cross product multiplication.
  template <typename T>
    inline matrix<T, 2>
    operator*(const matrix<T, 2>& a, const matrix_ref<T, 2>& b) 
    {
      matrix<T, 2> result (a.dims());
      matrix_product(a, b, result);
      return result;
    }

  template <typename T>
    inline matrix<T, 2>
    operator*(const matrix_ref<T, 2>& a, const matrix<T, 2>& b) 
    {
      matrix<T, 2> result (a.dims());
      matrix_product(a, b, result);
      return result;
    }



  //////////////////////////////////////////////////////////////////////////////
  // Matrix Product
  //
  // The usual meaning of the operation.
  //
  // FIXME: This is not terribly efficient. Implement a more efficient algorithm
  // than this brute force version.
  //
  // FIXME: I'm not at all sure that this generalizes to n dimensions. It might
  // be the case that we want all M's to be 2 dimensions (as they are now!).
  template <typename M1, typename M2, typename M3>
    void 
    matrix_product(const M1& a, const M2& b, M3& out)
    {
      static_assert(M1::order() == 2, "");
      static_assert(M2::order() == 2, "");
      static_assert(M3::order() == 2, "");
      assert(cols(a) == rows(b));
      assert(rows(a) == rows(out));
      assert(cols(b) == cols(out));

      using Size = Size_type<M3>;

      for (Size i = 0; i != rows(a); ++i) {
        for (Size j = 0; j < cols(b); ++j) {
          for (Size k = 0; k < rows(b); ++k)
            out(i, j) += a(i, k) * b(k, j);
        }
      }
    }

  
  //////////////////////////////////////////////////////////////////////////////
  // Hadamard Product
  //
  // The hadamard product can be easly generalized to N-dimensional matrices 
  // since the operation is performed elementwise. The operands only need the
  // same shape.
  template <typename M1, typename M2, typename M3>
    void
    hadamard_product(const M1& a, const M2& b, M3& out)
    {
      assert(a.shape() == b.shape());
      assert(a.shape() == out.shape());

      using Mul = std::multiplies<Value_type<M1>>;
      std::transform(a.begin(), a.end(), b.begin(), out.begin(), Mul{});
    }



  //////////////////////////////////////////////////////////////////////////////
  // Streaming
  //
  // Write the matrix to the the given output stream.
  template <typename C, typename T, typename M>
    inline Requires<Matrix<M>(), std::basic_ostream<C, T>&>
    operator<<(std::basic_ostream<C, T>& os, const M& m)
    {
      os << '[';
      for (std::size_t i = 0; i < rows(m); ++i) {
        os << m[i];
        if (i + 1 != rows(m))
          os << ',';
      }
      os << ']';
      return os;
    }

} // namespace origin

#endif
