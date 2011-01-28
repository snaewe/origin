// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ADJACENCY_MATRIX_IMPL_HPP
#define ORIGIN_GRAPH_ADJACENCY_MATRIX_IMPL_HPP

#include <cstddef>

/**
 * Contains implementation details for origin.adjacency matrix.
 *
 * Handles - vertex_t and edge_t act as keys to user-defined vertecies and edges
 *    TODO Consider embedding source/target data into edge_t
 */

namespace origin {
  namespace adj_mtx_impl_ {
    namespace handles {

      class handle_t
      {
      public:
        typedef std::size_t value_type;

        handle_t()
          : value(-1)
        { }

        explicit handle_t(value_type x)
          : value(x)
        { }

        handle_t(handle_t const&) = default;
        handle_t& operator=(handle_t const&) = default;

        inline bool operator==(handle_t x) const
        {
          return value == x.value;
        }

        inline bool operator<(handle_t x) const
        {
          return value < x.value;
        }

        inline bool valid() const
        {
          return value != value_type(-1);
        }

        value_type value;
      };

      /**
       * The vertex_t type implements a handle for graph data structures using
       * integral values for vertex handles.
       */
      class vertex_t
        : public handle_t
      {
      public:
        explicit vertex_t(value_type x)
          : handle_t(x)
        { }

        vertex_t() = default;
        vertex_t(vertex_t const&) = default;
        vertex_t& operator=(vertex_t const&) = default;
      };

      /**
       * The edge_t type implements a handle for graph data structures using
       * integral values for edge handles.
       */
      class edge_t
        : public handle_t
        , public writeable_facade<edge_t>
      {
      public:
        explicit edge_t(value_type x)
          : handle_t(x)
        { }

        edge_t() = default;
        edge_t(edge_t const&) = default;
        edge_t& operator=(edge_t const&) = default;
      };

    } // namespace handles
  }   // namespace adj_mtx_impl_
}     // namespace origin

// Allow interoperability with std::hash<>.
namespace std {
  template<>
  struct hash<origin::graph_::vertex_t>
    : public hash<std::size_t>
  {
    typedef std::size_t size_type;

    size_type operator()(origin::graph_::vertex_t const& v) const {
      return hash<size_type>::operator()(v.value);
    }
  };

  size_type operator()(origin::graph_::vertex_t const& v) const {
    return hash<size_type>::operator()(v.value);
  }
};

template<>
struct hash<origin::graph_::edge_t>
  : public hash<std::size_t>
{
  typedef std::size_t size_type;

  size_type operator()(origin::graph_::edge_t const& e) const {
    return hash<size_type>::operator()(e.value);
  }
};

} // namespace std

#endif

