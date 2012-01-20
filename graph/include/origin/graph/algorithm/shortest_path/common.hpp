// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_COMMON
#define ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_COMMON

namespace origin {

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
   * Default Bellman-Ford visitor
   */
  class default_bellman_ford_visitor
  {
  public:
    template<typename Graph, typename Edge>
    void examine_edge(Graph const&, Edge)
    { }
    
    template<typename Graph, typename Edge>
    void edge_relaxed(Graph const&, Edge)
    { }
    
    template<typename Graph, typename Edge>
    void edge_not_relaxed(Graph const&, Edge)
    { }
    
    template<typename Graph, typename Edge>
    void edge_minimized(Graph const&, Edge)
    { }
    
    template<typename Graph, typename Edge>
    void edge_not_minimized(Graph const&, Edge)
    { }
  };

  
  
}

#endif
