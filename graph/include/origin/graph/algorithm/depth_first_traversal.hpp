// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_DEPTH_FIRST_TRAVERSAL_HPP
#define ORIGIN_GRAPH_ALGORITHM_DEPTH_FIRST_TRAVERSAL_HPP

#include <origin/graph/traits.hpp>

namespace origin
{
  /**
   * Traverse a graph g in a depth-first fashion. This is an algorithm object
   * used to preserve state between vertices, thereby adapting typically linear
   * code depth-first traversal.
   *
   * @tparam Graph A directed graph.
   * @tparam Direction Normal or reverse traversal.
   * @tparam DiscoverVertexVisitor A visitor called when a vertex is discovered.
   * @tparam RevisitVertexVisitor A visitor called when a vertex is revisited.
   * @tparam DiscoverEdgeVisitor A visitor called when an edge is traversed.
   *
   * @note The type parameter uses directed and undirected graphs. In addition,
   * the graph can be traversed frontwards or in reverse.
   *
   * @note This algorithm is a rough draft of the algorithm.
   */

struct null_visitor {
  template<typename Traversal, typename Vertex>
  void operator()(Traversal const&, Vertex const&) const { }
};

template<
  typename Graph,
  Discover_Vertex_Visitor = null_visitor,
  Revisit_Vertex_Visitor = null_visitor,
  Discover_Edge_Visitor = null_visitor
>
class breadth_first_traversal
{
public:
  typedef Graph graph_type;
  typedef typename Graph::vertex vertex;
  typedef typename Graph::edge edge;
  typedef typename Graph::out_edge_range;
};

} // namesapce origin

#endif
