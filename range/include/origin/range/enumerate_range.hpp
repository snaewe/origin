// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_ENUMERATE_RANGE_HPP
#define ORIGIN_RANGE_ENUMERATE_RANGE_HPP

#include <utility>

#include <origin/iterator/enumerator.hpp>
#include <origin/range/support.hpp>

namespace origin
{
  /**
   * @ingroup range
   * The enumerated range "enumerates" the elements of a sequence, pairing
   * each dereferenced element with an integral offset (starting at 0). The
   * count type (integral) is the difference type of the Range's underlying
   * iterators.
   *
   * @tparam Range The underlying Range type
   * @tparam Distance The Integral type used for counting. This defaults to
   *    the difference type of the range.
   */
  template<typename Range,
           typename Distance = typename range_traits<Range>::difference_type>
  class enumerate_range
  {
    typedef typename range_traits<Range>::iterator base_iterator;
    typedef Distance difference_type;
  public:
    typedef enumerator<base_iterator, difference_type> iterator;

    // FIXME: GCC seems to have a problem accepting brace initializers on
    // the range_member. Update this when GCC is fixed.
    enumerate_range(Range& rng, difference_type n)
      : range_(rng), count_{n}
    { }

    iterator begin() const
    {
      using std::begin;
      return {begin(range_), count_};
    }

    iterator end() const
    {
      using std::end;
      return end(range_);
    }

  private:
    Range& range_;
    difference_type count_;
  };

  // FIXME: When make_enumerator is renamed, this is going to have to be
  // conditionally enabled on is_range.

  /**
   * @fn enumerate(rng, n)
   * @fn enumerate(rng)
   *
   * Construct an enumerate range over the underlying Range rng. A starting
   * value for the enumeration can also be provided.
   *
   * @tparam Range      The enumerated Range type
   * @tparam Distance   An Integral value, defaults to the range's difference
   *                    type
   *
   * @param rng   The range being enumerated
   * @param n     The starting value for the enumeration.
   */
  //@{
  template<typename Range,
           typename Distance = typename range_traits<Range>::difference_type>
  inline enumerate_range<Range, Distance>
  enumerate(Range& rng, Distance n = 0)
  { return {rng, n}; }

  template<typename Range,
           typename Distance = typename range_traits<Range>::difference_type>
  inline enumerate_range<Range const, Distance>
  enumerate(Range const& rng, Distance n = 0)
  { return {rng, n}; }
  //@}

} // namespace origin

#endif
