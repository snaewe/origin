// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_REVERSE_HPP
#define ORIGIN_RANGE_REVERSE_HPP

#include <origin/range/core.hpp>
#include <origin/iterator/reverse.hpp>

namespace origin
{
  // Reversed
  // The reversed function constructs a reverse range from its range arguments.

  template <typename I>
    inline bounded_range<reverse_iterator<I>> reversed(I first, I last)
    {
      static_assert(Bidirectional_iterator<I>(), "");
      assert(is_bounded_range(first, last));
      return {reversed(last), reversed(first)};
    }
    
  template <typename R>
    inline auto reversed(R& range)
      -> decltype(reversed(begin(range), end(range)))
    {
      static_assert(Bidirectional_range<R>(), "");
      return reversed(begin(range), end(range));
    }

} // namespace origin

#endif
