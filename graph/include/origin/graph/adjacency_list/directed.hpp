// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ADJACENCY_LIST_DIRECTED_HPP
#define ORIGIN_GRAPH_ADJACENCY_LIST_DIRECTED_HPP

#include <origin/utility/empty.hpp>
#include <origin/range/iterator_range.hpp>
#include <origin/graph/traits.hpp>
#include <origin/graph/adjacency_list/impl.hpp>

namespace origin
{
  /**
   * The directed adjacency list class implements a directed graph.
   *
   * @tparam Vertex A value type associated with vertices.By default, this
   * type is empty. Vertex must be a Regular type.
   *
   * @tparam Edge A value type associated with edges. By default, this type
   * is empty. Edge must be a Regular type.
   *
   * @tparam Alloc An Allocator type.
   */
  template<typename Vertex = empty_t,
           typename Edge = empty_t,
           typename Alloc = std::allocator<Vertex>>
  class directed_adjacency_list
    : protected adjacency_list_::impl<Vertex, Edge, Alloc>
  {
    typedef adjacency_list_::impl<Vertex, Edge, Alloc> base_type;
    typedef directed_adjacency_list<Vertex, Edge, Alloc> this_type;
  protected:
    typedef typename base_type::vertex_type vertex_type;
    typedef typename base_type::edge_type edge_type;
    typedef typename base_type::vertex_list vertex_list;
    typedef typename base_type::edge_list edge_list;
    typedef typename base_type::out_list out_list;
    typedef typename base_type::in_list in_list;
  public:
    struct graph_category : directed_graph_tag { };

    // Value types and allocator.
    typedef Vertex vertex_value_type;
    typedef Edge edge_value_type;
    typedef Alloc allocator_type;

    typedef typename base_type::size_type size_type;
    typedef typename base_type::difference_type difference_type;

    // Vertex and Edge types
    typedef vertex_type* vertex;
    typedef vertex_type const* const_vertex;
    typedef edge_type* edge;
    typedef edge_type const* const_edge;

  protected:
    // Iterator types.
    typedef adjacency_list_::node_iterator<vertex_list, vertex_type> vertex_iterator;
    typedef adjacency_list_::const_node_iterator<vertex_list, vertex_type> const_vertex_iterator;

    typedef adjacency_list_::node_iterator<edge_list, edge_type> edge_iterator;
    typedef adjacency_list_::const_node_iterator<edge_list, edge_type> const_edge_iterator;

    typedef adjacency_list_::node_iterator<out_list, edge_type> out_edge_iterator;
    typedef adjacency_list_::const_node_iterator<out_list, edge_type> const_out_edge_iterator;

    typedef adjacency_list_::node_iterator<in_list, edge_type> in_edge_iterator;
    typedef adjacency_list_::const_node_iterator<in_list, edge_type> const_in_edge_iterator;

  public:
    // Range types.
    typedef iterator_range<vertex_iterator> vertex_range;
    typedef iterator_range<const_vertex_iterator> const_vertex_range;

    typedef iterator_range<edge_iterator> edge_range;
    typedef iterator_range<const_edge_iterator> const_edge_range;

    typedef iterator_range<out_edge_iterator> out_edge_range;
    typedef iterator_range<const_out_edge_iterator> const_out_edge_range;

    typedef iterator_range<in_edge_iterator> in_edge_range;
    typedef iterator_range<const_in_edge_iterator> const_in_edge_range;

    /** @name Cosntruction, Assignment and Destruction. */
    //@{
    // Default construction and destruction.
    directed_adjacency_list(allocator_type const& alloc = allocator_type{})
      : base_type{alloc}
    { }

    // Copy semantics
    directed_adjacency_list(directed_adjacency_list const& x)
      : base_type{x}
    { }

    directed_adjacency_list& operator=(directed_adjacency_list const& x)
    { directed_adjacency_list tmp{x}; swap(tmp); return *this; }

    // Move semantics
    directed_adjacency_list(directed_adjacency_list&& x)
      : base_type{std::move(x)}
    { }

    directed_adjacency_list& operator=(directed_adjacency_list&& x)
    { swap(x); return *this; }

    // Order constructor
    directed_adjacency_list(size_type n, allocator_type const& alloc = allocator_type{});

    // FIXME: Range constructors? Others?

    ~directed_adjacency_list()
    { clear(); }
    //@}

    /** @name Equality */
    //@{
    bool operator==(directed_adjacency_list const& x) const
    { return base_type::operator==(x); }

    bool operator!=(directed_adjacency_list const& x) const
    { return base_type::operator!=(x); }
    //@}

    /** @name Properties */
    //@{
    allocator_type get_allocator() const
    { return base_type::get_allocator(); }

    constexpr size_type max_order() const
    { return base_type::max_size(); }

    constexpr size_type max_size() const
    { return base_type::max_size(); }

    //@}

    /** @name Graph Properties */
    //@{
    bool null() const
    { return base_type::null(); }

    size_type order() const
    { return base_type::order(); }

    bool empty() const
    { return base_type::empty(); }

    size_type size() const
    { return base_type::size(); }
    //@}

    /** @name Value Type Access */
    //@{
    vertex_value_type& operator[](vertex v)
    { return base_type::get_value(v); }

    vertex_value_type const& operator[](const_vertex v) const
    { return base_type::get_value(v); }

    edge_value_type& operator[](edge e)
    { return base_type::get_value(e); }

    edge_value_type const& operator[](const_edge e) const
    { return base_type::get_value(e); }
    //@}

    /** @name Graph Operations */
    //@{
    void swap(directed_adjacency_list& x)
    { base_type::swap(x); }

    void clear()
    { base_type::clear(); }
    //@}

    /** @name Vertex Operations and Properties */
    //@{
    vertex add_vertex(vertex_value_type const& x = vertex_value_type{})
    { return base_type::add_vertex(x); }

    void remove_vertex(vertex v)
    { base_type::remove_vertex(v); }

    // FIXME: Consider making degree an operation on the vertex iterator
    // and not the graph. What are the considerations?
    size_type degree(const_vertex v) const
    { return base_type::degree(v); }

    size_type out_degree(const_vertex v) const
    { return base_type::out_degree(v); }

    size_type in_degree(const_vertex v) const
    { return base_type::in_degree(v); }
    //@}

    /** @name Edge Operations and Properties */
    //@{
    edge add_edge(vertex u, vertex v, edge_value_type const& x = edge_value_type{})
    { return base_type::add_edge(u, v, x); }

    void remove_edge(edge e)
    { base_type::remove_edge(e); }

    void remove_edges(vertex u, vertex v)
    { base_type::remove_edges(u, v); }

    void remove_edges(vertex v)
    { base_type::remove_edges(v); }

    void remove_edges()
    { base_type::remove_edges(); }

    // Edge access
    edge get_edge(vertex u, vertex v)
    { return base_type::get_edge(u, v); }

    const_edge get_edge(vertex u, vertex v) const
    { return base_type::get_edge(u, v); }

    vertex source(edge e)
    { return base_type::source(e); }

    const_vertex source(const_edge e) const
    { return base_type::source(e); }

    vertex target(edge e)
    { return base_type::target(e); }

    const_vertex target(const_edge e) const
    { return base_type::target(e); }
    //@}

    /** @name Ranges */
    //@{
    vertex_range vertices()
    { return {this->cast(this->vertices_.head()), nullptr}; }

    const_vertex_range vertices() const
    { return {this->cast(this->vertices_.head()), nullptr}; }

    edge_range edges()
    { return {this->cast(this->edges_.head()), nullptr}; }

    const_edge_range edges() const
    { return {this->cast(this->edges_.head()), nullptr}; }

    out_edge_range out_edges(vertex v)
    {
      assert(v);
      return {this->cast(v->out.head()), nullptr};
    }

    const_out_edge_range out_edges(const_vertex v) const
    {
      assert(v);
      return {this->cast(v->out.head()), nullptr};
    }

    in_edge_range in_edges(vertex v)
    {
      assert(v);
      return {this->cast(v->in.head()), nullptr};
    }

    const_in_edge_range in_edges(const_vertex v) const
    {
      assert(v);
      return {this->cast(v->in.head()), nullptr};
    }
    //@}
  };

} // namesapce origin

#endif
