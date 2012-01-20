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
#include <origin/graph/algorithm/shortest_path/shortest_path_common.hpp>
#include <origin/numeric/clamp.hpp>
#include <origin/heap/binary_heap.hpp>
#include <origin/functional.hpp>
#include <origin/graph/edge.hpp>
#include <origin/graph/label.hpp>
#include <origin/graph/traits.hpp>

namespace origin {

  /**
   * Implementation of Dijkstra's shortest paths algorithm. This version does
   * not clamp the accumulate operator.
   *
   * @tparam Graph          A Graph
   * 
   * @tparam Vertex_Label   A writable vertex Label that assocates a distance
   * with every vertex.
   *
   * @tparam Edge_Label     A readable edge label that records the shortest
   * distance from the start to each other vertex
   * 
   * @tparam Accumulate_Op  An closed accumulation operation on distance types
   * 
   * @tparam Compare_Op     A Strict_Weak_Order over the weight_type
   *
   * @tparam Visitor        A Dijkstra_Visitor
   *
   * TODO Implement path recovery
   */
  
  template<typename Graph,
           typename Vertex_Label,
           typename Edge_Label,
           typename Accumulate_Op,
           typename Compare_Op,
           typename Visitor>
    class dijkstra_shortest_paths_impl
    {
    public:
      // Graph types
      typedef Vertex_type<Graph> vertex;
      // Associated labels
      typedef Edge_Label edge_label;
      typedef Vertex_Label vertex_label;

      // Distance operations
      typedef typename label_traits<
        vertex_label, vertex
      >::value_type weight_type;
      typedef Accumulate_Op accumulate_op;
      typedef Compare_Op compare_op;

    private:
      // Heap is ordered by distance labels associated with vertices
      struct heap_compare_op : std::binary_function<vertex, vertex, bool>
      {
        heap_compare_op(compare_op c, vertex_label d)
          : cmp(c), dist(d)
        { }

        bool operator()(vertex a, vertex b) const
        { return cmp(dist(b), dist(a)); }

        compare_op cmp;
        vertex_label dist;
      };

      typedef mutable_binary_heap<vertex, heap_compare_op> heap_type;

      // Static assertions
      // TODO Add this!

    public:
      // Constructor
      dijkstra_shortest_paths_impl(Graph const& g,
                                   vertex_label d,
                                   edge_label w,
                                   accumulate_op accum,
                                   compare_op cmp,
                                   weight_type zero,
                                   weight_type max,
                                   Visitor visitor)
        : g_(g), heap_(heap_compare_op(cmp, d)), d_(d), w_(w),
          accum_(accum, max), cmp_(cmp), zero_(zero), max_(max),
          visitor_(visitor)
      {
        for(vertex v : g_.vertices()) {
          d_(v) = max_;
          heap_.push(v);
        }
      }

      // Algorithm main
      void operator()(vertex start)
      {
        d_(start) = zero_;
        heap_.push(start);
        visitor_.discover_vertex(g_, start);

        while(!heap_.empty()) {
          vertex u = heap_.top();
          heap_.pop();

          visitor_.examine_vertex(g_, u);

          // Since this version fills the heap, there may be disconnected components
          weight_type du = d_(u);
          if(!cmp_(du, max_))
            return;

          // Find the minimum vertex from all neighbors
          for(auto e : out_edges(g_, u)) {
            visitor_.examine_edge(g_, e);

            // Dijkstra requires non-negative weights.
            assert(( !cmp_(w_(g_, e), zero_) ));

            // Get the distance of the neighbor vertex
            vertex v = g_.target(e);
            weight_type dv = d_(v);
            if(!cmp_(dv, max_))
              visitor_.discover_vertex(g_, v);

            // Relax the edge
            weight_type d = accum_(du, w_(g_, e));
            if(cmp_(d, dv)) {
              d_(v) = d;
              heap_.decrease(v);
              visitor_.edge_relaxed(g_, e);
            } else {
              visitor_.edge_not_relaxed(g_, e);
            }
          }
        }
      }

    private:
      Graph const& g_;
      heap_type heap_;
      vertex_label d_;
      edge_label w_;
      accumulate_op accum_;
      compare_op cmp_;
      weight_type zero_;
      weight_type max_;
      Visitor visitor_;
    };
  
  /**
   * Implementation of Dijkstra's shortest paths algorithm with clamping.
   *
   * @tparam Graph          A Graph
   * 
   * @tparam Vertex_Label   A writable vertex Label that assocates a distance
   * with every vertex.
   *
   * @tparam Edge_Label     A readable edge label that records the shortest
   * distance from the start to each other vertex
   * 
   * @tparam Accumulate_Op  An closed accumulation operation on distance types
   * 
   * @tparam Compare_Op     A Strict_Weak_Order over the weight_type
   *
   * @tparam Visitor        A Dijkstra_Visitor
   *
   * TODO Implement path recovery
   */
  template<typename Graph,
    typename Vertex_Label,
    typename Edge_Label,
    typename Accumulate_Op,
    typename Compare_Op,
    typename Visitor>
  class dijkstra_shortest_paths_noclamp_impl
  {
  public:
    // Graph types
    typedef Vertex_type<Graph> vertex;
    
    // Associated labels
    typedef Edge_Label edge_label;
    typedef Vertex_Label vertex_label;
    
    // Distance operations
    typedef typename label_traits<
    vertex_label, vertex
    >::value_type weight_type;
    // FIXME Design decision: should this be clamped?
    typedef Accumulate_Op accumulate_op;
    typedef Compare_Op compare_op;
    
