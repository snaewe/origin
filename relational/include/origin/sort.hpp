// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MULTISORT_HPP
#define ORIGIN_MULTISORT_HPP

#include <origin/algorithm.hpp>
#include <origin/relational.hpp>

namespace origin
{
  // Sort ascending (iterator)
  // Sort the elements in [first, last) in ascending order using the given 
  // attributes. The result type of each attribute type must be weakly ordered.
  template <typename I, typename... Attrs>
    inline void sort_ascending(I first, I last, Attrs... attrs)
    {
      auto comp = make_projects_less<Value_type<I>>(attrs...);
      o_sort(first, last, comp);
    }



  // Sort ascending (range)
  // Sort the elements in range in ascending order using the given attributes. 
  // The result type of each attribute type must be weakly ordered.
  template <typename R, typename... Attrs>
    inline void sort_ascending(R&& range, Attrs... attrs)
    {
      sort_ascending(o_begin(range), o_end(range), attrs...);
    }



  // Sort descending (iterator)
  // Sort the elements in [first, last) in descending order using the given 
  // attributes. The result type of each attribute type must be weakly ordered.
  template <typename I, typename... Attrs>
    inline void sort_descending(I first, I last, Attrs... attrs)
    {
      auto comp = make_projects_greater<Value_type<I>>(attrs...);
      o_sort(first, last, comp);
    }



  // Sort descending (range)
  // Sort the elements in range in descending order using the given attributes. 
  // The result type of each attribute type must be weakly ordered.
  template <typename R, typename... Attrs>
    inline void sort_descending(R&& range, Attrs... attrs)
    {
      sort_descending(o_begin(range), o_end(range), attrs...);
    }

} // namespace origin

#endif
