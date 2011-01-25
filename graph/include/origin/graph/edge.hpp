// Copyright (c) 2008-2011 Kent State University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_EDGE_HPP
#define ORIGIN_GRAPH_EDGE_HPP

// This header contains generic operations on edges. Note that conceptually,
// an edge is both an iterator, a bool-testable value, and provides access to
// its source and target (even for undirected edges!).

namespace origin
{
  /**
   *  Return an iterator to the source vertex of the given edge.
   */
  template<typename Edge>
  inline auto source(Edge const& e) -> decltype(e.source())
  { return e.source(); }

  /**
   * Return an iterator to the target vertex of the given edge.
   */
  template<typename Edge>
  inline auto target(Edge const& e) -> decltype(e.target())
  { return e.target(); }

} // namesapce origin

#endif
