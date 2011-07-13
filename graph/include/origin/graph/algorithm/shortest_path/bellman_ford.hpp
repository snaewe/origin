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
#include <origin/functional.hpp>
#include <origin/graph/traits.hpp>
#include <origin/graph/label.hpp>

namespace origin {

  /**
   * Implementation of Bellman-Ford's shortest paths algorithm.
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
   * @tparam Compare_Op     A Strict_Weak_Order over the distance_type
   *
   * @tparam Visitor        A Dijkstra_Visitor
   *
   * TODO Implement path recovery.
   */
  template<typename Graph,
           typename Vertex_Label,
           typename Edge_Label,
           typename Accumulate_Op,
           typename Compare_Op,
           typename Visitor>
    class bellman_ford_impl
    {
    public:
      // Graph types
      typedef typename graph_traits<Graph>::vertex vertex;

      // Associated labels
      typedef Edge_Label edge_label;
      typedef Vertex_Label vertex_label;

      // Distance operations
      typedef Accumulate_Op accumulate_op;
      typedef Compare_Op compare_op;
      typedef typename label_traits<
        Vertex_Label, vertex
      >::value_type distance_type;
      
    private:
      typedef detail::clamped_accumulate<
        accumulate_op, compare_op
      > clamped_accumulate_op;
      typedef typename graph_traits<Graph>::size_type size_type;

      // Static Assertions
      // FIXME Add these!

    public:
      // Constructor
      bellman_ford_impl(Graph const& g,
                        vertex_label d,
                        edge_label w,
                        accumulate_op acc,
                        compare_op cmp,
                        distance_type init,
                        distance_type max,
                        Visitor v)
        : g_(g), d_(d), w_(w), acc_(acc, max), cmp_(cmp), init_(init),
          max_(max), v_(v)
      {
        for(auto v : g_.vertices())
          d_(v) = max_;
      }

      // Algorithm Main
      void operator()(vertex start)
      {
        // Initialize distance label
        d_(start) = init_;

        for(size_type i = 0u; i < g_.order() - 1u; ++i) {
          // If we cannot relax any edges, we shouldn't repeat process
          bool edge_relaxed = false;
          for(auto e : g_.edges()) {
            v_.examine_edge(g_,e);
            distance_type dist = accum_(w_(g_,e), d_(g_.source(e)));
            if(cmp(dist, d_(g_.target(e)))) {
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
          if(cmp_(accum_(w_(g_,e), d_(g_.source(e))), d_(g_.target(e)))) {
            v_.edge_not_minimized(g_,e);
            assert(false);
          } else{
            v_.edge_minimized(g_,e);
          }
        }
      }

    private:
      Graph const& g_;
      vertex_label d_;
      edge_label w_;
      clamped_accumulate_op acc_;
      compare_op cmp_;
      distance_type init_;
      distance_type max_;
      Visitor v_;
    };

  /**
   * Bellman_ford Algorithm
   */
  template<typename Graph,
           typename Vertex_Label,
           typename Visitor = default_bellman_ford_visitor>
    void bellman_ford(Graph const& g,
                      typename graph_traits<Graph>::vertex start,
                      Vertex_Label d,
                      Visitor visitor = Visitor())
    {
      typedef typename graph_traits<Graph>::vertex vertex;
      typedef typename label_traits<
        Vertex_Label, vertex
      >::value_type distance_type;
      typedef std::plus<distance_type> accumulate_op;
      typedef std::less<distance_type> compare_op;
      typedef detail::edge_weight<Graph> edge_label;
      typedef bellman_ford_impl<
        Graph, Vertex_Label, edge_label, accumulate_op, compare_op, Visitor
      > Algorithm;

      edge_label w;
      accumulate_op accum;
      compare_op cmp;
      distance_type init = identity_element(accum);
      distance_type max = extreme_element(cmp);
      Algorithm algo(g, d, w, accum, cmp, init, max, visitor);

      algo(start);
    }

}

#endif //ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_BELLMAN_FORD
