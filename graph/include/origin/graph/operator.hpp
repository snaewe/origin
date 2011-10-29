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
  // The vertex label accessor returns the label associated with a vertex.
  template<typename G>
    struct label_accessor
    {
      template<typename Handle>
        auto operator()(G const& g, Handle x) const -> decltype(g[x])
        {
          return g[x];
        }
    };

  // The edge label default returns the default value of edge labels.
  template<typename G>
    struct label_default
    {
      typedef typename graph_traits<G>::vertex Vertex;
      typedef typename graph_traits<G>::vertex_value_type Vertex_Value;
      
      typedef typename graph_traits<G>::edge Edge;
      typedef typename graph_traits<G>::edge_value_type Edge_Value;
      
      Vertex_Value operator()(G const&, Vertex) const
      {
        return Vertex_Value{};
      }

      Edge_Value operator()(G const& g, Edge) const
      {
        return Edge_Value{};
      }
    };
    
  // NOTE: An identity label is a function Id : X -> N where X is either the 
  // vertex or edge handle of a graph, and N is the size type. It associates
  // a unique, ordinal identity with each vertex. 
  //
  // An identity map is function Map : N -> X  that maps an ordinal value to
  // its associated vertex or edge.
  //
  // A domain is a type that binds the pair (Id, Map) together.
  //
  // If a handle is ordinal and represents the identity of the vertex or edge, 
  // then it  is not necessary to construct an external domain--it is 
  // implicitly defined by the properties of the handle.
  //
  // The algorithms in this module are currently only defined for graphs with
  // ordinal vertex and edge handles.
  //
  // Note that there should be obvious specializations of these algorithms for 
  // adjacency matrices since a) adjacency matrices have ordinal handles and
  // b) all of these operations follow from related operations on matrices.


  // Copy the vertex properties of g into h using the vertex accessor va to
  // propagate the properties of g into h. Note that h must have the same
  // or greater order than g u.
  //
  // requires Graph<G> && Vertex_Accessor<Vertex_Acc, G>
  // precondition: order(h) >= order(g)
  // postcondition: equal(vertices(g), vertices(h))
  template<typename G, typename Vertex_Acc>
    void copy_vertices(G const& g, G& h, Vertex_Acc va)
    {
      assert(( order(g) >= order(h) ));
      for(auto v : vertices(g))
        h[v] = va(g, v);
    }

  // Copy the vertex properties of g into h. Note that h must have the same
  // or greater order than g.
  //
  // requires Graph<G>
  // precondition: order(h) >= order(g)
  template<typename G>
    inline void copy_vertices(G const& g, G& h)
    {
      copy_vertices(g, h, label_accessor<G>{});
    }

  // Return a new graph that has the same vertices as g. 
  //
  // requires: Graph<G>
  // postcondition: let h = copy_vertices(g) where
  //   - equal(vertices(g), vertices(h)).
  template<typename G>
    G copy_vertices(G const& g)
    {
      G result(order(g));
      copy_vertices(g, result);
      return std::move(result);
    }
    
  // Return a new graph that has the same vertices as g using the vertex
  // accessor va to propagate values from g to the result.
  //
  // requires: Graph<G> && Vertex_Accessor<Vertex_Acc>
  // postcondition: let h = copy_vertices(g) where
  //   - equal(vertices(g), vertices(h)).
  template<typename G, typename Vertex_Acc>
    G copy_vertices(G const& g, Vertex_Acc va)
    {
      G result(order(g));
      copy_vertices(g, result, va);
      return std::move(result);
    }


  // Copy edges from g to h using the edge accessor ea to propagate edge
  // properties. This does not check for the previous existence of edges in
  // h before adding new edges. That is, the operation can result in
  // multigraphs.
  template<typename G, typename Edge_Acc>
    void copy_edges(G const& g, G& h, Edge_Acc ea)
    {
      for(auto e : edges(g)) {
        add_edge(h, source(g, e), target(g, e), ea(g, e));
      }
    }

  // Copy edges from g to h.
  template<typename G>
    inline void copy_edges(G const& g, G& h)
    {
      copy_edges(g, h, label_accessor<G>{});
    }

    

  // Generate the complement of the edge (u, v) in G, adding it to the result
  // graph. The edge (u', v') is added to G iff (u, v) is not in G. F is an
  // Edge_Accessor returning the edge label.
  //
  // requires: Graph<G> && Edge_Accessor<Edge_Acc>
  template<typename G, typename V, typename Edge_Acc>
    void edge_complement(G const& g, V u, V v, G& result, Edge_Acc ea)
    {
      auto e = get_edge(g, u, v);
      if(!e)
        add_edge(result, u, v, ea(g, e));
    }


  // Compute the complement of the given graph. The complement of a graph is
  // the complement of its edge set. That is, let h be a graph with the same
  // order as g where, where two vertices are adjacent in h only if they are
  // not adjacent in g. 
  //
  // The vertex and edge accessor functions va and ea are used to propagate
  // values from g to the result graph.
  //
  // TODO: The algorithm only works for ordinal vertices. We need a vertex
  // bimap to handle this for the general case.
  //
  // TODO: Specialize this for a directed graph by performing a square 
  // iteration instead of a triangular iteration.
  //
  // requires: Graph<G> 
  //        && Vertex_Accessor<Vertex_Acc> 
  //        && Edge_Accessor<Edge_Acc>
  template<typename G, typename Vertex_Acc, typename Edge_Acc>
    G graph_complement(G const& g, Vertex_Acc va, Edge_Acc ea)
    {
      G result = copy_vertices(g, va);
      auto verts = vertices(g);
      for(auto i = std::begin(verts); i != std::end(verts); ++i)
        for(auto j = std::next(i); j != std::end(verts); ++j)
          edge_complement(g, *i, *j, result, ea);
      return std::move(result);
    }

  // Compute the complement of the given graph.
  template<typename G>
    G graph_complement(G const& g)
    {
      return std::move(graph_complement(g, label_accessor<G>{}, label_default<G>{}));
    }


  // Compute the transpose of a graph. For directed graphs, this returns a
  // graph with all edges pointing in the reverse direction. For undirected
  // graphs, this is an identity operation.
  //
  // Note that the name "transpose" is derived from the same operation as
  // applied to an adjacency matrix.
  //
  // requires Graph<G> && Edge_Accessor<F, G>
  template<typename G, typename Vertex_Acc, typename Edge_Acc>
    G graph_transpose(G const& g, Vertex_Acc va, Edge_Acc ea)
    {
      G result = copy_vertices(g, va);
      for(auto e : edges(g))
        add_edge(result, target(g, e), source(g, e), ea(g, e));
      return std::move(result);
    }

  // Compute the transpose of the graph. Values of reverse edges are copied
  // from their original source edge.
  template<typename G>
    inline G graph_transpose(G const& g)
    {
      return std::move(graph_transpose(g, label_accessor<G>{}, label_accessor<G>{}));
    }


  // Return a graph containing the union of the vertex sets of a and b using
  // the vertex accessor va to propagate vertex properties.
  template<typename G, typename Vertex_Acc>
    inline G union_vertices(G const& a, G const& b, Vertex_Acc va)
    {
      return std::move(copy_vertices(a.order() >= b.order() ? a : b, va));
    }

  // Same as above, but using the default propagation.
  template<typename G>
    inline G union_vertices(G const& a, G const& b)
    {
      return std::move(copy_vertices(a.order() >= b.order() ? a : b));
    }


  // Compute the union of the edge sets of graphs a and b into the result. Use
  // the given edge accessor to propagate edgfe properties.
  template<typename G, typename Edge_Acc>
    inline void union_edges(G const& a, G const& b, G& result, Edge_Acc ea)
    {
      copy_edges(a, result, ea);
      copy_edges(b, result, ea);
    }
  
  // Same as above, but using the default propagation.
  template<typename G>
    inline void union_edges(G const& a, G const& b, G& result)
    {
      union_edges(a, b, result, label_accessor<G>{});
    }


  // Compute the graph union, which is the (non-disjoint) union of the vertex
  // and edge sets of a and b. 
  //
  // This meaning of graph union is attributed to Knuth. This is called the 
  // graph_sum in Mathematica (assuming a and b have the same order).
  //
  // For adjacency matrices (of the same order) this is the same as their
  // (elementwise) sum. For matrices of different orders, the result has the 
  // maximum order.
  template<typename G, typename Vertex_Acc, typename Edge_Acc>
    G graph_union(G const& a, G const& b, Vertex_Acc va, Edge_Acc ea)
    {
      G result = union_vertices(a, b, va);
      union_edges(a, b, result, ea);
      return std::move(result);
    }
    
  // Compute the union of the two graphs.
  template<typename G>
    inline G graph_union(G const& a, G const& b)
    {
      G result = union_vertices(a, b);
      union_edges(a, b, result);
      return std::move(result);
    }


  // Compute the interserction of edges in a and b into the result. This 
  // iterates over the edges of a, searching the out edges of b.
  template<typename G, typename Edge_Acc>
    void left_intersect_edges(G const& a, G const& b, G& result, Edge_Acc ea)
    {
      for(auto e : edges(a)) {
        auto u = source(a, e);
        auto v = target(a, e);
        if(auto x = get_edge(b, u, v)) {
          add_edge(result, u, v, ea(a, e));
        }
      }
    }

  // Compute the intersection of edges in a and b into the result. Use the
  // edge accessor ea to propagate values.
  //
  // The algorithm iterates over the graph with the smaller edge set, searching
  // for edges in the larger edge set.
  template<typename G, typename Edge_Acc>
    void intersect_edges(G const& a, G const& b, G& result, Edge_Acc ea)
    {
      if(size(a) < size(b))
        left_intersect_edges(a, b, result, ea);
      else
        left_intersect_edges(b, a, result, ea);
    }

  // Same as above, but using the default propagation.
  template<typename G>
    inline void intersect_edges(G const& a, G const& b, G& result)
    {
      intersect_edges(a, b, result, label_accessor<G>{});
    }

  // Return the interscection of two graphs. The resulting graph contains only
  // the edges that are in both a and b. Note that the resulting graph is
  // defined on the union of the vertex sets of a and b.
  template<typename G, typename Vertex_Acc, typename Edge_Acc>
    G graph_intersection(G const& a, G const& b, Vertex_Acc va, Edge_Acc ea)
    {
      G result = union_vertices(a, b, va);
      intersect_edges(a, b, result, ea);
      return std::move(result);
    }
  
  template<typename G>
    G graph_intersection(G const& a, G const& b)
    {
      G result = union_vertices(a, b);
      intersect_edges(a, b, result);
      return std::move(result);
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

  // Also called the disjoint union.
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
