// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_SEARCH_BREADTH_FIRST_HPP
#define ORIGIN_GRAPH_ALGORITHM_SEARCH_BREADTH_FIRST_HPP

#include <queue>

#include <origin/graph/color.hpp>
#include <origin/graph/label.hpp>
#include <origin/graph/visitor.hpp>
#include <origin/graph/edge.hpp>

namespace origin
{
  /** 
   * @defgroup bfs Breadth First Search
   * @ingroup graph_search
   */

  /**
   * @ingroup bfs
   * 
   * The breadth first search visitor provides a callback interface for
   * the algorithms and ranges adaptors in this module.
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

  /**
   * @ingroup bfs
   * 
   * The bfs algorithm object implements a breadth first search over a graph.
   */
  template<typename Graph, typename Color_Label, typename Visitor>
    class bfs_algo
    {
    public:
      typedef typename graph_traits<Graph>::vertex vertex;
      typedef typename graph_traits<Graph>::edge edge;
      
      typedef typename label_traits<Color_Label, vertex>::value_type color_type;
      typedef color_traits<color_type> colors;
      
      bfs_algo(Graph& g, Color_Label c, Visitor& v)
        : graph(g), color(c), vis(v)
      {
        init_graph();
      }

      // Initialize the graph being searched. color_type all of the vertices white.
      void init_graph()
      {
        for(auto v : graph.vertices()) {
          color(v) = colors::white();
          vis.initialized_vertex(graph, v);
        }
      }

      void init_tree(vertex v)
      {
        color(v) = colors::gray();
        queue.push(v);
        vis.root_vertex(graph, v);
        vis.discovered_vertex(graph, v);
      }

      void examine_target(edge e)
      {
        vertex v = graph.target(e);
        if(color(v) == colors::white()) {
          vis.tree_edge(graph, e);
          color(v) = colors::gray();
          vis.discovered_vertex(graph, v);
          queue.push(v);
        } else {
          vis.nontree_edge(graph, e);
        }
      }

      void search_vertex(vertex v)
      {
        for(edge e : out_edges(graph, v)) {
          vis.started_edge(graph, e);

          // What action should we take for the vertex?
          action const act = vis.examine_edge(graph, e);
          if(act == action::handle) {
            // Examine the vertex for new edges/vertices
            examine_target(e);
            vis.finished_edge(graph, e);
          } else {
            // Either skip the edge or finish iterating over edges.
            vis.finished_edge(graph, e);
            if(act == action::ignore)
              continue;
            else
              break;
          }
        }
      }

      // Pop a vertex from the queue, returning it.
      vertex start_vertex()
      {
        vertex v = queue.front();
        queue.pop();
        vis.started_vertex(graph, v);
        return v;
      }

      // Indicate that the vertex has been searched.
      void finish_vertex(vertex v)
      {
        color(v) = colors::black();
        vis.finished_vertex(graph, v);
      }

      // Search the graph rooted at the tree s.
      void search_tree(vertex s)
      {
        init_tree(s);

        while(!queue.empty()) {
          vertex v = start_vertex();

          // What action should we take for the vertex?
          action const act = vis.examine_vertex(graph, v);
          if(act == action::handle) {
            search_vertex(v);
            finish_vertex(v);
          } else {
            finish_vertex(v);
            if(act == action::ignore)
              continue;
            else
              break;
          }
        }
      }
      
      // Execute a search on the entire graph.
      void search_graph()
      {
        for(vertex v : graph) {
          if(color(v) == colors::white()) {
            search_tree(v);
            
            // Examine (the root of) the tree that we've just searched. If
            // the algorithm accepts it, break.
            if(examine_tree(v) == action::accept)
              break;
          }
        }
      }

      void operator()(vertex v)
      {
        search_tree(v);
      }
      
      void operator()()
      {
        search_graph();
      }

      Graph& graph;
      std::queue<vertex> queue;
      Color_Label color;
      Visitor& vis;
    };

