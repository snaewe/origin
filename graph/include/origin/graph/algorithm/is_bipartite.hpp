// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_IS_BIPARTITE_HPP
#define ORIGIN_GRAPH_ALGORITHM_IS_BIPARTITE_HPP

namespace origin
{
  /**
   * A graph coloring error is a runtime error that indicates that a requested 
   * graph coloring cannot be computed.
   */
  class graph_coloring_error 
    : public std::runtime_error
  {
    graph_coloring_error(std::string const& what)
      : std::runtime_error(what)
    { };
  };

  /**
   * The two-color visitor attempts to assign a two-color assignment to each
   * vertex in a graph, as visited by a breadth- or depth-first search. If the
   * graph is not connected, then each root vertex is initially colored white.
   */
  template<typename Graph>
  struct two_color_visitor : bfs_visitor
  {
    typedef Graph graph_type;
    typedef typename graph_type::vertex vertex;
    typedef typename graph_type::const_vertex const_vertex;
    
    // Color each search tree root white.
    void root_vertex(Graph const& g, const_vertex v)
    { color(g) = white; }
    
    // Color the end of each edge as the opposite color of the source.
    void tree_edge(Graph const& g, const_edge e)
    { 
      const_vertex u = g.source(e);
      const_vertex v = g.target(e);
      color(v) = opposite(color(u));
    }
    
    // If the target of a nontree edge is colored the same as the source vertex
    // then the a two-coloring cannot be computed.
    void nontree_edge(Graph const& g, const_edge e)
    {
      const_vertex u = g.source(e);
      const_vertex v = g.target(e);
      if(color(u) == color(v)) {
        throw graph_coloring_error("cannot compute two-coloring");
      }
    }
    
    // Return the opposite of the given color.
    color_t opposite(color_t c)
    { return c == white ? black : white; }
    
    // FIXME: Choose an optimal mapping. Also note that if we choose bool
    // as the color type (and use an indexed graph), then we can achieve some
    // serious compression.
    std::unordered_map<const_vertex, color_t> color;
  };
  
  template<typename Graph>
  bool is_bipartite(Graph const& g)
  {
    bipartite_visitor<Graph> vis;
    try {
      breadth_first_search(g, vis);
    } catch(...) {
      return false;
    }
    return true;
  }

} // namespace origin

#endif

