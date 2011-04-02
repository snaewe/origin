// Copyright (c) 2008-2010 Kent State University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_DATA_RECTANGULAR_DYNARRAY_HPP
#define ORIGIN_DATA_RECTANGULAR_DYNARRAY_HPP

#include <stdexcept>

#include <origin/memory.hpp>
#include <origin/iterator/stride_iterator.hpp>
#include <origin/range/iterator_range.hpp>

namespace origin
{
  /**
   * The rectangular dynarray base implements the basic structure and memory
   * allocation requirements of a rectangular dynarray. The dynarray is
   * implemented in terms of a pointer its dimensions.
   */
  template<typename T, typename Alloc>
  struct rectangular_dynarray_base : private Alloc
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
    rectangular_dynarray_base(rectangular_dynarray_base const& x)
      : Alloc{x.alloc}, data{allocate(x.size())}, rows{x.rows}, cols{x.cols}
    { }

    // Move constructor
    rectangular_dynarray_base(rectangular_dynarray_base&& x)
      : Alloc{x.alloc}, data{x.data}, rows{x.rows}, cols{x.cols}
    {
      x.data = nullptr;
      x.rows = x.cols = 0;
    }

    // Default constructor
    rectangular_dynarray_base(allocator_type const& alloc)
      : Alloc{alloc}, data{nullptr}, rows{0}, cols{0}
    { }

    // Bound constructor
    rectangular_dynarray_base(size_type m,
                              size_type n,
                              allocator_type const& alloc)
      : Alloc{alloc}, data{allocate(m, n)}, rows{m}, cols{n}
    { }

    ~rectangular_dynarray_base()
    { deallocate(data); }

    size_type size() const
    { return rows * cols; }

    allocator_type& get_alloc()
    { return *this; }

    // Allocate n * n elements
    pointer allocate(size_type m, size_type n)
    { return get_alloc().allocate(m * n); }

    void deallocate(pointer p)
    { get_alloc().deallocate(p, size()); }

