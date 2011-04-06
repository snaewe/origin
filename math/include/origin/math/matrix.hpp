// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MATH_MATRIX_HPP
#define ORIGIN_MATH_MATRIX_HPP

#include <valarray>

#include <origin/math/slice.hpp>

namespace origin
{
  // FIXME: Consider parameterizing over row-major/column-major access and
  // 0-based/1-based offset access. That would allow the matrix type to
  // interoperate with Fortran arrays.

  // FIXME: Write actual matrix operations.
  /**
   * @brief A dynamically bounded matrix
   * The matrix template defines a dynamically allocated, dynamically bounded,
   * dense Matrix implementation. The implementation of the general matrix
   * class is intended to support high performance computing. It is implemented
   * in terms of the std::valarray class template.
   *
   * Note that value-initialization of matrices follows the valarray pattern.
   * The value is specified before the dimensions.
   *
   * @tparam T      A Ring type
   */
  template<typename T>
  class matrix
  {
    typedef std::valarray<T> array_type;
  public:
    typedef T value_type;
    typedef T& reference;
    typedef T const& const_reference;
    typedef std::size_t size_type;

    typedef slice_reference<T> slice_type;
    typedef slice_reference<T const> const_slice_type;

    /** @name Construction and destruction */
    //@{
    /**
     * @brief Default constructor
     * Construct a 0 x 0 matrix.
     */
    matrix()
      : data_{}
    { }

    // Copy/Move, Destructor should be implicitly generated

    /**
     * @brief Matrix constructor
     * Cosntruct an m x n rectangular matrix. All elements are default
     * initialized.
     *
     * @param m   The number of of rows
     * @param n   The number of columns
     */
    matrix(size_type m, size_type n)
      : data_(m * n), rows_(m), cols_(n)
    { }

    /**
     * @brief Matrix constructor
     * Cosntruct an m x n rectangular matrix. Elements on the diagonal are
     * initialized to the given value.
     *
     * @param x   The initial value of diagonal elements
     * @param m   The number of of rows
     * @param n   The number of columns
     */
    matrix(value_type const& x, size_type m, size_type n)
      : data_(m * n), rows_(m), cols_(n)
    {
      size_type k = std::min(m, n);
      for(size_type i = 0; i < k; ++i) {
        data_[offset(i, i)] = x;
      }
    }
    //@}

    /** @name Properties */
    //@{
    /**
     * @brief Number of rows
     * Return the number of rows in the matrix.
     */
    size_type rows() const
    { return rows_; }

    /**
     * @brief Number of columns
     * Return the number of columns in the matrix.
     */
    size_type cols() const
    { return cols_; }

    /**
     * @brief Matrix size
     * Return the number of elements in the matrix.
     */
    size_type size() const
    { return data_.size(); }

    /**
     * @brief Array offset
     * Return the offset into the underlying array of the indexes i and j.
     *
     * @param i   The ith row
     * @param j   The jth column
     */
    size_type offset(size_type i, size_type j) const
    { return i * cols() + j; }

    /**
     * @brief Matrix array
     * Return the valarray implementing the matrix.
     */
    array_type& array()
    { return data_; }

    array_type const& array() const
    { return data_; }

    //@}

    /** @name Element access */
    //@{
    /**
     * @brief Element access
     * Return the element at the row i and column j.
     *
     * @param i   A row index
     * @param j   A column index
     */
    reference operator()(size_type i, size_type j)
    { return data_[offset(i, j)]; }

    const_reference operator()(size_type i, size_t j) const
    { return data_[offset(i, j)]; }

    /**
     * @brief Row access
     * Return the ith row of the matrix.
     *
     * @param i   A row index
     */
    slice_type operator[](size_type i)
    { return row(i); }
    
    const_slice_type operator[](size_type i) const
    { return row(i); }

    /**
     * @brief Row slice
     * Return a slice reference for the ith row in the matrix.
     */
    slice_type row(size_type i)
    { return slice_type(data_, std::slice{i, rows(), cols()}); }

    slice_type row(size_type i) const
    { return slice_type(data_, std::slice{i, rows(), cols()}); }

    /**
     * @brief Column slice
     * Return a slice reference for the ith row in the matrix.
     */
    slice_type column(size_type i)
    { return slice_type(data_, std::slice{i, rows(), cols()}); }

    slice_type column(size_type i) const
    { return slice_type(data_, std::slice{i, rows(), cols()}); }
    //@}

  private:
    array_type data_;
    size_type rows_;
    size_type cols_;
  };

  // Range adaptation for matrices.
  template<typename T>
  auto begin(matrix<T>& m) -> decltype(std::begin(m.array()))
  { return std::begin(m.array()); }

  template<typename T>
  auto end(matrix<T>& m) -> decltype(std::end(m.array()))
  { return std::end(m.array()); }

  template<typename T>
  auto begin(matrix<T> const& m) -> decltype(std::begin(m.array()))
  { return std::begin(m.array()); }

  template<typename T>
  auto end(matrix<T> const& m) -> decltype(std::end(m.array()))
  { return std::end(m.array()); }
}

#endif
