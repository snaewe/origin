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
#include <origin/functional.hpp>
#include <origin/graph/edge.hpp>
#include <origin/graph/label.hpp>

namespace origin {

  /**
   * @tparam Graph  A Graph
   * 
   * @tparam Weight_Label   A Readable edge Label that assocates a weight
   * with every edge.
   * 
   * @tparam Weight_Accum   An associative binary operation that accumulates
   * edge weights.
   * 
   * @tparam Weight_Comp    A Strict_Weak_Order over the graph's edge weights
   *
   * @tparam Distance_Label A Writable vertex Label that records the cumulative
   * edge weight from a start vertex to every other vertex.
   * 
   * 
   * @tparam Visitor        A Dijkstra_Visitor
   *
   * TODO Put in static asserts!
   */
  template<typename Graph,
           typename Weight_Label,
           typename Weight_Accumulate,
           typename Weight_Compare,
           typename Distance_Label,
           typename Visitor>
    class dijkstra_shortest_paths_impl
    {
    public:
      // Graph types
      typedef typename graph_traits<Graph>::vertex vertex;
      typedef typename graph_traits<Graph>::edge edge;

      //Associated labels
      typedef Weight_Label weight_label;
      typedef Distance_Label distance_label;

      // FIXME Rename all mention of weight to distance or vice versa
      typedef typename label_traits<distance_label, vertex>::value_type distance_type;
      typedef Weight_Accumulate weight_accumulate;
      typedef Weight_Compare weight_compare;
      typedef Visitor visitor_type;

    private:
      // Clamp accumulate to infinity.
      // FIXME Clamped types exist in many languages (implemented or
      // documentation only). Should an abstraction exist for functor output?

      struct clamped_accumulate
        : std::binary_function<distance_type, distance_type, distance_type>
      {
        clamped_accumulate(weight_accumulate a, distance_type x)
          : accum(a), inf(x)
        { }

        distance_type operator()(distance_type x, distance_type y) const
        {
          if(x == inf || y == inf)
            return inf;
          return x + y;
        }

        weight_accumulate accum;
        distance_type inf;
      };

      // Distance compare - while the heap only stores vertices, comparisons are
      // performed on the distance labels.
      struct distance_compare : std::binary_function<vertex, vertex, bool>
      {
        distance_compare(weight_compare c, distance_label d)
          : comp{c}, dist{d}
        { }

        bool operator()(vertex a, vertex b) const
        { return comp(dist(b), dist(a)); }

        weight_compare comp;
        distance_label dist;
      };

    public:
      typedef mutable_binary_heap<vertex, distance_compare> heap_type;

      // Constructor
      dijkstra_shortest_paths_impl(Graph const& graph,
                                   weight_label weight_l,
                                   weight_accumulate accum,
                                   weight_compare compare,
                                   distance_label distance,
                                   distance_type init,
                                   distance_type infinity,
                                   visitor_type visitor)
        : graph_{graph}, heap_{distance_compare{compare, distance}},
          weight_(weight_l), accum_{accum, infinity}, compare_{compare},
          distance_{distance}, init_{init}, infinity_{infinity},
          visitor_(visitor)
      {
        for(vertex v : graph_.vertices()) {
          distance_(v) = infinity_;
          heap_.push(v);
        }
      }

      // Algorithm main
      void operator()(vertex start)
      {
        distance_(start) = init_;
        heap_.push(start);
        visitor_.discover_vertex(graph_, start);

        while(!heap_.empty()) {
          vertex u = heap_.top();
          heap_.pop();

          visitor_.examine_vertex(graph_, u);

          // Since this version fills the heap, there may be disconnected components
          distance_type du = distance_(u);
          if(!compare_(du, infinity_))
            return;

          // Find the minimum vertex from all neighbors
          for(edge e : out_edges(graph_, u)) {
            visitor_.examine_edge(graph_, e);

            // If the edge is negative, we use Bellman-Ford.
            assert(( !compare_(weight_(graph_, e), init_) ));

            // Get the distance of the neighbor vertex
            vertex v = graph_.target(e);
            distance_type dv = distance_(v);
            if(!compare_(dv, infinity_))
              visitor_.discover_vertex(graph_, v);

            // Relax the edge
            //distance_type d = accum_(du, weight_(graph_, e));
            distance_type d = 1.0f;
            if(compare_(d, dv)) {
              distance_(v) = d;
              heap_.decrease(v);
              visitor_.edge_relaxed(graph_, e);
            } else {
              visitor_.edge_not_relaxed(graph_, e);
            }
          }
        }
      }

    private:
      Graph const& graph_;
      heap_type heap_;
      weight_label weight_;
      clamped_accumulate accum_;
      weight_compare compare_;
      distance_label distance_;
      distance_type init_;
      distance_type infinity_;
      visitor_type visitor_;
    };

  /**
   * Test Cover
   */
  template<typename Graph,
           typename Distance_Label,
           typename Visitor>
    void dijkstra_shortest_paths(Graph const& g,
                                 typename Graph::vertex start,
                                 Distance_Label distance,
                                 Visitor vis)
    {
      typedef typename Graph::vertex Vertex;
      typedef typename label_traits<Distance_Label, Vertex>::value_type Distance_Type;
      typedef std::plus<Distance_Type> Weight_Accum;
      typedef std::less<Distance_Type> Weight_Compare;
      typedef edge_weight<Graph, edge_weight<Graph, Distance_Type>> Weight_Label;
      typedef dijkstra_shortest_paths_impl<
        Graph, Weight_Label, Weight_Accum, Weight_Compare, Distance_Label, Visitor
      > Algorithm;

      Weight_Label weight;
      Weight_Accum accum;
      Weight_Compare compare;
      Distance_Type init = identity_element(accum);
      Distance_Type infinity = extreme_element(compare);
      Algorithm algo{g, weight, accum, compare, distance, init, infinity, vis};
      algo(start);
    }

} // namespace origin

#endif // ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_DIJKSTRA
