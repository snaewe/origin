// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_distance_matrix_DISTANCE_MATRIX_HPP
#define ORIGIN_GRAPH_distance_matrix_DISTANCE_MATRIX_HPP

#include <cassert>

#include <origin/graph/distance_matrix/impl.hpp>
#include <origin/graph/vertex.hpp>

#include <origin/utility/empty.hpp>
#include <origin/dynarray/dynarray.hpp>
#include <origin/dynarray/square_dynarray.hpp>

namespace origin
{

  /**
   * @ingroup graph
   *
   * Distance Matrix is a specialized adjacency matrix
   *
   * @tparam Vertex  Type of vertices.
   * @tparam Matrix  Type of the edge matrix.
   * @tparam Alloc   Allocator type of the vertex list.
   */
  template<typename Vertex = empty_t,
           typename Matrix = square_dynarray<float>,
           typename Edge_Predicate = 
           typename Alloc = std::allocator<Vertex>>
  class distance_matrix
  {
    typedef distance_matrix<Vertex, Matrix, Alloc> this_type;
  public:
    typedef Matrix matrix_type;
    typedef Alloc vertex_allocator_type;
    typedef Vertex vertex_value_type;
    typedef typename Matrix::value_type edge_value_type;
  private:
    typedef dynarray<vertex_value_type> vertex_list;
  public:
    typedef typename vertex_allocator_type::size_type size_type;
    typedef typename vertex_allocator_type::difference_type difference_type;

    // Vertex and Edge data types
    typedef vertex_t vertex;
    typedef vertex_t const_vertex;
    typedef adj_mtx_impl_::edge_t edge;
    typedef adj_mtx_impl_::edge_t const_edge;

  private:
    // Iterator types
    typedef adj_mtx_impl_::vertex_iterator vertex_iterator;
    typedef adj_mtx_impl_::edge_iterator<this_type> edge_iterator;
    typedef adj_mtx_impl_::in_edge_iterator<this_type> in_edge_iterator;

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
    distance_matrix()
      : matrix_(), vertices_(), size_()
    { }

    distance_matrix(matrix_type&& m)
      : matrix_(move(m)), vertices_(get_order(m)), size_(count_edges(m))
    { }

    distance_matrix(size_type n)
      : matrix_(n), vertices_(n), size_(0)
    { }

    // Enable if matrix is constructed by # rows and # columns
    /*distance_matrix(size_type n)
      : matrix_{n, n}, vertices_{n}, size_{0}
    { assert((n == m)); }*/

    distance_matrix& operator=(distance_matrix const& x);
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
    
    inline bool empty() const
    { return size_ == 0; }
    
    inline size_type size() const
    { return size_; }
    //@}

    /** Data Accessors */
    //@{
    // FIXME Can modify size!
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
    //  Must be enable if'd. This does not work with bool types!
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
    void swap(distance_matrix& x);
    bool equal(distance_matrix const& x) const;
    //@}

  private:
    /** @internal Helper functions. */
    size_type count_edges(matrix_type const& m) { return 0u; }
    size_type get_order(matrix_type const& m) { return m.order(); }

    matrix_type matrix_;
    vertex_list vertices_;
    size_type size_;
  };

  template<typename V, typename M, typename A>
  auto distance_matrix<V,M,A>::operator=(distance_matrix const& x)
    -> distance_matrix&
  {
    if(this == &x) {
      matrix_ = x.matrix_;
      vertices_ = x.vertices_;
      size_ = x.size_;
    }
    return *this;
  }

  template<typename V, typename M, typename A>
  auto distance_matrix<V,M,A>::out_degree(const_vertex v) const -> size_type
  {
    size_type out_edge_count = 0;
    for(auto i = 0u; i < order(); ++i) {
      if(matrix_(v.value, i)) ++out_edge_count;
    }
    return out_edge_count;
  }

  template<typename V, typename M, typename A>
  auto distance_matrix<V,M,A>::in_degree(const_vertex v) const -> size_type
  {
    size_type in_edge_count = 0;
    for(auto i = 0u; i < order(); ++i) {
      if(matrix_(i, v.value)) ++in_edge_count;
    }
    return in_edge_count;
  }

  template<typename V, typename M, typename A>
  auto distance_matrix<V,M,A>::degree(const_vertex v) const -> size_type
  {
    size_type in_edge_count = 0;
    for(auto i = 0u; i < order(); ++i) {
      if(matrix_(i, v.value)) ++in_edge_count;
      if(matrix_(v.value, i) && i != v.value) ++in_edge_count;
    }
    return in_edge_count;
  }

  template<typename V, typename M, typename A>
  auto distance_matrix<V,M,A>::add_edge(vertex v, vertex u) -> edge
  {
    edge_value_type& ev = matrix_(v.value, u.value);
    if(!ev) {
      ++size_;
      ev = true;
    }
    return {v.value, u.value};
  }

