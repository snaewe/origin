// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>

#include <origin/utility/typestr.hpp>
#include <origin/graph/adjacency_list.hpp>
#include <origin/graph/algorithm/search/breadth_first.hpp>

using namespace std;
using namespace origin;

// Make sure that everything compiles with the given graph.
template<typename Graph>
  void sanity_check()
  {
    typedef typename Graph::vertex Vertex;
    typedef typename Graph::const_vertex Const_Vertex;
    typedef typename Graph::edge Edge;
    typedef typename Graph::const_edge Const_Edge;
    typedef bfs_visitor Visitor;

    // Build a trivial graph.
    Graph g;
    auto u = g.add_vertex('a');
    auto v = g.add_vertex('b');
    g.add_edge(u, v);

    // Get some const objects for const instantiations.
    Graph const& cg = g;
    Const_Vertex cu = u;

    Visitor vis;

    // Build a custom color map for various instantiations.
    vertex_map<Graph, basic_color_t> cm;
    auto color = label(cm);

    // Check search instantiations
    breadth_first_search(g, u, vis);
    breadth_first_search(g, u, color, vis);
    breadth_first_search(cg, cu, vis);
    breadth_first_search(cg, cu, color, vis);

    // Check traversal instantiations
    breadth_first_search_all(g, vis);
    breadth_first_search_all(g, color, vis);
    breadth_first_search_all(cg, vis);
    breadth_first_search_all(cg, color, vis);
  }

// Actually test a graph to see if it works right.
template<typename Graph>
  void test()
  {
    typedef typename Graph::vertex Vertex;
    typedef typename Graph::const_vertex Const_Vertex;

    /*
        a
       / \
      b   c
          |
          d
    */
    Graph g;
    Vertex a = g.add_vertex('a');
    Vertex b = g.add_vertex('b');
    Vertex c = g.add_vertex('c');
    Vertex d = g.add_vertex('d');
    g.add_edge(a, b);
    g.add_edge(a, c);
    g.add_edge(c, d);
    
    // Check distances
    vertex_map<Graph, int> dist(g.order());
    breadth_first_distance(g, a, label(dist));
    assert(( dist[a] == 0 ));
    assert(( dist[b] == 1 ));
    assert(( dist[c] == 1 ));
    assert(( dist[d] == 2 ));
    
    // Check predecessors
    vertex_map<Graph, Vertex> pred(g.order());
    breadth_first_search_tree(g, a, label(pred));
    assert(( pred[a] == a ));
    assert(( pred[b] == a ));
    assert(( pred[c] == a ));
    assert(( pred[d] == c ));
    
    auto vis = on_discover_vertex(ostream_visit(std::cout), bfs_visitor{});
    breadth_first_search(g, a, vis);
  }

int main()
{
  sanity_check<directed_adjacency_list<char>>();
  test<directed_adjacency_list<char>>();
}
