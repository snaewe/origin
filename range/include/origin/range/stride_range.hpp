// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_STRIDE_RANGE_HPP
#define ORIGIN_RANGE_STRIDE_RANGE_HPP

#include <cassert>
#include <iterator>

#include <origin/range/support.hpp>

namespace origin
{
  /** 
   * @internal
   * The stride iterator performs multiple increments in a single step. Note
   * that you can only compare and subtract two stride iterators with the same
   * stride value.
   *
   * @tparam Range  The Range into which the iterator refers
   * @tparam Iter   The underlying Iterator type
   */
  template<typename Range, typename Iter>
  class stride_range_iterator_
    : public random_access_iterator_facade<
        stride_range_iterator_<Range, Iter>,
        typename std::iterator_traits<Iter>::value_type,
        typename std::iterator_traits<Iter>::reference,
        typename std::iterator_traits<Iter>::pointer,
        typename std::iterator_traits<Iter>::difference_type,
        typename std::iterator_traits<Iter>::iterator_category
      >
  {
    typedef Range base_range;
    typedef Iter base_iterator;
    typedef random_access_iterator_facade<
      stride_range_iterator_<Range, Iter>,
      typename std::iterator_traits<Iter>::value_type,
      typename std::iterator_traits<Iter>::reference,
      typename std::iterator_traits<Iter>::pointer,
      typename std::iterator_traits<Iter>::difference_type
    > base_type;
  public:
    typedef typename base_type::reference reference;
    typedef typename base_type::difference_type difference_type;
    typedef typename base_type::iterator_category iterator_category;

    // FIXME: GCC seems to have a problem accepting brace initializers on
    // the range_ member. Perhapns because its a reference type? Update this 
    // when GCC is fixed.
    stride_range_iterator_(base_range& r,
                           base_iterator i = base_iterator{},
                           difference_type n = 1)
      : range_(r), iter_{i}, stride_{n}
    { }

    reference dereference() const
    { return *iter_; }

    bool equal(stride_range_iterator_ const& x) const
    {
      assert(( stride_ == x.stride_ ));
      return iter_ == x.iter_;
    }

    void increment()
    { 
      std::cout << typestr<iterator_category>() << "\n";
      assert(false);
      // next(stride_, iterator_category{}); 
    }

    void decrement()
    { prev(stride_, iterator_category{}); }

    void advance(difference_type n)
    { advance(n * stride_, iterator_category{}); }

    difference_type distance(stride_range_iterator_ const& x) const
    {
      assert(( stride_ == x.stride_ ));
      return (x.iter_ - iter_) / stride_;
    }

  private:
    // Specialized increment, decrement and advance operators ensure that we
    // don't "overstep" the bounds of the parent range. We could get a way with
    // a single advance operator, but that would require an extra conditional
    // to determine directionality.
    //
    // NOTE: that these are kind of like "safe" next, prev, and advance
    // functions. I wonder if there isn't some value in making these top-level
    // algorithms for the range library (e.g., next(i, rng)). Note that this
    // also speaks to the concept of "safe" iterators that explicitly refer
    // to their parent container.
    void next(difference_type n, std::input_iterator_tag)
    {
      for(difference_type i = n; iter_ != end(range_) && i != 0; --i)
        ++iter_;
    }

    void next(difference_type n, std::random_access_iterator_tag)
    {
      if(iter_ == end(range_))
        return;
      else if(end(range_) - iter_ < n)
        iter_ = end(range_);
      else
        iter_ += n;
    }

    void prev(difference_type n, std::input_iterator_tag)
    {
      for(difference_type i = stride_; iter_ != end(range_) && i < stride_; ++i)
        --iter_;
    }

    void prev(difference_type n, std::random_access_iterator_tag)
    {
      if(iter_ == begin(range_))
        return;
      else if(iter_ - begin(range_) < n)
        iter_ = begin(range_);
      else
        iter_ -= n;
    }

    template<typename Tag>
    void advance(difference_type n, Tag t)
    {
      if(n > 0) increment(n, t);
      else if(n < 0) decrement(n, t);
    }

  private:
    base_range& range_;
    base_iterator iter_;
    difference_type stride_;
  };

  /**
   * @ingroup range
   * A stride range iterates over an underlying range in "strides", such that
   * a single increment of the stride range iterator is implemented as a fixed
   * number of increments of on the underlying range's iterator.
   *
   * @tparam Range The Range being strided over.
   */
  template<typename Range>
  class stride_range
  {
    typedef typename range_traits<Range>::iterator base_iterator;
    typedef typename range_traits<Range>::difference_type difference_type;
  public:
    typedef stride_range_iterator_<Range, base_iterator> iterator;

    // FIXME: GCC seems to have a problem accepting brace initializers on
    // the range_member. Update this when GCC is fixed.
    stride_range(Range& rng, difference_type n)
      : range_(rng), stride_{n}
    { }

    iterator begin() const
    {
      using std::begin;
      return {range_, begin(range_), stride_};
    }

    iterator end() const
    {
      using std::end;
      return {range_, end(range_), stride_};
    }

  private:
    Range& range_;
    difference_type stride_;
  };

  /**
   * Return a strided range over the given range.
   *
   * @tparam Range The type of the underlying Range.
   * @tparam Stride An intregral type, must be interoperable with the
   *   difference type of the Range's iterator.
   *
   * @param rng The underlying range being strided over.
   * @param n The stride value
   */
  template<typename Range, typename Stride>
  inline stride_range<Range>
  stride(Range& rng, Stride n)
  { return {rng, n}; }

  // A const version of the function above.
  template<typename Range, typename Stride>
  inline stride_range<Range const>
  stride(Range const& rng, Stride n)
  { return {rng, n}; }

} // namespace origin

#endif
