// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>

#include <origin/graph/adjacency_list.hpp>
#include <origin/graph/io/edge_list.hpp>

using namespace std;
using namespace origin;

int main()
{
  typedef directed_adjacency_list<char> Graph;
  
  Graph g;
  read_edge_list(std::cin, g);  
  std::cout << "# " << g.order() << ' ' << g.size() << '\n';
  write_edge_list(std::cout, g);
}
