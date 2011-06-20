// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ADJACENCY_MATRIX_BASE_HPP
#define ORIGIN_GRAPH_ADJACENCY_MATRIX_BASE_HPP

#include <cassert>

#include <origin/graph/adjacency_matrix/impl.hpp>
#include <origin/graph/vertex.hpp>

#include <origin/utility/empty.hpp>
#include <origin/dynarray/dynarray.hpp>
#include <origin/dynarray/square_dynarray.hpp>


namespace origin
{

  /**
   * @ingroup graph
   *
   * Adjacency Matrix Base - This provides the base implemantation for
   * adjacency_matrix_base (boolean), distance_matrix and others. The only real
   * difference is the predicate used to identify edges.
   *
   * @tparam Vertex    Type of vertices.
   * @tparam Matrix    Type of the edge matrix.
   * @tparam EdgePred  The functor that determines if an edge exists.
   * @tparam Alloc     Allocator type of the vertex list.
   */
  template<typename Vertex,
           typename Matrix,
           typename EdgePred,
           typename Alloc>
  class adjacency_matrix_base
  {
    typedef adjacency_matrix_base<Vertex, Matrix,EdgePred , Alloc> this_type;
  public:
    typedef Matrix matrix_type;
    typedef Alloc vertex_allocator_type;
    typedef Vertex vertex_value_type;
    typedef typename Matrix::value_type edge_value_type;
  private:
    typedef dynarray<vertex_value_type, Alloc> vertex_list;
  public:
    typedef typename vertex_allocator_type::size_type size_type;
    typedef typename vertex_allocator_type::difference_type difference_type;

    // FIXME The matrix decides the directedness.
    struct graph_category : directed_graph_tag { };

    // Vertex and Edge data types
    typedef vertex_t vertex;
    typedef vertex_t const_vertex;
    typedef adj_mtx_impl_::edge_t edge;
    typedef adj_mtx_impl_::edge_t const_edge;

  protected:
    // Iterator types
    typedef adj_mtx_impl_::vertex_iterator vertex_iterator;
    typedef adj_mtx_impl_::edge_iterator<this_type, EdgePred>
      edge_iterator;
    typedef adj_mtx_impl_::in_edge_iterator<this_type, EdgePred>
      in_edge_iterator;

  public:
    // Range types
    typedef iterator_range<vertex_iterator> vertex_range;
    typedef iterator_range<vertex_iterator> const_vertex_range;
    
    typedef iterator_range<edge_iterator> edge_range;
    typedef iterator_range<edge_iterator> const_edge_range;
    
    typedef iterator_range<edge_iterator> out_edge_range;
    typedef iterator_range<edge_iterator> const_out_edge_range;
    
    typedef iterator_range<in_edge_iterator> in_edge_range;
    typedef iterator_range<in_edge_iterator> const_in_edge_range;

    /** @name Construction, Assignment and Destruction */
    //@{
    adjacency_matrix_base()
      : matrix_(), vertices_()
    { }

    adjacency_matrix_base(matrix_type&& m)
      : matrix_(move(m)), vertices_(get_order(m))
    { }

    /*adjacency_matrix_base(size_type n)
      : matrix_(n), vertices_(n)
    { remove_edges(); }*/

    // Enable if matrix is constructed by # rows and # columns
    adjacency_matrix_base(size_type n)
      : matrix_(n, n), vertices_(n)
    { remove_edges(); }

    adjacency_matrix_base& operator=(adjacency_matrix_base const& x);
    //@}

    /** @name Data Structure Properties */
    //@{
    constexpr size_type max_order()
    { return vertices_.max_size(); }

    constexpr size_type max_size()
    { return vertices_.max_size() * vertices_.max_size(); }
    //@}

    /** @name Graph Properties */
    //@{
    inline bool null() const
    { return vertices_.empty(); }
    
    inline size_type order() const
    { return vertices_.size(); }
    
    inline bool empty() const;
    
    inline size_type size() const;
    //@}

    /** Data Accessors */
    //@{
    edge_value_type& operator()(size_type i, size_type j)
    { return matrix_(i, j); }

    edge_value_type const& operator()(size_type i, size_type j) const
    { return matrix_(i, j); }

    vertex_value_type& operator[](vertex v)
    { return vertices_[v.value]; }

