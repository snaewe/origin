// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_FILTER_RANGE_HPP
#define ORIGIN_RANGE_FILTER_RANGE_HPP

#include <iterator>

#include <origin/range/support.hpp>

namespace origin
{
  /** @internal
   * The filter range iterator implements the filter iterator for filter
   * ranges. Note that this class differs a bit from the typical filter
   * iterators, which are not typically linked to their "parent" range. This
   * also allows the filtered range iterator to traverse backwards in a safe
   * manner, which is not typical of other filter iterator types.
   *
   * @todo Consider implementing an actual filter iterator in the Boost style.
   */
  template<typename Range, typename Iter, typename Pred>
  class filter_range_iterator_
    : public bidirectional_iterator_facade<
        filter_range_iterator_<Range, Iter, Pred>,
        typename std::iterator_traits<Iter>::value_type,
        typename std::iterator_traits<Iter>::reference,
        typename std::iterator_traits<Iter>::pointer,
        typename std::iterator_traits<Iter>::difference_type
      >
  {
    typedef Range base_range;
    typedef Iter base_iterator;
  public:
    typedef typename std::iterator_traits<Iter>::value_type value_type;
    typedef typename std::iterator_traits<Iter>::reference reference;
    typedef typename std::iterator_traits<Iter>::pointer pointer;
    typedef typename std::iterator_traits<Iter>::difference_type difference_type;

    // FIXME: GCC seems to have a problem accepting brace initializers on
    // the range_member. Update this when GCC is fixed.
    filter_range_iterator_(base_range& r,
                           base_iterator i = base_iterator{},
                           Pred p = Pred{})
      : range_(r), iter_{i}, pred_{p}
    {
      // Make sure that we actually start on a valid position.
      while(invalid()) {
        ++iter_;
      }
    }

    reference dereference() const
    { return *iter_; }

    bool equal(filter_range_iterator_ const& x) const
    { return iter_ == x.iter_; }

    void increment()
    {
      do {
        ++iter_;
      } while(invalid());
    }

    void decrement()
    {
      if(iter_ == begin(range_)) return;
      while(!pred(*--iter_)) ;
    }

  private:
    bool invalid() const
    { return iter_ != end(range_) && !pred_(*iter_); }

  private:
    base_range& range_;
    base_iterator iter_;

    // FIXME: Use the empty base optimization to reduce the memory requirements
    // of the iterator type.
    Pred pred_;
  };

  /**
   * @ingroup range
   * The filter range adaptor filters the elements of an underlying range such
   * that the elements dereferenced are only those for which the given
   * predicate returns true.
   *
   * @tparam Range The Range being filtered.
   * @tparam Pred A Predicate function.
   */
  template<typename Range, typename Pred>
  class filter_range
  {
    typedef typename range_traits<Range>::iterator base_iterator;
  public:
    typedef filter_range_iterator_<Range, base_iterator, Pred> iterator;

    // FIXME: GCC seems to have a problem accepting brace initializers on
    // the range_member. Update this when GCC is fixed.
    filter_range(Range& rng, Pred pred = Pred{})
      : range_(rng), pred_{pred}
    { }

    iterator begin() const
    {
      using std::begin;
      return {range_, begin(range_), pred_};
    }

    iterator end() const
    {
      using std::end;
      return {range_, end(range_), pred_};
    }

  private:
    Range& range_;
    Pred pred_;
  };

  /** Return a filtered range over the given range. */
  template<typename Range, typename Pred>
  inline filter_range<Range, Pred>
  filter(Range& rng, Pred pred)
  { return {rng, pred}; }

  template<typename Range, typename Pred>
  inline filter_range<Range const, Pred>
  filter(Range const& rng, Pred pred)
  { return {rng, pred}; }

} // namespace origin

#endif
