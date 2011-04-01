// Copyright (c) 2008-2010 Kent State University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_DATA_SQUARE_DYNARRAY_HPP
#define ORIGIN_DATA_SQUARE_DYNARRAY_HPP

#include <stdexcept>

#include <origin/memory.hpp>
#include <origin/iterator/stride_iterator.hpp>
#include <origin/range/iterator_range.hpp>

namespace origin
{
  /**
   * The square dynarray base implements the basic structure and memory
   * allocation requirements of a square dynarray. The dynarray is implemented
   * in terms of a pointer its order.
   */
  template<typename T, typename Alloc>
  struct square_dynarray_base : private Alloc
  {
    typedef Alloc allocator_type;
    typedef T value_type;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef typename allocator_type::size_type size_type;
    typedef typename allocator_type::difference_type difference_type;

    // Copy semantics
    square_dynarray_base(square_dynarray_base const& x)
      : Alloc{x.alloc}, data{allocate(x.order)}, order{x.order}
    { }

    // Move semantics
    square_dynarray_base(square_dynarray_base&& x)
      : Alloc{x.alloc}, data{x.data}, order{x.order}
    {
      x.data = nullptr;
      x.order = 0;
    }

    square_dynarray_base(allocator_type const& alloc)
      : Alloc{alloc}, data{nullptr}, order{0}
    { }

    square_dynarray_base(size_type n, allocator_type const& alloc)
      : Alloc{alloc}, data{allocate(n)}, order{n}
    { }

    ~square_dynarray_base()
    { deallocate(data); }

    allocator_type& get_alloc()
    { return *this; }

    // Allocate n * n elements
    pointer allocate(size_type n)
    { return get_alloc().allocate(n * n); }

    void deallocate(pointer p)
    { get_alloc().deallocate(p, order * order); }

    pointer data;
    size_type order;
  };

