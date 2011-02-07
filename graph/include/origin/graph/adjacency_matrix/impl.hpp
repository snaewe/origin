// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ADJACENCY_MATRIX_IMPL_HPP
#define ORIGIN_GRAPH_ADJACENCY_MATRIX_IMPL_HPP

#include <cstddef>
#include <iterator>
#include <limits>

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



    /** in_edge_iterator implementation. */
    template<typename Graph>
    class edge_iter
    {
      public:
      typedef typename Graph::edge_value_type value_type;
      typedef typename Graph::edge_value_type& reference;
      typedef typename Graph::edge_value_type* pointer;
      typedef std::ptrdiff_t difference_type;
      typedef std::bidirectional_iterator_tag iterator_category;

      typedef std::size_t size_type;

      edge_iter() = delete;

      edge_iter(Graph* g, size_type source, size_type target)
        : g_(g), source_(source), target_(target)
      { }

      edge_iter(edge_iter const&) = default;

      // Dereference
      reference operator*()
      { return (*g_)(source_, target_); }

      const reference operator*() const
      { return (*g_)(source_, target_); }

      pointer operator->()
      { return &((*g_)(source_, target_)); }

      const pointer operator->() const
      { return &((*g_)(source_, target_)); }

      // Implicit conversion to edge_t
      operator handles::edge_t()
      { return handles::edge_t(source_ * g_->order() + target_); }

      // Increment
      edge_iter& operator++()
      {
        // There is a better way to do this.
        if(target_ == g_->order() - 1)
        {
          target_ = 0;
          ++source_;
        }
        else
          ++target_;
        return *this;
      }

      edge_iter operator++(int)
      {
        edge_iter copy = *this;

        // There is a better way to do this.
        if(target_ == g_->order() - 1)
        {
          target_ = 0;
          ++source_;
        }
        else
          ++target_;

        return copy;
      }

      // Decrement
      edge_iter& operator--()
      {
        // There is a better way to do this
        if(target_ == 0)
        {
          target_ = g_->order() - 1;
          -- source_;
        }
        else
          --target_;

        return *this;
      }

      edge_iter operator--(int)
      {
        edge_iter copy(*this);

        // There is a better way to do this
        if(target_ == 0)
        {
          target_ = g_->order() - 1;
          -- source_;
        }
        else
          --target_;

        return copy;
      }

      // Equality
      friend inline bool operator==(edge_iter const& x, edge_iter const& y)
      { return x.source_ == y.source_ && x.target_ == y.target_; }

      friend inline bool operator!=(edge_iter const& x, edge_iter const& y)
      { return !(x == y); }

      // Order
      friend inline bool operator<(edge_iter const& x, edge_iter const& y)
      { return x.source_ < y.source_ && x.target_ < y.target_; }

      friend inline bool operator>(edge_iter const& x, edge_iter const& y)
      { return y < x; }

      friend inline bool operator<=(edge_iter const& x, edge_iter const& y)
      { return !(y < x); }

      friend inline bool operator>=(edge_iter const& x, edge_iter const& y)
      { return !(x < y); }

    private:
      // Helper functions
      void move_to_next_edge()
      {
      }
      void move_to_previous_edge()

      // Members
      size_type source_;
      size_type target_;
      Graph* g_;
    };

  }   // namespace adj_mtx_impl_
}     // namespace origin

// Allow interoperability with std::hash<>.
namespace std {
  template<>
  struct hash<origin::adj_mtx_impl_::handles::vertex_t>
    : public hash<std::size_t>
  {
    typedef std::size_t size_type;

    size_type operator()(origin::adj_mtx_impl_::handles::vertex_t const& v) const
    { return hash<size_type>::operator()(v.value); }
  };

  template<>
  struct hash<origin::adj_mtx_impl_::handles::edge_t>
    : public hash<std::size_t>
  {
    typedef std::size_t size_type;

    size_type operator()(origin::adj_mtx_impl_::handles::edge_t const& e) const
    { return hash<size_type>::operator()(e.value); }
  };

} // namespace std

#endif

