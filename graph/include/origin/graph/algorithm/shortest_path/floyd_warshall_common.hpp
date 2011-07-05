// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_FLOYD_WARSHALL_COMMON
#define ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_FLOYD_WARSHALL_COMMON

namespace origin {

  namespace detail {

    template<typename T, typename Compare>
    T min(T const& a, T const& b, Compare const& cmp)
    {
      if(cmp(a, b))
        return a;
      return b;
    }

  } // namespace detail

} // namespace origin

#endif // ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_FLOYD_WARSHALL_COMMON
