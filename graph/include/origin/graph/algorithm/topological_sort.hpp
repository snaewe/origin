// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_TOPOLOGICAL_SORT_HPP
#define ORIGIN_GRAPH_ALGORITHM_TOPOLOGICAL_SORT_HPP

#include <origin/graph/exception.hpp>
#include <origin/graph/traits.hpp>
#include <origin/graph/algorithm/search/depth_first.hpp>

namespace origin
{

  // The topological sort visitor records the topolgocial order of vertices
  // based on depth-first search order. 
  template<typename G, typename Out>
    struct topological_sort_visitor : dfs_visitor
    {
      typedef typename graph_traits<G>::vertex Vertex;
      typedef typename graph_traits<G>::edge Edge;

      topological_sort_visitor(Out& i)
        : iter(i)
      { }

      void back_edge(G const& g, Edge e)
      {
        throw graph_error("graph is not acyclic");
      }

      void started_vertex(G const& g, Vertex v) 
      {
        std::cout << g[v] << "\n";
      }

      void finished_vertex(G const& g, Vertex v) 
      {
        *iter++ = v;
      }
      
      Out& iter;
    };

  // Topologically sort the vertices in the directed graph g.
  template<typename G, typename Out>
    Out topological_sort(G& g, Out result)
    {
      topological_sort_visitor<G, Out> vis{result};
      depth_first_search(g, *begin(vertices(g)), vis);
      return result;
    }
  
} // namesapce origin

#endif
