// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_TRAITS_HPP
#define ORIGIN_GRAPH_TRAITS_HPP

#include <type_traits>

namespace origin
{
  /**
   * @defgroup graph_traits
   * @ingroup graph
   * The types and traits in this category support conceptual abstractions for
   * graph data structures.
   */
  //@{
  struct directed_graph_tag { };
  struct undirected_graph_tag { };

  /**
   * Return true if the graph G is directed.
   */
  template<typename G>
  struct is_directed_graph
    : std::is_convertible<typename G::graph_category, directed_graph_tag>::type
  { };

  /**
   * Return true if the graph G is undirected.
   */
  template<typename G>
  struct is_undirected_graph
    : std::is_convertible<typename G::graph_category, undirected_graph_tag>::type
  { };
  //@}
} // namesapce origin

#endif
