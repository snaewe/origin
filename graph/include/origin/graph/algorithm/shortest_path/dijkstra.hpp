// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_DIJKSTRA
#define ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_DIJKSTRA

#include <functional>


#include <origin/graph/algorithm/shortest_path/dijkstra_common.hpp>
#include <origin/heap/binary_heap.hpp>

namespace origin {


  // Perhaps this could ease some of the syntax by packing all of the
  // deducible stuff into a traits class.
  template<typename Distance>
  struct distance_traits {
    typedef Distance distance_type;

    static constexpr distance_type zero()
    { return distance_type(); }

    static constexpr distance_type infinity()
    { return std::numeric_limits<distance_type>::max(); }
  };


  // draft of dijkstras. All types and functors are lazy right now. Makes it
  // easier to spec out
  template<
    typename Graph,
    typename PredecessorMap,
    typename DistanceMap,
    typename WeightMap,
    typename Compare,
    typename DijkstraVisitor
  >
  class dijkstra_shortest_paths_draft
  {
    typedef typename Graph::vertex vertex;
    typedef typename Graph::edge edge;
    typedef int distance_type;

    // The heap only stores vertices. This simplifies things quite a bit.
    // However, the less operation compares against distance only. So we must
    // make a special less.
    template<typename Compare_>
    struct distance_compare
      : private Compare_
    {
      typedef typename Compare::first_argument_type first_argument_type;
      typedef typename Compare::second_argument_type second_argument_type;
      typedef typename Compare::result_type result_type;

      bool operator()(first_argument_type const& a, second_argument_type const& b)
      { return Compare::operator()(distance(a), distance(b)); }
    };

    typedef binary_heap<
      distance_type,
      distance_compare<std::greater<distance_type>>
    > vertex_heap_type;

    dijkstra_shortest_paths_draft(
      Graph const& graph,
      vertex start_vertex,
      distance_type infinity,
      distance_type zero,
      PredecessorMap predecessor = PredecessorMap(),
      DistanceMap distance = DistanceMap(),
      WeightMap weight = WeightMap(),
      Compare compare = Compare(),
      DijkstraVisitor visitor = DijkstraVisitor()
    ) : graph(graph), predecessor(predecessor), distance(distance),
        vertex_heap(), weight(weight), infinity(infinity), zero(zero),
        compare(compare)
    { }

    /**
     * Set all distances to infinity, save the start vertex and all
     * predecessors to themselves.
     */
    void init()
    {
      for(vertex& v : graph.vertices()) {
        predecessor(v) = v;
        distance(v) = std::limits;
      }
      distance(start_vertex) = 0.0f;
      vertex_heap.push(start_vertex);
      visitor.discover_vertex(graph, start_vertex);
    }

    /** Relax function. */
    void relax(edge& e)
    { }

    void start()
    {
      while(!vertex_heap.empty()) {
        vertex min_vertex = vertex_heap.top();
        vertex_heap.pop();

        visitor.examine_vertex(graph, min_vertex);

        distance_type min_vertex_distance = distance(min_vertex);

        if(min_vertex == infinity())
          return;

        // Find the minimum from the neighbors
        for(edge e : out_edges(graph, min_vertex)) {
          visitor.examine_edge(graph, e);

          // Assert that the edge is not negative
          assert(( !(weight(e) < zero) ));

          // Get the distance of each adjacent vertex
          vertex u = target(graph, e);
          distance_type u_distance = distance(u);
          bool u_is_undiscovered = !(u_distance < infinity);

          // Relax the edge
          bool edge_was_relaxed = relax(e);

          if(edge_was_relaxed) {
            vertex_heap.push(u);
            visitor.edge_relaxed(graph, e);
          }
          else
            visitor.edge_not_relaxed(e);

          if(u_is_undiscovered) {
            visitor.discover_vertex(graph, u);
            vertex_heap.push(u);
          }
        }
      }
    }

    Graph const& graph;
    vertex_heap_type vertex_heap;
    vertex start_vertex;
    PredecessorMap predecessor;
    DistanceMap distance;
    WeightMap weight;
    DijkstraVisitor visitor;
    distance_type infinity;
    distance_type zero;
    Compare compare;
  };

}

#endif // ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_DIJKSTRA
