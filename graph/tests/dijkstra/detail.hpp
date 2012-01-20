// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_TEST_DIJKSTRA_DETAIL
#define ORIGIN_GRAPH_TEST_DIJKSTRA_DETAIL

#include <vector>
#include <iostream>

#include <origin/graph/algorithm/shortest_path/dijkstra_common.hpp>

namespace origin {

  /*// Makes ordinal_map a labeling
  template<typename Ordinal, typename Value>
  struct ordinal_label : ordinal_map<Ordinal, Value>
  {
    typename ordinal_map<Ordinal, Value>::mapped_type& operator()
    (typename ordinal_map<Ordinal, Value>::key_type k)
    { return ordinal_map<Ordinal, Value>::operator[](k); }

    typename ordinal_map<Ordinal, Value>::mapped_type const& operator()
    (typename ordinal_map<Ordinal, Value>::key_type k) const
    { return ordinal_map<Ordinal, Value>::operator[](k); }
  };

  // Debug Dijkstra visitor
  template<typename Graph>
  struct debug_dijkstra_visitor
    : default_dijkstra_visitor
  {
    typedef typename Graph::vertex vertex;
    typedef typename Graph::edge edge;
    typedef std::vector<vertex> path_type;
    typedef ordinal_map<vertex, vertex> parent_label_type;

    debug_dijkstra_visitor(parent_label_type& p)
      : parents(p)
    { }

    void examine_vertex(Graph const& g, vertex v)
    { std::cerr << "Examining vertex: " << g[v] << '\n'; }

    void examine_edge(Graph const& g, edge e)
    { std::cerr << "Examining edge: " << g[g.source(e)] << '\n'; }

    void parent(Graph const& g, vertex u, vertex v)
    { parents[u] = v; }

    path_type get_path_to(vertex v)
    {
      path_type path;
      do {
        path.push_back(v);
        v = parents[v];
      } while(parents[v] != v);
      return path;
    }

    parent_label_type& parents;
  };*/

} // namespace origin

#endif // ORIGIN_GRAPH_TEST_DIJKSTRA_DETAIL
