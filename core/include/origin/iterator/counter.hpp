// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ITERATOR_COUNTER_HPP
#define ORIGIN_ITERATOR_COUNTER_HPP

#include <origin/iterator/facade.hpp>

namespace origin
{
  template <typename I, typename IncA>
    class counter;


  // An alias to the facade used by the counter class. Note that the reference 
  // type is the same as the underlying iterator type.
  template <typename I, typename Step>
    using Counter_facade = 
      iterator_facade<counter<I, Step>, I, I>;


  // Counter
  //
  // The counter adaptor transforms an incrementable type into an iterator
  // that dereferences to the current value of that type. Incrementing is
  // controlled by a function that returns an step or step value.
  //
  // The counter is parameterized over its underlying "counter" type and an
  // action that increments counter. The default action is to simply step
  // the counter. Note that counter increments are unchecked. If the action
  // is to advance by some number > 1, then it is possible to overrun the
  // limit of the range.
  //
  // A comparison (e.g., == or <) or operation (e.g., -) on counters is valid
  // only when those counters have the same actions. If the counters have
  // different actions, the computation results in undefined behavior.
  template <typename I, typename Step = one_type<Difference_type<I>>>
    class counter : public Counter_facade<I, Step>
    {
      static_assert(Weakly_incrementable<I>(), "");
      static_assert(Function<Step>(), "");
    public:
      using Base = Counter_facade<I, Step>;

      counter(I i = {}, Step step = {})
        : data {i, step}
      { }
      
      // Returns the underlying incrementable object. 
      I base() const { return iter(); }
      
      // Returns the advance action.
      Step step_func() const { return inc(); }

      // Returns the (current) step amount.
      constexpr Difference_type<I> step() const { return inc()(); }

      // Readable
      // Dereferencing a counter yields an object type I rather than a refernce
      // to the stored value. The reason for this is that subscript operator
      // cannot return a reference. Because the result types are expected to
      // be the same, 
      I operator*() const { return iter(); }
      using Base::operator[];

      // Step
      counter& operator++();
      using Base::operator++;
      
      // Decrement
      counter& operator--();
      using Base::operator--;

      // Advance
      counter& operator+=(Difference_type<I> n);
      counter& operator-=(Difference_type<I> n);

    private:
      I&       iter()       { return std::get<0>(data); }
      const I& iter() const { return std::get<0>(data); }
      
      const Step& inc() const { return std::get<1>(data); }
    
    private:
      std::tuple<I, Step> data;
    };


  template <typename I, typename Step>
    inline auto counter<I, Step>::operator++() -> counter&
    {
      advance(iter(), step()); 
      return *this;
    }

  template <typename I, typename Step>
    inline auto counter<I, Step>::operator--() -> counter&
    {
      advance(iter(), -step()); 
      return *this;
    }


  template <typename I, typename Step>
    inline auto counter<I, Step>::operator+=(Difference_type<I> n) -> counter&
    { 
      iter() += n * step(); 
      return *this; 
    }
      
  template <typename I, typename Step>
    inline auto counter<I, Step>::operator-=(Difference_type<I> n) -> counter&
    { 
      iter() -= n * step(); 
      return *this; 
    }


  // Equality_comparable
  //
  // FIXME: If A is stateful, and a.advance_act() != b.advance_act(), then
  // these cannot compare equal. Unfortunately, function objects are rarely
  // equality comparable How does this gel with the definition of ordering?
  template <typename I, typename A>
    bool operator==(const counter<I, A>& a, const counter<I, A>& b) 
    { 
      return *a == *b;
    }
  
  template <typename I, typename A>
    bool operator!=(const counter<I, A>& a, const counter<I, A>& b)
    {
      return !(a == b);
    }


  // Totally_ordered
  template <typename I, typename A>
    inline bool operator<(const counter<I, A>& a, const counter<I, A>& b)
    {
      return a.base() < b.base();
    }

  template <typename I, typename A>
    inline bool operator>(const counter<I, A>& a, const counter<I, A>& b)
    {
      return b < a;
    }

  template <typename I, typename A>
    inline bool operator<=(const counter<I, A>& a, const counter<I, A>& b)
    {
      return !(b < a);
    }

  template <typename I, typename A>
    inline bool operator>=(const counter<I, A>& a, const counter<I, A>& b)
    {
      return !(a < b);
    }


  // Difference
  template <typename I, typename A>
    inline Difference_type<I> operator-(const counter<I, A>& a, const counter<I, A>& b)
    {
      return (*a - *b) / a.step();
    }

      

  // Step counter
  //
  // A step counter is a counter whose step is given by a constant (but 
  // non-constexpr) integer value.
  template <typename I>
    using step_counter = counter<I, constant_function<Difference_type<I>>>;



  // Make counter
  //
  // Return a counter over the incrementable type. There are several overloads
  // of this function.

  // Returns a counter over i that is incremented by the given step function.
  // If not step function is given, it defaults to a single-value increment.
  template <typename I, typename Step = one_type<Difference_type<I>>>
    auto make_counter(const I& i, Step step = {})
      -> Requires<Function<Step>(), counter<I, Step>>
    {
      return {i, step};
    }

  // Returns a counter over i that is incremented by the given step.
  template <typename I>
    step_counter<I> make_counter(const I& i, Difference_type<I> n)
    {
      using F = constant_function<Difference_type<I>>;
      return {i, F{n}};
    }

} // namespace origin

#endif
