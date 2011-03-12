// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ADJACENCY_VECTOR_UNDIRECTED_HPP
#define ORIGIN_GRAPH_ADJACENCY_VECTOR_UNDIRECTED_HPP

#include <vector>

#include <origin/graph/adjacency_vector/directed.hpp>

namespace origin
{
  /** @internal */
  namespace adcacency_vector_
  {
    /**
     * An undirected edge_t is a triple consisting of a graph pointer, an edge
     * index and a vertex index. The vertex index represents the source vertex
     * of an edge and is used to reconstitute the target index from the edge
     * and graph.
     */
    template<typename Graph>
    class undirected_edge_t
      : public implicit_bool_facade<undirected_edge_t>
    {
    public:
      typedef typename graph_traits<Graph>::vertex vertex;
      typedef typename graph_traits<Graph>::edge edge;
    
      undirected_edge(Graph& g)
        : graph(&g), edge(), source()
      { }

      undirected_edge(Graph& g, edge e, vertex v)
        : graph(g), edge(e), source(v)
      { }

      bool valid() const
      { return edge_; }

      // Two undirected edges are equivalent if they refer to the same edge in
      // the same graph.
      bool equal(undirected_edge const& x) const
      { return graph_ == x.graph_ && edge_ == x.edge_; }
      
      edge get_edge() const
      { return edge_; }
      
      vertex source() const
      { return _source; }
      
      vertex target() const
      { return opposite(graph_, edge_, source_); }

    private:
      Graph& graph_;
      edge_t edge_;
      vertex_t source_;
    };
    
    /**
     * The undirected edge iterator implements an iterator 
     */
    class edge_iterator
      : public random_access_iterator_facade<edge_iterator, edge_t, std::size_t>
    {
      typedef random_access_iterator_facade<
        edge_iterator, edge_t, std::size_t
      > base_type;
    public:
      typedef typename base_type::reference reference;
      typedef typename base_type::difference_type difference_type;

      edge_iterator(edge_t v)
        : edge_(v)
      { }

      reference dereference() const
      { return edge_; }
      
      bool equal(edge_iterator iter) const
      { return edge_.value == iter.edge_.value; }
      
      bool less(edge_iterator iter) const
      { return edge_.value < iter.edge_.value; }

      void increment()
      { ++edge_.value; }
      
      void decrement()
      { --edge_.value; }
      
      void advance(difference_type n)
      { edge_.value += n; }
      
      difference_type distance(edge_iterator iter)
      { return iter.edge_.value - edge_.value ; }
      
    private:
      undirected_edge_t edge_;
    };
  } // namespace adjacency_vector
  
  /** 
   * The directed adjacency vector implements an Adjacency List. The vertex
   * and edge sets are implemented using vectors.
   */
  template<typename Vertex = empty_t, 
           typename Edge = empty_t, 
           typename Alloc = std::allocator<Vertex>>
  class undirected_adjacency_vector
  {
  public:
    // FIXME: Clean these guys up...
    typedef Alloc allocator_type;
    typedef std::size_t size_type;
    
    typedef Vertex vertex_value_type;
    typedef Edge edge_value_type;

    typedef adcacency_vector_::vertex_node<vertex_value_type> vertex_type;
    typedef adcacency_vector_::edge_node<edge_value_type> edge_type;

    typedef std::vector<vertex_type> vertex_list;
    typedef std::vector<edge_type> edge_list;
    
    typedef vertex_t vertex;
    typedef vertex_t const_vertex;
    typedef edge_t edge;
    typedef edge_t const_edge;
    
  public:
    typedef iterator_range<vertex_iterator> vertex_range;
    typedef iterator_range<vertex_iterator> const_vertex_range;
    
    typedef iterator_range<edge_iterator> edge_range;
    typedef iterator_range<edge_iterator> const_edge_range;
    
    typedef iterator_range<edge_iterator> out_edge_range;
    typedef iterator_range<edge_iterator> const_out_edge_range;
    typedef iterator_range<edge_iterator> in_edge_range;
    typedef iterator_range<edge_iterator> const_in_edge_range;
    
    /** @name Initialization and Assignment. */
    //@{
    directed_adjacency_vector()
      : vertices_(), edges_()
    { }
    //@}
    
    size_type order() const
    { return vertices_.size(); }
    
    bool null() const
    { return !order(); }
    
    size_type size() const
    { return edges_.size(); }
    
    bool empty() const
    { return !size(); }
    
    vertex_value_type& operator[](vertex_t v)
    { return get(v).value; }
    
    vertex_value_type const& operator[](vertex_t v) const
    { return get(v).value; }
    
    edge_value_type& operator[](edge_t e)
    { return get(e).value; }
    
    edge_value_type const& operator[](edge_t e) const
    { return get(e).value; }
    
    vertex add_vertex(vertex_value_type const& x = vertex_value_type{})
    {
      vertices_.push_back(vertex_type{x});
      return vertices_.size() - 1;
    }

    vertex get_vertex(std::size_t n)
    { return n; }
    
    const_vertex get_vertex(std::size_t n) const
    { return n; }
   
    size_type degree(const_vertex v) const
    { return get(v).degree(); }

    size_type out_degree(const_vertex v) const
    { return get(v).out_degree(); }

    size_type in_degree(const_vertex v) const
    { return get(v).in_degree(); }    
      
    /** @name Edge Operations */
    //@{
    edge add_edge(vertex u, vertex v, edge_value_type const& x = edge_value_type{})
    {
      edges_.push_back(edge_type{x});
      edge e = edges_.size() - 1;
      get(u).add_out(e);
      get(v).add_in(e);
      return e;
    }
    
    edge get_edge(std::size_t e)
    { return e; }
    
    const_edge get_edge(std::size_t e) const
    { return e; }

    edge get_edge(vertex u, vertex v)
    { return -1; }
    
    const_edge get_edge(vertex u, vertex v) const
    { return -1; }
        
    vertex source(edge e)
    { return get(e).source; }
    
    const_vertex source(const_edge e) const
    { return get(e).source; }
    
    vertex target(edge e)
    { return get(e).target; }
    
    const_vertex target(edge e) const
    { return get(e).target; }
    //@}

    /** @name Vertex and Edge Ranges */
    //@{
    vertex_range vertices()
    { 
      vertex_iterator f{0}, e{order()};
      return {f, e};
    }

    const_vertex_range vertices() const
    { 
      vertex_iterator f{0}, e{order()};
      return {f, e};
    }
    
    edge_range edges()
    { 
      edge_iterator f{0}, e{size()};
      return {f, e};
    }
    
    const_edge_range edges() const
    { 
      edge_iterator f{0}, e{size()};
      return {f, e};
    }
    
    out_edge_range out_edges(vertex v)
    { return {0, vertices_[v.value].out_degree()}; }
    
    const_out_edge_range out_edges(vertex v) const
    { return {0, vertices_[v.value].out_degree()}; }
    
    in_edge_range in_edges(vertex v)
    { return {0, vertices_[v.value].in_degree()}; }

    const_in_edge_range in_edges(vertex v) const
    { return {0, vertices_[v.value].in_degree()}; }
    //@}

  private:
    vertex_type& get(vertex v)
    { return vertices_[v.value]; }
    
    vertex_type const& get(const_vertex v) const
    { return vertices_[v.value]; }
    
    edge_type& get(edge e)
    { return edges_[e.value]; }
    
    edge_type const& get(const_edge e) const
    { return edges_[e.value]; }
  
  private:
    vertex_list vertices_;
    edge_list edges_;
  };

} // namespace origin

#endif
