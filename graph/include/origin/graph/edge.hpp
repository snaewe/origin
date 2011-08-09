// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_EDGE_HPP
#define ORIGIN_GRAPH_EDGE_HPP

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
  class edge_t
    : public implicit_bool_facade<edge_t>
  {
    typedef bool (edge_t::*unspecified_bool_type)() const;
  public:
    typedef std::size_t value_type;

    edge_t(value_type x = -1)
      : value{x}
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
    bool valid() const { return value != value_type{-1}; }

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
    struct hash<origin::edge_t> : public hash<std::size_t>
    {
      std::size_t operator()(origin::edge_t e) const
      { 
        return hash<std::size_t>::operator()(e.value); 
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
    typedef std::ptrdiff_t difference_type;

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
      return j->value - i->value; 
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
      typedef typename graph_traits<Graph>::vertex Edge;
      
      has_target_pred(Graph& g, Vertex v)
        : graph(g), vertex{v}
      { }
      
      bool operator()(Edge e) const
      {
        return graph.target(e) == vertex;
      }
      
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
      typedef typename graph_traits<Graph>::vertex Edge;
      
      has_source_pred(Graph& g, Vertex v)
        : graph(g), vertex{v}
      { }
      
      bool operator()(Edge e) const
      {
        return graph.source(e) == vertex;
      }
      
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

  // FIXME: The following functions do not belong in this file. I'm not
  // quite sure where they go. Maybe traits?

  // NOTE: We could *amlost* get the directed/undirected selection to work
  // without enable_if if undirected graphs don't derive from directed graphs.
  // Unfortunately, access violations don't count as SFINAE errors.

  // NOTE: If a graph is somehow both directed and undirected, then we're going
  // to have ambiguity problems.

  
  /**
   * @fn out_edges(g, v)
   * Return the set of out edges of a vertex. This function is overloaded for
   * both directed and undirected graphs. For undirected graphs, the set of
   * out edges is defined as the set of incident edges.
   */
  //@{
  // Specializations for directed graphs.
  template<typename Graph>
    typename std::enable_if<
      is_directed_graph<Graph>::value,
      typename Graph::out_edge_range
    >::type
    out_edges(Graph& g, typename Graph::vertex v)
    { 
      return g.out_edges(v); 
    }

  template<typename Graph>
    typename std::enable_if<
      is_directed_graph<Graph>::value,
      typename Graph::const_out_edge_range
    >::type
    out_edges(Graph const& g, typename Graph::const_vertex v)
    { 
      return g.out_edges(v); 
    }

  // Specializations for undirected graphs
  template<typename Graph>
    typename std::enable_if<
      is_undirected_graph<Graph>::value,
      typename Graph::incident_edge_range
    >::type
    out_edges(Graph& g, typename Graph::vertex v)
    { 
      return g.incident_edges(v); 
    }

  template<typename Graph>
    typename std::enable_if<
      is_undirected_graph<Graph>::value,
      typename Graph::const_incident_edge_range
    >::type
    out_edges(Graph const& g, typename Graph::const_vertex v)
    { 
      return g.incident_edges(v); 
    }
  //@}

  // FIXME: This is pretty hinky. It works, but I'm not terribly proud of it.
  /**
   * The range traits class provides a mechanism for accessing the results of
   * the out_edges function.
   */
  template<typename Graph>
    struct outward_graph_traits
    {
      typedef Graph graph_type;
      typedef typename graph_traits<Graph>::vertex vertex;
      typedef typename graph_traits<Graph>::edge edge;

      typedef decltype(out_edges(std::declval<Graph&>(), std::declval<vertex>())) range;
      typedef typename range_traits<range>::iterator iterator;
    };

  /**
   * Return the opposite end of the given edge.
   */
  template<typename Graph, typename Edge, typename Vertex>
    Vertex opposite(Graph& g, Edge e, Vertex v)
    { return g.source(e) == v ? g.target(e) : v; }

} // namesapce origin

#endif
