// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ADJACENCY_MATRIX_IMPL_HPP
#define ORIGIN_GRAPH_ADJACENCY_MATRIX_IMPL_HPP

#include <origin/graph/traits.hpp>

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

      edge_t(value_type value, value_type source, value_type target)
        : handle_t(value), source_(source), target_(target)
      { }

      edge_t() = default;
      edge_t(edge_t const&) = default;
      edge_t& operator=(edge_t const&) = default;

      vertex_t source()
      { return vertex_t(source_); }

      vertex_t target()
      { return vertex_t(target_); }

      value_type source_;
      value_type target_;
    };



    /** (out_)edge_iterator implementation. */
    template<typename Graph>
    class vertex_iter_t
    {
      public:
      typedef typename Graph::vertex value_type;
      typedef typename Graph::vertex reference;
      typedef std::ptrdiff_t difference_type;
      typedef std::bidirectional_iterator_tag iterator_category;

      typedef std::size_t size_type;

      vertex_iter_t() = delete;

      vertex_iter_t(size_type index)
        : index_(index)
      { }

      vertex_iter_t(vertex_iter_t const&) = default;

      // Dereference
      reference operator*()
      { return vertex_t(index_); }

      const reference operator*() const
      { return vertex_t(index_); }

      // Implicit conversion to edge_t
      operator vertex_t()
      { return vertex_t(index_); }

      // Increment
      vertex_iter_t& operator++()
      {
        ++index_;
        return *this;
      }

      vertex_iter_t operator++(int)
      {
        vertex_iter_t copy = *this;
        ++index_;
        return copy;
      }

      // Decrement
      vertex_iter_t& operator--()
      {
        --index_;
        return *this;
      }

      vertex_iter_t operator--(int)
      {
        vertex_iter_t copy(*this);
        --index_;
        return copy;
      }

      // Equality
      friend inline bool operator==(vertex_iter_t const& x, vertex_iter_t const& y)
      { return x.index_ == y.index_; }

      friend inline bool operator!=(vertex_iter_t const& x, vertex_iter_t const& y)
      { return !(x == y); }

      // Order
      friend inline bool operator<(vertex_iter_t const& x, vertex_iter_t const& y)
      { return x.index_ < y.index_; }

      friend inline bool operator>(vertex_iter_t const& x, vertex_iter_t const& y)
      { return y < x; }

      friend inline bool operator<=(vertex_iter_t const& x, vertex_iter_t const& y)
      { return !(y < x); }

      friend inline bool operator>=(vertex_iter_t const& x, vertex_iter_t const& y)
      { return !(x < y); }

    private:
      size_type index_;
    };



    /** (out_)edge_iterator implementation. */
    template<typename Graph>
    class edge_iter
    {
      public:
      typedef typename Graph::edge value_type;
      typedef typename Graph::edge reference;
      typedef typename Graph::edge_value_type* pointer;
      typedef std::ptrdiff_t difference_type;
      typedef std::bidirectional_iterator_tag iterator_category;

      typedef std::size_t size_type;

      edge_iter() = delete;

      edge_iter(Graph* g, size_type index)
        : g_(g), index_(index), order_(g->order())
      { move_to_next_edge(); }

      edge_iter(edge_iter const&) = default;

      // Dereference
      reference operator*()
      { return edge_t(index_, index_ / order_, index_ % order_); }

      const reference operator*() const
      { return edge_t(index_, index_ / order_, index_ % order_); }

      // Implicit conversion to edge_t
      operator edge_t()
      { return edge_t(index_); }

      // Increment
      edge_iter& operator++()
      {
        move_to_next_edge();
        return *this;
      }

      edge_iter operator++(int)
      {
        edge_iter copy = *this;
        move_to_next_edge();
        return copy;
      }

      // Decrement
      edge_iter& operator--()
      {
        move_to_previous_edge();
        return *this;
      }

      edge_iter operator--(int)
      {
        edge_iter copy(*this);
        move_to_previous_edge();
        return copy;
      }

      // Equality
      friend inline bool operator==(edge_iter const& x, edge_iter const& y)
      { return x.index_ == y.index_; }

      friend inline bool operator!=(edge_iter const& x, edge_iter const& y)
      { return !(x == y); }

      // Order
      friend inline bool operator<(edge_iter const& x, edge_iter const& y)
      { return x.index_ < y.index_; }

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
        while(!(*g_)[edge_t(index_)] && index_ < order_ * order_)
        { ++index_; }
      }
      void move_to_previous_edge()
      {
        do { --index_; }
        while(!(*g_)[edge_t(index_)] && index_ != 0);
      }

      // Members
      
      Graph* g_;
      size_type index_;
      // Order is in g, but used so often that it merits local scope.
      size_type order_;
    };

    /** in_edge_iterator implementation. 
        FIXME NOT IMPLEMENTED YET!*/
    template<typename Graph>
    class in_edge_iter
    {
      public:
      typedef typename Graph::edge value_type;
      typedef typename Graph::edge reference;
      typedef typename Graph::edge_value_type* pointer;
      typedef std::ptrdiff_t difference_type;
      typedef std::bidirectional_iterator_tag iterator_category;

      typedef std::size_t size_type;

      in_edge_iter() = delete;

      in_edge_iter(Graph* g, size_type index)
        : g_(g), index_(index), order_(g->order())
      { move_to_next_edge(); }

      in_edge_iter(in_edge_iter const&) = default;

      // Dereference
      reference operator*()
      { return edge_t(index_, index_ / order_, index_ % order_); }

      const reference operator*() const
      { return edge_t(index_, index_ / order_, index_ % order_); }

      // Implicit conversion to edge_t
      operator edge_t()
      { return edge_t(index_); }

      // Increment
      in_edge_iter& operator++()
      {
        move_to_next_edge();
        return *this;
      }

      in_edge_iter operator++(int)
      {
        in_edge_iter copy = *this;
        move_to_next_edge();
        return copy;
      }

      // Decrement
      in_edge_iter& operator--()
      {
        move_to_previous_edge();
        return *this;
      }

      in_edge_iter operator--(int)
      {
        in_edge_iter copy(*this);
        move_to_previous_edge();
        return copy;
      }

      // Equality
      friend inline bool operator==(in_edge_iter const& x, in_edge_iter const& y)
      { return x.index_ == y.index_; }

      friend inline bool operator!=(in_edge_iter const& x, in_edge_iter const& y)
      { return !(x == y); }

      // Order
      friend inline bool operator<(in_edge_iter const& x, in_edge_iter const& y)
      { return x.index_ < y.index_; }

      friend inline bool operator>(in_edge_iter const& x, in_edge_iter const& y)
      { return y < x; }

      friend inline bool operator<=(in_edge_iter const& x, in_edge_iter const& y)
      { return !(y < x); }

      friend inline bool operator>=(in_edge_iter const& x, in_edge_iter const& y)
      { return !(x < y); }

    private:
      // Helper functions
      void move_to_next_edge()
      {
        while(!(*g_)[edge_t(index_)] && index_ < order_ * order_)
        { index_ += order_; }
      }
      void move_to_previous_edge()
      {
        do { index_ -= order_; }
        while(!(*g_)[edge_t(index_)] && index_ >= order_);
      }

      // Members
      
      Graph* g_;
      size_type index_;
      // Order is in g, but used so often that it merits local scope.
      size_type order_;
    };

    /** Adjacency matrix categories */
    // Flesh out as more categories are added.
    struct d_adj_mtx_tag
      : directed_graph_tag
    { };

    struct und_adj_mtx_tag
      : undirected_graph_tag
    { };


  }   // namespace adj_mtx_impl_
}     // namespace origin

// Allow interoperability with std::hash<>.
namespace std {
  template<>
  struct hash<origin::adj_mtx_impl_::vertex_t>
    : public hash<std::size_t>
  {
    typedef std::size_t size_type;

    size_type operator()(origin::adj_mtx_impl_::vertex_t const& v) const
    { return hash<size_type>::operator()(v.value); }
  };

  template<>
  struct hash<origin::adj_mtx_impl_::edge_t>
    : public hash<std::size_t>
  {
    typedef std::size_t size_type;

    size_type operator()(origin::adj_mtx_impl_::edge_t const& e) const
    { return hash<size_type>::operator()(e.value); }
  };

} // namespace std

#endif

