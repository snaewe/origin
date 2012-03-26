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

// FIXME: Put in algorithms!

namespace origin
{
  // Multi-key sort (iterator)
  template <typename I, typename... Keys>
    void multi_sort(I first, I last, Keys&&... keys)
    {
      using Proj = std::tuple<Accessor_function<Forwarded<Keys>>...>;
      using Less = projection_less<Value_type<I>, Proj>;

      Proj proj {std::forward<Keys>(keys)... };
      Less comp {proj};

      comp(*first, *first);

      /*
      o_sort(first, last, comp);
      */
    }



  // Multi-key sort (range)
  template <typename R, typename... Keys>
    void multi_sort(R&& range, Keys&&... keys)
    {
      multi_sort(o_begin(range), o_end(range), 
                 std::forward<Forwarded<Keys>>(keys)...);
    }


} // namespace origin

#endif