  private:
    // Heap is ordered by distance labels associated with vertices
    struct heap_compare_op : std::binary_function<vertex, vertex, bool>
    {
      heap_compare_op(compare_op c, vertex_label d)
      : cmp(c), dist(d)
      { }
      
      bool operator()(vertex a, vertex b) const
      { return cmp(dist(b), dist(a)); }
      
      compare_op cmp;
      vertex_label dist;
    };
    
    typedef mutable_binary_heap<vertex, heap_compare_op> heap_type;
    
    // Static assertions
    // TODO Add this!
    
  public:
    // Constructor
    dijkstra_shortest_paths_noclamp_impl(Graph const& g,
                                 vertex_label d,
                                 edge_label w,
                                 accumulate_op accum,
                                 compare_op cmp,
                                 weight_type min,
                                 weight_type max,
                                 Visitor visitor)
    : g_(g), heap_(heap_compare_op(cmp, d)), d_(d), w_(w), accum_(accum, max),
      cmp_(cmp), min_(min), max_(max), visitor_(visitor)
    {
      for(vertex v : g_.vertices()) {
        d_(v) = max_;
        heap_.push(v);
      }
    }
    
    // Algorithm main
    void operator()(vertex start)
    {
      d_(start) = min_;
      heap_.push(start);
      visitor_.discover_vertex(g_, start);
      
      while(!heap_.empty()) {
        vertex u = heap_.top();
        heap_.pop();
        
        visitor_.examine_vertex(g_, u);
        
        // Since this version fills the heap, there may be disconnected components
        weight_type du = d_(u);
        if(!cmp_(du, max_))
          return;
        
        // Find the minimum vertex from all neighbors
        for(auto e : out_edges(g_, u)) {
          visitor_.examine_edge(g_, e);
          
          // Dijkstra requires non-negative weights.
          assert(( !cmp_(w_(g_, e), min_) ));
          
          // Get the distance of the neighbor vertex
          vertex v = g_.target(e);
          weight_type dv = d_(v);
          if(!cmp_(dv, max_))
            visitor_.discover_vertex(g_, v);
          
          // Relax the edge
          weight_type d = accum_(du, w_(g_, e));
          if(cmp_(d, dv)) {
            d_(v) = d;
            heap_.decrease(v);
            visitor_.edge_relaxed(g_, e);
          } else {
            visitor_.edge_not_relaxed(g_, e);
          }
        }
      }
    }
    
    /*void abstract_main(vertex start)
    {
      d_(start) = min_;
      heap_.push(start);
      
      while (!heap_.empty()) {
        vertex u = heap_.top();
        heap_.pop();
        
        weight_type du = d_(u);
        if(!cmp_(du, max_)
           return;
      }
    }*/
    /*template <
      typename Vertex,
      typename Vertex_label,
      typename Edge_label,
      typename Accumulate_op,
      typename Compare_op
    >
    bool relax(
      Vertex 
               Vertex_label d, Edge_label w, Vertex u, Vertex v, R comp = R())
    {
      auto d = accum_(du, w_(g_, e)); 
    }*/
    
  private:
    Graph const& g_;
    heap_type heap_;
    vertex_label d_;
    edge_label w_;
    accumulate_op accum_;
    compare_op cmp_;
    weight_type min_;
    weight_type max_;
    Visitor visitor_;
  };

  /**
   * Test Cover
   */
  template<typename Graph,
           typename Vertex_Label,
           typename Visitor = default_dijkstra_visitor>
    void dijkstra_shortest_paths(Graph const& g,
                                 Vertex_type<Graph> start,
                                 Vertex_Label d,
                                 Visitor visitor = Visitor())
    {
      typedef Vertex_type<Graph> vertex;
      typedef typename label_traits<
        Vertex_Label, vertex
      >::value_type weight_type;
      typedef detail::edge_weight<Graph> edge_label;
      typedef std::plus<weight_type> accumulate_op;
      typedef std::less<weight_type> compare_op;
      typedef dijkstra_shortest_paths_impl<
        Graph, Vertex_Label, edge_label, accumulate_op, compare_op, Visitor
      > Algorithm;

      edge_label w;
      accumulate_op accum;
      compare_op cmp;
      weight_type zero = identity_element(accum);
      weight_type max = extreme_element(cmp);
      Algorithm algo(g, d, w, accum, cmp, zero, max, visitor);
      algo(start);
    }

  /**
   * Complete algorithm
   */
  #ifndef DIST_TYPE__
  #define DIST_TYPE__ typename label_traits<Vertex_Label, typename \
                      graph_traits<Graph>::vertex>::value_type
  template<typename Graph,
           typename Vertex_Label,
           typename Edge_Label,
           typename Accumulate_Op,
           typename Compare_Op,
           typename Visitor>
    void dijkstra_shortest_path(Graph const& g,
                                Vertex_type<Graph> start,
                                Vertex_Label d,
                                Accumulate_Op accum,
                                Compare_Op cmp,
                                DIST_TYPE__ zero,
                                DIST_TYPE__ max, 
                                Visitor visitor)
    {
      typedef detail::edge_weight<Graph> edge_label;
      typedef dijkstra_shortest_paths_impl<
        Graph, Vertex_Label, edge_label, Accumulate_Op, Compare_Op, Visitor
      > Algorithm;

      edge_label w;
      Algorithm algo(g, d, w, accum, cmp, zero, max, visitor);
      algo(start);
    }

  #endif // DIST_TYPE__

} // namespace origin

#endif // ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_DIJKSTRA
