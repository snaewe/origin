// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

// FIXMEs
//  - add vertex and edge map types
//  - create a make_undirected_adjacency_vector function
//  - add_edges convienience function

#ifndef ORIGIN_GRAPH_ADJACECNY_VECTOR_UNDIRECTED_HPP
#define ORIGIN_GRAPH_ADJACECNY_VECTOR_UNDIRECTED_HPP

#include <tuple>

//#include <origin/graph/adjacency_vector/impl.hpp>

namespace origin {

// FIXME no need to use v.value except in indexing
  //template<typename V, typename E>
    class undirected_adjacency_vector
    {
      using size_type       = std::size_t;
      using internal_vertex = std::tuple<size_type,size_type>; //(target,edge)
      using internal_edge   = std::tuple<size_type,size_type>; //(source,target)
      using adj_vec         = std::vector<internal_vertex>;
      using vertex_vec      = std::vector<adj_list>;
      using edge_vec        = std::vector<internal_edge>;

    public:
      // Types
      using vertex = vertex_handle<size_type>;
      using edge   = undirected_edge_handle<size_type>;

      using vertex_range        = int;
      using edge_range          = int;
      using incident_edge_range = int;


      // Initializers
      undirected_adjacency_vector (size_type n = 0) : vertices_(n), edges_() {}


      // Graph Metrics
      bool empty()      const { return edges_.empty(); }
      size_type size()  const { return edges_.size(); }
      bool null()       const { return vertices_.empty(); }
      size_type order() const { return vertices_.size(); }


      // Structural Mutators
      vertex add_vertex();
      edge add_edge(vertex u, vertex u);
      void remove_edge(edge e);
      //void remove_edges(vertex u, vertex u);


      // Ranges
      vertex_range vertices() const;
      edge_range edges() const;
      incident_edge_range incident_edges() const;

    private:
      vertex_vec vertices_;
      edge_vec edges_;

      // Helper functions
      // Removes a vertex adjacent to vertex v incedent to edge e
      void remove_adjacent_vertex_by_edge(vertex v, edge e);
    };

  auto adjacency_vector::add_vertex() -> vertex
  {
    vertices_.push_back(adj_vec());
    return vertex(--order());
  }

  auto adjacency_vector::add_edge(vertex u, vertex v) -> edge
  {
    // add an edge to the edge list
    edges_.push_back(std::make_tuple(u.value(), v.value()));
    // add v to u's adjacent stuff
    vertices_[u.value()].pushback(std::make_tuple(v.value(),--size()));
    // add u to v's adjacent stuff, unless it is a loop
    if (v.value() != u.value())
      vertices_[v.value()].pushback(std::make_tuple(u.value(),--size()));

    // return edge
    return edge(--size(),u.value(), v.value());
  }

  void adjacency_vector::remove_edge(edge e)
  {
    // remove from edges
    edges_.erase(edges_.begin() + e.edge.handle());
    // remove adjacent stuff
    remove_adjacent_vertex_by_edge(e.source, e);
    if (e.source.value() != e.target.value()) // if e is not a loop
      remove_adjacent_vertex_by_edge(e.target, e);
  }

