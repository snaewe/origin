// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_DIJKSTRA_COMMON
#define ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_DIJKSTRA_COMMON

namespace origin {

  /**
   * @defgroup graph_shortest_path Dijkstra's Shortest Path
   *
   * [Text?]
   */

  /**
   * @ingroup graph_shortest_path
   *
   * The dijkstra visitor provides a callback interface for the algorithms and
   * range adaptors in this module.
   *
   * @note The visitor uses polymorphic visit functions to accomodate both
   * const and non-const visit instantiations. If the visiting algorithm is
   * instatiated over a const graph type, then the graph, vertex, and edge
   * parameters to each visit function will be const.
   */
  struct default_dijkstra_visitor
  {
    /**
     * Called after a vertex has been discovered.
     */
    template<typename Graph, typename Vertex>
    void discover_vertex(Graph& g, Vertex v) { }

    /**
     * Called when a vertex is next in queue for the algorithm.
     */
    template<typename Graph, typename Vertex>
    void examine_vertex(Graph& g, Vertex v) { }

    /**
     * Called when algorithm finishes relaxing a vertex's adjacenct vertices.
     */
    template<typename Graph, typename Vertex>
    void finish_vertex(Graph& g, Vertex v) { }

    /**
     * Called when we assign a child v to the parent u,
     * meaning (g, parent, child).
     */
    template<typename Graph, typename Vertex>
    void parent(Graph& g, Vertex u, Vertex v) { }

    /**
     * Called when we determine whether or not the algorithm will relax an edge.
     */
    template<typename Graph, typename Edge>
    void examine_edge(Graph& g, Edge e) { }

    /**
     * Called when an edge was relaxed.
     */
    template<typename Graph, typename Edge>
    void edge_relaxed(Graph& g, Edge e) { }

    /**
     * Called when an edge was not relaxed.
     */
    template<typename Graph, typename Edge>
    void edge_not_relaxed(Graph& g, Edge e) { }
  };

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

  /**
   * The default weight labeling. This assumes that the edge values are the
   * weight values.
   * FIXME This is begging for a concept
   */
  template<typename Graph>
  struct default_weight_label
  {
  private:
    typedef typename Graph::edge_value_type distance_type;
    typedef typename Graph::edge edge;

  public:
    distance_type operator()(Graph const& g, edge e)
    {
      return g[e];
    }
  };

} // namespace origin

#endif //ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_DIJKSTRA_COMMON
