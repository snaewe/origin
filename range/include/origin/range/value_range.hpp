// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_VALUE_RANGE_HPP
#define ORIGIN_RANGE_VALUE_RANGE_HPP

#include <iterator>

#include <origin/range/support.hpp>

namespace origin
{
  /**
   * The value iterator is a Bidirectional_Iterator that wraps an iterator from
   * an Associative_Container. When dereferenced, the value_iterator returns a
   * reference to the value type stored in the association.
   */
  template<typename Iter>
  class value_iterator
    : public bidirectional_iterator_facade<
        value_iterator<Iter>,
        typename std::iterator_traits<Iter>::value_type::second_type&
      >
  {
    typedef Iter base_iterator;
  public:
    typedef typename std::iterator_traits<Iter>::value_type::second_type value_type;
    typedef value_type& reference;
    typedef value_type* pointer;
    typedef typename std::iterator_traits<Iter>::difference_type difference_type;

    value_iterator(Iter i = base_iterator{})
      : iter_{i}
    { }

    bool equal(value_iterator const& x) const
    { return iter_ == x.iter_; }

    reference dereference() const
    { return (*iter_).second; }

    void increment()
    { ++iter_; }

    void decrement()
    { --iter_; }

  private:
    base_iterator iter_;
  };

  /**
   * The value range adapts a range of key/value pairs to one that selects only
   * the keys of the underlying range.
   */
  template<typename Range>
  class value_range
  {
    typedef typename range_traits<Range>::iterator base_iterator;
  public:
    typedef value_iterator<base_iterator> iterator;

    value_range(Range& rng)
      : range_(rng)
    { }

    iterator begin() const
    {
      using std::begin;
      return begin(range_);
    }

    iterator end() const
    {
      using std::end;
      return end(range_);
    }

  private:
    Range& range_;
  };

  /**
   * Return a range of keys over the given associative range.
   */
  template<typename Range>
  value_range<Range> values(Range& rng)
  { return rng; }

  template<typename Range>
  value_range<Range const> values(Range const& rng)
  { return rng; }
}

#endif

