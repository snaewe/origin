// Copyright (c) 2008-2011 Kent State University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <functional>
#include <limits>
#include <cstddef>

#include <origin/utility/typestr.hpp>
#include <origin/graph/adjacency_list.hpp>
#include <origin/graph/algorithm/search/breadth_first.hpp>
#include <origin/ordinal_map.hpp>

using namespace std;
using namespace origin;

struct ostream_visitor : bfs_visitor
{
  template<typename G, typename V>
    void discovered_vertex(G& g, V v)
    {
      cout << g[v] << "\n";
    }
};

int main() 
{


  return 0;
}
