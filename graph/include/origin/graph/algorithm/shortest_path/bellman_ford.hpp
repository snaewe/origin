// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_BELLMAN_FORD
#define ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_BELLMAN_FORD

#include <functional>
#include <cassert>

#include <origin/graph/algorithm/shortest_path/shortest_path_common.hpp>
#include <origin/graph/algorithm/shortest_path/bellman_ford_common.hpp>
#include <origin/graph/traits.hpp>
#include <origin/graph/label.hpp>

namespace origin {

  /**
   * Implementation of the Bellman-Ford algorithm.
   *
   * @tparam Graph  A graph
   *
   * @tparam Edge_Distance_Label  Label the vertices with shortest distances
   *
   * @tparam Distance_Accumulate
   *
   * @tparam Distance_Compare
   *
   * @tparam Vertex_Distance_Label
   *
   * @tparam Visitor
   */
  template<typename Graph,
           typename Edge_Distance_Label,
           typename Distance_Accumulate,
           typename Distance_Compare,
           typename Vertex_Distance_Label,
           typename Visitor>
    class bellman_ford_impl
    {
    public:
      // Graph types
      typedef typename graph_traits<Graph>::vertex vertex;

      // Associated labels, accumulator, order and distance type
      typedef Edge_Distance_Label edge_distance_label;
      typedef Vertex_Distance_Label vertex_distance_label;
      typedef Distance_Accumulate distance_accumulate;
      typedef Distance_Compare distance_compare;
      typedef typename label_traits<Vertex_Distance_Label, vertex>::value_type distance_type;
    private:
      typedef detail::clamped_accumulate<
        distance_accumulate, distance_compare
      > clamped_distance_accumulate;
      typedef typename graph_traits<Graph>::size_type size_type;

      // Static Assertions
      // FIXME Add these!

      // Constructor
      bellman_ford_impl(Graph const& g,
                        vertex_distance_label d,
                        edge_distance_label w,
                        distance_accumulate acc,
                        distance_compare cmp,
                        distance_type init,
                        distance_type max,
                        Visitor v)
        : g_(g), d_(d), w_(w), acc_(acc), cmp_(cmp), init_(init), max_(max), v_(v)
      { }

      // Initialization
      // TODO Move to algorithm main
      void initialize(vertex start)
      {
        for(auto v : g_.vertices())
          d_(v) = max_;
        d_(start) = init_;
      }

      // Algorithm Main
      void operator()()
      {
        for(size_type i = 0u; i < g_.order() - 1u; ++i) {
          // If we cannot relax any edges, we shouldn't repeat process
          bool edge_relaxed = false;
          for(auto e : g_.edges()) {
            v_.examine_edge(g_,e);
            distance_type dist = w_(g_,e) + d_(g_.source(e));
            if(dist < d_(g_.target(e))) {
              d_(g_.target(e)) = dist;
              v_.edge_relaxed(g_,e);
              edge_relaxed = true;
            } else {
              v_.edge_not_relaxed(g_,e);
            }
          }
          if(!edge_relaxed) break;
        }
        // Check to see that all of the edges are minimized
        for(auto e : g_.edges()) {
          if(w_(g_,e) + d_(g_.source(e)) < d_(g_.target(e))) {
            v_.edge_not_minimized(g_,e);
            assert(false);
          } else{
            v_.edge_minimized(g_,e);
          }
        }
      }

    private:
      Graph const& g_;
      vertex_distance_label d_;
      edge_distance_label w_;
      clamped_distance_accumulate acc_;
      distance_compare cmp_;
      distance_type init_;
      distance_type max_;
      Visitor v_;
    };

  /**
   * Bellman_ford Algorithm
   */
  template<typename Graph,
           typename Distance_Label,
           typename Visitor = default_bellman_ford_visitor>
    void bellman_ford(Graph const& g,
                      typename graph_traits<Graph>::vertex start,
                      Distance_Label distance,
                      Visitor vis = Visitor())
    {
      typedef typename graph_traits<Graph>::vertex vertex;
      typedef typename label_traits<
        Distance_Label, vertex
      >::value_type distance_type;
      typedef std::plus<distance_type> distance_accumulate;
      typedef std::less<distance_type> distance_compare;
      typedef detail::edge_weight<Graph> edge_distance_label;
    }

}

#endif //ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_BELLMAN_FORD
