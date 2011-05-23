// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_DATA_DYNARRAY_HPP
#define ORIGIN_DATA_DYNARRAY_HPP


#include <origin/exception.hpp>
#include <origin/memory.hpp>

namespace origin
{
  /**
   * The square dynarray base implements the basic structure and memory
   * allocation requirements of a square dynarray. The dynarray is implemented
   * in terms of a pointer its order.
   */
  template<typename T, typename Alloc>
  struct dynarray_base : private Alloc
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
    dynarray_base(dynarray_base const& x)
      : Alloc{x.get_alloc()}, first{allocate(x.size())}, last{first + x.size()}
    { }

    // Move semantics
    dynarray_base(dynarray_base&& x)
      : Alloc{x.get_alloc()}, first{x.first}, last{x.last}
    { x.first = x.last = nullptr; }

    dynarray_base(allocator_type const& alloc)
      : Alloc{alloc}, first{nullptr}, last{nullptr}
    { }

    dynarray_base(size_type n, allocator_type const& alloc)
      : Alloc{alloc}, first{allocate(n)}, last{first + n}
    { }

    ~dynarray_base()
    { deallocate(first); }

    size_type size() const
    { return last - first; }

    allocator_type& get_alloc()
    { return *this; }
    // FIXME Added because of const complaints
    allocator_type const& get_alloc() const
    { return *this; }

    // Allocate n * n elements
    pointer allocate(size_type n)
    { return get_alloc().allocate(n); }

    void deallocate(pointer p)
    { get_alloc().deallocate(p, size()); }

