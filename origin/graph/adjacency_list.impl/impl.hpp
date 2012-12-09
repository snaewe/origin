// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ADJACECNY_VECTOR_IMPL_HPP
#define ORIGIN_GRAPH_ADJACECNY_VECTOR_IMPL_HPP

#include <vector>
#include <iterator>

#include <origin/algorithm.hpp>

#include <origin/range.hpp>

#include <origin/graph/vertex.hpp>
#include <origin/graph/edge.hpp>

// Base implementation
// Base implementation is a simple vector of vectors.

// FIXME: Make this a 'dirty' data structure. It will leave holes when stuff
// is deleted. Every now and then it will be 'vacuumed'. At that time, the data
// will be made contiguous.

// Clarification of terms: The adjacency vector is a vector based implementation
// of a adjacency list. The list that holds the neighboring vertices is called
// the vertex list and the sublists are called the neighbor list.

// FIXME: consider scrapping the base impl in favor of two separate types

// Curerntly, this is a vertex cumulable graph base.

// Is there a deeper data structure here? 2 dimensional vector? Can it be
// extended to n-dimensions?

namespace origin
{

  namespace adjacency_vector_detail_
  {
    // Vertex iterator
    class vertex_iterator
    {
      // Assert concepts
      using index_type = std::size_t;
    public:
      using value_type        = vertex_handle<index_type>;
      using reference         = value_type;
      using pointer           = value_type;
      using difference_type   = std::ptrdiff_t;
      using iterator_category = std::bidirectional_iterator_tag;

      // Initializers
      vertex_iterator(index_type i = index_type{0})
        : index_(i)
        { }

      vertex_iterator(const vertex_iterator& v)
        : index_(v.index_)
      { }

      // Readable
      reference operator*() const { return value_type(index_); }
      //pointer   operator->() const { return ; }

      // Equality Comparable
      bool operator==(vertex_iterator x) const
        { return index_ == x.index_; }
      bool operator!=(vertex_iterator x) const
        { return index_ != x.index_; }

      bool operator<(vertex_iterator x) const
        { return index_ < x.index_; }
      bool operator>(vertex_iterator x) const
        { return index_ > x.index_; }
      bool operator<=(vertex_iterator x) const
        { return index_ <= x.index_; }
      bool operator>=(vertex_iterator x) const
        { return index_ >= x.index_; }

      // Increment
      vertex_iterator operator++() { ++index_; return vertex_iterator(index_); }
      vertex_iterator operator++(int)
        { value_type tmp(index_); ++index_; return vertex_iterator(index_); }

      // Decrement
      vertex_iterator operator--() { --index_; return vertex_iterator(index_); }
      vertex_iterator operator--(int)
        { vertex_iterator tmp(index_);--index_; return vertex_iterator(index_);}

    private:
      index_type index_;
    };

    class out_edge_iterator_impl {

      using size_type = std::size_t;
      using vertex = vertex_handle<size_type>;
      using base_iterator = std::vector<vertex>::iterator;
    public:
      using value_type        = undirected_edge_handle<size_type>;
      using reference         = value_type;
      using pointer           = value_type;
      using difference_type   = std::ptrdiff_t;
      using iterator_category = std::bidirectional_iterator_tag;


      // Initializers
      out_edge_iterator_impl(size_type source, base_iterator i)
        : source_(source), iterator_(i)
      { }

      out_edge_iterator_impl(out_edge_iterator_impl const& i)
        : source_(i.source_), iterator_(i.iterator_)
      { }


      // Readable
      reference operator*() const
      {
        auto target = *iterator_;
        return value_type(
          edge_handle<size_type>(f(target.value(),source_)),
          target, vertex(source_)
        );
      }
      //pointer   operator->() const { return ; }

      // Equality Comparable
      // By definition, only iterators from the same container are valid for
      // comparison. Hence, we don't bother comparing source.
      bool operator==(out_edge_iterator_impl x) const
        { return iterator_ == x.iterator_; }
      bool operator!=(out_edge_iterator_impl x) const
        { return iterator_ != x.iterator_; }

      bool operator<(out_edge_iterator_impl x) const
        { return iterator_ < x.iterator_; }
      bool operator>(out_edge_iterator_impl x) const
        { return iterator_ > x.iterator_; }
      bool operator<=(out_edge_iterator_impl x) const
        { return iterator_ <= x.iterator_; }
      bool operator>=(out_edge_iterator_impl x) const
        { return iterator_ >= x.iterator_; }

      // Increment
      out_edge_iterator_impl operator++()
      { return out_edge_iterator_impl(source_, ++iterator_); }
      out_edge_iterator_impl operator++(int)
      {
        base_iterator tmp(iterator_);
        ++iterator_;
        return out_edge_iterator_impl(source_, tmp);
      }

      // Decrement
      out_edge_iterator_impl operator--()
      { return out_edge_iterator_impl(source_, --iterator_); }
      out_edge_iterator_impl operator--(int)
      {
        base_iterator tmp(iterator_);
        --iterator_;
        return out_edge_iterator_impl(source_, iterator_);
      }

    private:
      // FIXME: Find a better bijection NxN -> N
      // Currently use Cantor Pairing function
      size_type f(size_type a, size_type b) const
      { return ((a + b)*(a + b + size_type{1})) / size_type{2} + b; }

