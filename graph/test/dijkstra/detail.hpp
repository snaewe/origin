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

  // Makes ordinal_map a labeling
  template<typename Ordinal, typename Value>
  struct ordinal_label : ordinal_map<Ordinal, Value>
  {
    typename ordinal_map<Ordinal, Value>::mapped_type& operator()
    (typename ordinal_map<Ordinal, Value>::key_type k)
    { return ordinal_map<Ordinal, Value>::operator[](k); }
  };

  // Debug Dijkstra visitor
  template<typename Graph>
  struct debug_dijkstra_visitor
    : default_dijkstra_visitor
  {
    typedef typename Graph::vertex vertex;
    typedef std::vector<vertex> path_type;

    void parent(Graph const& g, vertex u, vertex v)
    {
      std:: cerr << '(' << u.value << ',' << v.value << ")\n";
      tree[u] = v;
    }

    void print()
    {
      if(tree.empty())
        std::cout << "empty";
      else {
        std::cout << "(0," << tree[vertex(0)].value << ')';
        for(unsigned int i = 1; i < tree.size(); ++i) {
          std::cout << " (" << i << ',' << tree[vertex(i)].value << ')';
        }
      }
    }

    path_type get_path_to(vertex v)
    {
      path_type path;
      do {
        path.push_back(v);
        v = tree[v];
      } while(tree[v] != v);
      return path;
    }

    ordinal_map<vertex, vertex> tree;
  };
  

} // namespace origin

#endif // ORIGIN_GRAPH_TEST_DIJKSTRA_DETAIL
