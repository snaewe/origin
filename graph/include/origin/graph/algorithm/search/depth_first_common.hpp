// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_SEARCH_DEPTH_FIRST_COMMON_HPP
#define ORIGIN_GRAPH_ALGORITHM_SEARCH_DEPTH_FIRST_COMMON_HPP

#include <stack>

#include <origin/graph/color.hpp>
#include <origin/graph/label.hpp>
#include <origin/graph/edge.hpp>

namespace origin
{
  /**
   * @defgroup graph_dfs Depth First Search
   *
   * The depth-first search module provides two depth-first search frameworks.
   * The first is a standard visitor-based algorithm implementation, and the
   * second is a range-based implementation.
   */

  /**
   * @ingroup graph_dfs
   *
   * The depth first search visitor provides a callback interface for
   * the algorithms and range adaptors in this module.
   *
   * @note The visitor uses polymorphic visit functions to accomodate both
   * const and non-const visit instantiations. If the visiting algorithm is
   * instatiated over a const graph type, then the graph, vertex, and edge
   * parameters to each visit function will be const.
   */
  struct dfs_visitor
  {
    /**
     * Called after a vertex has been initialized.
     */
    template<typename Graph, typename Vertex>
    void initialized_vertex(Graph& g, Vertex v) { }

    /**
     * Called after a vertex has been discovered.
     */
    template<typename Graph, typename Vertex>
    void discovered_vertex(Graph& g, Vertex v) { }

    /**
     * Called after a vertex has been popped from the queue and before its
     * incident edges have been examined.
     */
    template<typename Graph, typename Vertex>
    void started_vertex(Graph& g, Vertex v) { }

    /**
     * Called after the vertex has been examined.
     */
    template<typename Graph, typename Vertex>
    void finished_vertex(Graph& g, Vertex v) { }

    /**
     * Called when a new search tree root is encountered. This is called
     * before the vertex is discovered, allowing derived visitors to perform
     * search-tree initialization before the discovery of the root vertex.
     */
    template<typename Graph, typename Vertex>
    void root_vertex(Graph& g, Vertex v) { }

    /**
     * Called before an incident edge is examined.
     */
    template<typename Graph, typename Edge>
    void started_edge(Graph& g, Edge e) { }

    /**
     * Called when an edge is determined to be in the search tree. Occurs
     * just before the target vertex is discovered.
     */
    template<typename Graph, typename Edge>
    void tree_edge(Graph& g, Edge e) { }

    /**
     * Called when an edge is determined to not be a back edge. A back edge
     * refers to a vertex that that is an ancestor in the search tree.
     */
    template<typename Graph, typename Edge>
    void back_edge(Graph& g, Edge e) { }

    /**
     * Called when an edge is determined to be neither a tree edge nor a back
     * edge. The edge could be a forward edge (referring to a descendent in
     * the search tree) or a cross edge (referring to a vertex that is neither
     * an ancestor or descendant). The algoortihm does not have sufficient
     * context to differentiate forward and cross edges.
     */
    template<typename Graph, typename Edge>
    void nontree_edge(Graph& g, Edge e) { }
  };

} // namespace origin

#endif