// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ITERATOR_COUNTING_ITERATOR_HPP
#define ORIGIN_ITERATOR_COUNTING_ITERATOR_HPP

#include <iterator>

#include <origin/iterator/facades.hpp>

namespace origin
{
  // FIXME: This class needs to be reviewed. Heavily. Technically, the counter
  // could be any other iterator, although that doesn't really make a lot of
  // sense since this would basically just be the identity operator.

  // FIXME: Should a counting iterator just be called a counter?

  /**
   * A counting iterator is an iterator that, when dereferenced, simply returns
   * the current count. Note that the count could actually be any forward
   * iterator.
   *
   * @tparam Count An incrementable type.
   */
  template<typename Count>
  class counting_iterator
    : public random_access_iterator_facade<
        counting_iterator<Count>,
        Count,                                  // value type
        Count const&,                           // reference
        Count const*,                           // pointer
        typename std::make_signed<Count>::type  // difference
      >
  {
  public:
    typedef Count value_type;
    typedef value_type reference;
    typedef value_type pointer;
    typedef Count difference_type;

    // FIXME: GCC seems to have a problem accepting brace initializers on
    // the range_member. Update this when GCC is fixed.
    counting_iterator(difference_type n = 0)
      : count_{n}
    { }

    reference dereference() const
    { return count_; }

    bool equal(counting_iterator const& x) const
    { return count_ == x.count_; }

    bool less(counting_iterator const& x) const
    { return count_ < x.count_; }

    void increment()
    { ++count_; }

    void decrement()
    { --count_; }

    void advance(difference_type n)
    { count_ += n; }

    void distance(counting_iterator const& x)
    { return x.count_- count_; }

  private:
    difference_type count_;
  };

  /** Make a counting_iterator that starts at the given value. */
  template<typename Count>
  inline counting_iterator<Count>
  make_counting_iterator(Count n)
  { return counting_iterator<Count>{n}; }

} // namespace origin

#endif
