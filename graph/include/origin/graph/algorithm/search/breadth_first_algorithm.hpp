// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_SEARCH_BREADTH_FIRST_ALGORITHM_HPP
#define ORIGIN_GRAPH_ALGORITHM_SEARCH_BREADTH_FIRST_ALGORITHM_HPP

#include <origin/graph/algorithm/search/breadth_first_common.hpp>

namespace origin
{
  /**
   * @internal
   * @ingroup graph_bfs
   *
   * Implementation of the breadth first search.
   *
   * @tparam Graph        A Graph type.
   * @tparam Visitor      A Search_Visitor type.
   * @tparam Color_Label  A label function associating a vertex with a Color
   *                      value.
   */
  template<typename Graph, typename Visitor, typename Color_Label>
  class bfs_impl
  {
  public:
    typedef Visitor visitor_type;

    typedef Graph graph_type;
    typedef typename graph_traits<graph_type>::vertex vertex;
    typedef typename graph_traits<graph_type>::edge edge;

    typedef Color_Label color_label;
    typedef typename label_traits<color_label, vertex>::value_type color_type;
    typedef origin::color_traits<color_type> color_traits;

    typedef std::queue<vertex> search_queue;

    bfs_impl(Graph& g, Visitor vis, Color_Label color)
      : graph(g), visitor(vis), queue(), color(color)
    { }

    void init()
    {
      for(auto v : graph.vertices()) {
        color(v) = color_traits::white();
        visitor.initialized_vertex(graph, v);
      }
    }

    // Perform search
    void operator()(vertex v)
    {
      color(v) = color_traits::gray();
      queue.push(v);
      visitor.root_vertex(graph, v);
      visitor.discovered_vertex(graph, v);

      while(!queue.empty()) {
        vertex u = queue.front();
        queue.pop();
        visitor.started_vertex(graph, u);

        for(edge e : out_edges(graph, u)) {
          visitor.started_edge(graph, e);
          vertex v = graph.target(e);

          if(color(v) == color_traits::white()) {
            color(v) = color_traits::gray();
            queue.push(v);
            visitor.tree_edge(graph, e);
            visitor.discovered_vertex(graph, v);
          } else {
            visitor.nontree_edge(graph, e);
          }
        }

        color(v) = color_traits::black();
        visitor.finished_vertex(graph, u);
      }
    }

    graph_type& graph;
    visitor_type visitor;
    search_queue queue;
    color_label color;
  };

  // NOTE: This the Vertex_Label concept prameterized over another concept?
  // It looks like it might be. I think I may have found a motivating argument
  // for concept concept parameters as constraints. God forbid somebody try
  // use the technique for some kind of freaky adaptive structure. It could be
  // the case.

  /**
   * @ingroup graph_bfs
   * @class bf_search_algo<Graph, Visitor, Color_Label>
   * @class bf_search_algo<Graph, Visitor>
   *
   * The breadth-first search algorithm object performs a breadth first
   * traversal on all vertices connected to a single starting vertex. Only
   * vertices reachable from the start vertex are visited.
   *
   * An optional color label may be specified as the template parameter of the
   * algorithm object. If given, this type must be a label function associating
   * each vertex in the graph with a Color value. If the label is ommitted,
   * then the algorithm object will internally allocate its own association
   * and label function.
   *
   * @tparam Graph        A Graph type.
   * @tparam Visitor      A Search_Visitor type.
   * @tparam Color_Label  If given, a Vertex_Label<Graph, Color> type.
   */
  //@{
  template<typename Graph,
           typename Visitor,
           typename Color_Label = default_t>
  class bfs_algo
  {
  public:
    typedef Graph graph_type;
    typedef typename graph_traits<graph_type>::vertex vertex;
    typedef typename graph_traits<graph_type>::edge edge;
  protected:
    typedef Visitor visitor_type;
    typedef Color_Label color_label;
    typedef bfs_impl<graph_type, visitor_type, color_label> search_impl;
  public:
    typedef typename search_impl::color_traits color_traits;

    bfs_algo(Graph& g, Visitor vis, Color_Label color)
      : impl(g, vis, color)
    { }

    void operator()(vertex v)
    { impl(v); }

  protected:
    search_impl impl;
  };

