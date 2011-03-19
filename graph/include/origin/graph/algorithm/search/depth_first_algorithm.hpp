// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_SEARCH_DEPTH_FIRST_ALGORITHM_HPP
#define ORIGIN_GRAPH_ALGORITHM_SEARCH_DEPTH_FIRST_ALGORITHM_HPP

#include <origin/graph/algorithm/search/depth_first_common.hpp>

namespace origin
{
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
  template<typename Graph, typename Visitor, typename Color_Label>
  class dfs_impl
  {
  public:
    typedef Visitor visitor_type;

    typedef Graph graph_type;
    typedef typename graph_traits<graph_type>::vertex vertex;
    typedef typename graph_traits<graph_type>::edge edge;
    typedef typename outward_graph_traits<graph_type>::range out_edge_range;

    typedef Color_Label color_label;
    typedef typename label_traits<color_label, vertex>::value_type color_type;
    typedef origin::color_traits<color_type> color_traits;

    // The vertex state stores the current search state for a vertex. This
    // includes the current iteration state of the vertex.
    typedef std::pair<vertex, out_edge_range> vertex_state;
    typedef std::stack<vertex_state> search_stack;

    dfs_impl(Graph& g, Visitor vis, Color_Label color)
      : graph(g), visitor(vis), stack(), color(color)
    { }

    void init()
    {
      for(auto v : graph.vertices()) {
        color(v) = color_traits::white();
        visitor.initialized_vertex(graph, v);
      }
    }

    // Perform search
    void operator()(vertex s)
    {
      color(s) = color_traits::gray();
      stack.push({s, out_edges(graph, s)});
      visitor.root_vertex(graph, s);
      visitor.discovered_vertex(graph, s);

      while(!stack.empty()) {
        vertex u = stack.top().first;
        out_edge_range rng = stack.top().second;
        stack.pop();
        visitor.started_vertex(graph, u);

        auto i = begin(rng), ie = end(rng);
        while(i != ie) {
          edge e = *i;
          visitor.started_edge(graph, e);
          vertex v = graph.target(e);

          if(color(v) == color_traits::white()) {
            visitor.tree_edge(graph, e);

            // Re-push the current vertex onto the queue so that we can
            // return to it after we descend through v. Not that the iteration
            // state is advanced.
            stack.push({u, out_edge_range{++i, ie}});

            // Make v the current vertex and
            u = v;
            color(u) = color_traits::gray();
            visitor.discovered_vertex(graph, u);

            // Swap the current iteration range with the that of the new
            // current vertex.
            rng = out_edges(graph, u);
            i = begin(rng);
            ie = end(rng);
          } else {
            // Determine if the edge is a back, forward, or cross edge.
            if(color(v) == color_traits::gray()) {
              visitor.back_edge(graph, e);
            } else {
              visitor.nontree_edge(graph, e);
            }

            // Don't forget to check the next vertex!
            ++i;
          }
        }

        color(u) = color_traits::black();
        visitor.finished_vertex(graph, u);
      }
    }

    graph_type& graph;
    visitor_type visitor;
    search_stack stack;
    color_label color;
  };

  // NOTE: This the Vertex_Label concept prameterized over another concept?
  // It looks like it might be. I think I may have found a motivating argument
  // for concept concept parameters as constraints. God forbid somebody try
  // use the technique for some kind of freaky adaptive structure. It could be
  // the case.

  /**
   * @ingroup graph_dfs
   * @class df_search_algo<Graph, Visitor, Color_Label>
   * @class df_search_algo<Graph, Visitor>
   *
   * The depth-first search algorithm object performs a depth first
   * traversal on all vertices connected to a single starting vertex. Only
   * vertices reachable from the start vertex are visited.
   *
   * An optional color label may be specified as the template parameter of the
   * algorithm object. If given, this type must be a label function associating
   * each vertex in the graph with a Color value. If the label is ommitted,
   * then the algorithm object will internally allocate its own association
   * and label function.
   *
   * @tparam Graph        A Graph type
   * @tparam Visitor      A Depth_First_Search_Visitor type
   * @tparam Color_Label  If given, a Label associating Graph's vertices with
   *                      a Three_Color
   */
  //@{
  template<typename Graph,
           typename Visitor,
           typename Color_Label = default_t>
  class dfs_algo
  {
  public:
    typedef Graph graph_type;
    typedef typename graph_traits<graph_type>::vertex vertex;
    typedef typename graph_traits<graph_type>::edge edge;
  protected:
    typedef Visitor visitor_type;
    typedef Color_Label color_label;
    typedef dfs_impl<graph_type, visitor_type, color_label> search_impl;
  public:
    typedef typename search_impl::color_traits color_traits;

    dfs_algo(Graph& g, Visitor vis, Color_Label color)
      : impl(g, vis, color)
    { }

    void operator()(vertex v)
    { impl(v); }

  protected:
    search_impl impl;
  };

