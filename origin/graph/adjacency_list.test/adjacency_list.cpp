// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <array>
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
  cout << "*** add edge ***\n";

  using G = directed_adjacency_list<char, int>;
  G g;
  auto u = g.add_vertex('u');
  auto v = g.add_vertex('v');
  auto w = g.add_vertex('w');

  auto e1 = g.add_edge(u, v, 1);
  assert(g.source(e1) == u);
  assert(g.target(e1) == v);
  assert(g(u, v));
  assert(!g(v, u));

  auto e2 = g.add_edge(v, w, 2);
  for (auto e : g.edges())
    cout << g(e) << ' ';
  cout << '\n';

  g.add_edge(u, w, 3);
  g.add_edge(w, v, 4);
  for (auto e : g.out_edges(u))
    cout << io::edge(g, e) << '\n';
  for (auto e : g.in_edges(v))
    cout << io::edge(g, e) << '\n';
}

void
check_remove_edge()
{
  cout << "*** remove first edge ***\n";

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


void
check_undir_add_edge()
{
  cout << "*** undirected add edge ***\n";
  using G = undirected_adjacency_list<char, int>;
  G g;
  constexpr int N = 3;
  for (int i = 0; i < N; ++i)
    g.add_vertex('a' + i);

  g.add_edge(0, 1);
  g.add_edge(2, 1);
  assert(g.size() == 2);
  cout << io::edge_list(g) << '\n';
}


void
check_undir_remove_edge()
{
  cout << "*** undirected add edge ***\n";
  using G = undirected_adjacency_list<char, int>;
  G g;
  constexpr int N = 3;
  for (int i = 0; i < N; ++i)
    g.add_vertex('a' + i);

  vector<G::edge> v;
  int k = 1;
  for (int i = 0; i < N; ++i) {
    for (int j = i; j < N; ++j)
      v.push_back(g.add_edge(i, j, k++));
  }
  cout << io::edge_list(g) << '\n';

  assert(g(0, 0) == v[0]);
  g.remove_edge(g(0, 0));
  cout << io::edge_list(g) << '\n';
  g.remove_edge(g(0, 1));
  cout << io::edge_list(g) << '\n';

  g.remove_edge(g(0, 2));
  g.remove_edge(g(1, 1));
  g.remove_edge(g(1, 2));
  g.remove_edge(g(2, 2));
  assert(g.empty());
  cout << io::vertex_list(g) << '\n';
}

// Returns true if the vertex v has the given degree values. The order given
// is out degree, in degree, total degree.

template<typename G>
  Requires<Directed_graph<G>(), bool>
  has_degrees(const G& g, Vertex<G> v, std::array<size_t, 3> a)
  {
    return g.out_degree(v) == a[0]
        && g.in_degree(v) == a[1];
  }

template<typename G>
  Requires<Undirected_graph<G>(), bool>
  has_degrees(const G& g, Vertex<G> v, std::array<size_t, 3> a)
  {
    return g.degree(v) == a[2];
  }



template<typename G>
  G build_n_graph(int n)
  {
    G g;
    for (int i = 0; i < n; ++i)
      g.add_vertex('a' + i);
    return g;
  }

// Construct an n-vertex reflexive clique. Note that the resulting edges are
// numbered 0..(n * (n + 1))/2.
template<typename G>
  G build_reflexive_clique(int n)
  {
    G g = build_n_graph<G>(n);
    int x = 0;
    for (int i = 0; i < n; ++i) {
      for (int j = i; j < n; ++j)
        g.add_edge(i, j, x++);
    }
    return g;
  }

// Construct an n-vertex reflexive bidirected clique. Note that the resulting
// edges are labeled 0..(n * (n + 1)).
template<typename G>
  G build_reflexive_bidi_clique(int n)
  {
    G g = build_n_graph<G>(n);
    int x = 0;
    for (int i = 0; i < n; ++i) {
      for (int j = i; j < n; ++j) {
        g.add_edge(i, j, x++);
        g.add_edge(j, i, x++);
      }
    }
    return g;
  }


template<typename G>
  void
  check_remove_specific_edge()
  {
    cout << "*** remove specific edge (" << typestr<G>() << ") ***\n";
    G g = build_reflexive_clique<G>(3);
    assert(g.size() == 6);
    cout << io::edge_list(g) << '\n';

    
    g.remove_edge(0); // Remove (0, 0)
    assert(g.size() == 5);
    assert(has_degrees(g, 0, {2, 0, 2})); // a--b, a--c | empty
    cout << io::edge_list(g) << '\n';

    g.remove_edge(1); // Remove (0, 1)
    assert(g.size() == 4);
    assert(has_degrees(g, 0, {1, 0, 1})); // a--c | empty
    assert(has_degrees(g, 1, {2, 1, 3})); // b--b, b--c | b--b
    cout << io::edge_list(g) << '\n';

    g.remove_edge(2); // Remove (0, 2)
    assert(g.size() == 3);
    assert(is_isolated(g, 0)); // empty | empty
    assert(has_degrees(g, 2, {1, 2, 3})); // c--c | b--c, c--c
    cout << io::edge_list(g) << '\n';

    // Just remove the rest.
    g.remove_edge(3);
    g.remove_edge(4);
    g.remove_edge(5);
    assert(g.empty());
  }

// Check the removal of single edges from a graph.
template<typename G>
  void
  check_remove_first_simple_edge()
  {
    cout << "*** remove first simple edge (" << typestr<G>() << ") ***\n";
    G g = build_reflexive_clique<G>(3);
    cout << io::edge_list(g) << '\n';

    // Remove a loop and test
    g.remove_edge(0, 0);
    cout << io::edge_list(g) << '\n';
    assert(has_degrees(g, 0, {2, 0, 2})); // a--b, a--c | empty

    // Remove a non-loop and test
    g.remove_edge(0, 1);
    cout << io::edge_list(g) << '\n';
    assert(has_degrees(g, 0, {1, 0, 1})); // a--c | empty
    assert(has_degrees(g, 1, {2, 1, 3})); // b--b, b--c | b--b

    g.remove_edge(0, 2);
    cout << io::edge_list(g) << '\n';
    assert(is_isolated(g, 0)); // empty | empty
    assert(has_degrees(g, 2, {1, 2, 3})); // c--c | b--c, c--c

    // Remove the rest and test
    g.remove_edge(1, 1);
    g.remove_edge(1, 2);
    g.remove_edge(2, 2);
    assert(g.empty());
  }

// Check the removal of the first simple edge in a multigraph. Basically
// the same as above except that the graph is more complex.
template<typename G>
  void
  check_remove_first_multi_edge()
  {
    cout << "*** remove first multi edge (" << typestr<G>() << ") ***\n";
    G g = build_reflexive_bidi_clique<G>(3);
    cout << io::edge_list(g) << '\n';

    // Remove a loop and test
    g.remove_edge(0, 0);
    cout << io::edge_list(g) << '\n';
    assert(has_degrees(g, 0, {3, 3, 6})); // a--a, a--b, a--c | a--a, b--a, c--a

    g.remove_edge(0, 0);
    cout << io::edge_list(g) << '\n';
    assert(has_degrees(g, 0, {2, 2, 4})); // a--b, a--c | b--a, c--a
    assert(has_degrees(g, 1, {4, 4, 8})); // b--a, b--b, b--b, b--c | a--b, b--b, b--b, c--b

    // Remove a non-loop and test
    g.remove_edge(0, 1);
    cout << io::edge_list(g) << '\n';
    assert(has_degrees(g, 0, {1, 2, 3})); // a--c | c--a, b--a
    assert(has_degrees(g, 1, {4, 3, 7})); // b--a, b--b, b--b, b--c | b--b, b--b, c--b

    g.remove_edge(1, 0);
    cout << io::edge_list(g) << '\n';
    assert(has_degrees(g, 0, {1, 1, 2})); // a--c | c--a
    assert(has_degrees(g, 1, {3, 3, 6})); // b--b, b--b, b--c | b--b, b--b, c--b

    // // Remove the rest and test
    g.remove_edge(0, 2);
    g.remove_edge(2, 0);
    g.remove_edge(1, 1);
    g.remove_edge(1, 1);
    g.remove_edge(1, 2);
    g.remove_edge(2, 1);
    g.remove_edge(2, 2);
    g.remove_edge(2, 2);
    assert(g.empty());
  }

template<typename G>
  void
  check_remove_multi_edge()
  {
    cout << "*** remove multi edge (" << typestr<G>() << ") ***\n";
    G g = build_reflexive_bidi_clique<G>(3);
    cout << io::edge_list(g) << '\n';

    g.remove_edges(0, 0); // Remove a--a,a--a
    cout << io::edge_list(g) << '\n';
    assert(has_degrees(g, 0, {2, 2, 4})); // a--b, a--c | b--a, c--a

    g.remove_edges(0, 1); // Removes a--b (and b--a if undirected)
    cout << io::edge_list(g) << '\n';
    if (Directed_graph<G>()) {
      assert(has_degrees(g, 0, {1, 2, 3})); // a--c | b--a, c--a
      assert(has_degrees(g, 1, {4, 3, 7})); // b--a, b--b, b--b, b--c | b--b, b--b, c--b
    } else {
      assert(has_degrees(g, 0, {1, 1, 2})); // a--c | c--a
      assert(has_degrees(g, 1, {3, 3, 6})); // b--b, b--b, b--c | b--b, b--b, c--b
    }

    g.remove_edges(0, 2);
    cout << io::edge_list(g) << '\n';
    if (Directed_graph<G>()) {
      assert(has_degrees(g, 0, {0, 2, 2})); // empty | b--a, c--a
      assert(has_degrees(g, 2, {4, 3, 7})); // c--a, c--b c--c, c--c | b--c, c--c, c--c
    } else {
      assert(has_degrees(g, 0, {0, 0, 0})); // empty | empty
      assert(has_degrees(g, 2, {3, 3, 6})); // c--b, c--c, c--c | b--c, c--c, c--c
    }

    // Remove the remaining edges
    g.remove_edges(1, 0); // No-op if undirected
    g.remove_edges(2, 0); // No-op if undirected
    g.remove_edges(1, 1);
    g.remove_edges(2, 2);
    g.remove_edges(1, 2);
    g.remove_edges(2, 1); // No-op if undirected
    assert(g.empty());
  }

template<typename G>
  void
  check_remove_vertex_edges()
  {
    cout << "*** remove vertex edges (" << typestr<G>() << ") ***\n";
    G g = build_reflexive_clique<G>(3);
    cout << io::edge_list(g) << '\n';

    g.remove_edges(0);
    cout << io::edge_list(g) << '\n';
    assert(has_degrees(g, 0, {0, 0, 0})); // empty | empty
    assert(has_degrees(g, 1, {2, 1, 3})); // b--b, b--c | b--b
    assert(has_degrees(g, 2, {1, 2, 3})); // c--c | b--c, c--c

    g.remove_edges(1);
    cout << io::edge_list(g) << '\n';
    assert(has_degrees(g, 1, {0, 0, 0})); // empty | empty
    assert(has_degrees(g, 2, {1, 1, 2})); // c--c | c--c

    g.remove_edges(2);
    assert(g.empty());
  }

template<typename G>
  void
  check_remove_all_edges()
  {
    cout << "*** remove all edges (" << typestr<G>() << ") ***\n";
    G g = build_reflexive_clique<G>(3);
    cout << io::edge_list(g) << '\n';

    assert(g.size() == 6);
    g.remove_edges();
    assert(g.empty());
  }

int main()
{
  trace_insert();

  // TODO: Write tests for adding vertices and edges. Even though those
  // features are thoroughly exercised by the remove edge tests, it might
  // be nice to include them here.

  using G = undirected_adjacency_list<char, int>;
  check_remove_specific_edge<G>();
  check_remove_first_simple_edge<G>();
  check_remove_first_multi_edge<G>();
  check_remove_multi_edge<G>();
  check_remove_vertex_edges<G>();
  check_remove_all_edges<G>();
  
  using D = directed_adjacency_list<char, int>;
  check_remove_specific_edge<D>();
  check_remove_first_simple_edge<D>();
  check_remove_first_multi_edge<D>();
  check_remove_multi_edge<D>();
  check_remove_vertex_edges<D>();
  check_remove_all_edges<G>();
}
