// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_SEARCH_BREADTH_FIRST_ALGORITHM_HPP
#define ORIGIN_GRAPH_ALGORITHM_SEARCH_BREADTH_FIRST_ALGORITHM_HPP

#include <origin/graph/label.hpp>
#include <origin/graph/algorithm/search/breadth_first_common.hpp>

namespace origin
{
  // NOTE: Despite the fact that all of the algorithms take references to
  // graphs, the actual Graph type might be deduced as const Graph. We need
  // to be const-correct.


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
  template<typename Graph, typename Visitor>
    inline void breadth_first_search(Graph& g, 
                                     typename graph_traits<Graph>::vertex v, 
                                     Visitor vis)
    {
      vertex_map<Graph, basic_color_t> colors(g.order());
      auto color = label(colors);
      
      bfs_algo<Graph, decltype(color), Visitor> algo(g, color, vis);
      algo(v);
    }

#if 0
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

  // color_type label version
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
   * @tparam Color_Label  A Read_Write_Label that maps vertices to a color_type
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

  // color_type label variant
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
#endif
  

} // namespace origin

#endif