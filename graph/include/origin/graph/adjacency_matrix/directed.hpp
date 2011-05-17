// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

/**
 * TODO Finish degree and iterators.
 */

#ifndef ORIGIN_GRAPH_ADJACENCY_MATRIX_DIRECTED_HPP
#define ORIGIN_GRAPH_ADJACENCY_MATRIX_DIRECTED_HPP

#include <vector>
#include <cassert>

#include <origin/utility/empty.hpp>
#include <origin/range/iterator_range.hpp>
#include <origin/optional.hpp>
#include <origin/dynarray.hpp>
#include <origin/graph/vertex.hpp>
#include <origin/graph/edge.hpp>

namespace origin
{
  // Use cases for an adjacency matrix are different than we had originally
  // thought. Here are a couple...
  //  * Boolean     Matrix elements are true or false indicating the adjacency 
  //                of two vertices or not.
  //  * Distance    Matrix elements are numeric and represent the distance or
  //                cost between vertices
  //  * Probability Matrix elements are reals in the range [0, 1], and can 
  //                represent state transitions in, e.g., a Markov chain or an
  //                adjacency function in Page Range (for example)
  //  * Laplacian   Matrix elements are integral and encode information about
  //                the adjacency and degree of graphs
  // There are probably a ton of other kinds of adjacency matrices.
  //
  // The problem is that each particular kind of matrix imposes its own 
  // interpretation of "missing" values. In a Boolean matrix, two vertices u
  // and v are not adjacent if M(u, v) == false. In the distance matrix, u and
  // v are not adjacent if M(u, v) == oo. For Probability, it's 0. For the
  // Laplacian, it's -1.

  /**
   * @ingroup graph
   *
   * The directed adjacency matrix...
   */
  template<typename Vertex = empty_t,
           typename Edge = bool,
           typename Alloc = std::allocator<Vertex>>
  class directed_adjacency_matrix
  {
    typedef directed_adjacency_matrix<Vertex, Edge, Alloc> this_type;    
  public:
    typedef Alloc allocator_type;
    typedef Vertex vertex_value_type;
    typedef Edge edge_value_type;
  private:
    typedef dynarray<vertex_value_type, Alloc> vertex_list;
    typedef dynarray<edge_value_type, Alloc> edge_matrix;
  public:
    typedef typename allocator_type::size_type size_type;
    typedef typename allocator_type::difference_type difference_type;

    // FIXME: Can we make this go away?
    struct graph_category : directed_graph_tag { };

    // Vertex and Edge data types
    typedef vertex_t vertex;
    typedef vertex_t const_vertex;
    typedef edge_t edge;
    typedef edge_t const_edge;

    // Range types
    typedef iterator_range<vertex_iterator> vertex_range;
    typedef iterator_range<vertex_iterator> const_vertex_range;
    
    typedef iterator_range<edge_iterator> edge_range;
    typedef iterator_range<edge_iterator> const_edge_range;
    
    typedef iterator_range<edge_iterator> in_edge_range;
    typedef iterator_range<edge_iterator> const_in_edge_range;
    
    typedef iterator_range<edge_iterator> out_edge_range;
    typedef iterator_range<edge_iterator> const_out_edge_range;

    /** @name Construction, Assignment and Destruction */
    //@{
    // Construction and destruction.
    directed_adjacency_matrix(allocator_type const& alloc = allocator_type{})
      : vertices_(), edges_(), size_()
    { }
      
    directed_adjacency_matrix(size_type n,
                              allocator_type const& alloc = allocator_type{})
      : vertices_(n), edges_(n * n), size_(0)
    { }
    //@}

    bool equal(directed_adjacency_matrix const&) const;

    /** @name Data Structure Properties */
    //@{
    allocator_type get_allocator() const
    { return allocator_type(); }

    // FIXME: Should probably return the isqrt of the max size
    static constexpr size_type max_order()
    { return vertices_.max_size(); }
    
    static constexpr size_type max_size()
    { return edges_.max_size(); }
    //@}

    /** @name Graph Properties */
    //@{
    inline bool null() const
    { return vertices_.empty(); }
    
    inline size_type order() const
    { return vertices_.size(); }
    
    inline bool empty() const
    { return size_ == 0; }
    
    inline size_type size() const
    { return size_; }
    //@}

    /** Data Accessors */
    //@{
    vertex_value_type& operator[](vertex v)
    { return vertices_[v.value]; }

    vertex_value_type const& operator[](const_vertex v) const
    { return vertices_[v.value]; }

