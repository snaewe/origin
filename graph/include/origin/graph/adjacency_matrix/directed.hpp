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

#include <origin/graph/adjacency_matrix/impl.hpp>

#include <vector>
#include <cassert>

#include <boost/optional.hpp>

#include <origin/utility/empty.hpp>
#include <origin/range/iterator_range.hpp>

namespace origin
{

  template<typename Vertex_T = empty_t,
           typename Edge_T = empty_t,
           typename Alloc = std::allocator<Vertex_T>>
  class directed_adjacency_matrix
  {
    typedef directed_adjacency_matrix<Vertex_T, Edge_T, Alloc> this_type;
    typedef Vertex_T vertex_type;
    typedef boost::optional<Edge_T> edge_type;
  public:
    typedef Vertex_T vertex_value_type;
    typedef typename edge_type::value_type edge_value_type;
    typedef Alloc allocator_type;
    typedef std::vector<vertex_value_type, Alloc> vertex_list;
    typedef std::vector<edge_value_type, Alloc> edge_matrix;

    typedef typename vertex_list::size_type size_type;
    typedef typename vertex_list::difference_type difference_type;
    typedef typename adj_mtx_impl_::d_adj_mtx_tag graph_category;

    // Vertex and Edge data types
    typedef adj_mtx_impl_::handles::vertex_t vertex;
    typedef const adj_mtx_impl_::handles::vertex_t const_vertex;
    typedef adj_mtx_impl_::handles::edge_t edge;
    typedef const adj_mtx_impl_::handles::edge_t const_edge;

    // Iterator types
    typedef typename vertex_list::iterator vertex_iterator;
    typedef typename vertex_list::const_iterator const_vertex_iterator;
    typedef adj_mtx_impl_::edge_iter<this_type> edge_iterator;
    typedef adj_mtx_impl_::edge_iter<const this_type> const_edge_iterator;
    // These are just out_edge with source and target_ swapped... (impl level)
    typedef float in_edge_iterator;
    typedef const float const_in_edge_iterator;
    typedef adj_mtx_impl_::edge_iter<this_type> out_edge_iterator;
    typedef adj_mtx_impl_::edge_iter<const this_type> const_out_edge_iterator;
    //typedef int adjacent_edge_iterator;

    // Range types
    typedef iterator_range<vertex_iterator> vertex_range;
    typedef iterator_range<const_vertex_iterator> const_vertex_range;
    typedef iterator_range<edge_iterator> edge_range;
    typedef iterator_range<const_edge_iterator> const_edge_range;
    typedef iterator_range<in_edge_iterator> in_edge_range;
    typedef iterator_range<const_in_edge_iterator> const_in_edge_range;
    typedef iterator_range<out_edge_iterator> out_edge_range;
    typedef iterator_range<const_out_edge_iterator> const_out_edge_range;

    /** @name Construction, Assignment and Destruction */
    //@{
    // Construction and destruction.
    directed_adjacency_matrix(allocator_type const& alloc = allocator_type{});
    directed_adjacency_matrix(size_type n,
                              allocator_type const& alloc = allocator_type{});

    // Copy semantics
    directed_adjacency_matrix(directed_adjacency_matrix const&);
    directed_adjacency_matrix& operator=(directed_adjacency_matrix const&);

    // Move Semantics
    directed_adjacency_matrix(directed_adjacency_matrix&&);
    directed_adjacency_matrix& operator=(directed_adjacency_matrix&&);
    //@}

    /** @name Data Structure Properties */
    //@{
    allocator_type get_allocator() const;
    inline constexpr size_type max_order() const;
    inline constexpr size_type max_size() const;
    //@}

    /** @name Graph Properties */
    //@{
    inline bool null() const;
    inline size_type order() const;
    inline bool empty() const;
    inline size_type size() const;
    //@}

    /** Data Accessors */
    //@{
    vertex_value_type& operator[](vertex v);
    vertex_value_type const& operator[](const_vertex v) const;
    edge_value_type& operator[](edge e);
    edge_value_type const& operator[](const_edge e) const;
    edge_value_type& operator()(size_type r, size_type c);
    edge_value_type const& operator()(size_type r, size_type c) const;
    //@}

    /** @name Data Structure Operations */
    //@{
    void swap(directed_adjacency_matrix&);
    //void clear();   // Clear edges? Vertices are fixed.
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
    const_vertex_range edges() const;
    out_edge_range out_edges(vertex_iterator);
    const_out_edge_range out_edges(const_vertex_iterator) const;

    in_edge_range in_edges(vertex_iterator);
    const_in_edge_range in_edges(const_vertex_iterator) const;
    //@}

