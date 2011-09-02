// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_SEARCH_DEPTH_FIRST_HPP
#define ORIGIN_GRAPH_ALGORITHM_SEARCH_DEPTH_FIRST_HPP

#include <stack>

#include <origin/graph/color.hpp>
#include <origin/graph/label.hpp>
#include <origin/graph/visitor.hpp>
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
     * Called after the vertex has been examined.
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
     * Called when an edge is determined to not be a back edge. A back edge
     * refers to a vertex that that is an ancestor in the search tree.
     */
    template<typename Graph, typename Edge>
      void back_edge(Graph& g, Edge e) 
      { }

    /**
     * Called when an edge is determined to be neither a tree edge nor a back
     * edge. The edge could be a forward edge (referring to a descendent in
     * the search tree) or a cross edge (referring to a vertex that is neither
     * an ancestor or descendant). The algortihm does not have sufficient
     * context to differentiate forward and cross edges.
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
   * @internal
   * @ingroup graph_dfs
   *
   * Implementation of the depth first search.
   *
   * @tparam Graph        An Outward_Graph type
   * @tparam Visitor      A Depth_First_Search_Visitor type
   * @tparam Color_Label  If given, a Label associating Graph's vertices with
   *                      a Three_Color
   *
   * @note The implementation of this algorithm follows the BGL implementation.
   * The search stack associates each vertex with its current iteration state.
   */
  template<typename Graph, typename Color_Label, typename Visitor>
  class dfs_algorithm
  {
  public:
    typedef Graph graph_type;
    typedef typename graph_traits<graph_type>::vertex vertex;
    typedef typename graph_traits<graph_type>::edge edge;
    
    typedef Color_Label color_label;
    typedef typename label_traits<color_label, vertex>::value_type color_type;
    typedef origin::color_traits<color_type> color_traits;
    
    typedef Visitor visitor_type;

  private:
    // The vertex state stores the current search state for a vertex. This
    // includes the current iteration state of the vertex.
    typedef decltype(out_edges(std::declval<Graph>(), std::declval<vertex>())) out_edge_range;
    typedef decltype(begin(std::declval<out_edge_range>())) out_edge_iterator;
    typedef std::pair<vertex, out_edge_range> vertex_state;
    typedef std::stack<vertex_state> search_stack;
  public:

    dfs_algorithm(graph_type& g, color_label c, visitor_type&& v)
      : graph(g), color{c}, vis(v), stack{}
    { 
      init_graph(); 
    }

    void init_graph()
    {
      for(auto v : vertices(graph)) {
        color(v) = color_traits::white();
        vis.initialized_vertex(graph, v);
      }
    }
    
    void init_tree(vertex v)
    {
      color(v) = color_traits::gray();
      stack.push({v, out_edges(graph, v)});
      vis.root_vertex(graph, v);
      vis.discovered_vertex(graph, v);
    }
    
    void examine_target(edge e)
    {
      vertex v = target(graph, e);
      if(color(v) == color_traits::white()) {
        vis.tree_edge(graph, e);

        // Re-push the current vertex onto the queue so that we can
        // return to it after we descend through v. Note that the iteration
        // state is advanced prior to re-pushing the state.
        stack.push({cur, out_edge_range{++iter, last}});

        // Make v the current vertex and visit it.
        cur = v;
        color(cur) = color_traits::gray();
        vis.discovered_vertex(graph, cur);

        // Swap the current iteration range with the that of the new
        // current vertex.
        auto rng = out_edges(graph, cur);
        iter = std::begin(rng);
        last = std::end(rng);
      } else {
        // Determine if the edge is a back, forward, or cross edge.
        if(color(v) == color_traits::gray())
          vis.back_edge(graph, e);
        else
          vis.nontree_edge(graph, e);
        
        // Don't forget to update the iterator.
        ++iter;
      }
    }
    
    // Search the incident edges of the current vertex for undiscovered tree 
    // edges.
    void search_vertex(vertex_state s)
    {
      using std::begin;
      using std::end;

      auto rng = s.second;
      iter = begin(rng);
      last = end(rng);
      while(iter != last) {
        edge e = *iter;
        vis.started_edge(graph, e);

        action const act = vis.examine_edge(graph, e);
        if(act == action::handle) {
          examine_target(e);
          vis.finished_edge(graph, e);
        } else {
          vis.finished_edge(graph, e);
          if(act == action::ignore) {
            ++iter;
            continue;
          } else
            break;
        }
      }
    }
    
    // Pop a vertex from the stack, setting it as the current vertex.
    vertex_state start_vertex()
    {
      vertex_state s = std::move(stack.top());
      stack.pop();
      cur = s.first;
      vis.started_vertex(graph, cur);
      return std::move(s);
    }
    
    void finish_vertex(vertex v)
    {
      color(v) = color_traits::black();
      vis.finished_vertex(graph, v);
    }

    // Perform a depth-first search rooted at the given vertex.
    void search_tree(vertex v)
    {
      init_tree(v);

      while(!stack.empty()) {
        vertex_state s = start_vertex();

        action const act = vis.examine_vertex(graph, s.first);
        if(act == action::handle) {
          search_vertex(s);
          finish_vertex(s.first);
        } else {
          finish_vertex(s.first);
          if(act == action::ignore)
            continue;
          else
            break;
        }
      }
    }

    // Perform a depth-first search on the entire graph.
    void search_graph()
    {
      for(auto v : vertices(graph)) {
        if(color(v) == color_traits::white())
          search_tree(v);
      }
    }
    
    // Perform a depth first search from the given vertex.
    void operator()(vertex v)
    {
      search_tree(v);
    }
    
    // Perform a depth first search on the graph.
    void operator()()
    {
      search_graph();
    }

    graph_type& graph;
    color_label color;
    visitor_type vis;
    search_stack stack;
  private:
    vertex cur;
    out_edge_iterator iter;
    out_edge_iterator last;
  };
  
  /**
   * @fn depth_first_search(g, v, vis, color)
   * @fn depth_first_search(g, v, vis)
   *
   * Perform a depth-first search on the graph starting from the given
   * vertex and using the given visitor. The color label, if specified records
   * the states of vertices during * traversal.
   *
   * @tparam Graph        An Outward_Graph type
   * @tparam Visitor      A Depth_First_Search_Visitor type
   * @tparam Color_Label  If given, a Label associating Graph's vertices with
   *                      a Three_Color.
   *
   * @param g       A Graph object
   * @param v       The vertex from which the search begins
   * @param vis     A Visitor object
   * @param color   A Color_Label
   */
  //@{
  template<typename Graph, typename Visitor, typename Color_Label>
    inline void depth_first_search(Graph& g,
                                   typename graph_traits<Graph>::vertex v,
                                   Color_Label color,
                                   Visitor&& vis)
    {
      dfs_algorithm<Graph, Color_Label, Visitor> algo(g, color, vis);
      algo(v);
    }

  template<typename Graph, typename Visitor>
    inline void depth_first_search(Graph& g, 
                                   typename graph_traits<Graph>::vertex v, 
                                   Visitor&& vis)
    {
      vertex_map<Graph, basic_color_t> c(g.order());
      depth_first_search(g, v, label(c), vis);
    }
  //@}

  /**
   * @fn depth_first_search_all(g, vis, color)
   * @fn depth_first_search_all(g, vis)
   *
   * Perform a depth-first traversal on the graph, visiting all vertices.
   * The color label, if specified, records the states of vertices during
   * traversal.
   *
   * @tparam Graph        A Graph type.
   * @tparam Visitor      A Breadth_First_Visitor type.
   * @tparam Color_Label  A Read_Write_Label that maps vertices to a Color
   *                      type supporting at least three colors.
   *
   * @param g       A Graph object.
   * @param v       The vertex from which the search begins.
   * @param vis     A visitor.
   * @param color   A color label.
   */
  //@{
  template<typename Graph, typename Color_Label, typename Visitor>
    inline void depth_first_search_all(Graph& g, Color_Label color, Visitor&& vis)
    {
      dfs_algorithm<Graph, Color_Label, Visitor> algo(g, color, vis);
      algo();
    }

  template<typename Graph, typename Visitor>
    inline void depth_first_search_all(Graph& g, Visitor&& vis)
    {
      vertex_map<Graph, basic_color_t> color;
      depth_first_search_all(g, label(color), vis);
    }
  //@}


  // Compute the distance from v to every other vertex, writing values to the
  // distance label dist.
  template<typename Graph, typename Distance_Label, typename Distance>
    void depth_first_distance(Graph& g, 
                              typename graph_traits<Graph>::vertex v,
                              Distance_Label dist,
                              Distance zero,
                              Distance inf)
    {
      depth_first_search(g, v, visit_distance(g, dist, zero, inf, dfs_visitor{}));
    }

  // Compute the distance from v to every other vertex in g, writing the
  // values to the output label dist.
  template<typename Graph, typename Distance_Label>
    void depth_first_distance(Graph& g, 
                              typename graph_traits<Graph>::vertex v,
                              Distance_Label dist)
    {
      depth_first_search(g, v, visit_distance(g, dist, dfs_visitor{}));
    }

  // Compute the depth first search tree for the graph rooted at the vertex v.
  template<typename Graph, typename Predecessor_Label>
    void depth_first_search_tree(Graph& g, 
                                 typename graph_traits<Graph>::vertex v,
                                 Predecessor_Label pred)
    {
      depth_first_search(g, v, visit_predecessor(g, pred, dfs_visitor{}));
    }

  // Compute teh depth first search tree.
  template<typename Graph, typename Predecessor_Label>
    void depth_first_search_forest(Graph& g, Predecessor_Label pred)
    {
      depth_first_search_all(g, visit_predecessor(g, pred, dfs_visitor{}));
    }

} // namespace origin

#endif
