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

float identity_element() { return 0.0f; }
float extreme_element() { return std::numeric_limits<float>::max(); }

namespace origin {

  /**
   * @tparam Graph  A Graph
   * 
   * @tparam Weight_Label   A Readable edge Label that assocates a weight
   * with every edge.
   * 
   * @tparam Weight_Accum   An associative (commutative?) binary operation 
   * that accumulates edge weights.
   * 
   * @tparam Weight_Comp    A Strict_Weak_Order over the graph's edge weights
   *
   * @tparam Distance_Label A Writable vertex Label that records the cumulative
   * edge weight from a start vertex to every other vertex.
   * 
   * 
   * @tparam Visitor        A Dijkstra_Visitor
   *
   * FIXME Replace float with result_of
   * TODO Put in static asserts!
   */
  template<typename Graph,
           typename Weight_Label,
           typename Weight_Accumulate,
           typename Weight_Compare,
           typename Distance_Label,
           typename Visitor>
    class dijkstra_shortest_paths_draft_full_heap
    {
    public:
      // Graph types
      typedef typename graph_traits<Graph>::vertex vertex;
      typedef typename graph_traits<Graph>::edge edge;

      //Associated labels
      typedef Weight_Label weight_label;
      typedef Distance_Label distance_label;

      typedef typename std::remove_reference<typename std::result_of<distance_label(vertex)>::type>::type distance_type;
      //typedef float distance_type;
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
      dijkstra_shortest_paths_draft_full_heap(Graph const& graph,
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
           typename Vertex,
           typename Distance_Label,
           typename Visitor>
    void dijkstra_shortest_paths(Graph const& g,
                                 Vertex start,
                                 Distance_Label distance,
                                 Visitor vis)
    {
      // This is almost correct. The distance label may return a reference.
      // In this case we have to remove it.
      typedef typename std::result_of<Distance_Label(Vertex)>::type ref_Distance_Type;
      typedef typename std::remove_reference<ref_Distance_Type>::type Distance_Type;

      typedef std::plus<Distance_Type> Weight_Accum;
      typedef std::less<Distance_Type> Weight_Compare;
      typedef edge_weight<Graph, default_weight_label<Graph>> Weight_Label;
      typedef dijkstra_shortest_paths_draft_full_heap<
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

  /**
   * Test cover that takes a user defined edge_weight
   */


  // draft of dijkstras. All types and functors are lazy right now. Makes it
  // easier to spec out
  template<
    typename Graph,
    typename DistanceLabel,
    typename WeightLabel,
    typename DijkstraVisitor = default_dijkstra_visitor
  >
  class dijkstra_shortest_paths_draft_old
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

    dijkstra_shortest_paths_draft_old(
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
        distance_(v) = ::extreme_element();
      }
      distance_(start_vertex_) = ::identity_element();
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

        if(min_vertex_distance == ::extreme_element())
        { return; }

        // Find the minimum from the neighbors
        for(edge e : out_edges(graph_, min_vertex)) {
          visitor_.examine_edge(graph_, e);

          // Assert that the edge is not negative
          assert(( !(weight_(graph_, e) < ::identity_element()) ));

          // Get the distance of each adjacent vertex
          vertex v = graph_.target(e);
          distance_type v_distance = distance_(v);
          bool v_is_undiscovered = !(v_distance < ::extreme_element());

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
