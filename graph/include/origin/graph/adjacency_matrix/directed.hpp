// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ADJACENCY_MATRIX_DIRECTED_HPP
#define ORIGIN_GRAPH_ADJACENCY_MATRIX_DIRECTED_HPP

#include <vector>

#include <origin/utility/empty.hpp>
#include <origin/range/iterator_range.hpp>
#include <origin/optional.hpp>

namespace origin
{

  template<typename Vertex_T = empty,
           typename Edge_T = empty/*,
           typename Alloc = std::allocator<Vertex_T>*/>
  class directed_adjacency_matrix {
    typedef directed_adjacency_matrix<Vertex_T, Edge_T, Alloc> this_type;
    typedef Vertex_T vertex_type;
    typedef Edge_T edge_type;
  public:
    typedef Vertex_T vertex_value_type;
    typedef optional<Edge_T> edge_value_type;
    typedef Alloc allocator_type;
    typedef std::vector<vertex_value_type> vertex_list;
    typedef std::vector<edge_value_type> edge_matrix;

    typedef typename vertex_list::size_type size_type;
    typedef typename vertex_list::difference_type difference_type;

    // Iterator types
    typedef typename vertex_list::iterator vertex_iterator;
    typedef typename vertex_list::const_iterator const_vertex_iterator;
    typedef int edge_iterator;
    typedef int const_edge_iterator;
    typedef float in_edge_iterator;
    typedef float const_in_edge_iterator;
    typedef edge_iterator out_edge_iterator;
    typedef const_edge_iterator const_out_edge_iterator;
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
                              allocator_type const& alloc = allocator_type{}
    );

    // Copy semantics
    directed_adjacency_matrix(directed_adjacency_matrix const&);
    directed_adjacency_matrix& operator=(directed_adjacency_matrix const&);

    // Move Semantics
    directed_adjacency_matrix(directed_adjacency_matrix&&);
    directed_adjacency_matrix& operator=(directed_adjacency_matrix&&);
    //@}

    /** @name Properties */
    //@{
    allocator_type get_allocator() const;
    constexpr size_type max_order() const;
    constexpr size_type max_size() const;
    //@}

    /** @name Graph Properties */
    //@{
    bool null() const;
    size_type order() const;
    bool empty() const;
    size_type size() const;
    //@}

    /** @name Data Structure Operations */
    //@{
    void swap(directed_adjacency_matrix&);
    // Are there clear semantics? This data structure has a fixed size.
    // What is a clear-like operation? make zero?
    //void clear();
    //@}

    /** @name Vertex Properties and Operations */
    //@{
    size_type degree(vertex_iterator) const;
    size_type in_degree(vertex_iterator) const;
    size_type out_degree(vertex_iterator) const;
    //@}

    /** @name Edge Properties and Operations */
    //@{
    edge_iterator add_edge(vertex_iterator,
                           vertex_iterator,
                           edge_value_type const& x = edge_value_type{});
    void remove_edge(edge_iterator);
    // Consider inheritence to reduce code duplication, seems trivial here
    void remove_edge(in_edge_iterator);
    // No overload because this is the same as an edge_iterator
    //void remove_edge(out_edge_iterator);
    void remove_edges(vertex_iterator, vertex_iterator);
    // TODO Since removing edges is complicated (in or out?), non-trivial edge
    // removal has been placed in impl.hpp; they are free operations.

    // TODO Source and target accessors go here.
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

  private:
    vertex_list vertices_;
    edge_matrix edges_;
  };

} // namesapce origin

#endif
