// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_NUMERIC_CLAMP_HPP
#define ORIGIN_NUMERIC_CLAMP_HPP

#include <functional>

namespace origin {
  
  /*template <typename T>
    T clamp(T const& x, T const& min, T const& max)
    { return x < min ? min : (max < x ? max : x); }*/
  
  template <typename T, typename R = std::less<T>>
    T clamp(T const& x, T const& min, T const& max, R comp = R())
    { return comp(x, min) ? min : (comp(max, x) ? max : x); }

  template <typename T, typename R = std::less<T>>
    T clamp_min(T const& x, T const& min, R comp = R())
    { return comp(x, min) ? min : x; }

  template <typename T, typename R = std::less<T>>
    T clamp_max(T const& x, T const& max, R comp = R())
    { return comp(max, x) ? max : x; }

}

#endif // ORIGIN_NUMERIC_CLAMP_HPP