  // This specialization uses a builtin color map.
  template<typename Graph, typename Visitor>
  class bfs_algo<Graph, Visitor, default_t>
  {
  public:
    typedef Graph graph_type;
    typedef typename graph_traits<graph_type>::vertex vertex;
    typedef typename graph_traits<graph_type>::edge edge;
  protected:
    typedef Visitor visitor_type;
    typedef vertex_map<graph_type, basic_color_t> color_map;
    typedef typename color_map::label_type color_label;
    typedef bfs_impl<Graph, Visitor, color_label> search_impl;
  public:
    typedef typename search_impl::color_traits color_traits;

    bfs_algo(Graph& g, Visitor vis)
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
   * @ingroup graph_bfs
   * @class bf_traversal_algo<Graph, Visitor, Color_Label>
   * @class bf_traversal_algo<Graph, Visitor>
   *
   * The breadth first traversal algorithm object implements a breadth first
   * search on each disconnected component of the graph. All vertices in the
   * graph are visited by this algorithm.
   *
   * An optional color label may be specified as the template parameter of the
   * algorithm object. If given, this type must be a label function associating
   * each vertex in the graph with a Color value. If the label is ommitted,
   * then the algorithm object will internally allocate its own association
   * and label function.
   */
  template<typename Graph,
           typename Visitor,
           typename Color_Label = default_t>
  struct bft_algo
    : private bfs_algo<Graph, Visitor, Color_Label>
  {
    typedef bfs_algo<Graph, Visitor, Color_Label> base_type;

    typedef Visitor visitor_type;
    typedef Graph graph_type;
    typedef typename base_type::vertex vertex;
    typedef typename base_type::edge edge;
    typedef typename base_type::color_traits color_traits;

    bft_algo(Graph& g, Visitor vis)
      : base_type(g, vis)
    { }

    bft_algo(Graph& g, Visitor vis, Color_Label label)
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
   * @fn breadth_first_search(g, v, vis)
   * @fn breadth_first_search(g, v, vis, color)
   *
   * Perform a breadth-first search on the graph starting from the given
   * vertex and using the given visitor. The color label, if specified records
   * the states of vertices during * traversal.
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
  inline void
  breadth_first_search(Graph& g, typename Graph::vertex v, Visitor vis)
  {
    bfs_algo<Graph, Visitor> algo(g, vis);
    algo(v);
  }

  // Const version of above.
  // FIXME: Should I be using graph_traits?
  template<typename Graph, typename Visitor>
  inline void
  breadth_first_search(Graph const& g,
                       typename Graph::const_vertex v,
                       Visitor vis)
  {
    bfs_algo<Graph const, Visitor> algo(g, vis);
    algo(v);
  }

  // Color label version
  template<typename Graph, typename Visitor, typename Color_Label>
  inline void
  breadth_first_search(Graph& g,
                      typename Graph::vertex v,
                      Visitor vis,
                      Color_Label color)
  {
    bfs_algo<Graph, Visitor, Color_Label> algo(g, vis, color);
    algo(v);
  }

  // Const version of above.
  template<typename Graph,
           typename Visitor,
           typename Color_Label>
  inline void
  breadth_first_search(Graph const& g,
                       typename Graph::const_vertex v,
                       Visitor vis,
                       Color_Label color)
  {
    bfs_algo<Graph const, Visitor, Color_Label> algo(g, vis, color);
    algo(v);
  }
  //@}

  /**
   * @fn breadth_first_traverse(g, vis)
   * @fn breadth_first_traverse(g, vis, color)
   *
   * Perform a breadth-first traversal on the graph, visiting all vertices.
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
  inline void breadth_first_traverse(Graph& g, Visitor vis)
  {
    bft_algo<Graph, Visitor> algo(g, vis);
    algo();
  }

  // Const version of above.
  template<typename Graph, typename Visitor>
  inline void breadth_first_traverse(Graph const& g, Visitor vis)
  {
    bft_algo<Graph const, Visitor> algo(g, vis);
    algo();
  }

  // Color label variant
  template<typename Graph, typename Visitor, typename Color_Label>
  inline void breadth_first_traverse(Graph& g, Visitor vis, Color_Label color)
  {
    bft_algo<Graph, Visitor, Color_Label> algo(g, vis, color);
    algo();
  }

  // Const version of above.
  template<typename Graph, typename Visitor, typename Color_Label>
  inline void breadth_first_traverse(Graph const& g, Visitor vis, Color_Label color)
  {
    bft_algo<Graph const, Visitor, Color_Label> algo(g, vis, color);
    algo();
  }
  //@}

} // namespace origin

#endif