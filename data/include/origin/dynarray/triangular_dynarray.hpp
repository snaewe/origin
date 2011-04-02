// Copyright (c) 2008-2010 Kent State University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_DATA_TRIANGULAR_DYNARRAY_HPP
#define ORIGIN_DATA_TRIANGULAR_DYNARRAY_HPP

#include <stdexcept>


#include <stdexcept>

#include <origin/memory.hpp>
#include <origin/iterator/stride_iterator.hpp>
#include <origin/range/iterator_range.hpp>

namespace origin
{
  // FIXME: This should be a random access iterator. Right now, it's just
  // a forward iterator.

  // FIXME: Is this a generalizeable iterator type?
  /**
   * A column iterator for triangular arrays. This is essentially a stride
   * iterator where the stride increases with each successive iteration.
   * The result is incrementally longer strides.
   */
  template<typename Iter>
  class tri_col_iterator
    : public random_access_iterator_facade<
        tri_col_iterator<Iter>,
        typename std::iterator_traits<Iter>::value_type,
        typename std::iterator_traits<Iter>::reference,
        typename std::iterator_traits<Iter>::pointer,
        typename std::iterator_traits<Iter>::difference_type
      >
  {
    typedef random_access_iterator_facade<
      tri_col_iterator<Iter>,
      typename std::iterator_traits<Iter>::value_type,
      typename std::iterator_traits<Iter>::reference,
      typename std::iterator_traits<Iter>::pointer,
      typename std::iterator_traits<Iter>::difference_type
    > base_type;
  public:
    typedef typename super_type::reference reference;
    typedef typename super_type::difference_type difference_type;

    //column_iterator() = delete;
    column_iterator(column_iterator const&) = default;

    // Create a column iterator starting at the given pointer and having the
    // initial increment. The increment can be left as the defualt for end
    // iterators.
    column_iterator(pointer ptr, difference_type step = 0)
      : _ptr(ptr), _step(step)
    { }

    reference dereference() const
    { return *_ptr; }

    void increment()
    { _ptr += _step++; }

    void decrement()
    { _ptr -= --_step; }

    bool equal(column_iterator const& x) const
    { return _ptr == x._ptr; }

    bool less(column_iterator const& x) const
    { return _ptr < x._ptr; }

  public:
    Iter iter_;
    difference_type step_;
  };

  template<typename> class symmetric_iterator;

  template<typename T>
  struct symmetric_iterator_base {
    typedef iterator_facade<
      symmetric_iterator<T>, std::forward_iterator_tag, T
    > type;
  };

