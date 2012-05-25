// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_FILTER_HPP
#define ORIGIN_RANGE_FILTER_HPP

#include <origin/range/core.hpp>
#include <origin/iterator/filter.hpp>

namespace origin
{
  // Filtered
  // The filtered function constructs a bounded range from its range arguments
  // and a unary prediate such that, pred(x) is true for all x in the 
  // constructed range. The constructor supports the following syntax:
  //
  //    filtered(first, last, func)
  //    filtered(range, func)
   
  // Returns a bounded range [first', last') where pred(x) is true for all 
  // elements in that range.
  template <typename I, typename P>
    inline auto filtered(I first, I last, P pred)
      -> bounded_range<filter_iterator<I, P>>
    {
      return {{first, last, pred}, {last, pred}};
    }
  
  // Returns a bounded range r' where pred(x) is true for all elements in that
  // range.
  template <typename R, typename P>
    inline auto filtered(R&& range, P pred)
      -> decltype(filtered(begin(range), end(range), pred))
    {
      return filtered(begin(range), end(range), pred);
    }

} // namespace origin

#endif
