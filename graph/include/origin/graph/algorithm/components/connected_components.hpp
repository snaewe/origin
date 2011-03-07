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
   * verex.
   */
  template<typename Graph, typename Component_Label>
  struct connected_components_visitor
  {
    typedef Graph graph_type;
    typedef graph_traits<Graph>::vertex vertex;
    typedef graph_traits<Graph>::edge edge;
    
    typedef Component_Label component_label;
        
    // FIXME: Use a simpler type trait for generating the value type.
    typedef typename std::remove_cv<
      typename std::remove_reference<
        typename std::result_of<component_label(vertex)>::type
      >::type
    >::type color_value;
    
    connected_components_visitor(component_label comp)
      : number(-1), component(comp)
    { }
    
    // When a new root vertex is discovered, increment the component count
    // indicating the start of a new component.
    void root_vertex(graph_type& g, vertex v)
    { ++number; }
    
    // When a new vertex is discovered, set its component 
    void discovered_vertex(graph_type& g, vertex v)
    { component(v) = number; }
    
    color_value     number;       ///< The current component number
    component_label component;    ///< The component label
  };


  /**
   * The connected components algorithm object computes the connected components
   * of the given graph.
   *
   * @tparam Graph A graph type
   * @tparam Color_Label A Read_Write_Label mapping vertices to a Color type.
   *         This defaults to an internal label mapping vertices to bool 
   *         values.
   */
  template<typename Graph, 
           typename Component_Label 
              = internal_label<Graph, typename graph_traits<Graph::size_type>>
  struct connected_components_algo
  {
    typedef Graph graph_type;
    typedef graph_traits<graph_type>::size_type size_type;

    typedef vertex_property<Graph, Component_Label> component_property;
    typedef typename component_property::label_type component_label;

    typedef connected_components_visitor<Graph, component_label> visitor_type;
    
    // FIXME: The BFS uses the default color label for its internal state.
    // Is there any graceful way to override this? Add a top-level color label
    // to allow "full-stack" customization.
    typedef bfs_algo<graph_type, visitor_type> bfs_type;
  
    // Construct the algorithm with a default two-color label.
    is_bipartite_algo(Graph& g)
      : comp(g), vis(comp.label), bfs(g, vis)
    { }
  
    // Construct the algorithm with a custom component label.
    is_bipartite_algo(Graph& g, Component_Label label)
      : comp(label), vis(comp.label), bfs(g, vis)
    { }

    size_type operator()()
    { 
      bfs(); 
      return vis.number;
    }
    
    component_property comp;    ///< The component label
    visitor_type vis;           ///< The component visitor
    bfs_type bfs;               ///< The underlying BFS object
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
    connected_components_algo<Graph, Component_Label> algo(g, comp);
    return algo();
  }

  template<typename Graph, typename Component_Label>
  inline typename graph_traits<Graph>::size_type
  connected_components(Graph const& g, Component_Label comp)
  {
    connected_components_algo<Graph const, Component_Label> algo(g, comp);
    return algo();
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
  //@{
  template<typename Graph>
  bool is_connected(Graph& g)
  {
    connected_components_algo<Graph> algo(g);
    return algo() == typename graph_traits<Graph>::size_type(1);
  }
  
  template<typename Graph>
  bool is_connected(Graph const& g)
  {
    connected_components_algo<Graph const> algo(g);
    return algo() == typename graph_traits<Graph const>::size_type(1);
  }

  template<typename Graph, typename Component_Label>
  inline typename graph_traits<Graph>::size_type
  is_connected(Graph& g, Component_Label comp)
  {
    connected_components_algo<Graph, Component_Label> algo(g, comp);
    return algo() == typename graph_traits<Graph>::size_type(1);
  }

  template<typename Graph, typename Component_Label>
  inline typename graph_traits<Graph>::size_type
  connected_components(Graph const& g, Component_Label comp)
  {
    connected_components_algo<Graph const, Component_Label> algo(g, comp);
    algo() == typename graph_traits<Graph const>::size_type(1);
  }
  //@}


  // HOWTO: Can I run connected_components without creating my own label?
  //
  // Yes. Just declare create a connected_components_algo object and run the
  // algorithm object, like this::
  //
  //    connected_components_algo<Graph> algo(g);
  //    algo();
  //    algo.comp(v) ...
  //
  // The comp member of the algorithm object is a label associates each vertex
  // with a componennt. Components are numbered 0 to n.

} // namespace origin

#endif
