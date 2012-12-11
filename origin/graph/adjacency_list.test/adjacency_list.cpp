// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>

#include <origin/graph/adjacency_list.hpp>

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
check_default()
{
  using G = directed_adjacency_list<char, int>;
  G g;
  assert(g.empty());
  assert(g.null()); // NOTE: null implies empty
}

void
check_add()
{
  using G = directed_adjacency_list<char, int>;
  G g;
  g.add_vertex('a');
  g.add_vertex('b');
  g.add_vertex('c');

  for (auto v : g.vertices())
    cout << g(v) << '\n';

  assert(!g.null());
  assert(g.order() == 3);
}

void
check_remove()
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
  auto u = g.add_vertex('a');
  auto v = g.add_vertex('b');

  auto e = g.add_edge(u, v);
  assert(g.source(e) == u);
  assert(g.target(e) == v);
}

int main()
{
  // trace_insert();

  check_default();
  check_add();
  check_remove();

  check_add_edge();
}
