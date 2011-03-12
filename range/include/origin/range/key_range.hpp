// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_KEY_RANGE_HPP
#define ORIGIN_RANGE_KEY_RANGE_HPP

#include <iterator>

#include <origin/range/support.hpp>

namespace origin
{
  // NOTE: This module introduces a new concept: An Associative_Range. This
  // is a range who's value type is a key/value pair.
  // FIXME: Would it be useful to develop key/value abstractions over pairs?
  // Maybe...

  /**
   * The value iterator is a Bidirectional_Iterator that wraps an iterator from
   * an Associative_Container. When dereferenced, the value_iterator returns a
   * reference to the key type stored in the association. Note that the key 
   * type in associative containers is const.
   */
  template<typename Iter>
  class key_iterator
    : public bidirectional_iterator_facade<
        key_iterator<Iter>,
        typename std::iterator_traits<Iter>::value_type::first_type,
        typename std::iterator_traits<Iter>::value_type::first_type&,
        typename std::iterator_traits<Iter>::value_type::first_type*,
        typename std::iterator_traits<Iter>::difference_type
      >
  {
    typedef Iter base_iterator;
    typedef bidirectional_iterator_facade<
      key_iterator<Iter>,
      typename std::iterator_traits<Iter>::value_type::first_type,
      typename std::iterator_traits<Iter>::value_type::first_type&,
      typename std::iterator_traits<Iter>::value_type::first_type*,
      typename std::iterator_traits<Iter>::difference_type
    > base_type;
  public:
    typedef typename base_type::reference reference;

    key_iterator(Iter i = base_iterator{})
      : iter_{i}
    { }

    bool equal(key_iterator const& x) const
    { return iter_ == x.iter_; }

    reference dereference() const
    { return (*iter_).first; }

    void increment()
    { ++iter_; }

    void decrement()
    { --iter_; }

  private:
    base_iterator iter_;
  };


  /**
   * The key range adapts a range of key/value pairs to one that selects only
   * the keys of the underlying range.
   */
  template<typename Range>
  class key_range
  {
    typedef typename range_traits<Range>::iterator base_iterator;
  public:
    typedef key_iterator<base_iterator> iterator;

    key_range(Range& rng)
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
  key_range<Range> keys(Range& rng)
  { return rng; }

  template<typename Range>
  key_range<Range const> keys(Range const& rng)
  { return rng; }
}

#endif

