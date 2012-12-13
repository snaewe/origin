// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>

#include <origin/graph/adjacency_list.hpp>
#include <origin/graph/io.hpp>

using namespace std;
using namespace origin;

// A little helper class to help trace initializations and data movements.
struct X
{
  static bool trace;

  X()         { if (trace) cout << "default\n"; }
  X(X&&)      { if (trace) cout << "move\n"; }
  X(const X&) { if (trace) cout << "copy\n"; }
};

bool X::trace = false;

void 
trace_insert()
{
  using G = directed_adjacency_list<X>;
  G g;
  X x;
  
  // Each insertion should require exactly one initialization of X.
  X::trace = true;
  g.add_vertex();
  g.add_vertex(std::move(x));
  g.add_vertex(x);
  X::trace = false;
}

void
check_default_init()
{
  using G = directed_adjacency_list<char, int>;
  G g;
  assert(g.empty());
  assert(g.null()); // NOTE: null implies empty
}

void
check_add_vertex()
{
  using G = directed_adjacency_list<char, int>;
  G g;
  g.add_vertex('a');
  g.add_vertex('b');
  g.add_vertex('c');

  // Also check vertex traversal and data access.
  for (auto v : g.vertices())
    cout << g(v) << ' ';
  cout << '\n';

  assert(!g.null());
  assert(g.order() == 3);
}

void
check_remove_vertex()
{
  using G = directed_adjacency_list<char, int>;
  G g;
  g.add_vertex('a');
  g.add_vertex('b');
  g.add_vertex('c');
  assert(g.order() == 3);

  g.remove_vertex(0);
  assert(g.order() == 2);

  g.remove_vertex(1);
  assert(g.order() == 1);

  g.remove_vertex(2);
  assert(g.order() == 0);
}

void
check_add_edge()
{
  using G = directed_adjacency_list<char, int>;
  G g;
  auto u = g.add_vertex('u');
  auto v = g.add_vertex('v');
  auto w = g.add_vertex('w');

  auto e1 = g.add_edge(u, v, 1);
  assert(g.source(e1) == u);
  assert(g.target(e1) == v);

  auto e2 = g.add_edge(v, w, 2);
  for (auto e : g.edges())
    cout << g(e) << ' ';
  cout << '\n';
}

void
check_remove_edge()
{
  cout << "*** remove edge ***\n";

  // Build a graph.
  using G = directed_adjacency_list<char, int>;
  G g;
  constexpr int N = 3;
  for (int i = 0; i < N; ++i)
    g.add_vertex('a' + i);

  int k = 1;
  for (int i = 0; i < N; ++i) {
    for (int j = i; j < N; ++j)
      g.add_edge(i, j, k++);
  }
  cout << io::edge_list(g) << '\n';

  // TODO: Assert degree properties of edges.

  g.remove_edge(0, 1);
  cout << io::edge_list(g) << '\n';
  g.remove_edge(0, 2);
  cout << io::edge_list(g) << '\n';
  g.remove_edge(1, 2);
  cout << io::edge_list(g) << '\n';

  // Remove loops
  g.remove_edge(0, 0);
  cout << io::edge_list(g) << '\n';

  g.remove_edge(1, 1);
  g.remove_edge(2, 2);
  cout << io::edge_list(g) << '\n';
  assert(g.empty());
  assert(g.out_degree(0) == 0);
  assert(g.in_degree(0) == 0);
}

void
check_remove_multi_edge()
{
  cout << "*** remove multi edge ***\n";

  // Build a multigraph
  using G = directed_adjacency_list<char, int>;
  G g;
  constexpr int N = 3;
  for (int i = 0; i < N; ++i)
    g.add_vertex('a' + i);

  // Build a fully connected directed graph.
  int k = 1;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      g.add_edge(i, j, k++);
      g.add_edge(j, i, k++);
    }
  }
  cout << io::edge_list(g) << '\n';

  // TODO: Assert degree properties of edges.

  g.remove_edges(0, 0);
  cout << io::edge_list(g) << '\n';
  g.remove_edges(1, 2);
  cout << io::edge_list(g) << '\n';
  g.remove_edges(0, 2);
  cout << io::edge_list(g) << '\n';

  g.remove_edges(0, 1);
  g.remove_edges(1, 1);
  g.remove_edges(2, 2);
  cout << io::edge_list(g) << '\n';

  g.remove_edges(1, 0);
  g.remove_edges(2, 0);
  g.remove_edges(2, 1);
  assert(g.empty());
}

void
check_remove_vertex_edges()
{
  cout << "*** remove vertex edges ***\n";

  // Build a multigraph
  using G = directed_adjacency_list<char, int>;
  G g;
  constexpr int N = 3;
  for (int i = 0; i < N; ++i)
    g.add_vertex('a' + i);

  // Build a fully connected directed graph.
  int k = 1;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      g.add_edge(i, j, k++);
      g.add_edge(j, i, k++);
    }
  }
  cout << io::edge_list(g) << '\n';

  g.remove_edges(0);
  assert(g.degree(0) == 0);
  cout << io::edge_list(g) << '\n';

  g.remove_edges(1);
  assert(g.degree(1) == 0);
  cout << io::edge_list(g) << '\n';

  g.remove_edges(2);
  assert(g.empty());
}

void
check_remove_all_edges()
{
  cout << "*** remove all edges ***\n";

  // Build a multigraph
  using G = directed_adjacency_list<char, int>;
  G g;
  constexpr int N = 3;
  for (int i = 0; i < N; ++i)
    g.add_vertex('a' + i);

  // Build a fully connected directed graph.
  int k = 1;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      g.add_edge(i, j, k++);
      g.add_edge(j, i, k++);
    }
  }
  cout << io::edge_list(g) << '\n';

  g.remove_edges();
  assert(g.empty());
  cout << io::vertex_list(g) << '\n';
}


int main()
{
  trace_insert();

  check_default_init();
  check_add_vertex();
  check_remove_vertex();

  check_add_edge();
  check_remove_edge();
  check_remove_multi_edge();
  check_remove_vertex_edges();
  check_remove_all_edges();
}
