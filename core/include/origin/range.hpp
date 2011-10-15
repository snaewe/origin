// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_HPP
#define ORIGIN_RANGE_HPP

#include <origin/iterator.hpp>
#include <origin/range/traits.hpp>

namespace origin
{
  // Return the size of the given range by calculating its distance.
  template<typename R>
    inline auto size(R const& r) 
      -> typename std::enable_if<
        !has_member_size<R>::value,
        decltype(std::distance(std::begin(r), std::end(r)))
      >::type
    {
      return std::distance(std::begin(r), std::end(r));
    }
  
  // Return the size of the given range, if the range is counted (i.e., has a
  // size member function). This is expected to be a constant-time operation.
  template<typename R>
    inline auto size(R const& r) -> decltype(r.size())
    {
      return r.size();
    }


  // Return true if r is empty. A range is empty if its beginning value
  // is equal to its limit (end).
  template<typename R>
    inline auto empty(R const& r)
      -> typename std::enable_if<!has_member_size<R>::value, bool>::type
    {
      return std::begin(r) == std::end(r);
    }

  // Return true if the counted range r is emtpy.
  template<typename R>
    inline auto empty(R const& r) -> decltype(r.empty())
    {
      return r.empty();
    }

} // namespace origin

// Include range constructors
#include <origin/range/array.hpp>
#include <origin/range/filter.hpp>

#endif
