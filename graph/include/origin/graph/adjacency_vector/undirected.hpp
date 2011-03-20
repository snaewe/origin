// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ADJACENCY_VECTOR_UNDIRECTED_HPP
#define ORIGIN_GRAPH_ADJACENCY_VECTOR_UNDIRECTED_HPP

#include <vector>

#include <origin/utility/facades.hpp>
#include <origin/graph/adjacency_vector/directed.hpp>

namespace origin
{
  /** @internal */
  namespace adjacency_vector_
  {
    // FIXME: The undirected edge_t has both dependent and dependent parts.
    // Is it worth separating them? The non-dependent version can only operate
    // on the edge and source vertices. Returning the target and comparing for
    // equality are not possible.

    /**
     * An undirected edge_t is a triple consisting of a graph pointer, an edge
     * index and a vertex index. The vertex index represents the source vertex
     * of an edge and is used to reconstitute the target index from the edge
     * and graph.
     */
    template<typename Graph>
    class undirected_edge_t
      : public implicit_bool_facade<undirected_edge_t<Graph>>
    {
    public:
      undirected_edge_t(Graph& g = nullptr, 
                        edge_t e = edge_t{},
                        vertex_t v = vertex_t{})
        : graph_(g), edge_(e), source_(v)
      { }

      bool valid() const
      { return edge_; }

      // Two undirected edges are equivalent if they refer to the same edge in
      // the same graph. Note that the source vertex is irrelevant in this
      // comparison.
      bool equal(undirected_edge_t const& x) const
      { return &graph_ == &x.graph_ && edge_ == x.edge_; }
      
      edge_t get_edge() const
      { return edge_; }
      
      vertex_t source() const
      { return source_; }
      
      vertex_t target() const
      { return opposite(graph_.base(), edge_, source_); }

    private:
      Graph& graph_;
      edge_t edge_;
      vertex_t source_;
    };
    
    // FIXME: As with above, we can split the iterator into dependent and 
    // non-dependent parts.
    
    /**
     * The undirected edge iterator implements a random access iterator over 
     * undirected edges. Dereferencing an undirected edge iterator results in 
     * an undirected edge.
     *
     * The iterator is constructed over a pointer to the underlying graph
     * and an edge index into the implementation. The dereferenced edge 
     * iterator is constructed from that data.
     */
    template<typename Graph>
    class undirected_edge_iterator
      : public random_access_iterator_facade<
          undirected_edge_iterator<Graph>, 
          undirected_edge_t<Graph>, 
          undirected_edge_t<Graph>, 
          undirected_edge_t<Graph>
        >
    {
      typedef random_access_iterator_facade<
          undirected_edge_iterator<Graph>, 
          undirected_edge_t<Graph>, 
          undirected_edge_t<Graph>, 
          undirected_edge_t<Graph>
        > base_type;
    public:
      typedef typename base_type::reference reference;
      typedef typename base_type::difference_type difference_type;

      undirected_edge_iterator(Graph& g, edge_t v)
        : graph_(&g), edge_(v)
      { }

      reference dereference() const
      { return {graph_, edge_, graph_.base().source(edge_)}; }
      
      bool equal(undirected_edge_iterator x) const
      { return &graph_ == &x.graph_ && edge_.value == x.edge_.value; }
      
      bool less(undirected_edge_iterator x) const
      { return edge_.value < x.edge_.value; }

      void increment()
      { ++edge_.value; }
      
      void decrement()
      { --edge_.value; }
      
      void advance(difference_type n)
      { edge_.value += n; }
      
      difference_type distance(undirected_edge_iterator iter) const
      { return iter.edge_.value - edge_.value ; }

    private:
      Graph& graph_;
      edge_t edge_;
    };
  } // namespace adjacency_vector_
  
  /** 
   * The directed adjacency vector implements an Adjacency List. The vertex
   * and edge sets are implemented using vectors.
   */
  template<typename Vertex = empty_t, 
           typename Edge = empty_t, 
           typename Alloc = std::allocator<Vertex>>
  class undirected_adjacency_vector
  {
    typedef directed_adjacency_vector<Vertex, Edge, Alloc> base_type;
    typedef undirected_adjacency_vector<Vertex, Edge, Alloc> this_type;
  public:
    typedef Alloc allocator_type;
    typedef std::size_t size_type;
    
    typedef Vertex vertex_value_type;
    typedef Edge edge_value_type;