    // FIXME: This won't be correct if we optimize the optional out of empty
    // edge specifications
    edge_value_type& operator[](edge e)
    { return *edges_[e.value]; }

    edge_value_type const& operator[](const_edge e) const
    { return *edges_[e.value]; }

    // Matrix edge access
    edge operator()(vertex u, vertex v)
    { return get_edge(u, v); }
    
    const_edge operator()(vertex u, vertex v) const
    { return get_edge(u, v); }
    //@}

    /** @name Vertex Properties and Operations */
    //@{
    size_type out_degree(const_vertex v) const;
    size_type in_degree(const_vertex v) const;
    size_type degree(const_vertex v) const;
    //@}

    /** @name Edge Properties and Operations */
    //@{
    edge add_edge(vertex u, vertex v);
    edge add_edge(vertex u, vertex v, edge_value_type const& e);

    void remove_edge(edge e);
    void remove_edges(vertex u, vertex v);
    void remove_edges();

    edge get_edge(vertex u, vertex v);
    const_edge get_edge(const_vertex u, const_vertex v) const;

    vertex source(edge e);
    const_vertex source(const_edge e) const;
    vertex target(edge e);
    const_vertex target(const_edge e) const;
    //@}

    /** @name Ranges */
    //@{
    vertex_range vertices();
    const_vertex_range vertices() const;
    edge_range edges();
    const_edge_range edges() const;
    out_edge_range out_edges(vertex);
    const_out_edge_range out_edges(const_vertex) const;
    in_edge_range in_edges(vertex);
    const_in_edge_range in_edges(const_vertex) const;
    //@}

    /** @name Data Structure Operations */
    //@{
    void swap(directed_adjacency_matrix& x)
    {
      std::swap(vertices_, x.vertices_);
      std::swap(edges_, x.edges_);
      std::swap(num_edges_, x.num_edges_);
    }
    //@}

  private:
    vertex_list vertices_;
    edge_matrix edges_;
    size_type size_;
  };

  template<typename V, typename E, typename A>
  bool
  directed_adjacency_matrix<V,E,A>::equal(directed_adjacency_matrix const& x) const
  {
    return num_edges_ == d.num_edges_ 
        && vertices_ == d.vertices 
        && edges_ == d.edges_;
  }

  template<typename V, typename E, typename A>
<<<<<<< .mine
  directed_adjacency_matrix<V,E,A>::directed_adjacency_matrix
  (directed_adjacency_matrix&& c)
    : vertices_(c.vertices_), edges_(c.edges_), num_edges_(c.num_edges_)
  { }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::operator=
  (directed_adjacency_matrix&& c) -> directed_adjacency_matrix&
  {
    vertices_ = c.vertices_;
    edges_ = c.edges_;
    num_edges_ = c.num_edges_;
    return *this;
  }

  template<typename V, typename E, typename A>
  bool directed_adjacency_matrix<V,E,A>::equal
  (directed_adjacency_matrix const& d) const
  {
    return num_edges_ == d.num_edges_ &&
      vertices_ == d.vertices_ &&
      edges_ == d.edges_;
  }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::get_allocator() const -> allocator_type
  { return allocator_type{}; }

  // Since the order of the graph is fixed, return size/capacity of verticies.
  template<typename V, typename E, typename A>
  constexpr auto directed_adjacency_matrix<V,E,A>::max_order() const -> size_type
  { return vertices_.size(); }

  template<typename V, typename E, typename A>
  constexpr auto directed_adjacency_matrix<V,E,A>::max_size() const -> size_type
  { return edges_.size(); }

  template<typename V, typename E, typename A>
=======
>>>>>>> .r129
  bool directed_adjacency_matrix<V,E,A>::null() const
  { return vertices_.empty(); }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::order() const -> size_type
  { return vertices_.size(); }

  template<typename V, typename E, typename A>
  bool directed_adjacency_matrix<V,E,A>::empty() const
  { return num_edges_ == 0u; }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::size() const -> size_type
  { return num_edges_; }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::operator[](vertex v) -> vertex_value_type&
  { return vertices_[v.value]; }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::operator[](const_vertex v) const
    -> vertex_value_type const&
  { return vertices_[v.value]; }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::operator[](edge e) -> edge_value_type&
  {
    return edges_[e.value];
  }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::operator[](const_edge e) const
    -> edge_value_type const&
  {
    return *edges_[e.value];
  }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::operator()(size_type r, size_type c)
    -> edge_value_type&
  {
    return *edges[edge(r * order() + c)];
  }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::operator()(size_type r, size_type c) const
    -> edge_value_type const&
  {
    return *edges[edge(r * order() + c)];
  }

