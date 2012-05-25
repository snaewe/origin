// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_STRIDE_RANGE_HPP
#define ORIGIN_RANGE_STRIDE_RANGE_HPP

#include <origin/range/core.hpp>
#include <origin/iterator/stride.hpp>

namespace origin
{
  // Stride constructors
  // There are 4 constructors for stride ranges. The first two construct a
  // range with a non-constant stride (or step) n:
  //
  //    stride(first, last, n)
  //    stride(range, n)
    
  // Stride over the elements in [first, last) where n is multiple of 
  // distance(first, last).
  template <typename I>
    inline auto stride(I first, I last, Difference_type<I> n)
      -> bounded_range<stride_iterator<I>>
    {
      static_assert(Input_iterator<I>(), "");
      assert(is_bounded_range(first, last));
      return {
        make_stride_iterator(first, last, n), 
        make_stride_iterator(last, n)
      };
    }

  template <typename I, typename Step>
    inline auto stride(I first, I last, Step step)
      -> Requires<Function<Step>(), bounded_range<stride_iterator<I, Step>>>
    {
      return {
        make_stride_iterator(first, last, step),
        make_stride_iterator(last, step)
      };
    }

  // Stride over the elements in range with step n where n is a multiple of 
  // size(range).
  template <typename R>
    inline auto stride(R&& range, Difference_type<R> n)
      -> Requires<Range<Auto<R>>(), 
                  decltype(stride(begin(range), end(range), n))>
    {
      return stride(begin(range), end(range), n);
    }

  template <typename R, typename Step>
    inline auto stride(R&& range,  Step step)
      -> Requires<Range<Auto<R>>(),
                  decltype(stride(begin(range), end(range), step))>
    {
      return stride(begin(range), end(range), step);
    }
  
} // namespace origin

#endif
