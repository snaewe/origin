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
  // The edge label accessor returns the label associated with a given graph.
  template<typename G>
    struct edge_label_accessor
    {
      typedef typename graph_traits<G>::edge E;
      
      auto operator()(G const& g, E e) const -> decltype(g[e])
      {
        return g[e];
      }
    };
  
  // NOTE: An identity label is a function Id : X -> N where X is either the 
  // vertex or edge handle of a graph, and N is the size type. It associates
  // a unique, ordinal identity with each vertex. 
  //
  // An identity map is function Map : N -> X  that maps an ordinal value to
  // its associated vertex.
  //
  // A Domain is a type that binds the pair (Id, Map) together.
  //
  // If a vertex handle is ordinal and represents the identity of the vertex,
  // then it is not necessary to construct a domain.
    
  // Construct a new, empty graph containing the same vertices as g. This 
  // requires ordinal vertex handles and assumes that vertices are identified
  // by those handles.
  template<typename G>
    G copy_vertices(G const& g)
    {
      G result(g.order());
      auto verts = vertices(g);
      for(auto i = begin(verts); i != end(verts); ++i) {
        result[*i] = g[*i];
      }
      return std::move(result);
    }


  // Construct a new. empty graph containing the same vertices as g. The
  // identity label assigns an ordinal identity to each vertex in the graph.
  template<typename G, typename Map>
    G copy_vertices(G const& g, Map map)
    {
      typedef typename graph_traits<G >::size_type Size;

      G result(g.order());
      for(Size i = 0; i < g.order(); ++i) {
        result[map(i)] = g[map(i)];
      }
    }
    
  // TODO: What is the meaning of these algorithms for multigraphs?
  
  // Generate the complement of the edge (u, v) in G, adding it to the result
  // graph. The edge (u', v') is added to G iff (u, v) is not in G. F is an
  // EdgeFunction returning the edge label.
  template<typename G, typename V, typename F>
    void edge_complement(G const& g, V u, V v, G& result, F f)
    {
      auto e = get_edge(g, u, v);
      if(!e)
        add_edge(result, u, v, f(g, e));
    }
  
  // Compute the complement of the given graph. The complement of a graph is
  // the complement of its edge set...
  //
  // The function F is an edge function that returns the label associated with
  // an edge (u, v).
  //
  // TODO: Give a formal definition of complement of the edge set.
  //
  // TODO: The algorithm only works for ordinal vertices. We need a vertex
  // bimap to handle this for the general case.
  //
  // TODO: Specialize this for a directed graph by performing a square 
  // iteration instead of a triangular iteration.
  template<typename G, typename F>
    G graph_complement(G const& g, F f)
    {
      G result = copy_vertices(g);
      auto verts = vertices(g);
      for(auto i = begin(verts); i != end(verts); ++i)
        for(auto j = next(i); j != end(verts); ++j)
          edge_complement(g, *i, *j, result, f);
      return std::move(result);
    }

  // Compute the complement of the given graph.
  template<typename G>
    G graph_complement(G const& g)
    {
      return std::move(graph_complement(g, edge_label_accessor<G>{}));
    }

  
  
  // TODO: Is there any reason to generalize the transpose operation for an
  // arbitrary set of vertices? It's a non-trivial implementation since you
  // have to reverse the subgraph induced by that set of vertices.

  // Compute the transpose of a graph. For directed graphs, this returns a
  // graph with all edges pointing in the reverse direction. For undirected
  // graphs, this is an identity operation.
  //
  // Note that the name "transpose" is derived from the same operation as
  // applied to an adjacency matrix.
  //
  // requires Graph<G> && EdgeAccessor<F, G>
  template<typename G, typename F>
    G graph_transpose(G const& g, F f)
    {
      G result = copy_vertices(g);
      for(auto e : edges(g))
        add_edge(result, target(g, e), source(g, e), f(g, e));
      return std::move(result);
    }

  // Compute the transpose of the given graph using the given vertex identity
  // label. 
  template<typename G, typename Dom, typename F>
    G graph_transpose(G const& g, Dom dom, F f)
    {
      G result = copy_edges(g, dom.map);
      for(auto e : edges(g))
        add_edge(result, dom.id(target(g, e)), dom.id(source(g, e)), f(g, e));
      return std::move(result);
    }

  template<typename G>
    inline G graph_transpose(G const& g)
    {
      return std::move(graph_transpose(g, edge_label_accessor<G>{}));
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
