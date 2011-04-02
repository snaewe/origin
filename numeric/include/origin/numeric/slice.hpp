// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_NUMERIC_SLICE_HPP
#define ORIGIN_NUMERIC_SLICE_HPP

#include <valarray>

#include <origin/iterator/facades.hpp>

namespace origin
{
  // FIXME: Why does the slice_array NOT provide member access. It does
  // everything else! If I want to preserve optimal behavior using the row
  // accessors, I have to make the slice reference behave excatly like the
  // slice array. I'll probably get to do the same with gslice.
  //
  // The problem may be more systemic.

  /**
   * The slice reference encapsualtes a reference to a slice of a valarray
   * and provides access operators for elements of the slice. Otherwise, this
   * class is similar to std::slice_array.
   *
   * The implementation is motivated by the  Slice_iter class in The C++
   * Programming Language (3rd edition) by Bjarne Stroustrup.
   *
   * A constant slice iterator can be instantiated if the template parameter T
   * is const.
   *
   * @tparam T  A type
   */
  template<typename T>
  class slice_reference
  {
    typedef std::valarray<T> array_type;
    typedef std::size_t size_type;
  public:
    typedef T value_type;
    typedef T& reference;
    typedef T const& const_reference;

    /**
     * @brief Default constructor
     * Construct an uninitialized slice reference. The refernce is invalid
     * after construction.
     */
    slice_reference()
      : array_{nullptr}, slice_{}
    { }

    /**
     * @brief Slice constructor
     * Construct a slice iterator over the given valarray and slice.
     *
     * @param a   A valarray
     * @param s   A slice
     */
    slice_reference(array_type& a, std::slice s)
      : array_{&a}, slice_{s}
    { }

    slice_reference(array_type const& a, std::slice s)
      : array_{const_cast<array_type*>(&a)}, slice_{s}
    { }

    /** @name Protocols */
    //@{
    // FIXME: These probably aren't correct.
    /**
     * @brief Equality comparison
     * Return true if this slice reference is equal to the other.
     */
    bool equal(slice_reference const& x) const
    {
      return slice_.stride() == x.slice_.stride()
          && slice_.start() == x.slice_.start();
    }

    /**
     * @brief Less than comparison
     * Return true if this slice reference is less than the other.
     */
    bool less(slice_reference const& x) const
    {
      return slice_.start() < x.slice_.start()
          && slice_.stride() == x.slice_.stride();
    }
    //@}

    /** @name Element Access */
    //@{
    reference operator[](size_type n)
    { return (*array_)[slice_.start() + n * slice_.stride()]; }

    const reference operator[](size_type n) const
    { return (*array_)[slice_.start() + n * slice_.stride()]; }
    //@}

  private:
    std::valarray<value_type>* array_;
    std::slice slice_;
  };
}

#endif
