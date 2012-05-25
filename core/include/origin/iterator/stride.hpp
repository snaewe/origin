// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas S&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ITERATOR_STRIDE_HPP
#define ORIGIN_ITERATOR_STRIDE_HPP

#include <origin/iterator/facade.hpp>

namespace origin
{
  template <typename I, typename S>
    class stride_iterator;


  // An alias for the facade used by the stride iterator.
  template <typename I, typename S>
    using Stride_iterator_facade = iterator_facade<stride_iterator<I, S>, I>;


  // Stride iterator
  //
  // The stride iterator adaptor wraps an underlying iterator in such a way
  // that advancing a single stride (or step) will cause the underlying iterator
  // to advance a specified number of steps. 
  //
  // The step function for a stride iterator must return a value greater than 0.
  // S step of 0 will not result in bounded ranges. Also note that that
  // reversing a stride iterator can be achieved using a reverse iterator
  // adaptor.
  //
  // Note: The bidirectional iterator operations are unchecked, meaning it
  // is possible to access elements that are before the first element of
  // the initial iterator value. Ensuring that these operations are checked
  // would require the addition of an extra iterator (the first iterator) and
  // checking those operations.
  //
  // Invariants:
  //    this->step() > 0
  template<typename I, typename Step = constant_function<Difference_type<I>>>
    class stride_iterator : public Stride_iterator_facade<I, Step>
    {
      static_assert(Iterator<I>(), "");
      static_assert(Function<Step>(), "");
    public:
      using Base = Stride_iterator_facade<I, Step>;

      stride_iterator(I first, I last, Step step = {})
        : data{first, last, step}
      { 
        assert(step() > 0);
      }

      stride_iterator(I last, Step step = {})
        : stride_iterator(last, last, step)
      { }


      // Returns the underlying iterator
      I base() const { return iter(); }
      
      // Returns the advance action.
      Step step_func() const { return inc(); }

      // Returns the step or stride for the iterator.
      constexpr Difference_type<I> step() const { return inc()(); }
      
      
      // Readable
      Iterator_reference<I> operator*() const { return *iter(); }
      using Base::operator[];

      // Incrementable
      stride_iterator& operator++();
      using Base::operator++;
      
      // Decrementable
      stride_iterator& operator--();
      using Base::operator--;

      // Advance
      stride_iterator& operator+=(Difference_type<I> n);
      stride_iterator& operator-=(Difference_type<I> n);

    private:
      I&       iter()       { return std::get<0>(data); }
      const I& iter() const { return std::get<0>(data); }
      
      I&       last()       { return std::get<1>(data); }
      const I& last() const { return std::get<1>(data); }

      const Step& inc() const { return std::get<2>(data); }
      
    private:
      std::tuple<I, I, Step> data;
    };


  template <typename I, typename S>
    inline auto stride_iterator<I, S>::operator++() -> stride_iterator&
    { 
      iter() = bounded_next(iter(), step(), last());
      return *this; 
    }

  template <typename I, typename S>
    inline auto stride_iterator<I, S>::operator--() -> stride_iterator&
    { 
      iter() = prev(iter(), step());
      return *this; 
    }

  template <typename I, typename S>
    inline auto stride_iterator<I, S>::operator+=(Difference_type<I> n)
      -> stride_iterator&
    { 
      iter() = bounded_next(iter(), step() * n, last());
      return *this;
    }

  template <typename I, typename S>
    inline auto stride_iterator<I, S>::operator-=(Difference_type<I> n) 
      -> stride_iterator&
    { 
      iter() *= prev(iter(), step * n);
      return *this; 
    }


  // Equality_comparable
  //
  // Two stride iterators are equal if they refer to the same object and have
  // the same step function.
  template <typename I, typename S>
    inline bool operator==(const stride_iterator<I, S>& a, const stride_iterator<I, S>& b)
    { 
      return a.base() == b.base();
    }

  template <typename I, typename S>
    inline bool operator!=(const stride_iterator<I, S>& a, const stride_iterator<I, S>& b)
    { 
      return !(a == b);
    }
    

  // Totally_ordered
  //
  // The ordering of stride iterators is defined by the ordering of the 
  // underlying iterator. The order is undefined if the iterators have different
  // step functions.
  template <typename I, typename S>
    inline bool operator<(const stride_iterator<I, S>& a, const stride_iterator<I, S>& b)
    { 
      return a.base() < b.base();
    }

  template <typename I, typename S>
    inline bool operator>(const stride_iterator<I, S>& a, const stride_iterator<I, S>& b)
    { 
      return a.base() > b.base();
    }

  template <typename I, typename S>
    inline bool operator<=(const stride_iterator<I, S>& a, const stride_iterator<I, S>& b)
    { 
      return a.base() <= b.base();
    }

  template <typename I, typename S>
    inline bool operator>=(const stride_iterator<I, S>& a, const stride_iterator<I, S>& b)
    { 
      return a.base() >= b.base();
    }
    

  // Difference
  //
  // The difference between two stride iterators is undefined if a and b have
  // different step functions.
  template <typename I, typename S>
    Difference_type<I> operator-(const stride_iterator<I, S>& a, const stride_iterator<I, S>& b)
    {
      return (a.base() - b.base()) / a.stride();
    }



  // Stride iter
  // Construct a stride iterator with some predetermined stride or step.
 
  // Returns a stride iterator over [first, last) stepping by n.
  template <typename I>
    inline auto make_stride_iterator(I first, I last, Difference_type<I> n)
      -> Requires<Iterator<I>(), stride_iterator<I>>
    {
      using F = constant_function<Difference_type<I>>;
      return {first, last, F{n}};
    }

  // Returns a stride iterator over [last, last) stepping by n. This is
  // generally used to construct end iterators.
  template <typename I>
    inline auto make_stride_iterator(I last, Difference_type<I> n)
      -> Requires<Iterator<I>(), stride_iterator<I>>
    {
      using F = constant_function<Difference_type<I>>;
      return {last, F{n}};
    }


  // Returns a stride iterator over i with a step specified by the given
  // function.
  template <typename I, typename Step>
    inline auto make_stride_iterator(I first, I last, Step step) 
      -> Requires<Iterator<I>() && Function<Step>(), stride_iterator<I, Step>>
    {
      return {first, last, step};
    }

  template <typename I, typename Step>
    inline auto make_stride_iterator(I last, Step step)
      -> Requires<Iterator<I>() && Function<Step>(), stride_iterator<I, Step>>
    {
      return {last, step};
    }

} // namespace origin

#endif
