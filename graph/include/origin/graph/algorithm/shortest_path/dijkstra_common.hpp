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
    void discover_vertex(Graph const& g, Vertex v) { }

    /**
     * Called when a vertex is next in queue for the algorithm.
     */
    template<typename Graph, typename Vertex>
    void examine_vertex(Graph const& g, Vertex v) { }

    /**
     * Called when algorithm finishes relaxing a vertex's adjacenct vertices.
     */
    template<typename Graph, typename Vertex>
    void finish_vertex(Graph const& g, Vertex v) { }

    /**
     * Called when we assign a child v to the parent u,
     * meaning (g, parent, child).
     */
    template<typename Graph, typename Vertex>
    void parent(Graph const& g, Vertex u, Vertex v) { }

    /**
     * Called when we determine whether or not the algorithm will relax an edge.
     */
    template<typename Graph, typename Edge>
    void examine_edge(Graph const& g, Edge e) { }

    /**
     * Called when an edge was relaxed.
     */
    template<typename Graph, typename Edge>
    void edge_relaxed(Graph const& g, Edge e) { }

    /**
     * Called when an edge was not relaxed.
     */
    template<typename Graph, typename Edge>
    void edge_not_relaxed(Graph const& g, Edge e) { }
  };

  /**
   * The edge_weight function object abstracts the weight operation on a
   * Weighted graph.
   * 
   * @tparam Graph  A Weighted Graph
   */
  template<typename Graph>
    struct edge_weight
    {
      // FIXME: Do I need to formally specify the result type? I think the
      // idiom to do so is a nested result struct that is partially specialized
      // over the different function signatures.

      
      // FIXME: This needs to result in a reference. Does it?
      auto operator()(Graph& g, typename Graph::edge e) 
        -> decltype(weight(g, e))
      {
        return weight(g, e);
      }
      
      auto operator()(Graph const& g, typename Graph::const_edge e) const
        -> const decltype(weight(g, e))
      {
        return weight(g, e);
      }
    };

} // namespace origin

#endif //ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_DIJKSTRA_COMMON
