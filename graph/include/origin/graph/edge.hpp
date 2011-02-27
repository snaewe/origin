// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_EDGE_HPP
#define ORIGIN_GRAPH_EDGE_HPP

#include <origin/graph/traits.hpp>

namespace origin
{
  // NOTE: We could *amlost* get the directed/undirected selection to work
  // without enable_if if undirected graphs don't derive from directed graphs.
  // Unfortunately, access violations don't count as SFINAE errors.
  
  // NOTE: If a graph is somehow both directed and undirected, then we're going
  // to have ambiguity problems.

  /**
   * Return the set of out edges of a vertex. This function is overloaded for
   * both directed and undirected graphs. For undirected graphs, the set of
   * out edges is defined as the set of incident edges.
   */
  
  // Specializations for directed graphs.
  template<typename Graph>
  typename std::enable_if<
    is_directed_graph<Graph>::value,
    typename Graph::out_edge_range
  >::type
  out_edges(Graph& g, typename Graph::vertex v) 
  { return g.out_edges(v); }

  template<typename Graph>
  typename std::enable_if<
    is_directed_graph<Graph>::value,
    typename Graph::const_out_edge_range
  >::type
  out_edges(Graph const& g, typename Graph::const_vertex v) 
  { return g.out_edges(v); }

  // Specializations for undirected graphs
  template<typename Graph>
  typename std::enable_if<
    is_undirected_graph<Graph>::value,
    typename Graph::incident_edge_range
  >::type
  out_edges(Graph& g, typename Graph::vertex v) 
  { return g.incident_edges(v); }

  template<typename Graph>
  typename std::enable_if<
    is_undirected_graph<Graph>::value,
    typename Graph::const_incident_edge_range
  >::type
  out_edges(Graph const& g, typename Graph::const_vertex v) 
  { return g.incident_edges(v); }

} // namesapce origin

#endif
