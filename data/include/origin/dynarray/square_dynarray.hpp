// Copyright (c) 2008-2010 Kent State University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_DATA_SQUARE_DYNARRAY_HPP
#define ORIGIN_DATA_SQUARE_DYNARRAY_HPP

#include <stdexcept>
#include <utility>

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

    // Copy constructor
    square_dynarray_base(square_dynarray_base const& x)
      : Alloc{x.get_alloc()}, data{allocate(x.order)}, order{x.order}
    { }

    // Move constructor
    square_dynarray_base(square_dynarray_base&& x)
      : Alloc{x.get_alloc()}, data{x.data}, order{x.order}
    {
      x.data = nullptr;
      x.order = 0;
    }

    // Default constructor
    square_dynarray_base(allocator_type const& alloc)
      : Alloc{alloc}, data{nullptr}, order{0}
    { }

    // Bounds constructor
    square_dynarray_base(size_type n, allocator_type const& alloc)
      : Alloc{alloc}, data{allocate(n)}, order{n}
    { }

    ~square_dynarray_base()
    { deallocate(data); }

    size_type size() const
    { return order * order; }

    allocator_type& get_alloc()
    { return *this; }
    // FIXME Added because of const complaints
    allocator_type const& get_alloc() const
    { return *this; }

    // Allocate n * n elements
    pointer allocate(size_type n)
    { return get_alloc().allocate(n * n); }

    void deallocate(pointer p)
    { get_alloc().deallocate(p, size()); }

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
    typedef std::pair<size_type, size_type> order_type;

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
    /**
     * @brief Default constructor
     * Cosntruct an empty square dynarray.
     */
    square_dynarray(allocator_type const& alloc = allocator_type{})
      : base_type{alloc}
    { }

    // Copy semantics
    /**
     * @brief Default constructor
     * Cosntruct a copy of the square dynarray x.
     *
     * @param x   A square dynarray
     */
    square_dynarray(square_dynarray const& x)
      : base_type{x}
    { uninitialized_copy(this->get_alloc(), x.begin(), x.end(), begin()); }

    /**
     * @brief Default constructor
     * Assign this as a copy of the square dynarray x.
     *
     * @param x   A square dynaray
     */
    square_dynarray& operator=(square_dynarray const& x)
    { square_dynarray tmp(x); swap(tmp); return *this; }

    // Move semantics
    /**
     * @brief Default constructor
     * Construct a square dynarray by moving data out of the square dynarray x.
     * After construction x is left in a moved-from state.
     *
     * @param x   A square dynarray
     */
    square_dynarray(square_dynarray&& x)
      : base_type{std::move(x)}
    { }

    /**
     * @brief Default constructor
     * Move the state of the square dynarray x into this object. After
     * assignment x is left in a moved-from state.
     *
     * @param x   A square dynarray
     */
    square_dynarray& operator=(square_dynarray&& x)
    { square_dynarray tmp{std::move(x)}; swap(tmp); return *this; }

    /**
     * @brief Order constructor
     * Construct a square dynarray with n rows and columns. Elements are
     * default initialized or initialized to the given value. An allocator
     * may be given.
     *
     * @param n       The number of rows and columns
     * @param x       The initial value of the array elements
     * @param alloc   An allocator object
     */
    explicit square_dynarray(size_type n,
                             value_type const& x = value_type{},
                             allocator_type alloc = allocator_type{})
      : base_type{n, alloc}
    { uninitialized_fill(this->get_alloc(), begin(), end(), x); }

    /**
     * @brief Initializer list constructor
     * Construct a square dynarray over nested initializer as lists::
     *
     *    square_dynarray<T> x = {
     *      {a, b},
     *      {c, d}
     *    };
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
    { return order() == 0; }

    /**
     * @brief Number of elements
     * Return the total number of elements in the square dynarray. This is
     * equivalent to m.rows() * m.rows().
     */
    size_type size() const
    { return base_type::size(); }

    /**
     * @brief Matrix order
     * Return the order of the square dynarray. The order is the number of
     * rows and columns of a square dynarray.
     */
    size_type order() const
    { return base_type::order; }

    /**
     * @brief Number of rows
     * Return the number of rows in the square dynarray. This is equivalent to
     * the m.order().
     */
    size_type rows() const
    { return base_type::order; }

    /**
     * @brief Number of columns
     * Return the number of columns in the square dynarray. This is equivalent
     * to the m.order().
     */
    size_type cols() const
    { return base_type::order; }

    /**
     * @brief Array data
     * Return a pointer to the stored data.
     */
    pointer data()
    { return base_type::data; }

    const_pointer data() const
    { return base_type::data; }

    /**
     * @brief Array offset
     * Given indexes m and n, return the offset into the array.
     */
    size_type offset(size_type m, size_type n) const
    { return m * order() + n; }

    /**
     * @brief Allocator
     * Return the allocator object used by this object.
     */
    allocator_type get_allocator() const
    { return this->get_alloc(); }
    //@}

    /** @name Data Accessors */
    //@{
    reference operator[](size_type n)
    { return data()[n]; }

    const_reference operator[](size_type n) const
    { return data()[n]; }

    reference operator()(size_type m, size_type n)
    { return data()[offset(m, n)]; }

    const_reference operator()(size_type m, size_type n) const
    { return data()[offset(m, n)]; }

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
    { return *data(); }

    const_reference front() const
    { return *data(); }

    reference back()
    { return *(data() + size() - 1); }

    const_reference back() const
    { return *(data() + size() - 1); }
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
    { return data(); }

    iterator end()
    { return data() + size(); }

    const_iterator begin() const
    { return data(); }

    const_iterator end() const
    { return data() + size(); }


    // Row access
    row_iterator begin_row(size_type n)
    { return data() + n * order(); }

    row_iterator end_row(size_type n)
    { return data() + (n + 1) * order(); }

    const_row_iterator begin_row(size_type n) const
    { data() + n * order(); }

    const_row_iterator end_row(size_type n) const
    { data() + (n + 1) * order(); }


    // Row range access
    row_range row(size_type n)
    { return {begin_row(n), end_row(n)}; }

    const_row_range row(size_type n) const
    { return {begin_row(n), end_row(n)}; }


    // Column access
    col_iterator begin_column(size_type n)
    { return {data() + n, base_type::order}; }

    col_iterator end_column(size_type n)
    { return {data() + size() + n, base_type::order}; }

    const_col_iterator begin_column(size_type n) const
    { return {data() + n, base_type::order}; }

    const_col_iterator end_column(size_type n) const
    { return {data() + size() + n, base_type::order}; }


    // Column range access
    col_range column(size_type n)
    { return {begin_column(n), end_column(n)}; }

    const_col_range column(size_type n) const
    { return {begin_column(n), end_column(n)}; }
    //@}

    /** @name Swap */
    //@{
    void swap(square_dynarray& x) {
      std::swap(base_type::data, x.base_type::data);
      std::swap(base_type::order, x.base_type::order);
    }
    //@}

  private:
    // Helper fucntion for checked access
    value_type& get(size_type n) {
      if(n >= size()) {
        throw std::out_of_range("square_dynarray: out of range");
      }
      return *(data() + n);
    }
  };

} // namespace origin

#endif
