// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_DYNARRAY_DYNARRAY_HPP
#define ORIGIN_DYNARRAY_DYNARRAY_HPP

#include <memory>

#include <origin/exception.hpp>

namespace origin 
{

  /**
   * The dynarray (dynamic array) class implements a dynamically allocated
   * array.
   *
   * The dynarray defined in n2648. Not all of the functionality is implemented,
   * but the interface is similar to that defined in n2648 by Lawrence Crowl and
   * Matt Austern. We have added swap semantics and const reverse iterators.
   *
   * http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2648.html
   */
  template<typename T, typename Allocator = std::allocator<T>>
  class dynarray 
  {
  public:
    typedef Allocator allocator_type;
    typedef T value_type;
    typedef typename allocator_type::reference reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::pointer pointer;
    typedef typename allocator_type::const_pointer const_pointer;
    typedef typename allocator_type::size_type size_type;
    typedef typename allocator_type::difference_type difference_type;

    typedef pointer iterator;
    typedef const_pointer const_iterator;

    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    /** 
     * @name Construction, Assignment, and Destruction
     */
    //@{
    dynarray(allocator_type const& alloc = allocator_type{})
      : data_{allocate(1)}, size_{1}, alloc_{alloc}
    { }

    // Copy semantics.
    dynarray(dynarray const& x, allocator_type const& alloc = allocator_type{})
      : data_{}, size_{}, alloc_{alloc}
    { std::copy(x.begin(), x.end(), begin()); }

    dynarray& operator=(dynarray const& x)
    { dynarray tmp{x}; swap(tmp); return *this; }

    // Move semantics
    dynarray(dynarray&& x, allocator_type const& alloc = allocator_type{})
      : data_{}, size_{}, alloc_{alloc}
    { swap(data_, x.data_); }
    
    dynarray& operator=(dynarray&& x)
    { swap(data_, x.data_); return *this; }


    // FIXME: Implement a range-based constructor and assignment.

    // FIXME: Implemnt an initializer-list constructor and assignment

    // FIXME: Implement a corresponding assign() function.
    explicit dynarray(size_type n, allocator_type const& alloc = allocator_type{})
      : data_{allocate(n)}, size_{n}
    { }

    ~dynarray()
    { delete [] data_; }
    //@}
    
    /**
     * @name Equality and Ordering
     */
    //@{
    bool equal(dynarray const& x) const
    { return std::equal(begin(), end(), x.begin()); }
    
    bool less(dynarray const& x) const
    { return std::lexicographical_compare(begin(), end(), x.begin(), x.end()); }
    //@}

    /**
     * @name Iterators 
     */
    //@{
    iterator begin()
    { return data_; }

    iterator end()
    { return data_ + size_; }


    const_iterator begin() const
    { return data_; }

    const_iterator end() const
    { return data_ + size_; }


    const_iterator cbegin() const
    { return data_; }

    const_iterator cend() const
    { return data_ + size_; }


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

    /** @name Capacity */
    //@{
    size_type size() const 
    { return size_; }

    constexpr size_type max_size() const 
    { return alloc_.max_size(); }

    bool empty() const 
    { return data_; }
    //@}

    /** @name Accessors */
    //@{
    reference operator[](size_type n) 
    { return data_[n]; }

    const_reference operator[](size_type n) const 
    { return data_[n]; } 
    
    reference front() 
    { return *data_; }

    const_reference front() const 
    { return *data_; }

    reference back() 
    { return data_[size_ - 1]; }
        
    const_reference back() const 
    { return data_[size_ - 1]; }

    reference at(size_type n) 
    { range_check(n); return data_[n]; }

    const_reference at(size_type n) const 
    { range_check(n); return data_[n]; }

    T* data() 
    { return data_; }

    const T* data() const 
    { return data_; }
    //@}

    /** @name Swap */
    //@{
    void swap(dynarray & x) {
      std::swap(size_, x.size_);
      std::swap(data_, x.data_);
    }
    //@}

  private:
    // Helper function for bounds checking
    void range_check(size_type n) const
    { if(n >= size_) throw std::out_of_range("dynarray: out of range"); }

    // Helper function to allocate data
    // FIXME: n2648 uses new char[n * sizeof(T)]
    pointer allocate(size_type n)
    { return new T[n]; }

  private:
    // FIXME: Use two pointers.
    pointer data_;
    size_type size_;
    
    // FIXME: Optimize storage costs using EBO.
    allocator_type alloc_;
  };

  template<typename T>
  void swap(dynarray<T>& a, dynarray<T>& b) 
  { a.swap(b); }

} // namespace origin

#endif