  template<typename V, typename E, typename A>
  void directed_adjacency_matrix<V,E,A>::swap(directed_adjacency_matrix& c)
  {
  }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::out_degree(const_vertex v) const -> size_type
  { return 0; }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::in_degree(const_vertex v) const -> size_type
  { return 0; }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::degree(const_vertex v) const -> size_type
  { return 0; }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::add_edge(vertex v, vertex u) -> edge
  {
    size_type edge_index = v.value * order() + u.value;
    // Should this assert? (that it isn't initialized)
    //assert((!edges_[v.value * order() + u.value]));
    // Or should it return the old edge
    if(!edges_[edge_index])
    {
      ++num_edges_;
      edges_[edge_index] = edge_value_type();
    }

    return edge(edge_index);
  }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::add_edge
  (vertex v, vertex u, edge_value_type const& e) -> edge
  {
    size_type edge_index = v.value * order() + u.value;
    if(!edges_[edge_index])
    {
      ++num_edges_;
      edges_[edge_index] = e;
    }

    return edge(edge_index);
  }

  template<typename V, typename E, typename A>
  void directed_adjacency_matrix<V,E,A>::remove_edge(edge e)
  {
    if(edges_[e.value])
    {
      --num_edges_;
      edge_type new_edge;
      swap(edges_[e.value], new_edge);
    }
  }

  // FIXME Add uninitialize to optional
  template<typename V, typename E, typename A>
  void directed_adjacency_matrix<V,E,A>::remove_edges(vertex v, vertex u)
  {
    size_type edge_index = v.value * order() + u.value;
    if(edges_[edge_index])
    {
      --num_edges_;
      edge_type e;
      swap(edges_[edge_index], e);
    }
  }

  template<typename V, typename E, typename A>
  void directed_adjacency_matrix<V,E,A>::remove_edges()
  { edges_.clear(); }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::get_edge(vertex v, vertex u) -> edge
  { return edge(v.value * order() + u.value); }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::get_edge
  (const_vertex v, const_vertex u) const
    -> const_edge
  { return const_edge(v.value * order() + u.value); }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::source(edge e) -> vertex
  { return vertex(e.value / order()); }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::source(const_edge e) const -> const_vertex
  { return const_vertex(e.value / order()); }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::target(edge e) -> vertex
  { return vertex(e.value % order()); }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::target(const_edge e) const -> const_vertex
  { return const_vertex(e.value % order()); }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::vertices() -> vertex_range
  { vertex_range(vertices_); }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::vertices() const -> const_vertex_range
  { const_vertex_range(vertices_); }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::edges() -> edge_range
  {
    return edge_range(
      edge_iterator(this, 0, order() * order()),
      edge_iterator(this, order() * order(),  order() * order()));
  }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::edges() const -> const_edge_range
  {
    return const_edge_range(
      const_edge_iterator(this, 0, order() * order()),
      const_edge_iterator(this, order() * order(), order() * order())
    );
  }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::out_edges(vertex v) -> out_edge_range
  {
    return out_edge_range(
      out_edge_iterator(this, v.value * order(), (v.value + 1) * order()),
      out_edge_iterator(this, (v.value + 1) * order(), (v.value + 1) * order())
    );
  }
  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::out_edges(vertex v) const
    -> const_out_edge_range
  {
    return const_out_edge_range(
      const_out_edge_iterator(this, v.value * order(), (v.value + 1) * order()),
      const_out_edge_iterator(
        this,
        (v.value + 1) * order(),
        (v.value + 1) * order()
      )
    );
  }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::in_edges(vertex v) -> in_edge_range
  {
    auto one_past_last = v.value + 1 + (order() - 1) * order();
    return in_edge_range(
      in_edge_iterator(this, v.value, one_past_last),
      in_edge_iterator(this, one_past_last, one_past_last)
    );
  }
  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::in_edges(vertex v) const
    -> const_in_edge_range
  {
    auto one_past_last = v.value + 1 + (order() - 1) * order();
    return const_in_edge_range(
      const_in_edge_iterator(this, v.value, one_past_last),
      const_in_edge_iterator(this, one_past_last, one_past_last)
    );
  }
  //@}
  /** @internal Free functions. */
  //@{
  template<typename V, typename E, typename A>
  bool operator==(directed_adjacency_matrix<V,E,A> const& a,
                  directed_adjacency_matrix<V,E,A> const& b)
  { return a.equal(b); }
  //@}


} // namesapce origin

#endif