    pointer data;
    size_type rows;
    size_type cols;
  };

  /**
   * @brief A dynamically allocated rectangular array
   * The rectangular_dynarray class implements a dynamically allocated,
   * rectangular array. The array is generally initialized with m rows and
   * n columns. The size of the array is m * n. Array elements are indexed in
   * row-major order.
   *
   * @tparam T      The value type of the dynarray
   * @tparam Alloc  An Allocator type
   */
  template<typename T, typename Alloc = std::allocator<T>>
  class rectangular_dynarray
    : rectangular_dynarray_base<T, Alloc>
  {
    typedef rectangular_dynarray_base<T, Alloc> base_type;
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
    /**
     * @brief Default constructor
     * Construct an empty dynamic array with 0 rows and 0 columns.
     */
    rectangular_dynarray(allocator_type const& alloc = allocator_type{})
      : base_type{alloc}
    { }

    // Copy semantics
    /**
     * @brief Copy constructor
     * Construct a copy of the rectangular dynarray.
     *
     * @tparam x  A rectangular dynarray
     */
    rectangular_dynarray(rectangular_dynarray const& x)
      : base_type{x}
    { uninitialized_copy(this->get_alloc(), x.begin(), x.end(), begin()); }

    /**
     * @brief Copy assignment
     */
    rectangular_dynarray& operator=(rectangular_dynarray const& x)
    { rectangular_dynarray tmp(x); swap(tmp); return *this; }

    // Move semantics
    /**
     * @brief Move constructor
     *
     * @tparam x  A rectangular dynarray
     */
    rectangular_dynarray(rectangular_dynarray&& x)
      : base_type{std::move(x)}
    { }

    /**
     * @brief Move assignment
     */
    rectangular_dynarray& operator=(rectangular_dynarray&& x)
    { rectangular_dynarray tmp{std::move(x)}; swap(tmp); return *this; }

    /**
     * @brief Order constructor
     * Construct a rectangular dynarray with order n. Elements are initialized
     * to the given value, or default initialized if not explicitly given.
     *
     * @param x       The initial value of the array elements
     * @param alloc   An allocator object
     */
    explicit rectangular_dynarray(size_type m,
                                  size_type n,
                                  value_type const& x = value_type{},
                                  allocator_type alloc = allocator_type{})
      : base_type{m, n, alloc}
    { uninitialized_fill(this->get_alloc(), begin(), end(), x); }

    /**
     * @brief Initializer list constructor
     * Construct a rectangular dynarray over nested initializer as lists::
     *
     *    rectangular_dynarray<T> x = {
     *      {a, b, c},
     *      {d, e, f}
     *    };
     *
     * This results in the construction of a 2x3 rectangular dynarray. The size
     * of all inner initializer lists must be the same length.
     */
    rectangular_dynarray(std::initializer_list<std::initializer_list<value_type>> list,
                    allocator_type const& alloc = allocator_type{})
      : base_type{list.size(), list.begin()->size(), alloc}
    {
      iterator i = begin();
      size_type n = list.begin()->size();
      for(auto const& r : list) {
        assert(( r.size() == n ));
        uninitialized_copy(this->get_alloc(), r.begin(), r.end(), i);
        i += cols();
      }
    }

    ~rectangular_dynarray()
    { destroy(this->get_alloc(), begin(), end()); }
    //@}

    /** @name Properties */
    //@{
    /**
     * @brief Empty
     * Return true if the rectangular matrix has zero elements. This only the
     * case if the matrix has m.rows() == 0 or m.cols() == 0.
     */
    bool empty() const
    { return base_type::size() == 0; }

    /**
     * @brief Number of elements
     * Return the number of eleemnts in the rectangular dynarray. This is
     * equivalent to m.rows() * m.cols().
     */
    size_type size() const
    { return rows() * cols(); }

    /**
     * @brief Number of rows
     * Return the number of rows in the rectangular dynarray.
     */
    size_type rows() const
    { return base_type::rows; }

    /**
     * @brief Number of columns
     * Return the number of columns in the rectangular dynarray.
     */
    size_type cols() const
    { return base_type::cols; }

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
     * Given row and column m and n, return the offset into the array.
     */
    size_type offset(size_type m, size_type n) const
    { return m * cols() + n; }

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
     * Return true if this rectangular dynarray is equivalent to the other. Two
     * rectangular dynarrays are equal if they have the same dimensions and
     * equal ranges.
     *
     * @param x   A rectangular dynarray.
     */
    bool equal(rectangular_dynarray const& x) const
    {
      return rows() == x.rows()
          && cols() == x.cols
          && std::equal(begin(), end(), x.begin());
    }

    /**
     * @brief Order protocol
     * Return true if this rectangular dynarray is less than the other. The
     * order of dynarrays is computed lexicographically over the ranges of
     * the dynarrays.
     *
     * @param x   A rectangular dynarray
     */
    bool less(rectangular_dynarray const& x) const
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
    { return data() + n * cols(); }

    row_iterator end_row(size_type n)
    { return data() + (n + 1) * cols(); }

    const_row_iterator begin_row(size_type n) const
    { return data() + n * cols(); }

    const_row_iterator end_row(size_type n) const
    { return data() + (n + 1) * cols(); }


    // Row range access
    row_range row(size_type n)
    { return {begin_row(n), end_row(n)}; }

    const_row_range row(size_type n) const
    { return {begin_row(n), end_row(n)}; }


    // Column access
    col_iterator begin_column(size_type n)
    { return {data() + n, cols()}; }

    col_iterator end_column(size_type n)
    { return {data() + size() + n, cols()}; }

    const_col_iterator begin_column(size_type n) const
    { return {data() + n, cols()}; }

    const_col_iterator end_column(size_type n) const
    { return {data() + size() + n, cols()}; }


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
        throw std::out_of_range("rectangular_dynarray: out of range");
      }
      return *(data() + n);
    }
  };

} // namespace origin

#endif
