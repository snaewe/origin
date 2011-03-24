// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_COUNT_RANGE_HPP
#define ORIGIN_RANGE_COUNT_RANGE_HPP

#include <cassert>

#include <origin/iterator/counter.hpp>
#include <origin/range/support.hpp>

namespace origin
{

  // FIXME: Does it make sense to make the ranges empty if they don't have
  // the right order? For example, range(1, 0) will currently cause an
  // exception. Perhaps, I should just modify first so that the range becomes
  // range(0, 0) and doesn't execute.

  /**
   * @ingroup range
   *
   * The count range class implements a counted range that visits each
   * integral value in a half-open interval [first, last).
   *
   * @tparam Count    The underlying counted type. An Integral type
   * @tparam Step     A statically configured increment, 1 by default
   */
  template<typename Count, int Step = 1>
  class count_range
  {
    static_assert(Step != 0, "Step cannot be 0");
    typedef bool_constant<(Step > 0)> pos_type;
  public:
    typedef counter<Count, Step> iterator;

    count_range(Count f, Count l)
      : first_(initial(f, l)), last_(final(f, l))
    { }

    iterator begin() const
    { return first_; }

    iterator end() const
    { return last_; }

  private:
    // Compute the actual first iterator. If the last iterator is not reachable
    // from the first, then set the initial iterator to the final iterator,
    // creating an empty range.
    static Count initial(Count f, Count l)
    { return initial(f, l, pos_type{}); }

    static Count initial(Count f, Count l, std::true_type)
    { return f < l ? f : final(f, l); }

    static Count initial(Count f, Count l, std::false_type)
    { return f > l ? f : final(f, l); }


    // Compute the actual last iterator. The final iterator is adjusted so that
    // that a running iterator will land exactly on the final ending iterator.
    // The number of iterations (per step) is guaranteed to be the same. Note
    // that these counts are adjusted for negative counters.
    static Count final(Count f, Count l)
    { return final(f, l, pos_type{}); }

    static Count final(Count f, Count l, std::true_type)
    { return f + (1 + (l - 1 - f) / Step) * Step; }

    static Count final(Count f, Count l, std::false_type)
    { return f + (1 + (f - 1 - l) / -Step) * Step; }

    // Check the relative order of the f and l counts. The invariant depends
    // on whether Step is > 0 or not.
    static void check(Count f, Count l, std::true_type)
    { assert(( f <= l )); }

    static void check(Count f, Count l, std::false_type)
    { assert(( f >= l )); }

  private:
    Count first_;
    Count last_;
  };

  /**
   * @fn range(l)
   * @fn range(f, l)
   *
   * Return a range that traverses integer numbers. If only one parameter is
   * given, the resulting range traverse [0, l). If the step is negative, then
   * the resulting range will cover values in [l, 0). If two parameters are
   * given, the range traverses integers in [f, l).
   *
   * Note that in general, for the range [f, l), if the step > 0 then f <= l.
   * If step < 0, then f >= l.
   *
   * If n does not terminate on a multiple of step (past m), the range is
   * constructed over an adjusted value of n.
   *
   * @tparam Count    The underlying counted type. An integral type
   * @tparam Step     The counter step
   */
  //@{
  template<int Step = 1, typename Count>
  inline count_range<Count, Step> range(Count first, Count last)
  { return {first, last};
  }

  template<int Step = 1, typename Count>
  inline count_range<Count, Step> range(Count last)
  {
    // Swap the ordering for negative step.
    // FIXME: Should this delegate to the range constructor? Probably.
    if(Step > 0)
      return range<Step>(0, last);
    else
      return range<Step>(last, 0);
  }
  //@}


  /**
   * @ingroup range
   *
   * The step range...
   */
  template<typename Count,
           typename Step = typename std::make_signed<Count>::type>
  class step_range
  {
  public:
    typedef step_counter<Count, Step> iterator;

    step_range(Count f, Count l, Step s)
      : first_(initial(f, l, s)), last_(final(f, l, s)), step_(s)
    { }

    iterator begin() const
    { return first_; }

    iterator end() const
    { return last_; }

  private:
    Count initial(Count f, Count l, Step s)
    {
      assert(( s != 0 ));
      if(s > 0)
        return f < l ? f : final(f, l, s);
      else
        return f > l ? f : final(f, l, s);
    }

    Count final(Count f, Count l, Step s)
    {
      assert(( s != 0 ));
      if(s > 0)
        return f + (1 + (l - 1 - f) / s) * s;
      else
        return f + (1 + (f - 1 - l) / -s) * s;
    }

  private:
    Count first_;
    Count last_;
    Step step_;
  };

  /**
   * @fn range(f, l, s)
   */
  template<typename Count, typename Step>
  inline step_range<Count, Step> range(Count first, Count last, Step step)
  { return {first, last, step}; }


} // namespace origin

#endif