    vertex_value_type const& operator[](const_vertex v) const
    { return vertices_[v.value]; }

    // FIXME Can modify size!
    edge_value_type& operator[](edge e)
    { return matrix_(e.source, e.target); }

    edge_value_type const& operator[](const_edge e) const
    { return matrix_(e.source, e.target); }

    matrix_type& matrix() const
    { return matrix_; }
    //@}

    /** @name Vertex Properties and Operations */
    //@{
    size_type out_degree(const_vertex v) const;
    size_type in_degree(const_vertex v) const;
    size_type degree(const_vertex v) const;
    //@}

    /** @name Edge Properties and Operations */
    //@{
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
    void swap(adjacency_matrix_base& x);
    bool equal(adjacency_matrix_base const& x) const;
    //@}

  private:
    matrix_type matrix_;
    vertex_list vertices_;
  };

  template<typename V, typename M, typename E, typename A>
  auto adjacency_matrix_base<V,M,E,A>::operator=(adjacency_matrix_base const& x)
    -> adjacency_matrix_base&
  {
    if(this != &x) {
      matrix_ = x.matrix_;
      vertices_ = x.vertices_;
    }
    return *this;
  }

  template<typename V, typename M, typename E, typename A>
  auto adjacency_matrix_base<V,M,E,A>::size() const -> size_type
  {
    const size_type mtx_order = order();
    size_type edge_count = 0;

    for(auto i = 0u; i < mtx_order; ++i)
      for(auto j = 0u; j < mtx_order; ++j)
        if(E::edge(matrix_(i,j))) ++edge_count;

    return edge_count;
  }

  template<typename V, typename M, typename E, typename A>
  auto adjacency_matrix_base<V,M,E,A>::empty() const -> bool
  {
    const size_type mtx_order = order();
    for(auto i = 0u; i < mtx_order; ++i)
      for(auto j = 0u; j < mtx_order; ++j)
        if(E::edge(matrix_(i,j)))
          return false;
    return true;
  }

  template<typename V, typename M, typename E, typename A>
  auto adjacency_matrix_base<V,M,E,A>::out_degree(const_vertex v) const -> size_type
  {
    size_type out_edge_count = 0;
    for(auto i = 0u; i < order(); ++i) {
      if(E::edge(matrix_(v.value, i))) ++out_edge_count;
    }
    return out_edge_count;
  }

  template<typename V, typename M, typename E, typename A>
  auto adjacency_matrix_base<V,M,E,A>::in_degree(const_vertex v) const -> size_type
  {
    size_type in_edge_count = 0;
    for(auto i = 0u; i < order(); ++i) {
      if(E::edge(matrix_(i, v.value))) ++in_edge_count;
    }
    return in_edge_count;
  }

  template<typename V, typename M, typename E, typename A>
  auto adjacency_matrix_base<V,M,E,A>::degree(const_vertex v) const -> size_type
  {
    size_type in_edge_count = 0;
    for(auto i = 0u; i < order(); ++i) {
      if(E::edge(matrix_(i, v.value))) ++in_edge_count;
      if(E::edge(matrix_(v.value, i)) && i != v.value) ++in_edge_count;
    }
    return in_edge_count;
  }

  template<typename V, typename M, typename E, typename A>
  auto adjacency_matrix_base<V,M,E,A>::add_edge
  (vertex v, vertex u, edge_value_type const& e) -> edge
  {
    assert(( E::edge(e) ));

    edge_value_type& ev = matrix_(v.value, u.value);

    assert(( !E::edge(ev) ));

    ev = e;
    return edge(v.value, u.value);
  }

  template<typename V, typename M, typename E, typename A>
  void adjacency_matrix_base<V,M,E,A>::remove_edge(edge e)
  {
    edge_value_type& ev = matrix_(e.source, e.target);

    assert(( E::edge(ev) ));

    ev = E::null_edge_value();
  }

  template<typename V, typename M, typename E, typename A>
  void adjacency_matrix_base<V,M,E,A>::remove_edges(vertex v, vertex u)
  {
    edge_value_type& ev = matrix_(v.value, u.value);

    assert(( E::edge(ev) ));

    ev = E::null_edge_value();
  }