  /**
   * A general purpose, dynamically allocated, square array. An NxN square array
   * has size N, and extent N^2. This class provides a backing store for square
   * matrices, but does not implement matrix-specific functionality.
   *
   * The maximum size of a square matrix is `sqrt(numeric_limits<size_t>::max())`.
   */
  template<typename T, typename Alloc = std::allocator<T>>
  class square_dynarray
    : square_dynarray_base<T, Alloc>
  {
    typedef square_dynarray_base<T, Alloc> base_type;
  public:
    typedef Alloc allocator_type;
    typedef T value_type;
    typedef typename base_type::reference reference;
    typedef typename base_type::const_reference const_reference;
    typedef typename base_type::pointer pointer;
    typedef typename base_type::const_pointer const_pointer;
    typedef typename base_type::size_type size_type;
    typedef typename base_type::difference_type difference_type;

    // FIXME: Do we need reverse iterators also?
    typedef T* iterator;
    typedef T const* const_iterator;
    typedef T* row_iterator;
    typedef T const* const_row_iterator;
    typedef stride_iterator<T*> col_iterator;
    typedef stride_iterator<T const*> const_col_iterator;

    typedef iterator_range<row_iterator> row_range;
    typedef iterator_range<const_row_iterator> const_row_range;
    typedef iterator_range<col_iterator> col_range;
    typedef iterator_range<const_col_iterator> const_col_range;

    /** @name Construction and Destruction */
    //@{
    square_dynarray(allocator_type const& alloc = allocator_type{})
      : base_type{0, alloc}
    { }

    // Copy semantics
    square_dynarray(square_dynarray const& x)
      : base_type{x}
    { uninitialized_copy(this->get_alloc(), x.begin(), x.end(), begin()); }

    square_dynarray& operator=(square_dynarray const& x)
    { square_dynarray tmp(x); swap(tmp); return *this; }

    // Move semantics
    square_dynarray(square_dynarray&& x)
      : base_type{std::move(x)}
    { }

    square_dynarray& operator=(square_dynarray&& x)
    { square_dynarray tmp{std::move(x)}; swap(tmp); return *this; }

    /**
     * @brief Order constructor
     * Construct a square dynarray with order n. Elements are initialized to
     * the given value, or default initialized if not explicitly given.
     *
     * @param x       The initial value of the array elements
     * @param alloc   An allocator object
     */
    explicit square_dynarray(size_type n,
                             value_type const& x = value_type{},
                             allocator_type alloc = allocator_type{})
      : base_type{n, alloc}
    { uninitialized_fill(this->get_alloc(), begin(), end(), x); }

    // FIXME: Write a 1d initializer list? The size of the list must be a
    // perfect square.

    // FIXME: Write a range constructor? The distance of the range must be
    // a perfect square.

    /**
     * @brief Initializer list constructor
     * Construct a square dynarray over nested initializer as lists::
     *
     *    square_dynarray<T> = {
     *      {a, b},
     *      {c, d}
     *
     * The size of each inner initializer list must be the same as the outer
     * initializer list.
     */
    square_dynarray(std::initializer_list<std::initializer_list<value_type>> list,
                    allocator_type const& alloc = allocator_type{})
      : base_type{list.size(), alloc}
    {
      iterator i = begin();
      for(auto const& r : list) {
        assert(( r.size() == order() ));
        uninitialized_copy(this->get_alloc(), r.begin(), r.end(), i);
        i += order();
      }
    }

    ~square_dynarray()
    { destroy(this->get_alloc(), begin(), end()); }
    //@}

    /** @name Properties */
    //@{
    bool empty() const
    { return base_type::order == 0; }

    size_type size() const
    { return base_type::order * base_type::order; }

    size_type order() const
    { return base_type::order; }

    pointer data()
    { return base_type::data; }

    const_pointer data() const
    { return base_type::data; }

    size_type offset(size_type m, size_type n) const
    { return m * base_type::order + n; }

    allocator_type get_allocator() const
    { return this->get_alloc(); }
    //@}

    /** @name Data Accessors */
    //@{
    reference operator[](size_type n)
    { return base_type::data[n]; }

    const_reference operator[](size_type n) const
    { return base_type::data[n]; }

    reference operator()(size_type m, size_type n)
    { return base_type::data[offset(m, n)]; }

    const_reference operator()(size_type m, size_type n) const
    { return base_type::data[offset(m, n)]; }

    reference at(size_type n)
    { return get(n); }

    const_reference at(size_type n) const
    { return get(n); }

    reference at(size_type m, size_type n)
    { return get(offset(m, n)); }

    const_reference at(size_type m, size_type n) const
    { return get(offset(m, n)); }

    // FIXME: Do these really belong here?
    reference front()
    { return *base_type::data; }

    const_reference front() const
    { return *base_type::data; }

    reference back()
    { return *(base_type::data + size() - 1); }

    const_reference back() const
    { return *(base_type::data + size() - 1); }
    //@}

    /** @name Protocols */
    //@{
    /**
     * @brief Equality protocol
     * Return true if this square dynarray is equivalent to the other. Two
     * square dynarrays are equal if they have the same order and have the same
     * elements.
     *
     * @param x   A square dynarray.
     */
    bool equal(square_dynarray const& x) const
    { return order() == x.order() && std::equal(begin(), end(), x.begin()); }

    /**
     * @brief Order protocol
     * Return true if this square dynarray is less than the other. The order of
     * dynarrays is computed lexicographically.
     *
     * @param x   A square dynarray
     */
    bool less(square_dynarray const& x) const
    { return std::lexicographical_compare(begin(), end(), x.begin(), x.end()); }
    //@}

    /** @name Iterators */
    //@{
    iterator begin()
    { return base_type::data; }

    iterator end()
    { return base_type::data + size(); }

    const_iterator begin() const
    { return base_type::data; }

    const_iterator end() const
    { return base_type::data + size(); }


    // Row access
    row_iterator begin_row(size_type n)
    { return base_type::data + n * base_type::order; }

    row_iterator end_row(size_type n)
    { return base_type::data + (n + 1) * base_type::order; }

    const_row_iterator begin_row(size_type n) const
    { base_type::data + n * base_type::order; }

    const_row_iterator end_row(size_type n) const
    { base_type::data + (n + 1) * base_type::order; }


    // Row range access
    row_range row(size_type n)
    { return {begin_row(n), end_row(n)}; }

    const_row_range row(size_type n) const
    { return {begin_row(n), end_row(n)}; }


    // Column access
    col_iterator begin_column(size_type n)
    { return {base_type::data + n, base_type::order}; }

    col_iterator end_column(size_type n)
    { return {base_type::data + size() + n, base_type::order}; }

    const_col_iterator begin_column(size_type n) const
    { return {base_type::data + n, base_type::order}; }

    const_col_iterator end_column(size_type n) const
    { return {base_type::data + size() + n, base_type::order}; }


    // Column range access
    col_range column(size_type n)
    { return {begin_column(n), end_column(n)}; }

    const_col_range column(size_type n) const
    { return {begin_column(n), end_column(n)}; }
    //@}

  private:
    // Helper fucntion for checked access
    value_type& get(size_type n) {
      if(n >= size()) {
        throw std::out_of_range("square_dynarray: out of range");
      }
      return *(base_type::data_ + n);
    }
  };

} // namespace origin

#endif
