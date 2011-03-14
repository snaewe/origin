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

template<typename Graph>
void test()
{
  typedef typename Graph::vertex Vertex;
  typedef typename Graph::const_vertex Const_Vertex;
  typedef typename Graph::edge Edge;
  typedef typename Graph::const_edge Const_Edge;
  typedef bfs_visitor Visitor;

  Graph g;
  auto u = g.add_vertex();
  auto v = g.add_vertex();
  g.add_edge(u, v);

  // Get some const objects for const instantiations.
  Graph const& cg = g;
  Const_Vertex cu = u;

  Visitor vis;

  // Build a custom color map for various instantiations.
  // I think that keys in these maps HAVE to be const in order to be generic.
  // Note that the underlying map will actually make the key const, but that
  // constness has to preserved in the label argument. Otherwise, we can end
  // up with situations where a visitor over a const graph tries to call into
  // a non-const label. That is a compiler error.
  unordered_map<Const_Vertex, basic_color_t> cm;
  auto color = [&](Const_Vertex v) -> basic_color_t& { return cm[v]; };

  // Check search instantiations
  breadth_first_search(g, u, vis);
  breadth_first_search(g, u, vis, color);
  breadth_first_search(cg, cu, vis);
  breadth_first_search(cg, cu, vis, color);

  // Check traversal instantiations
  breadth_first_traverse(g, vis);
  breadth_first_traverse(cg, vis);
  breadth_first_traverse(g, vis, color);
  breadth_first_traverse(cg, vis, color);

  // Check bfs range implementations.
  // FIXME: Add visitor-specific instantiations.
  for(auto x : bfs(g, u)) ;
  for(auto x : bfs(g, u, color));
  for(auto x : bfs(cg, cu)) ;
  for(auto x : bfs(cg, cu, color));

  for(auto x : bfs(g));
  for(auto x : bfs(cg));
  for(auto x : bfs(g, color));
  for(auto x : bfs(cg, color));
}

int main()
{
  test<directed_adjacency_list<>>();
  // test<undirected_adjacency_list<>>();

  // Test a case where the graph owns its own coloring.
  {
    typedef directed_adjacency_list<basic_color_t> Graph;
    typedef bfs_visitor Visitor;

    Graph g;
    auto u = g.add_vertex();
    auto v = g.add_vertex();
    g.add_edge(u, v);

    // Note that the label here is *needs* non-const label arguments. Otherwise,
    // we might might end up trying to return a const reference to the stored
    // color property, which will result in a compiler error. Futhermore, note
    // that if the graph owns its own label, then you can't instantiate the
    // algorithm over a const value.
    Visitor vis;
    auto color = [&g](Graph::vertex v) -> basic_color_t& { return g[v]; };
    breadth_first_search(g, v, vis, color);
  }


  {
    typedef directed_adjacency_list<basic_color_t> Graph;
    typedef bfs_visitor Visitor;

    Graph g;
    auto u = g.add_vertex();
    auto v = g.add_vertex();
    g.add_edge(u, v);

    // NOTE: If the graph binds to the const version of the algorithm, then
    // we probably have to be careful to use parameterize the map and label
    // over the const vertex type.
    // Basically, we're getting const-correctness through and through. That's
    // a good thing.
    Graph const& cg = g;
    Visitor vis;
    unordered_map<Graph::const_vertex, basic_color_t> colors;
    auto color = [&colors](Graph::const_vertex v) -> basic_color_t& { return colors[v]; };
    breadth_first_search(cg, v, vis, color);
  }

  /*
  {
    typedef undirected_adjacency_list<char> Graph;
    typedef bfs_visitor Visitor;

    Graph g;
    auto u = g.add_vertex('a');
    auto v = g.add_vertex('b');
    g.add_edge(u, v);

    for(auto x : bfs(g, v)) {
      cout << g[x] << "\n";
    }

    for(auto x : bfs(g)) {
      cout << g[x] << "\n";
    }
  }
  */
}
