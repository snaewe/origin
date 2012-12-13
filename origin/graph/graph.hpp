// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef GRAPH_HPP
#define GRAPH_HPP

namespace origin
{
  // ------------------------------------------------------------------------ //
  //                                                             [graph.generic]
  //                          Generic Graph Interface
  //
  // The generic graph interface is a set of types and operations defined
  // commonly for various graph data structures.

  template<typename G>
    using Vertex = typename G::vertex;

  template<typename G>
    using Edge = typename G::edge;

} // namespace origin


#endif