  // This specialization uses a builtin color map.
  template<typename Graph, typename Visitor>
  class dfs_algo<Graph, Visitor, default_t>
  {
  public:
    typedef Graph graph_type;
    typedef typename graph_traits<graph_type>::vertex vertex;
    typedef typename graph_traits<graph_type>::edge edge;
  protected:
    typedef Visitor visitor_type;
    typedef vertex_map<graph_type, basic_color_t> color_map;
    typedef typename color_map::label_type color_label;
    typedef dfs_impl<Graph, Visitor, color_label> search_impl;
  public:
    typedef typename search_impl::color_traits color_traits;

    dfs_algo(Graph& g, Visitor vis)
      : colors(g), impl(g, vis, colors.label)
    { }

    void operator()(vertex v)
    { impl(v); }

  protected:
    color_map colors;
    search_impl impl;
  };
  //@}


  /**
   * @ingroup graph_dfs
   * @class df_traversal_algo<Graph, Visitor, Color_Label>
   * @class df_traversal_algo<Graph, Visitor>
   *
   * The depth first traversal algorithm object implements a depth first
   * search on each disconnected component of the graph. All vertices in the
   * graph are visited by this algorithm.
   *
   * An optional color label may be specified as the template parameter of the
   * algorithm object. If given, this type must be a label function associating
   * each vertex in the graph with a Color value. If the label is ommitted,
   * then the algorithm object will internally allocate its own association
   * and label function.
   *
   * @tparam Graph        An Outward_Graph type
   * @tparam Visitor      A Depth_First_Search_Visitor type
   * @tparam Color_Label  If given, a Label associating Graph's vertices with
   */
  template<typename Graph,
           typename Visitor,
           typename Color_Label = default_t>
  struct dft_algo
    : private dfs_algo<Graph, Visitor, Color_Label>
  {
    typedef dfs_algo<Graph, Visitor, Color_Label> base_type;

    typedef Visitor visitor_type;
    typedef Graph graph_type;
    typedef typename base_type::vertex vertex;
    typedef typename base_type::edge edge;
    typedef typename base_type::color_traits color_traits;

    dft_algo(Graph& g, Visitor vis)
      : base_type(g, vis)
    { }

    dft_algo(Graph& g, Visitor vis, Color_Label label)
      : base_type(g, vis, label)
    { }

    void operator()()
    {
      for(auto v : impl.graph.vertices()) {
        if(impl.color(v) == color_traits::white()) {
          base_type::operator()(v);
        }
      }
    }

  private:
    using base_type::impl;
  };

  /**
   * @fn depth_first_search(g, v, vis)
   * @fn depth_first_search(g, v, vis, color)
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
  template<typename Graph, typename Visitor>
  inline void
  depth_first_search(Graph& g, typename Graph::vertex v, Visitor vis)
  {
    dfs_algo<Graph, Visitor> algo(g, vis);
    algo(v);
  }

  // Const version of above.
  // FIXME: Should I be using graph_traits?
  template<typename Graph, typename Visitor>
  inline void
  depth_first_search(Graph const& g,
                       typename Graph::const_vertex v,
                       Visitor vis)
  {
    dfs_algo<Graph const, Visitor> algo(g, vis);
    algo(v);
  }

  // Color label version
  template<typename Graph, typename Visitor, typename Color_Label>
  inline void
  depth_first_search(Graph& g,
                      typename Graph::vertex v,
                      Visitor vis,
                      Color_Label color)
  {
    dfs_algo<Graph, Visitor, Color_Label> algo(g, vis, color);
    algo(v);
  }

  // Const version of above.
  template<typename Graph,
           typename Visitor,
           typename Color_Label>
  inline void
  depth_first_search(Graph const& g,
                       typename Graph::const_vertex v,
                       Visitor vis,
                       Color_Label color)
  {
    dfs_algo<Graph const, Visitor, Color_Label> algo(g, vis, color);
    algo(v);
  }
  //@}

  /**
   * @fn depth_first_traverse(g, vis)
   * @fn depth_first_traverse(g, vis, color)
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
  template<typename Graph, typename Visitor>
  inline void depth_first_traverse(Graph& g, Visitor vis)
  {
    dft_algo<Graph, Visitor> algo(g, vis);
    algo();
  }

  // Const version of above.
  template<typename Graph, typename Visitor>
  inline void depth_first_traverse(Graph const& g, Visitor vis)
  {
    dft_algo<Graph const, Visitor> algo(g, vis);
    algo();
  }

  // Color label variant
  template<typename Graph, typename Visitor, typename Color_Label>
  inline void depth_first_traverse(Graph& g, Visitor vis, Color_Label color)
  {
    dft_algo<Graph, Visitor, Color_Label> algo(g, vis, color);
    algo();
  }

  // Const version of above.
  template<typename Graph, typename Visitor, typename Color_Label>
  inline void depth_first_traverse(Graph const& g, Visitor vis, Color_Label color)
  {
    dft_algo<Graph const, Visitor, Color_Label> algo(g, vis, color);
    algo();
  }
  //@}

} // namespace origin

#endif