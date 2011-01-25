// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_REVERSE_RANGE_HPP
#define ORIGIN_RANGE_REVERSE_RANGE_HPP

#include <iterator>

#include <origin/range/support.hpp>

namespace origin
{
  /**
   * @ingroup range
   * A reverse range reverses the order of iteration. The underlying range is
   * required to support bidirectional iterators.
   *
   * @tparam Range The Range being reversed
   */
  template<typename Range>
  class reverse_range
  {
    typedef typename range_traits<Range>::iterator base_iterator;
  public:
    typedef std::reverse_iterator<base_iterator> iterator;

    // FIXME: GCC seems to have a problem accepting brace initializers on
    // the range_member. Update this when GCC is fixed.
    reverse_range(Range& rng)
      : range_(rng)
    { }

    iterator begin() const
    { return iterator{std::end(range_)}; }

    iterator end() const
    { return iterator{std::begin(range_)}; }

  private:
    Range& range_;
  };

  /**
   * Return a reversed range.
   *
   * @tparam Range The type of the underlying Range.
   *
   * @param rng The range being reversed.
   *
   * @note The function is not called "reverse" because that would collide
   * with the std::reverse algorithm.
   */
  template<typename Range>
  inline reverse_range<Range> rev(Range& rng)
  { return rng; }

  // A const version of the function above.
  template<typename Range>
  inline reverse_range<Range const> rev(Range const& rng)
  { return rng; }

} // namespace origin

#endif
