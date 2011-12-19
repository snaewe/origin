// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ITERATOR_COUNTING_ITERATOR_HPP
#define ORIGIN_ITERATOR_COUNTING_ITERATOR_HPP

#include <iterator>

namespace origin
{
  // FIXME: Is int really the best type for the count? It's certainly the
  // easiest. I can actually write it so that the type of the ICE parameter
  // is derived from count (as make_signed<Count>::type, that actually works),
  // but I can't easily do the same for the accessor function, which is really
  // too bad. I'm not so sure that it's really worth the effort.

  // A counter is an integral value masquerading as a random access iterator.
  // When dereferenced, counters simply return the current value.
  //
  // The counter is parameterized over its underlying count type and a
  // statically defined step value, which defaults to 1. If the step is
  // negative, then the counter will decrement.
  //
  // Note that if the step is not 1 or -1, care must be taken with comparisons
  // against iterators. Use < or > for comparisons in those cases.
  template<typename Count, int Step = 1>
    class counter 
    {
      static_assert(Integral<Count>(), "");
      static_assert(Step != 0, "");
    public:
      using value_type = Count;
      using reference = const Count&;
      using pointer = const Count*;
      using Make_signed

      counter(value_type n = 0)
        : count_{n}
      { }

      // Return the step increment step.
      static constexpr difference_type step()
      { return Step; }

      reference dereference() const
      { return count_; }

      bool equal(counter const& x) const
      { return count_ == x.count_; }

      bool less(counter const& x) const
      { return count_ < x.count_; }

      void increment()
      { count_ += Step; }

      void decrement()
      { count_ -= Step; }

      void advance(difference_type n)
      { count_ += n * Step; }

      difference_type distance(counter const& x)
      {
        return distance(count_, x.count_, bool_constant<(Step > 0)>{});
      }

    private:
      static difference_type distance(value_type x, value_type y, std::true_type)
      { return (x - y) / Step; }

      static difference_type distance(value_type x, value_type y, std::false_type)
      { return (y - x) / -Step; }

    private:
      value_type count_;
    };

  /**
   * Create a counter that starts from the given value n.
   *
   * A constant step can be specified by explicitly instantiating the template
   * over an integral value. For example `make_counter<-1>(10)` produces a
   * counter that counts backwards starting at 10.
   *
   * @tparam Count    The underlying counted value. An Integral type
   * @tparam Step     The step for counter
   *
   * @param n         The starting value of the counter
   */
  template<int Step = 1, typename Count>
  inline counter<Count, Step> make_counter(Count n)
  { return counter<Count, Step>{n}; }


  // FIXME: We should be able to optimize the call to difference by making
  // the step type either pos<T> or neg<T>. Then we can just specialize the
  // algorithm to avoid the branch. Or, we could just tell users to write
  // pos/neg to guarantee the optimal performance. Of course, this requires
  // that we actually write the pos/neg qualifier classes.

  // NOTE: The step doesn't _really_ need to be signed, only if it's negative.
  // In other words, its possible to write a count with an unsigned step and
  // then give it a negative value.

  /**
   * @ingroup iter
   *
   * A step counter is a counter with a dynamically specified step. A negative
   * step value results in a counter that counts downwards.
   *
   * @see counter
   *
   * @tparam Count  An integral type
   * @tparam Step   A signed integral type
   */
  template<typename Count,
           typename Step = typename std::make_signed<Count>::type>
  class step_counter
    : public random_access_iterator_facade<
        step_counter<Count, Step>,
        Count,          // value type
        Count const&,   // reference
        Count const*,   // pointer
        Step            // difference
      >
  {
    // FIXME: Is it an invariant that Step is the signed version of Count or
    // simply that Count's difference type can be converted to Count? I don't
    // know...

    typedef random_access_iterator_facade<
      step_counter<Count, Step>, Count, Count const&, Count const*, Step
    > base_type;
  public:
    typedef typename base_type::value_type value_type;
    typedef typename base_type::reference reference;
    typedef typename base_type::difference_type difference_type;

    step_counter(value_type n, difference_type s = 1)
      : count_(n), step_(s)
    { }

    // Return the step increment step.
    difference_type step() const
    { return step_; }

    reference dereference() const
    { return count_; }

    bool equal(step_counter const& x) const
    { return count_ == x.count_; }

    bool less(step_counter const& x) const
    { return count_ < x.count_; }

    void increment()
    { count_ += step_; }

    void decrement()
    { count_ -= step_; }

    void advance(difference_type n)
    { count_ += n * step_; }

    difference_type distance(step_counter const& x)
    {
      if(step_ > 0) {
        return (count_ - x.count_) / step_;
      } else {
        return (x.count_ - count_) / -step_;
      }
    }

  private:
    value_type count_;
    difference_type step_;
  };

  /**
   * @fn make_step_counter(n)
   * @fn make_step_counter(n, s)
   *
   * Return a step counter starting at the value n. A step may optionally be
   * specified.
   *
   * @tparam Count    The underlying counted type. An Integral type.
   * @tparam Step     The step increment type., A signed Integral type.
   */
  //@{
  template<typename Count>
  inline step_counter<Count> make_step_counter(Count n)
  { return {n}; }

  template<typename Count, typename Step>
  inline step_counter<Count, Step> make_step_counter(Count n, Step s)
  { return {n, s}; }
  //@}

} // namespace origin

#endif
