// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_RANGE_UTILITY_HPP
#define ORIGIN_RANGE_UTILITY_HPP

namespace origin
{
  // The range proxy class is the result type of the post-increment operator.
  // It can be dereferenced, but that's about it.
  template<typename R>
    class range_proxy
    {
      range_proxy(R const* r)
        : range(r)
      { }
      
      R const& operator*() const { return *range; }
      
      R const* range;
    };

} // namespace origin

#endif
