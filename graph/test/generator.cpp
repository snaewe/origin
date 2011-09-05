// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <cassert>

#include <origin/utility/typestr.hpp>
#include <origin/graph/adjacency_list.hpp>
#include <origin/graph/generator.hpp>
#include <origin/graph/io.hpp>

using namespace std;
using namespace origin;

int main()
{
  typedef directed_adjacency_list<char, int> Graph;
  typedef graph_traits<Graph>::vertex Vertex;

  { // null graph
    Graph g = make_null_graph<Graph>();
    assert(( g.null() ));
  }
  
  { // trivial graph
    Graph g1 = make_trivial_graph<Graph>();
    assert(( g1.order() == 1 ));
  }
  
  { // path graphs
    cout << "==== P5 ====\n";
    Graph g = iota_path_graph<Graph>({'a', 'b', 'c', 'd', 'e'}, 0);
    assert(( g.order() == 5 ));
    assert(( g.size() == 4 ));
    write_edge_list(cout, g);
  }
  
  { // cycle graphs
    cout << "==== C5 ====\n";
    Graph g = iota_cycle_graph<Graph>({'a', 'b', 'c', 'd', 'e'}, 0);
    assert(( g.order() == 5 ));
    assert(( g.size() == 5 ));
    write_edge_list(cout, g);
  }

  { // complete graphs
    cout << "==== K5 ====\n";
    Graph g = iota_complete_graph<Graph>({'a', 'b', 'c', 'd', 'e'}, 0);
    assert(( g.order() == 5 ));
    assert(( g.size() == 10 ));
    write_edge_list(cout, g);
  }

  { // star graphs
    cout << "==== S5 ====\n";
    Graph g = iota_star_graph<Graph>({'a', 'b', 'c', 'd', 'e'}, 0);
    assert(( g.order() == 5 ));
    assert(( g.size() == 4 ));
    write_edge_list(cout, g);
  }

  { // wheel graphs
    cout << "==== W5 ====\n";
    Graph g = iota_wheel_graph<Graph>({'a', 'b', 'c', 'd', 'e'}, 0);
    assert(( g.order() == 5 ));
    assert(( g.size() == 8 ));
    write_edge_list(cout, g);
  }

}
