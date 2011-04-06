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
   * The edge_t type represents in index into indexable graph.
   */
  class edge_t
    : public implicit_bool_facade<edge_t>
  {
  public:
    typedef std::size_t value_type;

    edge_t()
      : value(-1)
    { }

    edge_t(std::size_t x)
      : value(x)
    { }

    bool equal(edge_t x) const
    { return value == x.value; }

    bool less(edge_t x) const
    { return value < x.value; }

    bool valid() const
    { return value != value_type(-1); }

    value_type value;
  };
} // namespace origin

// Support std::hash interoperability.
namespace std
{
  template<>
  struct hash<origin::edge_t>
    : public hash<std::size_t>
  {
    typedef std::size_t size_type;

    size_type operator()(origin::edge_t const& e) const
    { return hash<size_type>::operator()(e.value); }
  };

} // namespace std

namespace origin
{
  /**
   * The edge iterator provides a random access iterator over an edge index
   * types. The result of dereferencing an edge iterator is an edge_t object.
   */
  class edge_iterator
    : public random_access_iterator_facade<
        edge_iterator, edge_t, edge_t, edge_t
      >
  {
    typedef random_access_iterator_facade<
        edge_iterator, edge_t, edge_t, edge_t
    > base_type;
  public:
    typedef typename base_type::reference reference;
    typedef typename base_type::difference_type difference_type;

    edge_iterator(edge_t v)
      : edge_(v)
    { }

    reference dereference() const
    { return edge_; }

    bool equal(edge_iterator iter) const
    { return edge_.value == iter.edge_.value; }

    bool less(edge_iterator iter) const
    { return edge_.value < iter.edge_.value; }

    void increment()
    { ++edge_.value; }

    void decrement()
    { --edge_.value; }

    void advance(difference_type n)
    { edge_.value += n; }

    difference_type distance(edge_iterator iter)
    { return iter.edge_.value - edge_.value ; }

  private:
    edge_t edge_;
  };

  // FIXME: The following functions do not belong in this file. I'm not
  // quite sure where they go, but it's not here.

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
  { return g.out_edges(v); }

  template<typename Graph>
  typename std::enable_if<
    is_directed_graph<Graph>::value,
    typename Graph::const_out_edge_range
  >::type
  out_edges(Graph const& g, typename Graph::const_vertex v)
  { return g.out_edges(v); }

  // Specializations for undirected graphs
  template<typename Graph>
  typename std::enable_if<
    is_undirected_graph<Graph>::value,
    typename Graph::incident_edge_range
  >::type
  out_edges(Graph& g, typename Graph::vertex v)
  { return g.incident_edges(v); }

  template<typename Graph>
  typename std::enable_if<
    is_undirected_graph<Graph>::value,
    typename Graph::const_incident_edge_range
  >::type
  out_edges(Graph const& g, typename Graph::const_vertex v)
  { return g.incident_edges(v); }
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

    typedef decltype(
      out_edges(std::declval<Graph&>(), std::declval<vertex>())
    ) range;
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