    typedef vertex_t vertex;
    typedef vertex_t const_vertex;
    typedef adjacency_vector_::undirected_edge_t<this_type> edge;
    typedef adjacency_vector_::undirected_edge_t<this_type const> const_edge;
  private:
    typedef adjacency_vector_::undirected_edge_iterator<this_type> 
      edge_iterator;
    typedef adjacency_vector_::undirected_edge_iterator<this_type const> 
      const_edge_iterator;
  public:
    typedef iterator_range<vertex_iterator> vertex_range;
    typedef iterator_range<vertex_iterator> const_vertex_range;
    
    typedef iterator_range<edge_iterator> edge_range;
    typedef iterator_range<const_edge_iterator> const_edge_range;
    
    // FIXME: This isn't right at all.
    typedef iterator_range<edge_iterator> incident_edge_range;
    typedef iterator_range<edge_iterator> const_incident_edge_range;
    
    /** @name Initialization and Assignment. */
    //@{
    undirected_adjacency_vector()
      : base_()
    { }
    //@}

    /**
     * Return the undirected graph as its directed implementation.     
     */
    base_type& base()
    { return base_; }
    
    base_type const& base() const
    { return base_; }
    
    size_type order() const
    { return base_.order(); }
    
    bool null() const
    { return base_.null(); }
    
    size_type size() const
    { return base_.size(); }
    
    bool empty() const
    { return base_.empty(); }
    
    vertex_value_type& operator[](vertex v)
    { return base_[v]; }
    
    vertex_value_type const& operator[](vertex v) const
    { return base_[v]; }
    
    edge_value_type& operator[](edge e)
    { return base_[e.get_edge()]; }
    
    edge_value_type const& operator[](edge e) const
    { return base_[e.get_edge()]; }
    
    vertex add_vertex(vertex_value_type const& x = vertex_value_type{})
    { return base_.add_vertex(x); }

    vertex get_vertex(size_type n)
    { 
      assert(( n < order() ));
      return n; 
    }
    
    const_vertex get_vertex(size_type n) const
    { 
      assert(( n < order() ));
      return n; 
    }
   
    size_type degree(const_vertex v) const
    { return base_.degree(v); }
      
    /** @name Edge Operations */
    //@{
    edge add_edge(vertex u, vertex v, edge_value_type const& x = edge_value_type{})
    { return make_edge(base_.add_edge(u, v, x), u); }
    
    edge get_edge(size_type n)
    {
      assert(( n < size() ));
      return make_edge(edge_t{n});
    }
    
    const_edge get_edge(size_type n) const
    {
      assert(( n < size() ));
      return make_edge(edge_t{n});
    }

    // FIXME: I think that this might be optimized by searching the vertex
    // with the smaller degree first.
    edge get_edge(vertex u, vertex v)
    {
      typename base_type::edge e = base_.get_edge(u, v);
      return e ? make_edge(e, u) : make_edge(base_.get_edge(v, u), v);
    }
    
    const_edge get_edge(vertex u, vertex v) const
    {
      typename base_type::edge e = base_.get_edge(u, v);
      return e ? make_edge(e, u) : make_edge(base_.get_edge(v, u), v);
    }
        
    vertex source(edge e)
    { return e.source(); }
    
    const_vertex source(const_edge e) const
    { return e.source(); }
    
    vertex target(edge e)
    { return e.target(); }
    
    const_vertex target(edge e) const
    { return e.target(); }
    //@}

    /** @name Vertex and Edge Ranges */
    //@{
    vertex_range vertices()
    { return base_.vertices(); }

    const_vertex_range vertices() const
    { return base_.vertices(); }
    
    edge_range edges()
    { 
      edge_iterator f{this, edge_t{0}}, l{this};
      return {f, l};
    }
    
    const_edge_range edges() const
    { 
      edge_iterator f{this, edge_t{0}}, l{this};
      return {f, l};
    }
    
    incident_edge_range out_edges(vertex v)
    { return {}; }
    
    const_incident_edge_range out_edges(vertex v) const
    { return {}; }
    //@}

  private:
    // Create an empty edge
    edge make_edge()
    { return {*this}; }

    const_edge make_edge() const
    { return {*this}; }
  
    // Create an undirected edge over the underlying edge type. Use underlying
    // source of the edge as the source vertex.
    edge make_edge(edge_t e)
    { return {*this, e, base_.source(e)}; }
    
    const_edge make_edge(edge_t e) const
    { return {*this, e, base_.source(e)}; }
    
    // Create an undirected edge using the underlying edge and source vertex.
    edge make_edge(edge_t e, vertex_t v)
    { return {*this, e, v}; }

    const_edge make_edge(edge_t e, vertex_t v) const
    { return {*this, e, v}; }

  private:
    base_type base_;
  };

} // namespace origin

#endif