  /**
  * The symmetric row iterator generates over the sequence of values in the
  * triangular array such that symmetry is observed in the iterative sequence.
  *
  * @note Row and column iteration are symmetric. You get the same values and
  * in the same order for each row or column. We implement this by iterating
  * each row and then "turning the corner" to iterate down the column.
  *
  * @todo Make this a bidirectional iterator. Consider making it a random access
  * iterator, which requires column_iterator above to also be random access.
  */
  template<typename T>
  class symmetric_iterator
    : public symmetric_iterator_base<T>::type
  {
    typedef typename symmetric_iterator_base<T>::type super_type;
  public:
    typedef triangular_dynarray<T> array_type;
    typedef typename array_type::size_type size_type;

  private:
    typedef typename array_type::row_iterator row_iterator;
    typedef typename array_type::column_iterator column_iterator;

    union iterator {
      iterator(row_iterator r) : row(r) { }
      iterator(column_iterator c) : col(c) { }
      row_iterator row;
      column_iterator col;
    };

  public:
    typedef typename super_type::reference reference;

    symmetric_iterator() = delete;
    symmetric_iterator(symmetric_iterator const&) = default;

    // Construct an iterator over the first element of the nth row. This is
    // appropriate for constructing begin iterators.
    symmetric_iterator(array_type* a, size_type n)
      : _array(a), _iter(a->begin_row(n)), _stripe(n), _corner(false)
    { }

    // Construct an iterator over the given iterator in the nth row or column.
    // If i is not in the nth row, then the program will result in undefined
    // behavior.
    symmetric_iterator(array_type* a, size_type n, row_iterator i)
      : _array(a), _iter(i), _stripe(n), _corner(false)
    { }

    // Construct an iterator over the given iterator in the nth row or column.
    symmetric_iterator(array_type* a, size_type n, column_iterator i)
      : _array(a), _iter(i), _stripe(n), _corner(true)
    { }

    reference dereference() const {
      return _corner ? *_iter.col : *_iter.row;
    }

    void increment() {
      if(_corner) {
        ++_iter.col;
      } else {
        ++_iter.row;
        if(_iter.row == _array->end_row(_stripe)) {
          _iter.col = ++_array->begin_column(_stripe);
          _corner = true;
        }
      }
    }

    void decrement() {
      // FIXME: Implemnt me!
    }

    bool equal(symmetric_iterator const& x) const {
      if(_corner != x._corner) {
        return false;
      } else if(_corner) {
        return _iter.col == x._iter.col;
      } else {
        return _iter.row == x._iter.row;
      }
    }

  private:
    array_type* _array;
    iterator    _iter;

    // FIXME:   These can be compacted using a bitfield
    size_type   _stripe;    // The row or column being iterated.
    bool        _corner;    // True if we've "turned the corner"
  };

  } // namespace triangular_dynarray_

  // FIXME Add policies for horizontal versus diagonal storage.
  //
  /**
  * The triangular array implements an array capable of storing elements in
  * lower triangular arrangement (i.e., for all coordinates (i, j), i >= j.
  * Unlike a triangular matrix, triangular arrays do not implement symmetric
  * access or iteration of the elements in question. This only implements
  * the required storage.
  */
  template<typename T>
  class triangular_dynarray {
  private:
    typedef triangular_dynarray this_type;
  public:
    typedef T value_type;
    typedef T& reference;
    typedef T const& const_reference;
    typedef T* pointer;
    typedef T const* const_pointer;
    typedef std::size_t size_type;

    typedef T* iterator;
    typedef T const* const_iterator;

    typedef T* row_iterator;
    typedef iterator_range<row_iterator> row_range;
    typedef T const* const_row_iterator;
    typedef iterator_range<const_row_iterator> const_row_range;

    typedef triangular_dynarray_::column_iterator<T> column_iterator;
    typedef iterator_range<column_iterator> column_range;
    typedef triangular_dynarray_::column_iterator<T const> const_column_iterator;
    typedef iterator_range<const_column_iterator> const_column_range;

    typedef triangular_dynarray_::symmetric_iterator<T> symmetric_iterator;
    typedef iterator_range<symmetric_iterator> symmetric_range;
    typedef triangular_dynarray_::symmetric_iterator<T const> const_symmetric_iterator;
    typedef iterator_range<symmetric_iterator> const_symmetric_range;

    /** @name Construction and Destruction. */
    //@{
    triangular_dynarray(size_type n)
      : _data(new value_type[sum(n)]), _order(n)
    { }

    triangular_dynarray(size_type n, value_type const& x)
      : _data(new value_type[sum(n)]), _order(n)
    {
      std::fill(begin(), end(), x);
    }

    ~triangular_dynarray() {
      delete [] _data;
    }

    triangular_dynarray(triangular_dynarray const& x)
      : _data(new value_type[sum(x.size())]), _order(x._order)
    {
      std::copy(x.begin(), x.end(), _data);
    }

    triangular_dynarray& operator=(triangular_dynarray const& x) {
      if(&x != this) {
        triangular_dynarray tmp(x);
        swap(tmp);
      }
      return *this;
    }

    // FIXME Add move semantics
    //@}

    /** @name Properties */
    //@{
    bool empty() const {
      return false;
    }
    size_type size() const {
      return sum(_order);
    }
    size_type order() const {
      return _order;
    }

    size_type offset(size_type m, size_type n) const {
      return sum(m) + n;
    }

    // FIXME: Find a constant-time computation for this property.
    size_type row_num(size_type n) const {
      return -1;
    }

    // FIXME: Find a constant-time computation for this property
    size_type column_num(size_type n) const {
      return -1;
    }

    // FIXME: Implement this function.
    std::pair<size_type, size_type> row_and_column(size_type n) const {
      return -1;
    }

    pointer data() const {
      return _data;
    }
    //@}

    /** @name Accessors. */
    //@{
    reference operator[](size_type n) {
      return *(_data + n);
    }
    const_reference operator[](size_type n) const {
      return *(_data + n);
    }

    reference operator()(size_type m, size_type n) {
      return *(_data + offset(m, n));
    }
    const_reference operator()(size_type m, size_type n) const {
      return *(_data + offset(m, n));
    }

    reference at(size_type n) {
      return get(n);
    }
    const_reference at(size_type n) const {
      return get(n);
    }

    reference at(size_type m, size_type n) {
      return get(offset(m, n));
    }
    const_reference at(size_type m, size_type n) const {
      return get(offset(m, n));
    }

    reference front() {
      return *_data;
    }
    const_reference front() const {
      return *_data;
    }

    reference back() {
      return *(_data + size() - 1);
    }
    const_reference back() const {
      return *(_data + size() - 1);
    }
    //@}

    /** @name Iterators. */
    //@{
    iterator begin() {
      return _data;
    }
    iterator end() {
      return _data + size();
    }

    const_iterator begin() const {
      return _data;
    }
    const_iterator end() const {
      return _data + size();
    }


    // non-const row iteration
    row_iterator begin_row(size_type n) {
      return _data + sum(n);
    }
    row_iterator end_row(size_type n) {
      return _data + sum(n + 1);
    }
    row_range row(size_type n) {
      return row_range(begin_row(n), end_row(n));
    }

    // const row iteration
    const_row_iterator begin_row(size_type n) const {
      return _data + sum(n);
    }
    const_row_iterator end_row(size_type n) const {
      return _data + sum(n + 1);
    }
    const_row_range row(size_type n) const {
      return row_range(begin_row(n), end_row(n));
    }

    // non-const column iteration
    column_iterator begin_column(size_type n) {
      return column_iterator(_data + first_column(n), n + 1);
    }
    column_iterator end_column(size_type n) {
      return column_iterator(_data + last_column(n));
    }
    column_range column(size_type n) {
      return column_range(begin_column(n), end_column(n));
    }

    // const column iteration
    const_column_iterator begin_column(size_type n) const {
      return column_iterator(_data + first_column(n), n + 1);
    }
    const_column_iterator end_column(size_type n) const {
      return column_iterator(_data + last_column(n));
    }
    const_column_range column(size_type n) const {
      return const_column_range(begin_column(n), end_column(n));
    }

    // non-const symmetric row iteration
    symmetric_iterator begin_symmetric_row(size_type n) {
      return symmetric_iterator(this, n);
    }
    symmetric_iterator end_symmetric_row(size_type n) {
      return symmetric_iterator(this, n, end_column(n));
    }
    symmetric_range symmetric_row(size_type n) {
      return symmetric_range(begin_symmetric_row(n), end_symmetric_row(n));
    }

    // const symmetric row iteration
    const_symmetric_iterator begin_symmetric_row(size_type n) const {
      return symmetric_iterator(this, n);
    }
    const_symmetric_iterator end_symmetric_row(size_type n) const {
      return symmetric_iterator(this, n, end_symmetric_column(n));
    }
    const_symmetric_range symmetric_row(size_type n) const {
      return const_symmetric_range(begin_symmetric_row(n), end_symmetric_row(n));
    }

    // non-const symmetric column iteration
    symmetric_iterator begin_symmetric_column(size_type n) {
      return symmetric_iterator(this, n);
    }
    symmetric_iterator end_symmetric_column(size_type n) {
      return symmetric_iterator(this, n, end_column(n));
    }
    symmetric_range symmetric_column(size_type n) {
      return symmetric_range(begin_symmetric_column(n), end_symmetric_column(n));
    }

    // const symmetric column iteration
    const_symmetric_iterator begin_symmetric_column(size_type n) const {
      return symmetric_iterator(this, n);
    }
    const_symmetric_iterator end_symmetric_column(size_type n) const {
      return symmetric_iterator(this, n, end_column(n));
    }
    const_symmetric_range symmetric_column(size_type n) const {
      return const_symmetric_range(begin_symmetric_column(n),
                                  end_symmetric_column(n));
    }
    //@}

    void swap(triangular_dynarray&);

  private:
    // Return the sum of integers from 1 to n. Note that sum(n) also denotes the
    // offset of the 1st element in the nth row.
    size_type sum(size_type n) const {
      return n * (n + 1) / 2;
    }

    // Return the sum of integers from m to n.
    size_type sum(size_type m, size_type n) const {
      return (n - m + 1) * (n + m) / 2;
    }


    // Return the offset of the first element in the nth column.
    size_type first_column(size_type n) const {
      return sum(n + 1) - 1;
    }

    // Return the offset of the last element in the nth column.
    size_type last_column(size_type n) const {
      return first_column(n) + sum(n + 1, _order);
    }

    // Helper fucntion for checked access
    value_type& get(size_type n) {
      if(n >= size()) {
        throw std::out_of_range("square_dynarray: out of range");
      }
      return *(_data + n);
    }

  public:
    T* _data;
    size_type _order;
  };

  template<typename T>
  void triangular_dynarray<T>::swap(triangular_dynarray& c) {
    std::swap(_data, c._data);
    std::swap(_order, c._order);
  }

  template<typename T>
  void swap(triangular_dynarray<T>& a, triangular_dynarray<T>& b) {
    a.swap(b);
  }

} // namespace origin

#endif
