// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_SEARCH_BREADTH_FIRST_COMMON_HPP
#define ORIGIN_GRAPH_ALGORITHM_SEARCH_BREADTH_FIRST_COMMON_HPP

#include <queue>

#include <origin/graph/color.hpp>
#include <origin/graph/label.hpp>
#include <origin/graph/edge.hpp>

namespace origin
{
  /**
   * @defgroup graph_bfs Breadth First Search
   *
   * The breadth-first search module provides two breadth-first search
   * frameworks. The first is a standard visitor-based algorithm implementation.
   * The second is a range-based implementation.
   *
   * This module encompasses both search and traversal methods. A search
   * is rooted at a single vertex and visits all connected vertices. A
   * traversal starts at an arbitrary vertex and visits all vertices in the
   * graph, regardless of whether or not they are connected. The following
   * abbreviations are used in this module:, bfs (search) and bft (traversal).
   */

  /**
   * The action enumeration defines a set of actions that can be returned by
   * observers in order to influence the control of an algorithm.
   */
  enum class action 
  {
    /**
     * Process the vertex or edge in the usual way.
     */
    handle,
    
    /**
     * Ignore the vertex or edge and continue processing as normal. This 
     * action is typically used to reduce the search space by eliminating
     * paths.
     */
    ignore,
    
    /**
     * Accept the vertex or edge as a best among candidates. This typically
     * indicates a terminating condition for the algorithm.
     */
    accept
  };
  
  /**
   * @ingroup graph_bfs
   * The breadth first search visitor provides a callback interface for
   * the algorithms and range adaptors in this module.
   *
   * @note The visitor uses polymorphic visit functions to accomodate both
   * const and non-const visit instantiations. If the visiting algorithm is
   * instatiated over a const graph type, then the graph, vertex, and edge
   * parameters to each visit function will be const.
   */
  struct bfs_visitor
  {
    /** @name Events */
    //@{
    /**
     * Called after a vertex has been initialized.
     */
    template<typename Graph, typename Vertex>
      void initialized_vertex(Graph& g, Vertex v) 
      { }

    /**
     * Called after a vertex has been discovered.
     */
    template<typename Graph, typename Vertex>
      void discovered_vertex(Graph& g, Vertex v) 
      { }

    /**
     * Called after a vertex has been popped from the queue and before its
     * incident edges have been examined.
     */
    template<typename Graph, typename Vertex>
      void started_vertex(Graph& g, Vertex v) 
      { }

    /**
     * Called after the vertex has been examined and possibly ignored or
     * accepted.
     */
    template<typename Graph, typename Vertex>
      void finished_vertex(Graph& g, Vertex v) 
      { }

    /**
     * Called before an incident edge is examined.
     */
    template<typename Graph, typename Edge>
      void started_edge(Graph& g, Edge e) 
      { }
    
    /**
     * Called after the edge has been examined, and possibly ignored or 
     * accepted.
     */
    template<typename Graph, typename Edge>
      void finished_edge(Graph& g, Edge e)
      { }
    //@}

    /** @name Classifiers */
    //@{
    /**
     * Called when a new search tree root is encountered. This is called
     * before the vertex is discovered, allowing derived visitors to perform
     * search-tree initialization before the discovery of the root vertex.
     */
    template<typename Graph, typename Vertex>
      void root_vertex(Graph& g, Vertex v) 
      { }

    /**
     * Called when an edge is determined to be in the search tree. Occurs
     * just before the target vertex is discovered.
     */
    template<typename Graph, typename Edge>
      void tree_edge(Graph& g, Edge e) 
      { }

    /**
     * Called when an edge is determined to not be in the search tree.
     */
    template<typename Graph, typename Edge>
      void nontree_edge(Graph& g, Edge e) 
      { }
    //@}

    /** @name Observers */
    //@{
    /**
     * Called to determine the action to be taken for the given vertex. 
     * Corresponding actions are:
     *  - handle - The vertex is searched
     *  - ignore - The vertex is not searched
     *  - accept - The vertex is not searched and the search of the current
     *             tree is terminated.
     * The default implementation returns handle.
     */
    template<typename Graph, typename Vertex>
      constexpr action examine_vertex(Graph& g, Vertex v)
      {
        return action::handle;
      }
    
    /**
     * Called to determine the action to be taken for the given edge. 
     * Corresponding actions are:
     *  - handle - The edge is examined
     *  - ignore - The edge is not examined
     *  - accept - The edge is not examined and edge traversal is terminated
     * The default implementation returns handle.
     */
    template<typename Graph, typename Edge>
      constexpr action examine_edge(Graph& g, Edge e)
      {
        return action::handle;
      }

    /**
     * Called to determine the action to be taken for the root of the given
     * search tree. This observer is only called when performing a search on
     * the entire graph. Corresponding actions are:
     *  - handle - No action is taken
     *  - ignore - No action is taken
     *  - accept - The search is terminated
     * The default implementation returns handle.
     */
    template<typename Graph, typename Vertex>
      constexpr action examine_tree(Graph& g, Vertex v)
      {
        return action::handle;
      }
    //@}
  };

} // namespace origin

#endif