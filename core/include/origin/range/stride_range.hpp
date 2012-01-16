// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_STRIDE_RANGE_HPP
#define ORIGIN_RANGE_STRIDE_RANGE_HPP

#include <origin/range.hpp>
#include <origin/iterator/stride_iterator.hpp>

namespace origin
{

  // A stride range adapts an pair of iterators into a bounded range that is
  // iterated in steps of some predetermined stride (or step).
  //
  // TODO: Consider implementing a checked stride range that asserts or throws
  // when a stride iterator exceeds its range.
  template<typename I, typename Act = advance_action<I>>
    class stride_range
    {
      static_assert(Iterator<I>(), "");
      static_assert(Advance_action<Act, I&>(), "");
    public:
      using value_type = Value_type<I>;
      using difference_type = Distance_type<I>;
      using iterator = stride_iterator<I, Act>;

      stride_range(I first, I last, Act adv)
        : data{first, last, adv}
      { }

      // Returns the advance action.
      Act advance_act() const { return adv(); }
      
      // Returns the increment of the advance function.
      difference_type increment() { return get_increment(adv()); }
      
      // Begin and end.
      iterator begin() const { return {first(), adv()}; }
      iterator end() const   { return {last(), adv()}; }
      
    private:
      const I& first() const { return std::get<0>(data); }
      const I& last() const  { return std::get<1>(data); }
      const Act& adv() const { return std::get<2>(data); }
    
    private:
      std::tuple<I, I, Act> data;
    };
    

    
  // Stride constructors
  // There are 4 constructors for stride ranges. The first two construct a
  // range with a non-constant stride (or step) n:
  //
  //    stride(first, last, n)
  //    stride(range, n)
  //
  // These constructors allow a range to be constructed with a constant
  // stride:
  //
  //    stride<N>(first, last)
  //    stride<N>(range)
    
  // Stride (iterator range)
  // Stride over the elements in [first, last) where n is multiple of 
  // distance(first, last).
  template<typename I>
    inline auto stride(I first, I last, Distance_type<I> n)
      -> stride_range<I, advance_action<I>>
    {
      static_assert(Input_iterator<I>(), "");
      assert(( is_bounded_range(first, last) ));
      assert_if(Forward_iterator<I>(), distance(first, last) % n == 0);

      return {first, last, advance_action<I>{n}};
    }

  
  // Stride (range)
  // Stride over the elements in range with step n where n is a multiple of 
  // size(range).
  template<typename R>
    inline auto stride(R& range, Distance_type<R> n)
      -> Requires<
          Range<R>(), 
          stride_range<Iterator_type<R>, advance_action<Iterator_type<R>>>
        >
    {
      assert_if(Forward_range<R>(), size(range) % n == 0);
      
      return {std::begin(range), std::end(range), advance_action<Iterator_type<R>>{n}};
    }

    
    
  // Stride (const range)
  // Stride over the elements in range with step n where n is a multiple of 
  // size(range).
  template<typename R>
    inline auto stride(const R& range, Distance_type<R> n)
      -> Requires<
          Range<R>(), 
          stride_range<Iterator_type<const R>, advance_action<Iterator_type<const R>>>
        >
    {
      assert_if(Forward_range<R>(), size(range) % n == 0);

      return {range, advance_action<Iterator_type<R>>{n}};
    }



  // Stride (constant, iterator range)
  // Stride over the elements in [first, last) where n is multiple of 
  // distance(first, last).
  template<std::ptrdiff_t N, typename I>
    inline auto stride(I first, I last)
      -> Requires<Iterator<I>(), stride_range<I, static_advance_action<I, N>>>
    {
      return {first, last, static_advance_action<I, N>{}};
    }


  
  // Stride (constant, range)
  // Stride over the elements in range with step n where n is a multiple of 
  // size(range).
  template<std::ptrdiff_t N, typename R>
    inline auto stride(R& range)
      -> Requires<
          Range<R>(), 
          stride_range<
            Iterator_type<R>, 
            static_advance_action<Iterator_type<R>, N>
          >
        >
    {
      assert_if(Forward_range<R>(), size(range) % N == 0);
      
      return {std::begin(range), std::end(range), static_advance_action<Iterator_type<R>, N>{}};
    }
    
    
  // Stride (constant, range)
  // Stride over the elements in range with step n where n is a multiple of 
  // size(range).
  template<std::ptrdiff_t N, typename R>
    inline auto stride(const R& range)
      -> Requires<
          Range<R>(), 
          stride_range<
            Iterator_type<const R>, static_advance_action<Iterator_type<const R>, N>
          >
        >
    {
      assert_if(Forward_range<R>(), size(range) % N == 0);
      
      return {std::begin(range), std::end(range), static_advance_action<Iterator_type<R>, N>{}};
    }

  
} // namespace origin

#endif
