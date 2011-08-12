// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <cassert>

#include <origin/utility/typestr.hpp>
#include <origin/graph/adjacency_vector.hpp>
#include <origin/graph/generator.hpp>

using namespace std;
using namespace origin;

int main()
{
  typedef directed_adjacency_vector<char, int> Graph;
  typedef graph_traits<Graph>::vertex Vertex;
  
  {
    Graph g(3);
    fill_path(g, begin(g.vertices()), end(g.vertices()), 0);
    assert(( g.size() == 2 ));
  }
  {
    Graph g = make_filled_path<Graph>({'a', 'b', 'c'}, 0);
    assert(( g.order() == 3 ));
    assert(( g.size() == 2 ));
  }

}