    pointer first;
    pointer last;
  };

  /**
   * @brief A dynamically allocated array with fixed bounds
   * The dynarray (dynamic array) class implements a dynamically allocated
   * array.
   *
   * The dynarray defined in n2648. Not all of the functionality is implemented,
   * but the interface is similar to that defined in n2648 by Lawrence Crowl and
   * Matt Austern. We have added swap semantics and const reverse iterators.
   *
   * http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2648.html
   */
  template<typename T, typename Alloc = std::allocator<T>>
  class dynarray
    : dynarray_base<T, Alloc>
  {
    typedef dynarray_base<T, Alloc> base_type;
  public:
    typedef Alloc allocator_type;
    typedef T value_type;
    typedef typename base_type::reference reference;
    typedef typename base_type::const_reference const_reference;
    typedef typename base_type::pointer pointer;
    typedef typename base_type::const_pointer const_pointer;
    typedef typename base_type::size_type size_type;
    typedef typename base_type::difference_type difference_type;

    typedef pointer iterator;
    typedef const_pointer const_iterator;

    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    /**
     * @name Construction, Assignment, and Destruction
     */
    //@{
    /**
     * @brief Default constructor
     * Construct an empty dynarray.
     *
     * @param alloc   An allocator object
     */
    dynarray(allocator_type const& alloc = allocator_type{})
      : base_type{alloc}
    { }

    // Copy semantics
    /**
     * @brief Copy constructor
     * Construct a copy of the dynarray x.
     *
     * @param x   A dynarray
     */
    dynarray(dynarray const& x)
      : base_type{x}
    { std::copy(x.begin(), x.end(), begin()); }

    /**
     * @brief Copy assignment
     * Assign this object as a copy of the dynarray x.
     *
     * @param x   A dynarray
     */
    dynarray& operator=(dynarray const& x)
    { dynarray tmp{x}; swap(tmp); return *this; }

    // Move semantics
    /**
     * @brief Move constructor
     * Construct a dynarray by moving data out of the dynarray x. After
     * construction x is left in a moved-from state.
     *
     * @param x   A dynarray
     */
    dynarray(dynarray&& x)
      : base_type{std::move(x)}
    { }

    /**
     * @brief Move assignment
     * Move the state of the dynarray x into this object. After assignment
     * x is left in a moved-from state.
     *
     * @param x   A dynarray
     */
    dynarray& operator=(dynarray&& x)
    { dynarray tmp{std::move(x)}; swap(tmp); return *this; }

    /**
     * @brief Initializer list constructor
     * Construct a dynarray over an initializer list.
     *
     * @param list    An initializer list
     * @param alloc   An allocator object
     */
    dynarray(std::initializer_list<value_type> list,
             allocator_type const& alloc = allocator_type{})
      : base_type{list.size(), alloc}
    { std::copy(list.begin(), list.end(), begin()); }

    // FIXME: Implement an iterator range constructor. This is going to have
    // to be specialized for input and forward iterators. This is going to
    // require the concepts library.

    /**
     * @brief Fill constructor
     * Construct a dynarray with n elements all either default initialized
     * or initialized to the value x.
     *
     * @param n       The required size of the dynarray
     * @param x       The initial value
     * @param alloc   An allocator object
     */
    explicit dynarray(size_type n,
                      value_type const& x = value_type{},
                      allocator_type const& alloc = allocator_type{})
      : base_type{n, alloc}
    { uninitialized_fill(this->get_alloc(), begin(), end(), x); }

    ~dynarray()
    { destroy(this->get_alloc(), begin(), end()); }
    //@}

    /** @name Capacity */
    //@{
    size_type size() const
    { return base_type::size(); }

    constexpr size_type max_size() const
    { return this->get_alloc().max_size(); }

    bool empty() const
    { return base_type::size() == 0; }
    //@}

    /** @name Accessors */
    //@{
    reference operator[](size_type n)
    { return *(this->first + n); }

    const_reference operator[](size_type n) const
    { return *(this->first + n); }

    reference front()
    { return *this->first; }

    const_reference front() const
    { return *this->first; }

    reference back()
    { return *(this->last - 1); }

    const_reference back() const
    { return *(this->last - 1); }

    reference at(size_type n)
    { range_check(n); return *(this->first + n); }

    const_reference at(size_type n) const
    { range_check(n); return *(this->first + n); }

    T* data()
    { return this->first; }

    const T* data() const
    { return this->first; }
    //@}


    /** @name Equality and Ordering */
    //@{
    /**
     * @brief Equality protocol
     * Return true if this dynarray is equivalent to the other. Two dynarrays
     * are equal if they have the same size and have the same elements.
     *
     * @param x   A dynarray.
     */
    bool equal(dynarray const& x) const
    { return size() == x.size() && std::equal(begin(), end(), x.begin()); }

    /**
     * @brief Order protocol
     * Return true if this dynarray is less than the other. The order of
     * dynarrays is computed lexicographically.
     *
     * @param x   A dynarray
     */
    bool less(dynarray const& x) const
    { return std::lexicographical_compare(begin(), end(), x.begin(), x.end()); }
    //@}

    /** @name Iterators */
    //@{
    iterator begin()
    { return this->first; }

    iterator end()
    { return this->last; }

    const_iterator begin() const
    { return this->first; }

    const_iterator end() const
    { return this->last; }

    const_iterator cbegin() const
    { return this->first; }

    const_iterator cend() const
    { return this->last; }

    reverse_iterator rbegin()
    { return {end()}; }

    reverse_iterator rend()
    { return {begin()}; }

    const_reverse_iterator rbegin() const
    { return {cend()}; }

    const_reverse_iterator rend() const
    { return {cbegin()}; }

    const_reverse_iterator crbegin() const
    { return {cend()}; }

    const_reverse_iterator crend() const
    { return {cbegin()}; }
    //@}


    /** @name Swap */
    //@{
    void swap(dynarray & x)
    {
      std::swap(this->first, x.first);
      std::swap(this->last, x.last);
    }
    //@}

  private:
    // Helper function for bounds checking
    void range_check(size_type n) const
    { if(n >= size()) throw std::out_of_range("dynarray: out of range"); }
  };

  // FIXME: Do we need this if swap uses Move? I don't think so, but I'm
  // hesitant to remove it.
  template<typename T>
  void swap(dynarray<T>& a, dynarray<T>& b)
  { a.swap(b); }

} // namespace origin

#endif
