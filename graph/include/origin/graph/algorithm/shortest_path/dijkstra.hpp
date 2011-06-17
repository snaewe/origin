// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_DIJKSTRA
#define ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_DIJKSTRA

#include <functional>
#include <limits>

#include <origin/graph/algorithm/shortest_path/dijkstra_common.hpp>
#include <origin/heap/binary_heap.hpp>
#include <origin/graph/edge.hpp>

float identity_element_plus() { return 0.0f; }
float extreme_element() { return std::numeric_limits<float>::max(); }

namespace origin {

  // draft of dijkstras. All types and functors are lazy right now. Makes it
  // easier to spec out
  template<
    typename Graph,
    typename DistanceLabel,
    typename WeightLabel,
    typename DijkstraVisitor = default_dijkstra_visitor
  >
  class dijkstra_shortest_paths_draft
  {
  public:
    typedef typename Graph::vertex vertex;
    typedef typename Graph::edge edge;
    typedef float distance_type;

  private:
    // The heap only stores vertices. This simplifies things quite a bit.
    // However, the less operation compares against distance only. So we must
    // make a special less.
    template<typename Distance_>
    struct distance_compare
      : std::binary_function<vertex, vertex, bool>
    {
      distance_compare(Distance_& distance) : distance_(distance)
      { }

      bool operator()(vertex a, vertex b) const
      { return distance_(a) > distance_(b); }

    private:
      Distance_& distance_;
    };

  public:

    typedef binary_heap<vertex, distance_compare<DistanceLabel>> vertex_heap_type;

    dijkstra_shortest_paths_draft(
      Graph const& graph,
      vertex start_vertex,
      DistanceLabel distance = DistanceLabel(),
      WeightLabel weight = WeightLabel(),
      DijkstraVisitor visitor = DijkstraVisitor()
    ) : graph_(graph),
        start_vertex_(start_vertex),
        distance_(distance),
        vertex_heap_(distance_),
        weight_(weight),
        visitor_(visitor)
    { init(); }

    /**
     * Set all distances to infinity, save the start vertex and all
     * predecessors to themselves.
     */
    void init()
    {
      for(vertex v : graph_.vertices()) {
        visitor_.parent(graph_, v, v);
        distance_(v) = extreme_element();
      }
      distance_(start_vertex_) = identity_element_plus();
      vertex_heap_.push(start_vertex_);
      visitor_.discover_vertex(graph_, start_vertex_);
    }

    /** Relax function. */
    void relax(edge& e)
    { }

    void operator()(void)
    {
      while(!vertex_heap_.empty()) {
        vertex min_vertex = vertex_heap_.top();
        vertex_heap_.pop();

        visitor_.examine_vertex(graph_, min_vertex);

        distance_type min_vertex_distance = distance_(min_vertex);

        if(min_vertex_distance == extreme_element())
          return;

        // Find the minimum from the neighbors
        for(edge e : out_edges(graph_, min_vertex)) {
          visitor_.examine_edge(graph_, e);

          // Assert that the edge is not negative
          assert(( !(weight_(graph_, e) < identity_element_plus()) ));

          // Get the distance of each adjacent vertex
          vertex v = graph_.target(e);
          distance_type v_distance = distance_(v);
          bool v_is_undiscovered = !(v_distance < extreme_element());

          // Relax the edge
          bool edge_was_relaxed;
          distance_type distance_u_and_weight =
            distance_(min_vertex) + weight_(graph_, e);

          if(distance_(v) > distance_u_and_weight) {
            distance_(v) = distance_u_and_weight;
            visitor_.parent(graph_, min_vertex, v);
            edge_was_relaxed = true;
          }
          else
            edge_was_relaxed = false;
          

          if(edge_was_relaxed) {
            vertex_heap_.push(v);
            visitor_.edge_relaxed(graph_, e);
          }
          else
            visitor_.edge_not_relaxed(graph_, e);

          if(v_is_undiscovered) {
            visitor_.discover_vertex(graph_, v);
            vertex_heap_.push(v);
          }
        }
      }
    }

  //private:
    Graph const& graph_;
    vertex start_vertex_;
    DistanceLabel distance_;
    vertex_heap_type vertex_heap_;
    WeightLabel weight_;
    DijkstraVisitor visitor_;
  };

}

#endif // ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_DIJKSTRA
