// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ADJACENCY_MATRIX_IMPL_HPP
#define ORIGIN_GRAPH_ADJACENCY_MATRIX_IMPL_HPP

#include <origin/graph/traits.hpp>
#include <origin/graph/vertex.hpp>

#include <cstddef>

/**
 * @internal
 * @group graph_adjacency
 *
 * Contains all implementation details for adjacency matrix.
 */

namespace origin
{
  namespace adj_mtx_impl_
  {
    /**
     * @internal
     * @group graph_adjacency
     *
     * Custom edge handle for adjacency matrices that contains row (source) and
     * column (target).
     */
    class edge_t
    {
    public:
      typedef std::size_t value_type;

      edge_t()
        : source(-1), target(-1)
      { }

      edge_t(value_type source, value_type target)
        : source(source), target(target)
      { }

      edge_t(edge_t const&) = default;
      edge_t& operator=(edge_t const&) = default;

      inline bool operator==(edge_t x) const
      { return source == x.source && target == x.target; }

      inline bool valid() const
      { return source != value_type(-1) || target != value_type(-1); }

      value_type source;
      value_type target;
    };

    // FIXME These are temporary jury rigged predicates that define what an edge
    //       is relative to the different adjacency matrix types.
    struct bool_mtx
    {
      template<typename Boolean>
      inline static bool edge(Boolean b)
      { return b; }
    };

    struct dist_mtx
    {
      template<typename Distance>
      inline static bool edge(Distance d)
      { return d != Distance(); }
    };

    /**
     * @internal
     * @group graph_adjacency
     *
     * Vertex iterator for adjacency matrix.
     */
    class vertex_iterator
      : public random_access_iterator_facade<
          vertex_iterator,
          vertex_t,
          vertex_t,
          vertex_t
        >
    {
    public:
      typedef std::size_t size_type;
      typedef std::ptrdiff_t difference_type;

      vertex_iterator() = delete;

      vertex_iterator(size_type n)
        : index_(n)
      { }

      vertex_iterator(vertex_iterator const&) = default;

      operator vertex_t()
      { return vertex_t(index_); }

      reference dereference() const
      { return vertex_t(index_); }

      bool equal(vertex_iterator const& x) const
      { return index_ == x.index_; }

      bool less(vertex_iterator const& x) const
      { return index_ < x.index_; }

      void increment()
      { ++index_; }

      void decrement()
      { --index_; }

      void advance(difference_type n)
      { index_ += n; }

      difference_type distance(vertex_iterator const& x)
      { return ((signed int)index_) - ((signed int)x.index_); }

    private:
      size_type index_;
    };

    /**
     * @internal
     * @group graph_adjacency
     *
     * Edge iterator (and out edge iterator) for boolean adjacency matrix.
     *
     * FIXME end_ can be removed.
     */
    template<typename Graph, typename EdgePred>
    class edge_iterator
      : public forward_iterator_facade<
          edge_iterator<Graph, EdgePred>,
            edge_t,
            edge_t,
            edge_t
          >
    {
      typedef typename Graph::edge_value_type edge_value_type;
    public:
      typedef std::size_t size_type;
      typedef std::ptrdiff_t difference_type;
      typedef typename forward_iterator_facade<
          edge_iterator<Graph, EdgePred>,
            edge_t,
            edge_t,
            edge_t
          >::reference reference;

      edge_iterator() = delete;

      edge_iterator(Graph& g, size_type n)
        : graph_(g), index_(n), order_(g.order()), end_(g.order() * g.order())
      {
        if(!EdgePred::template edge<edge_value_type>(graph_(0,0)))
          next_edge();
      }

      edge_iterator(edge_iterator const&) = default;

      operator edge_t()
      { return edge_t(index_ / order_, index_ % order_); }

      reference dereference() const
      { return edge_t(index_ / order_, index_ % order_); }

      bool equal(edge_iterator const& x) const
      { return index_ == x.index_; }

      bool less(edge_iterator const& x) const
      { return index_ < x.index_; }

      void increment()
      { next_edge(); }

    private:
      void next_edge()
      {
        do { ++index_; 
          /*std::cerr << '(' << index_ / order_ << ',' << index_ % order_ << ')';*/
        }
        while(
          !EdgePred::template edge<edge_value_type>(
            graph_(index_ / order_, index_ % order_)
          ) && index_ < end_);
      }

      Graph& graph_;
      size_type index_;
      size_type order_;
      size_type end_;
    };

    /**
     * @internal
     * @group graph_adjacency
     *
     * In iterator (and out edge iterator) for boolean adjacency matrix.
     *
     * FIXME end_ can be removed.
     */
    template<typename Graph, typename EdgePred>
    class in_edge_iterator
      : public forward_iterator_facade<
          in_edge_iterator<Graph, EdgePred>,
            edge_t,
            edge_t,
            edge_t
          >
    {
      typedef typename Graph::edge_value_type edge_value_type;
    public:
      typedef std::size_t size_type;
      typedef std::ptrdiff_t difference_type;
      typedef typename forward_iterator_facade<
          in_edge_iterator<Graph, EdgePred>,
            edge_t,
            edge_t,
            edge_t
          >::reference reference;

      in_edge_iterator() = delete;

      //*boolean_edge_iterator(Graph& g, size_type n)
      in_edge_iterator(Graph& g, size_type n)
        : graph_(g), index_(n), order_(g.order()), end_(g.order() * g.order())
      {
        //std::cerr << n << '\n';
        if(!EdgePred::template edge<edge_value_type>(
             graph_(index_ / order_, index_ % order_)
        ))
          next_edge();
      }

      in_edge_iterator(in_edge_iterator const&) = default;

      operator edge_t()
      { return edge_t(index_ / order_, index_ % order_); }

      reference dereference() const
      { return edge_t(index_ / order_, index_ % order_); }

      bool equal(in_edge_iterator const& x) const
      { return index_ == x.index_; }

      bool less(in_edge_iterator const& x) const
      { return index_ < x.index_; }

      void increment()
      { next_edge(); }

    private:
      void next_edge()
      {
        do { index_ += order_;}
             //if(index_ >= end_) std::cerr << index_ << ':' << order_ << ':' << end_ << '\n';}
        while(//(!EdgePred::template edge<edge_value_type>(
          //graph_(index_ / order_, index_ % order_)
        /*)) &&*/ index_ < end_);
      }

      Graph& graph_;
      size_type index_;
      size_type order_;
      size_type end_;
    };

  } //namespace adj_mtx_impl_
} // namespace origin

#endif //ORIGIN_GRAPH_ADJACENCY_MATRIX_IMPL_HPP
