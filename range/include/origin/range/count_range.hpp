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

    count_range(Count f)
      : first_{0}, last_{f}
    { adjust(pos_type{}); }

    count_range(Count f, Count l)
      : first_{f}, last_{final(f, l, pos_type{})}
    { }

    iterator begin() const
    { return first_; }

    iterator end() const
    { return last_; }

  private:
    // Compute the actual last iterator. The final iterator is adjusted so that
    // that a running iterator will land exactly on the final ending iterator.
    // The number of iterations (per step) is guaranteed to be the same. Note
    // that these counts are adjusted for negative counters.
    //
    // If last is not reachable from first, then first as the final count value,
    // producing an empty range.
    static Count final(Count f, Count l, std::true_type)
    {
      if(f > l)
        return f;
      else
        return f + (1 + (l - 1 - f) / Step) * Step;
    }

    static Count final(Count f, Count l, std::false_type)
    {
      if(f < l)
        return f;
      else
        return f + (1 + (f - 1 - l) / -Step) * Step;
    }

    // Adjust the initialized last value according to the step and its sign.
    // This supports partially initialized ranges. For positive sign, we
    // simply compute the final value.
    void adjust(std::true_type)
    { last_ = final(first_, last_, pos_type{}); }

    // For negative sign, swap first and last (so that 0 becomes the last
    // value) and then readjust the final count value.
    void adjust(std::false_type)
    {
      std::swap(first_, last_);
      last_ = final(first_, last_, pos_type{});
    }

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
  { return {first, last}; }

  template<int Step = 1, typename Count>
  inline count_range<Count, Step> range(Count last)
  { return {last}; }
  //@}


  /**
   * @ingroup range
   *
   * The step range defines a count range with a variable, but immutable step
   * or inrement (in contrast to the count_range's integral constant step).
   */
  template<typename Count,
           typename Step = typename std::make_signed<Count>::type>
  class step_range
  {
  public:
    typedef step_counter<Count, Step> iterator;

    // FIXME: Write a single-count constructor to implictly iterate the
    // range [0, l) or [l, 0) (if Count is signed and negative). This is going
    // to require some nifty metaprogramming.

    step_range(Count f, Count l)
      : step_{step(f, l)}, first_{f}, last_{final(f, l, step_)}
    { }

    step_range(Count f, Count l, Step s)
      : step_{s}, first_{f}, last_{final(f, l, s)}
    { assert(( s != 0 )); }

    iterator begin() const
    { return {first_, step_}; }

    iterator end() const
    { return {last_, step_}; }

  private:
    // Deduce the step from the range. It's either -1 or +1. Note that if
    // f - l is zero, this is still a valid and empty range.
    Step step(Count f, Count l)
    {
      if(f < l)
        return 1;
      else
        return -1;
    }

    // Return the actual last count value, depending on the step value and
    // sign. Note that if l is not reachable from s, then return f, creating
    // an empty range.
    Count final(Count f, Count l, Step s)
    {
      if(s > 0) {
        if(f > l)
          return f;
        else
          return f + (1 + (l - 1 - f) / s) * s;
      } else {
        if(f < l)
          return f;
        else
          return f + (1 + (f - 1 - l) / -s) * s;
      }
    }

  private:
    Step step_;
    Count first_;
    Count last_;
  };

  // FIXME: If I only I could overload on constexpr, I could have the function
  // return a count_range with parameterized over the constexpr step. Alas,
  // I have no such capability.

  /**
   * @fn range(f, l, s)
   *
   * Return a stepped range over the range [f, l) with the increment s.
   *
   * @tparam Count  The underlying counted type. An integral type
   * @tparam Step   The increment type. A signed integral type
   *
   * @param first   The first value in the range
   * @param last    A value past the end of the range
   * @param step    The increment or decrement value
   *
   * @return A step range.
   */
  template<typename Count, typename Step>
  inline step_range<Count, Step> range(Count first, Count last, Step step)
  { return {first, last, step}; }


} // namespace origin

#endif
