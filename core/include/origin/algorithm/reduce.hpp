// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ALGORITHM_REDUCE_HPP
#define ORIGIN_ALGORITHM_REDUCE_HPP

namespace origin
{
  // Reduce (iterator)
  // The reduce algorithm evaluates value = f(*i, value) to each iterator i in
  // [first, last) from left to right, accumulating the given value.
  template <typename I, typename T, typename F>
    inline T reduce(I first, I last, T value, F f)
    {
      while (first != last) {
        value = f(*first, value);
        ++first;
      }
      return value;
    }


  // Reduce (range)
  // The reduce algorithm evaluates value = f(*i, value) to each iterator i in
  // range from left to right, accumulating the given value.
  template <typename R, typename T, typename F>
    inline T reduce(const R& range, T init, F f)
    {
      return reduce(o_begin(range), o_end(range), init, f);
    }
}

#endif