  /**
   * @ingroup bfs
   * @fn breadth_first_search(g, v, vis)
   * @fn breadth_first_search(g, v, vis, color)
   *
   * Perform a breadth-first search on the graph starting from the given
   * vertex and using the given visitor. The color label, if specified records
   * the states of vertices during * traversal.
   *
   * @tparam Graph        A Graph type.
   * @tparam Visitor      A Breadth_First_Visitor type.
   * @tparam Color_Label  A Read_Write_Label that maps vertices to a color_type
   *                      type supporting at least three colors.
   *
   * @param g       A Graph object.
   * @param v       The vertex from which the search begins.
   * @param vis     A visitor.
   * @param color   A color label.
   */
  //@{
  template<typename Graph, typename Color_Label, typename Visitor>
    inline void breadth_first_search(Graph& g, 
                                     typename graph_traits<Graph>::vertex v, 
                                     Color_Label color,
                                     Visitor& vis)
    {
      bfs_algo<Graph, decltype(color), Visitor> algo(g, color, vis);
      algo(v);
    }

  template<typename Graph, typename Visitor>
    inline void breadth_first_search(Graph& g, 
                                     typename graph_traits<Graph>::vertex v, 
                                     Visitor& vis)
    {
      vertex_map<Graph, basic_color_t> c(g.order());
      breadth_first_search(g, v, label(c), vis);
    }
  //@}

  /**
   * @ingroup bfs
   * @fn breadth_first_search_all(g, vis)
   * @fn breadth_first_search_all(g, vis, color)
   *
   * Perform a breadth-first search on the graph, visiting all vertices. The 
   * color label, if specified, records the states of vertices during
   * traversal.
   *
   * @tparam Graph        A Graph type.
   * @tparam Visitor      A Breadth_First_Visitor type.
   * @tparam Color_Label  A Read_Write_Label that maps vertices to a color_type
   *                      type supporting at least three colors.
   *
   * @param g       A Graph object
   * @param vis     A visitor
   * @param color   A color label
   */
  template<typename Graph, typename Color_Label, typename Visitor>
    inline void breadth_first_search_all(Graph& g, Color_Label color, Visitor& vis)
    {
      bfs_algo<Graph, Color_Label, Visitor> algo(g, color, vis);
      algo();
    }
  
  template<typename Graph, typename Visitor>
    inline void breadth_first_search_all(Graph& g, Visitor& vis)
    {
      vertex_map<Graph, basic_color_t> c(g.order());
      breadth_first_search_all(g, label(c), vis);
    }

  /**
   * @fn breadth_first_distance(g, v, dist)
   * @fn breadth_first_distance(g, v, dist, zero, inf)
   * 
   * Compute the distance between the given vertex and all other reachable 
   * vertices in the graph. Distance is counted as the number of edges that 
   * need to be traversed in order to reach another vertex.
   * 
   * @tparam Graph            A Graph
   * @tparam Distance_Label   A Mutable_Label
   */
  template<typename Graph, typename Distance_Label, typename Distance>
    void breadth_first_distance(Graph& g, 
                                typename graph_traits<Graph>::vertex v,
                                Distance_Label dist,
                                Distance zero,
                                Distance inf)
    {
      typedef distance_visitor<Graph, Distance_Label, bfs_visitor> Visitor;
      Visitor vis(dist, zero, inf);
      breadth_first_search(g, v, vis);
    }

  template<typename Graph, typename Distance_Label>
    void breadth_first_distance(Graph& g, 
                                typename graph_traits<Graph>::vertex v,
                                Distance_Label dist)
    {
      typedef typename graph_traits<Graph>::vertex Vertex;
      typedef typename label_traits<Distance_Label, Vertex>::value_type Distance;

      Distance zero = 0;
      Distance inf = std::numeric_limits<Distance>::max();
      breadth_first_distance(g, v, dist, zero, inf);
    }
    
  /**
   * @fn breadth_first_search_tree(g, v, par)
   * @fn breadth_first_search_forest(g, par)
   * 
   * Compute the distance between the given vertex and all other reachable 
   * vertices in the graph. Distance is counted as the number of edges that 
   * need to be traversed in order to reach another vertex.
   * 
   * @tparam Graph            A Graph
   * @tparam Parent_Label     A Mutable_Label
   */
  template<typename Graph, typename Parent_Label>
    void breadth_first_search_tree(Graph& g, 
                                   typename graph_traits<Graph>::vertex v,
                                   Parent_Label par)
    {
      typedef parent_visitor<Graph, Parent_Label, bfs_visitor> Visitor;
      Visitor vis(par);
      breadth_first_search(g, v, vis);
    }

  template<typename Graph, typename Parent_Label>
    void breadth_first_search_forest(Graph& g, Parent_Label par)
    {
      typedef parent_visitor<Graph, Parent_Label, bfs_visitor> Visitor;
      Visitor vis(par);
      breadth_first_search_all(g, vis);
    }


} // namespace origin

#endif
