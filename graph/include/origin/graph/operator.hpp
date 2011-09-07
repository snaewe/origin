// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_OPERATION_HPP
#define ORIGIN_GRAPH_OPERATION_HPP

#include <algorithm>

#include <origin/graph/traits.hpp>

namespace origin
{
  
  // Generate the complement of the edge (u, v) in G, adding it to the result
  // graph. The edge (u', v') is added to G iff (u, v) is not in G. The function
  // F is an EdgeFunction that returns the value type to be provided for the
  // new edge.
  template<typename G, typename V, typename F>
    void edge_complement(G const& g, V u, V v, G& result, F f)
    {
      if(!get_edge(g, u, v))
        add_edge(result, u, v, f(g, u, v));
    }
  
  
  // Return the complement of the given graph with respect to the given
  // subset of vertices in the range [first, last).
  //
  // TODO: The algorithm only works for ordinal vertices. We need a vertex
  // bimap to handle this for the general case.
  //
  // TODO: Specialize this for a directed graph by performing a square 
  // iteration instead of a triangular iteration.
  template<typename G, typename Iter, typename F>
    G graph_complement(G const& g, Iter first, Iter last, F f)
    {
      G result(first, last);
      while(first != last) {
        for(auto i = next(first); i != last; ++i)
          edge_complement(g, *first, *i, result, f);
        ++first;
      }
      return std::move(result)
    }
  
  template<typename G>
    G graph_complement(G const& g)
    {
      graph_complement(g, begin(vertices(g)), end(vertices(g)));
    }
    
  template<typename G>
    G graph_transpose(G const& g)
    {
    }

  // Compute the graph union according to Knuth. This is called the graph_sum
  // in Mathematica (assuming a and b have the same order).
  //
  // For adjacency matrices (of the same order) this is the same as their
  // (elementwise) sum. For matrices of different orders, the result has the 
  // maximum order.
  template<typename G>
    G graph_union(G const& a, G const& b)
    {
    }

  // Return the interscection of two graphs.
  template<typename G>
    G graph_intersection(G const& a, G const& b)
    {
    }

  template<typename G>
    G graph_difference(G const& a, G const& b)
    {
    }

  // TODO: This may be the same as the odd product.
  template<typename G>
    G graph_symmetric_difference(G const& a, G const& b)
    {
    }

  // Also called the disjoint union. This is more related 
  template<typename G>
    G graph_direct_sum(G const& a, G const& b)
    {
    }

  template<typename G>
    G graph_join(G const& a, G const& b)
    {
    }
    
  // Also called simply called the product.
  template<typename G>
    G graph_cartesian_product(G const& a, G const& b)
    {
    }
    
  // Also called the conjunction or categorical product.
  template<typename G>
    G graph_direct_product(G const& a, G const& b)
    {
    }
    
  // Also called the composition.
  template<typename G>
    G graph_lexicographical_product(G const& a, G const& b)
    {
    }

  template<typename G>
    G graph_odd_product(G const& a, G const& b)
    {
    }

  // TODO: I don't like that this has a different name, but graph_line doesn't
  // make a lot of sense. I could equivalently call it graph_conjugate.
  template<typename G>
    G line_graph(G const& g)
    {
    }
  
} // namespace origin

#endif
