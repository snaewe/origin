// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_FLOYD_WARSHALL
#define ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_FLOYD_WARSHALL

#include<iostream>

#include <cassert>

#include <origin/graph/traits.hpp>
#include <origin/functional.hpp>

#include <origin/graph/algorithm/shortest_path/floyd_warshall_common.hpp>
#include <origin/graph/algorithm/shortest_path/shortest_path_common.hpp>

namespace origin {

  /**
   * Returns all pairs shortest paths in the form of a matrix.
   *
   * @tparam Graph  A Graph
   *
   * @tparam Distance_Matrix  A matrix that stores shortest path information.
   * 
   * @tparam Weight_Label     A Readable edge Label that assocates a weight
   * with every edge.
   * 
   * @tparam Weight_Accum     An associative binary operation that accumulates
   * edge weights.
   * 
   * @tparam Weight_Compare   A Strict_Weak_Order over the graph's edge weights
   *
   * TODO Put in static asserts!
   * TODO Clamp the accumulator, compare, etc.
   */
  template<typename Graph,
           typename Distance_Matrix,
           typename Weight_Label,
           typename Weight_Accumulator,
           typename Weight_Compare>
    class floyd_warshall_impl
    {
    public:
      // Graph types
      //typedef typename graph_traits<Graph>::vertex_iterator vertex_iterator;
      typedef typename graph_traits<Graph>::edge edge;

      // Associated labels?

      // Other
      typedef typename std::result_of<Weight_Label(Graph, edge)>::type distance_type;

      // Assert static requirements
      //assert that distance_mtx conforms, etc...

      floyd_warshall_impl(Graph const& g,
                          Distance_Matrix& d,
                          Weight_Label w,
                          Weight_Accumulator acc,
                          Weight_Compare cmp,
                          distance_type zero,
                          distance_type max)
        : g_(g), d_(d), w_(w), acc_(acc), cmp_(cmp), zero_(zero), max_(max)
      { }

      void initialize_matrix()
      {
        for(auto u : g_.vertices())
          for(auto v : g_.vertices())
            d_(u,v) = max_;

        for(auto v : g_.vertices())
          d_(v,v) = zero_;

        // Set distance matrix to edge_weights
        for(auto e : g_.edges()) {
          if(d_(g_.source(e),g_.target(e)) < max_)
            d_(g_.source(e),g_.target(e)) =
              min(w_(g_, e), d_(g_.source(e),g_.target(e)), cmp_);
          else
            d_(g_.source(e),g_.target(e)) = w_(g_, e);
        }

        // If g is undirected, for each u,v in edges(g), we must also account
        // for v,u.
        if(is_undirected_graph<Graph>::type::value) {
          for(auto e : g_.edges()) {
            if(d_(g_.target(e),g_.source(e)) < max_)
              d_(g_.target(e),g_.source(e)) =
                min(w_(g_, e), d_(g_.target(e),g_.source(e)), cmp_);
            else
              d_(g_.target(e),g_.source(e)) = w_(g_, e);
          }
        }
      }

      void operator()(void)
      {
        for(auto i : g_.vertices())
          for(auto j : g_.vertices())
            if(d_(j,i) < max_)
              for(auto k : g_.vertices())
                if(d_(i,k) < max_){
                  distance_type dist = acc_(d_(j,i), d_(i,k));
                  if(dist < d_(j,k))
                    d_(j,k) = dist;
                }

        // No negative weight loops may be in the graph
        for(auto v : g_.vertices())
        { assert(( !cmp_(d_(v,v), zero_) )); }
      }

    private:
      Graph const& g_;
      Distance_Matrix& d_;
      Weight_Label w_;
      Weight_Accumulator acc_;
      Weight_Compare cmp_;
      distance_type zero_;
      distance_type max_;
    };

  /**
   * Floyd-Warshall algorithm with uninitialized matrix.
   */
  template<typename Graph, typename Distance_Matrix>
    void all_pairs_shortest_paths(Graph const& g, Distance_Matrix& d)
    {
      typedef typename Distance_Matrix::value_type distance_type;
      typedef detail::edge_weight<Graph> edge_weight_label;
      typedef std::plus<distance_type> accumulate_type;
      typedef std::less<distance_type> compare_type;
      typedef floyd_warshall_impl<
        Graph, Distance_Matrix, edge_weight_label, accumulate_type, compare_type
      > Algorithm;

      edge_weight_label weight;
      accumulate_type accumulate;
      compare_type compare;
      distance_type zero = identity_element(accumulate);
      distance_type maximum = extreme_element(compare);

      Algorithm algo{g, d, weight, accumulate, compare, zero, maximum};
      algo.initialize_matrix();
      algo();
    }

} // namespace origin

#endif // ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_FLOYD_WARSHALL