  //private:
    vertex_list vertices_;
    edge_matrix edges_;
  };

  /** @Internal Member Definitions */
  //@{
  template<typename V, typename E, typename A>
  directed_adjacency_matrix<V,E,A>::directed_adjacency_matrix
  (allocator_type const& alloc)
    : vertices_(), edges_()
  { }

  template<typename V, typename E, typename A>
  directed_adjacency_matrix<V,E,A>::directed_adjacency_matrix
  (size_type n, allocator_type const& alloc)
    : vertices_(n), edges_(n * n)
  { }

  template<typename V, typename E, typename A>
  directed_adjacency_matrix<V,E,A>::directed_adjacency_matrix
  (directed_adjacency_matrix const& c)
    : vertices_(c.vertices_), edges_(c.edges_)
  { }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::operator=
  (directed_adjacency_matrix const& c) -> directed_adjacency_matrix&
  {
    vertices_ = c.vertices_;
    edges_ = c.edges_;
    return *this;
  }

  template<typename V, typename E, typename A>
  directed_adjacency_matrix<V,E,A>::directed_adjacency_matrix
  (directed_adjacency_matrix&& c)
    : vertices_(c.vertices_), edges_(c.edges_)
  { }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::operator=
  (directed_adjacency_matrix&& c) -> directed_adjacency_matrix&
  {
    vertices_ = c.vertices_;
    edges_ = c.edges_;
    return *this;
  }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::get_allocator() const -> allocator_type
  { return allocator_type{}; }

  // Since the order of the graph is fixed, return size/capacity of verticies.
  template<typename V, typename E, typename A>
  constexpr auto directed_adjacency_matrix<V,E,A>::max_order() const -> size_type
  { return vertices_.capacity(); }

  template<typename V, typename E, typename A>
  constexpr auto directed_adjacency_matrix<V,E,A>::max_size() const -> size_type
  { return edges_.max_size(); }

  template<typename V, typename E, typename A>
  bool directed_adjacency_matrix<V,E,A>::null() const
  { return vertices_.empty(); }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::order() const -> size_type
  { return vertices_.size(); }

  template<typename V, typename E, typename A>
  bool directed_adjacency_matrix<V,E,A>::empty() const
  { return edges_.empty(); }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::size() const -> size_type
  { return edges_.size(); }

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
    assert(((bool)edges_[e.value]));
    return edges_[e.value];
  }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::operator[](const_edge e) const
    -> edge_value_type const&
  {
    assert(((bool)edges_[e.value]));
    return *edges_[e.value];
  }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::operator()(size_type r, size_type c)
    -> edge_value_type&
  {
    assert(((bool)edges[vertex(r * order() + c)]));
    return *edges[edge(r * order() + c)];
  }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::operator()(size_type r, size_type c) const
    -> edge_value_type const&
  {
    assert(((bool)edges[vertex(r * order() + c)]));
    return *edges[edge(r * order() + c)];
  }

  template<typename V, typename E, typename A>
  void directed_adjacency_matrix<V,E,A>::swap(directed_adjacency_matrix& c)
  {
    std::swap(vertices_, c.vertices_);
    std::swap(edges_, c.edges_);
  }

  // TODO Requires some thought. What happens to vertices?
  /*template<typename V, typename E, typename A>
  void directed_adjacency_matrix<V,E,A>::clear()
  { edges_.clear(); }*/

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
      edges_[edge_index] = edge_value_type();

    return edge(edge_index);
  }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::add_edge
  (vertex v, vertex u, edge_value_type const& e) -> edge
  {
    size_type edge_index = v.value * order() + u.value;
    if(!edges_[edge_index])
      edges_[edge_index] = e;

    return edge(edge_index);
  }

  template<typename V, typename E, typename A>
  void directed_adjacency_matrix<V,E,A>::remove_edge(edge e)
  { /* How does this work?*/ }

  template<typename V, typename E, typename A>
  void directed_adjacency_matrix<V,E,A>::remove_edges(vertex v, vertex u)
  { /* How does this work?*/ }

  template<typename V, typename E, typename A>
  void directed_adjacency_matrix<V,E,A>::remove_edges()
  { edges_.clear(); }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::get_edge(vertex v, vertex u) -> edge
  { return edge(v.value * order() + u.value); }

  template<typename V, typename E, typename A>
  auto directed_adjacency_matrix<V,E,A>::get_edge(const_vertex v, const_vertex)
    -> const_edge
  { return const_edge(v.value * order() + u.value); }
  //@}

} // namesapce origin

#endif
