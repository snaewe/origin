// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_IO_EDGE_LIST_HPP
#define ORIGIN_GRAPH_IO_EDGE_LIST_HPP

#include <iosfwd>

#include <origin/graph/traits.hpp>

namespace origin
{

  /**
   * @ingroup graph_io
   * @defgroup graph_io_edgelist
   *
   * This module I/O support for edge list-formatted data. Edge list data
   * is typically given as a sequence of tuples, where the first two values
   * represent vertex identifiers and the remaining values in the tuple (if
   * any) are edge properties.
   */
  
  // FIXME: There are some serious requirements on the graphs here. Namely,
  // they have to support edge set iteration. Also, the vertex and edge value
  // types must be io-streamable. For reading, there's also a question of
  // mapping.
  
  template<typename Graph, typename Value, typename Map>
  typename Graph::vertex map_vertex(Graph& g, Value const& x, Map& map)
  {
    auto result = map.insert({x, typename Graph::vertex{}});
    if(result.second) {
      auto v = g.add_vertex(x);
      result.first->second = v;
      return v;
    } else {
      return result.first->second;
    }
  }
  
  template<typename Char, typename Traits, typename Graph>
  void read_edge_list(std::basic_istream<Char, Traits>& is, Graph& g)
  {
    typedef typename Graph::vertex Vertex;
    typedef typename Graph::edge Edge;
    typedef typename Graph::vertex_value_type Vertex_Value;
    typedef typename Graph::edge_value_type Edge_Value;
    
    // FIXME: Is it possible to choose an optimal mapping here? Probably not.
    // What if the value type is not hashable? Should we try to downgrade
    // to a map? 
    typedef std::unordered_map<Vertex_Value, Vertex> Vertex_Map;
    Vertex_Map map;
    
    Vertex_Value uid, vid;
    Edge_Value eid;
    while(is >> uid >> vid >> eid) {
      Vertex u = map_vertex(g, uid, map);
      Vertex v = map_vertex(g, vid, map);
      g.add_edge(u, v, eid);
    }
  }
  
  template<typename Char, typename Traits, typename Graph>
  void write_edge_list(std::basic_ostream<Char, Traits>& os, Graph const& g)
  {
    for(auto e : g.edges()) {
      auto u = g.source(e);
      auto v = g.target(e);
      os << g[u] << ' ' << g[v] << ' ' << g[e] << '\n';
    }
  }

} // namesapce origin

#endif