  template<typename V, typename M, typename E, typename A>
  void adjacency_matrix_base<V,M,E,A>::remove_edges()
  {
    edge_value_type e_null = E::null_edge_value();
    for(auto i = 0u; i < vertices_.size(); ++i)
      for(auto j = 0u; j < vertices_.size(); ++j)
        matrix_(i,j) = e_null;
  }

  template<typename V, typename M, typename E, typename A>
  auto adjacency_matrix_base<V,M,E,A>::get_edge(vertex u, vertex v) -> edge
  { return E::edge(matrix_(u.value, v.value)) ? edge{u.value, v.value} : edge{}; }

  template<typename V, typename M, typename E, typename A>
  auto adjacency_matrix_base<V,M,E,A>::get_edge(const_vertex u, const_vertex v) const
    -> const_edge
  { return E::edge(matrix_(u.value, v.value)) ? edge{u.value, v.value} : edge{}; }

  template<typename V, typename M, typename E, typename A>
  auto adjacency_matrix_base<V,M,E,A>::source(edge e) -> vertex
  { return {e.source}; }

  template<typename V, typename M, typename E, typename A>
  auto adjacency_matrix_base<V,M,E,A>::source(const_edge e) const -> const_vertex
  { return {e.source}; }

  template<typename V, typename M, typename E, typename A>
  auto adjacency_matrix_base<V,M,E,A>::target(edge e) -> vertex
  { return {e.target}; }

  template<typename V, typename M, typename E, typename A>
  auto adjacency_matrix_base<V,M,E,A>::target(const_edge e) const -> const_vertex
  { return {e.target}; }

  template<typename V, typename M, typename E, typename A>
  auto adjacency_matrix_base<V,M,E,A>::vertices() -> vertex_range
  { return {vertex_iterator(0), vertex_iterator(order())}; }

  template<typename V, typename M, typename E, typename A>
  auto adjacency_matrix_base<V,M,E,A>::vertices() const -> const_vertex_range
  { return {vertex_iterator(0), vertex_iterator(order())}; }

  template<typename V, typename M, typename E, typename A>
  auto adjacency_matrix_base<V,M,E,A>::edges() -> edge_range
  {
    return edge_range(
      edge_iterator(*this, 0),
      edge_iterator(*this, order() * order())
    );
  }

  template<typename V, typename M, typename E, typename A>
  auto adjacency_matrix_base<V,M,E,A>::edges() const -> const_edge_range
  { return {edge_iterator(*this, 0), edge_iterator(*this, order() * order())}; }

  template<typename V, typename M, typename E, typename A>
  auto adjacency_matrix_base<V,M,E,A>::out_edges(vertex v) -> out_edge_range
  {
    //std::cerr << 
    return {
      edge_iterator(*this, v.value * order()),
      edge_iterator(*this, (v.value + 1) * order())
    };
  }

  template<typename V, typename M, typename E, typename A>
  auto adjacency_matrix_base<V,M,E,A>::out_edges(const_vertex v) const 
    -> const_out_edge_range
  {
    return {
      edge_iterator(*this, v.value * order()),
      edge_iterator(*this, (v.value + 1) * order())
    };
  }

  template<typename V, typename M, typename E, typename A>
  auto adjacency_matrix_base<V,M,E,A>::in_edges(vertex v) -> in_edge_range
  {
    return in_edge_range(
      in_edge_iterator(*this, v.value),
      in_edge_iterator(*this, order() * order() + v.value)
    );
  }

  template<typename V, typename M, typename E, typename A>
  auto adjacency_matrix_base<V,M,E,A>::in_edges(const_vertex v) const
    -> const_in_edge_range
  {
    return const_in_edge_range(
      in_edge_iterator(*this, v.value),
      in_edge_iterator(*this, order() * order() + v.value)
    );
  }

  template<typename V, typename M, typename E, typename A>
  void adjacency_matrix_base<V,M,E,A>::swap(adjacency_matrix_base& x)
  {
    swap(matrix_, x.matrix_);
    std::swap(vertices_, x.vertices_);
  }

  template<typename V, typename M, typename E, typename A>
  bool adjacency_matrix_base<V,M,E,A>::equal(adjacency_matrix_base const& x) const
  {
    return matrix_ == x.matrix_ && vertices_ == x.vertices_;
  }

} // origin

#endif