  void adjacency_vector::remove_adjacent_vertex_by_edge(vertex v, edge e)
  {
    // find vertex
    size_type v_index = v.value();
    auto pos = find_if(
      vertices_[v.value()],
      [v_index](internal_vertex v){ return std::get<1>(v) == v_index; });
    // remove edge
    vertices_[v.value()].erase(pos);
  }

} // namespace origin


  // Undirected adjacecny vector
  // This is a graph type whose representation is a vector of vectors. It uses
  // the adjacency_vector_base type. Undirected behavior is implemented
  // redundantly. Esentially, an edge is stored twice.

  // FIXME How is this going to store user data?
  // NOTE 
  /*class undirected_adjacency_vector
    : private adjacency_vector_detail_::adjacency_vector_base
  {
    using base_type = adjacency_vector_detail_::adjacency_vector_base;
    using this_type = undirected_adjacency_vector;
    using size_type = base_type::size_type;
  public:

    // Types
    using vertex = base_type::vertex;
    using edge = base_type::edge;
    using vertex_range = base_type::vertex_range;
    using out_edge_range = base_type::out_edge_range;


    // Initializers
    undirected_adjacency_vector(size_type n = 0) : base_type(n) { }
    undirected_adjacency_vector(this_type const& g);


    // Graph Metrics
    using base_type::empty;
    using base_type::size;
    using base_type::null;
    using base_type::order;

    using base_type::out_degree;
    size_type in_degree(vertex v) const;
    size_type degree(vertex v) const;


    // Structural Mutators
    using base_type::add_vertex;
    void remove_vertex(vertex v);
    edge add_edge(vertex u, vertex v);
    void remove_edge(vertex u, vertex v);


    // Structural Queries
    bool is_edge(edge e) const;
    bool is_edge(vertex u, vertex v) const;


    // Data accessors
    using base_type::vertices;
    void edges();
    using base_type::out_edges;
    void in_edges(vertex v);
    void incident_edges(vertex v);
  };*/
  /*template<typename Vertex_data, typename Edge_data>
  class undirected_adjacency_vector
  {
    // This is the vector that holds the vectors oof adjacent vertices
    using sub_vector_type = std::vector<vertex_handle<std::size_t>>;
    using main_vector_type = std::vector<sub_vector_type>;
    using size_type = main_vector_type::size_type;
  public:
    // Types
    using vertex = vertex_handle<std::size_t>;
    using edge = undirected_edge_handle<std::size_t>;

    using vertex_range =
    using edge_range =
    using incident_edge_range = 


    // Initializers
    undirected_adjacency_vector(size_type n = 0)
      : edge_count_{0}, main_vec_(n)
    { }
    undirected_adjacency_vector(this_type const& g)
      : edge_count_{g.edge_count_}, main_vec_(g.main_vec_)
    { }


    // Graph Metrics
    bool empty()      const { return edges_.empty(); }
    size_type size()  const { return edges_.size(); }
    bool null()       const { return main_vec_.empty(); }
    size_type order() const { return main_vec_.size(); }


    // Structural Mutators
    vertex add_vertex();
    edge add_edge(vertex u, vertex u);
    void remove_edge(edge e);
    void remove_edges(vertex u, vertex u);

  private:
    std::vector<edge> edges_;
    main_vector_type main_vec_;
  };*/

/*============================================================================*/
 /* auto undirected_adjacency_vector::add_vertex() -> vertex
  {
    main_vec_.push_back();
    return --order();
  }

  auto undirected_adjacency_vector::add_edge(vertex v, vertex u) -> edge
  {
    //make edge
    edge e(--edges_.size(), v.value, u.value);
    
    return e;
  }

  void undirected_adjacency_vector::remove_edge(edge e)
  {

  }

  auto undirected_adjacency_vector::in_degree(vertex v) const -> size_type
  { return out_degree(v); }

  auto undirected_adjacency_vector::degree(vertex v) const -> size_type
  { return out_degree(v); }*/




/*============================================================================*/
  /*class undirected_adjacency_vector_other
  {
  public:

    // Graph metrics
S   empty
S   size
S   null
S   order

    // Structural mutators
S   add_vertex
D   remove_vertex
D   add_edge
D   remove_edge

    // Structural queries
    ?

    // Data accessors
S   vertices
D   edges
U   incident_edges
  };

  class directed_adjacency_vector_other
  {
  public:

    // Graph metrics
S   empty
S   size
S   null
S   order

    // Structural mutators
S   add_vertex
D   remove_vertex
D   add_edge
D   remove_edge

    // Structural queries
    ?

    // Data accessors
S   vertices
    edges
U   out_edges
U   in_edges
  };


}*/

#endif // ORIGIN_GRAPH_ADJACECNY_VECTOR_UNDIRECTED_HPP