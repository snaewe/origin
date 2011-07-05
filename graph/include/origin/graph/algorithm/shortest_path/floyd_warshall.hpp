// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_FLOYD_WARSHALL
#define ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_FLOYD_WARSHALL

#include <origin/graph/traits.hpp>

#include <origin/graph/algorithm/shortest_path/floyd_warshall_common.hpp>

namespace origin {

  /**
   * Returns all pairs shortest path in the form of a distance label.
   *
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
   * FIXME Replace float with result_of
   * TODO Put in static asserts!
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
      typedef typename std::result_of<Weight_Label(edge)>::type distance_type;

      // Assert static requirements
      //assert that distance_mtx conforms, etc...

      floyd_warshall_impl(Graph const& g,
                          Distance_Matrix& d,
                          Weight_Label w,
                          Weight_Accumulator acc,
                          Weight_Compare cmp,
                          distance_type max,
                          distance_type zero)
        : g_(g), d_(d), w_(w), acc_(acc), cmp_(cmp), max_(max), zero_(zero)
      { }

      void initialize_matrix()
      {
        for(auto u : g_.vertices())
          for(auto v : g_.vertices())
            d_[u][v] = max_;

        // Is this really necessary? What about loops?
        // Oh. Negative loops do strange things to this...
        for(auto v : g_.vertices())
          d_[v][v] = zero_;

        // Set distance matrix to edge_weights
        for(auto e : g_.vertices()) {
          if(d_[g_.source(e)][g_.target(e)] < max_)
            d_[g_.source(e)][g_.target(e)] =
              min(w_(e), d_[g_.source(e)][g_.target(e)], cmp_);
          else
            d_[g_.source(e)][g_.target(e)] = w_(e);
        }

        // If g is undirected, for each u,v in edges(g), we must also account
        // for v,u.
        bool g_is_undirected = is_undirected_graph<Graph>::type::value;
        if(g_is_undirected) {
          for(auto e : g_.vertices()) {
            if(d_[g_.target(e)][g_.source(e)] < max_)
              d_[g_.target(e)][g_.source(e)] =
                min(w_(e), d_[g_.target(e)][g_.source(e)], cmp_);
            else
              d_[g_.target(e)][g_.source(e)] = w_(e);
          }
        }
      }

      void operator()(void)
      {
        for(auto i : g_.vertices())
          for(auto j : g_.vertices())
            if(d_[j][i] < max_)
              for(auto k : g_.vertices())
                if(d_[i][k] < max_){
                  distance_type dist = acc_(d_[j][i], d_[i][k]);
                  if(d_[j][k] < dist)
                    d_[j][k] = dist;
                }

        // No negative weight loops can be in the graph
        for(auto v : g_.vertices())
        { assert(( !cmp_(d_[v][v], zero_) )); }
      }

    private:
      Graph const& g_;
      Distance_Matrix& d_;
      Weight_Label w_;
      Weight_Accumulator acc_;
      Weight_Compare cmp_;
      distance_type max_;
      distance_type zero_;
    };

} // namespace origin

#endif // ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_FLOYD_WARSHALL
