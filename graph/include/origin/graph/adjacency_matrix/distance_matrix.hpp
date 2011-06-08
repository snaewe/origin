// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ADJACENCY_MATRIX_DISTANCE_MATRIX_HPP
#define ORIGIN_GRAPH_ADJACENCY_MATRIX_DISTANCE_MATRIX_HPP

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
   * Distance Matrix is a specialized adjacency matrix
   *
   * @tparam Vertex  Type of vertices.
   * @tparam Matrix  Type of the edge matrix.
   * @tparam Alloc   Allocator type of the vertex list.
   */
  template<typename Vertex = empty_t,
           typename Matrix = square_dynarray<float>,
           typename Alloc = std::allocator<Vertex>>
  class distance_matrix
    : private adjacency_matrix_base<
        Vertex,
        Matrix,
        adj_mtx_impl_::dist_detail<typename Matrix::value_type>,
        Alloc
      >
  {
    typedef adjacency_matrix_base<
              Vertex,
              Matrix,
              adj_mtx_impl_::dist_detail<typename Matrix::value_type>,
              Alloc
            > base_type;
  public:
    typedef typename base_type::matrix_type        matrix_type;
    typedef typename base_type::vertex_allocator_type vertex_allocator_type;
    typedef typename base_type::vertex_value_type  vertex_value_type;
    typedef typename base_type::edge_value_type    edge_value_type;

    typedef typename base_type::size_type          size_type;
    typedef typename base_type::difference_type    difference_type;

    // Vertex and Edge data types
    typedef typename base_type::vertex             vertex;
    typedef typename base_type::const_vertex       const_vertex;
    typedef typename base_type::edge               edge;
    typedef typename base_type::const_edge         const_edge;

    // Range types
    typedef typename base_type::vertex_range       vertex_range;
    typedef typename base_type::const_vertex_range const_vertex_range;
    typedef typename base_type::edge_range         edge_range;
    typedef typename base_type::const_edge_range   const_edge_range;
    typedef typename base_type::out_edge_range     out_edge_range;
    typedef typename base_type::const_out_edge_range const_out_edge_range;
    typedef typename base_type::in_edge_range      in_edge_range;
    typedef typename base_type::const_in_edge_range const_in_edge_range;

    /** @name Construction, Assignment and Destruction */
    //@{
    distance_matrix()
      : base_type()
    { }

    distance_matrix(matrix_type&& matrix)
      : base_type(matrix)
    { }

    distance_matrix(size_type n)
      : base_type(n)
    { }
    //@}

    /** @name Data Structure Properties */
    //@{
    using base_type::max_order;
    using base_type::max_size;
    //@}

    /** @name Graph Properties */
    //@{
    using base_type::null;
    using base_type::order;
    using base_type::empty;
    using base_type::size;
    //@}

    /** Data Accessors */
    //@{
    using base_type::operator();
    using base_type::operator[];
    using base_type::matrix;
    //@}

    /** @name Vertex Properties and Operations */
    //@{
    using base_type::out_degree;
    using base_type::in_degree;
    using base_type::degree;
    //@}

    /** @name Edge Properties and Operations */
    //@{
    using base_type::add_edge;
    using base_type::remove_edge;
    using base_type::remove_edges;
    using base_type::get_edge;
    using base_type::source;
    using base_type::target;
    //@}

    /** @name Ranges */
    //@{
    using base_type::vertices;
    using base_type::edges;
    using base_type::out_edges;
    using base_type::in_edges;
    //@}

    /** @name Data Structure Operations */
    //@{
    inline void swap(distance_matrix& x)
    { return base_type::swap(x); }
    inline bool equal(distance_matrix const& x) const
    { return base_type::equal(x); }
    //@}
  };

} // origin

#endif
