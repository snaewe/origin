// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_TRANSFORM_RANGE_HPP
#define ORIGIN_RANGE_TRANSFORM_RANGE_HPP

#include <origin/iterator/transform_iterator.hpp>
#include <origin/range/support.hpp>

namespace origin
{
  /**
   * @ingroup range
   * The transform range adaptor applies a transformation function to each
   * element of the underlying range.
   *
   * @tparam Range The Range being transformed.
   * @tparam Transform A Transform function.
   */
  template<typename Range, typename Transform>
  class transform_range
  {
    typedef typename range_traits<Range>::iterator base_iterator;
  public:
    typedef transform_iterator<base_iterator, Transform> iterator;

    // FIXME: GCC seems to have a problem accepting brace initializers on
    // the range_member. Update this when GCC is fixed.
    transform_range(Range& rng, Transform func = Transform{})
      : range_(rng), func_{func}
    { }

    iterator begin() const
    {
      using std::begin;
      return {begin(range_), func_};
    }

    iterator end() const
    {
      using std::end;
      return {end(range_), func_};
    }

  private:
    Range& range_;
    Transform func_;
  };

  // NOTE: We don't call this function 'transform' because of the std algorithm
  // by the same name. The reason that this is called 'xform' is that the
  // (eventual) range-operable version of the transform has the exact same
  // signature as the 'xform' function below. Where the transform algorithm
  // is applied eagerly, this is applied lazily.

  /** Return a transform range over the given range. */
  template<typename Range, typename Transform>
  inline transform_range<Range, Transform>
  xform(Range& rng, Transform func)
  { return {rng, func}; }

  template<typename Range, typename Transform>
  inline transform_range<Range const, Transform>
  xform(Range const& rng, Transform func)
  { return {rng, func}; }

} // namespace origin

#endif
