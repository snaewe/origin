// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_EDGE_HPP
#define ORIGIN_GRAPH_EDGE_HPP

#include <iterator>
#include <functional>

#include <origin/iterator/facades.hpp>
#include <origin/range/support.hpp>
#include <origin/graph/traits.hpp>

namespace origin
{
 /**
  * The edge_t type represents an ordinal reference to an edge in a Graph.
  * The integral value -1u corresponds to a null edge.
  */
  class edge_t : public implicit_bool_facade<edge_t>
  {
    typedef bool (edge_t::*unspecified_bool_type)() const;
  public:
    typedef std::size_t value_type;
    
    edge_t()
      : value{-1u}
    { }

    edge_t(value_type n)
      : value{n}
    { }

    // Equatable
    bool operator==(edge_t x) const { return value == x.value; }
    bool operator!=(edge_t x) const { return value != x.value; }
    
    // Totally_Ordered
    bool operator<(edge_t x) const { return value < x.value; }
    bool operator>(edge_t x) const { return value > x.value; }
    bool operator<=(edge_t x) const { return value <= x.value; }
    bool operator>=(edge_t x) const { return value >= x.value; }
    
    // Safe bool
    bool valid() const { return value != value_type{-1u}; }

    value_type value;
  };
  
  /**
   * Return the ordinal value of the given edge.
   */
  edge_t::value_type ord(edge_t e)
  {
    return e.value;
  }
} // namespace origin

// FIXME: When are we going to have Origin::Hashable?
// Support std::hash interoperability.
namespace std
{
  template<>
    struct hash<origin::edge_t>
    {
      std::size_t operator()(origin::edge_t e) const
      { 
        return std::hash<std::size_t>{}(e.value); 
      }
    };
} // namespace std

namespace origin
{
  /**
   * The edge iterator provides a random access iterator over an edge index
   * types. The result of dereferencing an edge iterator is an edge_t object.
   */
  class edge_iterator
  {
  public:
    typedef edge_t value_type;
    typedef edge_t const& reference;
    typedef edge_t const* pointer;
    typedef std::ptrdiff_t difference_type;
    typedef std::random_access_iterator_tag iterator_category;

    edge_iterator(edge_t e)
      : edge{e}
    { }

    edge_t const& operator*() const { return edge; }
    edge_t const* operator->() const { return &edge; }

    // Equatable
    bool operator==(edge_iterator x) const { return edge == x.edge; }
    bool operator!=(edge_iterator x) const { return edge != x.edge; }

    // TotallyOrdered
    bool operator<(edge_iterator x) const { return edge < x.edge; }
    bool operator>(edge_iterator x) const { return edge > x.edge; }
    bool operator<=(edge_iterator x) const { return edge <= x.edge; }
    bool operator>=(edge_iterator x) const { return edge >= x.edge; }

    // Increment
    edge_iterator& operator++() { ++edge.value; return *this; }
    edge_iterator operator++(int) { edge_iterator tmp{*this}; ++*this; return tmp; }

    // Decrement
    edge_iterator& operator--() { --edge.value; return *this; }
    edge_iterator operator--(int) { edge_iterator tmp{*this}; --*this; return tmp; }

    // Advance
    edge_iterator& operator+=(difference_type n) { edge.value += n; return *this; }
    edge_iterator& operator-=(difference_type n) { edge.value -= n; return *this; }

    // Next/Prev
    friend edge_iterator operator+(edge_iterator i, difference_type n) { i += n; return i; }
    friend edge_iterator operator+(difference_type n, edge_iterator i) { i += n; return i; }
    friend edge_iterator operator-(edge_iterator i, difference_type n) { i -= n; return i; }
    
    // Distance
    friend difference_type operator-(edge_iterator i, edge_iterator j) 
    {
      return i->value - j->value; 
    }

  private:
    edge_t edge;
  };

  /**
   * The has_target Predicate is used to evaluate whether or not an edge
   * has a given vertex as its target.
   */
  template<typename Graph>
    struct has_target_pred
    {
      typedef typename graph_traits<Graph>::vertex Vertex;
      typedef typename graph_traits<Graph>::edge Edge;
      
      has_target_pred(Graph& g, Vertex v)
        : graph(g), vertex{v}
      { }
      
      bool operator()(Edge e) const { return graph.target(e) == vertex; }
      
      Graph& graph;
      Vertex vertex;
    };
    
  /**
   * Return a predicate that can be used to determine if the given graph has
   * an edge with the specified vertex.
   */
  template<typename Graph>
    inline has_target_pred<Graph> 
    if_has_target(Graph& g, typename graph_traits<Graph>::vertex v)
    {
      return {g, v};
    }

  // FIXME: Requires bidirectional edges?
  /**
   * The has_source Predicate is used to evaluate whether or not an edge
   * has a given vertex as its source.
   */
  template<typename Graph>
    struct has_source_pred
    {
      typedef typename graph_traits<Graph>::vertex Vertex;
      typedef typename graph_traits<Graph>::edge Edge;
      
      has_source_pred(Graph& g, Vertex v)
        : graph(g), vertex{v}
      { }
      
      bool operator()(Edge e) const { return graph.source(e) == vertex; }
      
      Graph& graph;
      Vertex vertex;
    };

  /**
   * Return a predicate that can be used to determine if the given graph has
   * an edge with the specified vertex.
   */
  template<typename Graph>
    inline has_source_pred<Graph> 
    if_has_source(Graph& g, typename graph_traits<Graph>::vertex v)
    {
      return {g, v};
    }
  
} // namesapce origin

#endif