  template<typename V, typename M, typename A>
  auto distance_matrix<V,M,A>::add_edge
  (vertex v, vertex u, edge_value_type const& e) -> edge
  {
    assert((e != edge_value_type()));

    edge_value_type& ev = matrix_(v.value, u.value);
    if(!ev) {
      ++size_;
      ev = e;
    }
    return {v.value, u.value};
  }

  // Consider soft remove edges...
  template<typename V, typename M, typename A>
  void distance_matrix<V,M,A>::remove_edge(edge e)
  {
    edge_value_type& ev = matrix_(e.source, e.target);

    assert((ev != edge_value_type()));

    --size_;
    ev = edge_value_type();
  }

  template<typename V, typename M, typename A>
  void distance_matrix<V,M,A>::remove_edges(vertex v, vertex u)
  {
    edge_value_type& ev = matrix_(v.value, u.value);

    assert((ev != edge_value_type()));

    --size_;
    ev = edge_value_type();
  }

  template<typename V, typename M, typename A>
  void distance_matrix<V,M,A>::remove_edges()
  {
    size_ = 0;

    edge_value_type ev_init = edge_value_type();
    for(auto i = 0u; i < vertices_.size(); ++i)
      for(auto j = 0u; j < vertices_.size(); ++j)
        matrix_(i,j) = ev_init;
  }

  template<typename V, typename M, typename A>
  auto distance_matrix<V,M,A>::get_edge(vertex u, vertex v) -> edge
  { return matrix_(u.value, v.value) ? edge{u.value, v.value} : edge{}; }

  template<typename V, typename M, typename A>
  auto distance_matrix<V,M,A>::get_edge(const_vertex u, const_vertex v) const
    -> const_edge
  { return matrix_(u.value, v.value) ? edge{u.value, v.value} : edge{}; }

  template<typename V, typename M, typename A>
  auto distance_matrix<V,M,A>::source(edge e) -> vertex
  { return {e.source}; }

  template<typename V, typename M, typename A>
  auto distance_matrix<V,M,A>::source(const_edge e) const -> const_vertex
  { return {e.source}; }

  template<typename V, typename M, typename A>
  auto distance_matrix<V,M,A>::target(edge e) -> vertex
  { return {e.target}; }

  template<typename V, typename M, typename A>
  auto distance_matrix<V,M,A>::target(const_edge e) const -> const_vertex
  { return {e.target}; }

  template<typename V, typename M, typename A>
  auto distance_matrix<V,M,A>::vertices() -> vertex_range
  { return {vertex_iterator(0), vertex_iterator(order())}; }

  template<typename V, typename M, typename A>
  auto distance_matrix<V,M,A>::vertices() const -> const_vertex_range
  { return {vertex_iterator(0), vertex_iterator(order())}; }

  template<typename V, typename M, typename A>
  auto distance_matrix<V,M,A>::edges() -> edge_range
  { return {edge_iterator(*this, 0), edge_iterator(*this, order() * order())}; }

  template<typename V, typename M, typename A>
  auto distance_matrix<V,M,A>::edges() const -> const_edge_range
  { return {edge_iterator(*this, 0), edge_iterator(*this, order() * order())}; }

  template<typename V, typename M, typename A>
  auto distance_matrix<V,M,A>::out_edges(vertex v) -> out_edge_range
  {
    return {
      edge_iterator(*this, v.value * order()),
      edge_iterator(*this, v.value * (order() + 1))
    };
  }

  template<typename V, typename M, typename A>
  auto distance_matrix<V,M,A>::out_edges(const_vertex v) const 
    -> const_out_edge_range
  {
    return {
      edge_iterator(*this, v.value * order()),
      edge_iterator(*this, v.value * (order() + 1))
    };
  }

  template<typename V, typename M, typename A>
  auto distance_matrix<V,M,A>::in_edges(vertex v) -> in_edge_range
  {
    //std::cerr << v.value << ':';
    return const_in_edge_range(
      in_edge_iterator(*this, v.value),
      in_edge_iterator(*this, order() * order() + v.value)
    );
  }

  template<typename V, typename M, typename A>
  auto distance_matrix<V,M,A>::in_edges(const_vertex v) const
    -> const_in_edge_range
  {
    //std::cerr << v.value << ':';
    return const_in_edge_range(
      in_edge_iterator(*this, v.value),
      in_edge_iterator(*this, order() * order() + v.value)
    );
  }

  template<typename V, typename M, typename A>
  void distance_matrix<V,M,A>::swap(distance_matrix& x)
  {
    swap(matrix_, x.matrix_);
    std::swap(vertices_, x.vertices_);
    std::swap(size_, x.size_);
  }

  template<typename V, typename M, typename A>
  bool distance_matrix<V,M,A>::equal(distance_matrix const& x) const
  {
    return matrix_ == x.matrix_ && vertices_ == x.vertices_ && size_ == x.size_;
  }

} // origin

#endif
