// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_NUMERIC_MATRIX_HPP
#define ORIGIN_NUMERIC_MATRIX_HPP

#include <origin/dynarray/rectangular_dynarray.hpp>
#include <origin/dynarray/square_dynarray.hpp>

namespace origin
{
  /**
   * @internal
   * @class is_square_data<T>
   * Return true if the given data store is square. Note that the Data parameter
   * must not be a matrix. It should be the matrix data type.
   *
   * @tparam Data   A Two_Dimensional_Array type
   * @tparam Dummy  A type
   */
  //@{
  template<typename Data, typename Dummy>
  struct is_square_data
    : std::false_type
  { };

  template<typename T, typename A, typename Dummy>
  struct is_square_data<square_dynarray<T, A>, Dummy>
    : std::true_type
  { };
  //@}

  /**
   * @brief A dynamically bounded matrix
   * The matrix template defines a dynamically allocated, dynamically bounded
   * Matrix implementation. The matrix is parameterized over its element
   * type (also called the value type) and its storage implementation.
   *
   * The Storage parameter defines the element access sytsem that underlies
   * the matrix. By default, this is a rectangular dynarray, which supports
   * dense, rectangular matrices.
   *
   * The storage mechanism determines if the matrix is rectangular or strictly
   * square. A square matrix statically enforces the constraint that
   * m.rows() == m.cols(). Different constructors are used when the matrix
   * is backed by square data.
   *
   * @tparam T      A Ring type
   * @tparam Store  A Two_Dimensional_Array type
   */
  template<typename T, typename Store = rectangular_dynarray<T>>
  class matrix
  {
  public:
    typedef Store data_type;
    typedef T value_type;
    typedef typename data_type::reference reference;
    typedef typename data_type::const_reference const_reference;
    typedef typename data_type::pointer pointer;
    typedef typename data_type::const_pointer const_pointer;
    typedef typename data_type::size_type size_type;
    typedef typename data_type::allocator_type allocator_type;

    typedef typename data_type::iterator iterator;
    typedef typename data_type::const_iterator const_iterator;

    /** @name Construction and destruction */
    //@{
    /**
     * @brief Default constructor
     * Construct a 0 x 0 matrix. An allocator may optionally be given.
     *
     * @param alloc   An allocator object
     */
    matrix(allocator_type const& alloc = allocator_type{})
      : impl_{}
    { }

    // Copy/Move, Destructor should be implicitly generated

    // FIXME: What are the semantics of initialization. Does scalar value
    // initialzation, M{m, n, 2} imply fill initialization or is it actually
    // scalar * matrix multiplication.

    /**
     * @brief Square matrix constructor
     * Cosntruct an n x n square matrix. Elements on the diaganol are
     * initialized to the given initial value or to a default value. All other
     * elements are default initialized. An allocator may also be given.
     *
     * @param n       The order of the square matrix
     * @param x       The initial on the diaganol
     * @param alloc   An allocator object
     */
    template<typename Size>
      explicit matrix(Size n,
                      value_type const& x = value_type{},
                      allocator_type const& alloc = allocator_type{},
                      typename std::enable_if<
                        is_square_data<data_type, Size>::value, void
                      >::type* = nullptr)
        : impl_{n, value_type{}, alloc}
      {
        // FIXME; I'm double-initializing the diaganol. Removing the double
        // initialization requires that I be able to partially construct the
        // underlying data store. I'm not sure if it's worth the effort.

        // FIXME: Yuck. There should be a more graceful way to do this. I'd
        // like to write: copy(impl_.diag(), x) or something like that.
        for(size_type i = 0; i < impl_.order(); ++i) {
          impl_(i, i) = x;
        }
      }

    /**
     * @brief Rectangular matrix constructor
     * Cosntruct an m x n rectangular matrix. Elements on the diaganol are
     * initialized to the given initial value or to a default value. All other
     * elements are default initialized. An allocator may also be given.
     *
     * @param n       The order of the square matrix
     * @param x       The initial on the diaganol
     * @param alloc   An allocator object
     */
    template<typename Size>
      matrix(Size m,
             Size n,
             value_type const& x = value_type{},
             allocator_type const& alloc = allocator_type{},
             typename std::enable_if<
               !is_square_data<data_type, Size>::value, void
             >::type* = nullptr)
        : impl_{m, n, value_type{}, alloc}
      {
        // FIXME: This is even worse than the square matrix initialization.
        // Note that for diagonal matrices, the diagonal is the lenght of
        // the minimum dimension.
        size_type mind = std::min(m, n);
        for(size_type i = 0; i < mind; ++i) {
          impl_(i, i) = x;
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
    { return impl_.rows(); }

    /**
     * @brief Number of columns
     * Return the number of columns in the matrix.
     */
    size_type cols() const
    { return impl_.cols(); }

    /**
     * @brief Matrix storage
     * Return the backing store for the matrix.
     */
    data_type& storage()
    { return impl_; }

    data_type const& storage() const
    { return impl_; }
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
    { return impl_(i, j); }

    const_reference operator()(size_type i, size_t j) const
    { return impl_(i, j); }

    // FIXME: Write checked access.
    //@}

    /** @name Iterators*/
    //@{

    /**
     * @brief Begin iterator
     * Return an iterator to the first stored element.
     */
    iterator begin()
    { return impl_.begin(); }

    /**
     * @brief End iterator
     * Return an iterator past the last stored element.
     */
    iterator end()
    { return impl_.end(); }

    const_iterator begin() const
    { return impl_.begin(); }

    const_iterator end() const
    { return impl_.end(); }
    //@}

  private:
    data_type impl_;
  };
}

#endif
