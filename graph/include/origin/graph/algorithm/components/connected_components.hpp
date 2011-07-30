// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_BIPARTITE_HPP
#define ORIGIN_GRAPH_ALGORITHM_BIPARTITE_HPP

#include <origin/graph/algorithm/search/breadth_first.hpp>

namespace origin
{

  /**
   * The connected components visitor assigns a component number to each
   * vertex.
   */
  template<typename Graph, typename Component_Label>
  struct component_visitor
  {
    typedef graph_traits<Graph>::vertex Vertex;
    typedef graph_traits<Graph>::edge Edge;
    typedef typename label_traits<Component_Label(Vertex)>::type Color;
    
    component_visitor(Component_Label)
      : num{-1}, comp{c}
    { }
    
    // When a new root vertex is discovered, increment the component count
    // indicating the start of a new component.
    void root_vertex(Graph& g, Vertex v)
    { 
      ++num; 
    }
    
    // When a new vertex is discovered, set its component to that of the
    // current root.
    void discovered_vertex(graph_type& g, vertex v)
    { 
      comp(v) = num; 
    }
    
    Color num;            // The current component number
    Component_Label comp; // The component label
  };

  /**
   * @function connected_components(g, comp)
   *
   * Compute the connected components of the graph. The results are stored in
   * the given component label.
   *
   * @tparam Graph            A Graph type
   * @tparam Component_Label  A Writeable_Label that maps vertices to an 
   *                          Integral value.
   *
   * @param g     A graph object.
   * @param comp  A component label object.
   *
   * @return The number of components in the graph.
   */
  //@{
  template<typename Graph, typename Component_Label>
    inline typename graph_traits<Graph>::size_type
    connected_components(Graph& g, Component_Label comp)
    {
      typedef component_visitor<Graph, Component_Label> Visitor;
      
      Visitor vis(comp);
      breadth_first_search_all(g, vis);
    }
  //@}

  /**
   * @function is_connected(g)
   * @function is_connected(g, comp)
   *
   * Returns true if the given graph is connected. A graph is connected if
   * there is only one connected component.
   *
   * @tparam Graph            A Graph type
   * @tparam Component_Label  A Writeable_Label that maps vertices to an 
   *                          Integral value.
   *
   * @param g     A graph object.
   * @param comp  A component label object.
   *
   * @return True if the graph is connected. False otherwise.
   */
  template<typename Graph, typename Component_Label>
    bool is_connected(Graph& g, Component_Label comp)
    {
      typedef typename graph_traits<Graph>::size_type Size;
      return connected_components(g, comp) == Size{1};
    }

  template<typename Graph>
    bool is_connected(Graph& g)
    {
      typedef typename graph_traits<Graph>::size_type Size;
      vertex_map<Graph, Size> comp(g.order());
      return is_connected(g, label(comp));
    }

} // namespace origin

#endif