      const size_type source_;
      std::vector<vertex>::iterator iterator_;
    };

    class vertex_range_impl
    {
      using size_type = std::size_t;
    public:
      using value_type = Value_type<vertex_iterator>;
      using difference_type = Distance_type<vertex_iterator>;
      using iterator = vertex_iterator;

      vertex_range_impl(size_type a)
        : begin_(vertex_iterator(0)), end_(vertex_iterator(a))
      { }

      vertex_range_impl(size_type a, size_type b)
        : begin_(vertex_iterator(a)), end_(vertex_iterator(b))
      { }

      vertex_range_impl(vertex_iterator a, vertex_iterator b)
        : begin_(a), end_(b)
      { }

      vertex_iterator begin() { return begin_; }
      vertex_iterator end()   { return end_; }
    private:
      vertex_iterator begin_, end_;
    };

    // This is essentially the undirected vertex
    // Does not currently support multigraph
    class adjacency_vector_base
    {
    public:

      // Type definitions

      using vertex = vertex_handle<std::size_t>;
      using edge = undirected_edge_handle<std::size_t>;

      using vertex_vector = std::vector<vertex>;

      using vertex_range = vertex_range_impl;
      using edge_range = int;
      using out_edge_range = vertex_vector;

      using size_type = vertex_vector::size_type;



      // Initializers

      adjacency_vector_base(size_type n = 0)
        : vertex_vector_(n)
      { }



      // Graph metrics

      bool empty() const { return size_type{0} == edge_count_; }
      size_type size() const { return edge_count_; }
      bool null() const { return vertex_vector_.empty(); }
      size_type order() const { return vertex_vector_.size(); }

      size_type adjacent_size(vertex v) const;

      bool predecessor(vertex v, vertex u) {
        return some_equal(vertex_vector_[v.value()], u)
            || some_equal(vertex_vector_[u.value()], v);
      }

      // Graph structure modifiers
      vertex add_vertex();
      void remove_vertex(vertex v);  // deletes an entry from the vertex
      edge add_edge(vertex v, vertex u);
      void erase_edge(edge e);
      void remove_edges(vertex u, vertex v);

      // Helper functions for derived classes
      void increment_edge_count() { ++edge_count_; }
      void decrement_edge_count() { --edge_count_; }

      vertex_range vertices() const
        { return vertex_range(vertex_iterator(0),vertex_iterator(order())); }
      out_edge_range out_edges(vertex v) const
        { return vertex_vector_[v.value()]; }


    private:
      size_type edge_count_;
      std::vector<vertex_vector> vertex_vector_;
    };

    // Thin wrapper over vector<vector<T>> tailored for adj vector
    /*class vertex_vector_vector_base
    {
    public:

      // Type definitions

      using vertex = vertex_handle<std::size_t>;
      using edge = undirected_edge_handle<std::size_t>;

      using vertex_vector = std::vector<vertex>;

      using vertex_range = vertex_range_impl;
      using edge_range = int;
      using out_edge_range = vertex_vector;

      using size_type = vertex_vector::size_type;



      // Initializers
      vertex_vector_vector_base(size_type n = 0) : main_vector_(n) { }



      // Graph metrics
      bool empty()      const { return size_type{0} == edge_count_; }
      size_type size()  const { return edge_count_; }
      bool null()       const { return vertex_vector_.empty(); }
      size_type order() const { return vertex_vector_.size(); }

      size_type subvector_size(vertex v) const;

      bool contains(vertex v, vertex u) {
        return some_equal(vertex_vector_[v.value()], u)
            || some_equal(vertex_vector_[u.value()], v);
      }

      // Graph structure modifiers
      vertex add_subvector();
      void remove_subvector(vertex v);  // deletes a vector from the list
      edge subvector_push(vertex v, vertex u); // Adds u to v
      void subvector_erase(edge e);
      void subvector_erase_all(vertex u, vertex v); // erases occurances of v in u

      // Data accessors

      vertex_range vertices() const
        { return vertex_range(vertex_iterator(0),vertex_iterator(order())); }
      out_edge_range out_edges(vertex v) const
        { return vertex_vector_[v.value()]; }


    private:
      size_type edge_count_;
      std::vector<vertex_vector> main_vector_;
    };*/

    auto adjacency_vector_base::add_vertex() -> vertex
    {
      vertex_vector_.push_back(vertex_vector());
      return vertex(order());
    }

    void adjacency_vector_base::remove_vertex(vertex v);
    {
      // remove entry in the main vector
      vertex_vector_.erase(vertex_vector_.begin() + v.value);
    }

    auto adjacency_vector_base::add_edge(vertex v, vertex u) -> edge
    {
      //assert(( !find(edge_vertex_[v.value], v) ));

      ++edge_count_;
      (vertex_vector_[v.value()]).push_back(u);
      return edge(edge_handle<std::size_t>(v.value()),v,u);
    }


  } // namespace adjacency_vector_detail_

} // namespace origin


#endif // ORIGIN_GRAPH_ADJACECNY_VECTOR_IMPL_HPP





class adj_vec_base
{
public:

};