// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_BREADTH_FIRST
#define ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_BREADTH_FIRST

#include <limits>
#include <functional>

#include <origin/graph/algorithm/search/breadth_first.hpp>

namespace origin 
{
  // FIXME: How general is this visitor? Does it need to be a BFS visitor or
  // can it be something more general? Probably the latter, but we need to
  // figure out what that migth be.
  template<typename Graph, typename Distance_Label>
    struct distance_visitor : bfs_visitor
    {
      typedef typename graph_traits<Graph>::vertex vertex;
      typedef typename graph_traits<Graph>::edge edge;
      
      distance_visitor(Distance_Label d)
        : dist{d}
      { }
      
//       void tree_edge(Graph& g, edge e)
//       {
//         dist(g.target(e)) = dist(g.source(e)) + 1;
//       }
      
      Distance_Label dist;
    };
    
  template<typename Graph, 
           typename Distance_Label, 
           typename Distance>
    void breadth_first_shortest_paths(Graph const& g, 
                                      typename Graph::const_vertex start,
                                      Distance_Label dist,
                                      Distance zero,
                                      Distance inf)
    {
      // Start by setting all initial distances to infinity and distance to the
      // start vertex to zero.
      for(auto v : g.vertices())
        dist(v) = inf;
      dist(start) = zero;

      // Run BFS, recording the distance for each hop.
      // NOTE: If inf is relatively small (i.e., we're clamping to an easily
      // surmountable maximim distance), then we shouldn't keep searching when
      // we exceed that maximum value. However, if we always test for this
      // condition, then we add conditions that might otherwise be used
      // infrequently. I think these same observations apply to other SSSP
      // algorithms also.
      distance_visitor<Graph const, Distance_Label> vis{dist};
      breadth_first_search(g, start, vis);
    }

  template<typename Graph, typename Distance_Label>
    void breadth_first_shortest_paths(Graph const& g, 
                                      typename Graph::const_vertex start,
                                      Distance_Label dist)
    {
      typedef typename graph_traits<Graph>::vertex Vertex;
      typedef typename label_traits<Distance_Label, Vertex>::value_type Distance;

      Distance zero = 0;
      Distance inf = std::numeric_limits<Distance>::max();
      breadth_first_shortest_paths(g, start, dist, zero, inf);
    }

} // namespace origin

#endif
