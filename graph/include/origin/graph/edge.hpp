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
  // The edge_t type represents an ordinal reference to an edge in a Graph.
  // The integral value -1u corresponds to a null edge.
  class edge_t : public implicit_bool_facade<edge_t>
  {
    typedef bool (edge_t::*unspecified_bool_type)() const;
  public:
    typedef std::size_t value_type;
    
    edge_t()
      : value{-1ul}
    { }

    edge_t(value_type n)
      : value{n}
    { }

    // Equality_comparable
    bool operator==(edge_t x) const { return value == x.value; }
    bool operator!=(edge_t x) const { return value != x.value; }
    
    // Totally_ordered
    bool operator<(edge_t x) const { return value < x.value; }
    bool operator>(edge_t x) const { return value > x.value; }
    bool operator<=(edge_t x) const { return value <= x.value; }
    bool operator>=(edge_t x) const { return value >= x.value; }
    
    // Safe bool
    bool valid() const { return value != value_type{-1ul}; }

    value_type value;
  };

  // Ordinal<edge_t>
  inline edge_t::value_type ord(edge_t e)
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
  // The edge iterator provides a random access iterator over an edge index
  // types. The result of dereferencing an edge iterator is an edge_t object.
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

  // The undirected_edge_t is a triple containing an edge handle, and two
  // vertex handles: the source and target vertices, respectively. Note that 
  // equality and inequality comparisons are predicated on the underlying edge 
  // and do not include the end points in comparison.
  class undirected_edge_t : public implicit_bool_facade<edge_t>
  {
  public:
    undirected_edge_t()
      : edge{}, source{}, target{}
    { }

    // Initialize the graph over a triple of values describing the current
    // edge handle, the source vertex, and the target vertex.
    undirected_edge_t(edge_t e, vertex_t s, vertex_t t)
      : edge{e}, source{s}, target{t}
    { }

    // Equality_comparable
    bool operator==(undirected_edge_t const& x) const { return edge == x.edge; }
    bool operator!=(undirected_edge_t const& x) const { return edge != x.edge; }

    // Totally_ordered
    bool operator<(undirected_edge_t const& x) const { return edge < x.edge; }
    bool operator>(undirected_edge_t const& x) const { return edge > x.edge; }
    bool operator<=(undirected_edge_t const& x) const { return edge <= x.edge; }
    bool operator>=(undirected_edge_t const& x) const { return edge >= x.edge; }

    // Safe bool
    bool valid() const { return edge; }

    edge_t edge;
    vertex_t source;
    vertex_t target;
  };

  // Semi_ordinal<undirected_edge_t>
  // Undirected edges are not Ordinal because they cannot be uniquely 
  // constructed from an ordinal value. The source and target vertex handles
  // must be assigned by a graph.
  inline std::size_t ord(undirected_edge_t const& e)
  {
    return ord(e.edge);
  }

} // namespace origin

// FIXME: This is gross. A better solution would be to abandon the std's overly
// strict hash model and adopt our own.
namespace std {
  // Hashable<undirected_edge_t>.
  template<>
    struct hash<origin::undirected_edge_t>
    {
      std::size_t operator()(origin::undirected_edge_t const& x) const
      {
        hash<origin::edge_t> h;
        return h(x.edge);
      }
    };
} // namespace std

namespace origin {

  // The undirected edge iterator is used to iterate over the edges in the
  // edge set of an undirected graph.
  //
  // TODO: I'm fairly certain that this can be reused for both adjacency 
  // matrices and static undirected graphs.
  template<typename Graph>
    class undirected_edge_iterator
    {
    public:
      typedef undirected_edge_t               value_type;
      typedef undirected_edge_t const&        reference;
      typedef undirected_edge_t const*        pointer;
      typedef typename std::ptrdiff_t         difference_type;
      typedef std::random_access_iterator_tag iterator_category;

      undirected_edge_iterator()
        : graph{nullptr}, edge{}
      { }
      
      undirected_edge_iterator(Graph& g, undirected_edge_t e)
        : graph{&g}, edge{e}
      { }

      // Readable
      reference operator*() const { return edge; }
      
      // Equality_comparable
      bool operator==(undirected_edge_iterator const& x) const { return edge == x.edge; }
      bool operator!=(undirected_edge_iterator const& x) const { return edge != x.edge; }

      // Totally_ordered
      bool operator<(undirected_edge_iterator const& x) const { return edge == x.edge; }
      bool operator>(undirected_edge_iterator const& x) const { return edge != x.edge; }
      bool operator<=(undirected_edge_iterator const& x) const { return edge == x.edge; }
      bool operator>=(undirected_edge_iterator const& x) const { return edge != x.edge; }

      // Random_access_iterator: advance
      undirected_edge_iterator& operator++() { advance_edge(1); return *this; }
      undirected_edge_iterator& operator--() { advance_edge(-1); return *this; }
      undirected_edge_iterator& operator+=(difference_type n) { advance_edge(n); return *this; }
      undirected_edge_iterator& operator-=(difference_type n) { advance_edge(-n); return *this; }
      
      undirected_edge_iterator operator++(int) 
      {
        undirected_edge_iterator tmp{*this}; 
        this->operator++();
        return tmp;
      }

      friend undirected_edge_iterator 
      operator+(undirected_edge_iterator i, difference_type n) 
      { 
        return i += n; 
      }
      
      friend undirected_edge_iterator 
      operator+(difference_type n, undirected_edge_iterator i) 
      { 
        return i += n; 
      }

      undirected_edge_iterator operator--(int) 
      {
        undirected_edge_iterator tmp{*this}; 
        this->operator--();
        return tmp;
      }

      friend undirected_edge_iterator 
      operator-(undirected_edge_iterator i, difference_type n)
      {
        return i -= n;
      }

      // Random_access_iterator: distance
      friend difference_type 
      operator-(undirected_edge_iterator const& a, undirected_edge_iterator const& b) 
      { 
        return a.value() - b.value();
      }

    private:
      std::size_t value() const
      {
        return edge.edge.value;
      }

      void advance_edge(difference_type n)
      {
        edge = graph->get_edge(value() + 1);
      }

    private:
      Graph* graph;
      undirected_edge_t edge;
    };

  // The undirected incident edge iterator abstracts the notion of a sequence
  // of incident edges by iterating over the chained in- and out-edge ranges
  // of an undirected graph's underlying directed implementation.
  //
  // Note that we don't cache the referenced edge, so we dereference a copy.
  // This means that you can't use -> with these iterators.
  template<typename Graph>
    class undirected_incident_edge_iterator
    {
      typedef typename graph_traits<Graph>::size_type Size;
    public:
      typedef undirected_edge_t value_type;
      typedef undirected_edge_t reference;
      typedef undirected_edge_t pointer;
      typedef std::ptrdiff_t difference_type;
      typedef std::random_access_iterator_tag iterator_category;
      
      // Initialize the iterator so that it refers to the nth incident edge
      // of v where n < degree(g, v).
      undirected_incident_edge_iterator(Graph& g, vertex_t v, Size n)
        : graph{&g}, source{v}, index{n}
      { }
      
      // Readable
      reference operator*() const { return get_edge(); }
      pointer operator->() const { return get_edge(); }
      
      // Equality_comparable
      bool operator==(undirected_incident_edge_iterator const& x) const { return index == x.index; }
      bool operator!=(undirected_incident_edge_iterator const& x) const { return index != x.index; }

      // Totally_ordered
      bool operator<(undirected_incident_edge_iterator const& x) const { return index == x.index; }
      bool operator>(undirected_incident_edge_iterator const& x) const { return index != x.index; }
      bool operator<=(undirected_incident_edge_iterator const& x) const { return index == x.index; }
      bool operator>=(undirected_incident_edge_iterator const& x) const { return index != x.index; }

      // Random_access_iterator: advance
      undirected_incident_edge_iterator& operator++() { ++index; return *this; }
      undirected_incident_edge_iterator& operator--() { --index; return *this; }
      
      undirected_incident_edge_iterator operator++(int) 
      {
        undirected_incident_edge_iterator tmp{*this}; 
        this->operator++();
        return tmp;
      }

      friend undirected_incident_edge_iterator 
      operator+(undirected_incident_edge_iterator i, difference_type n) 
      { 
        return i += n; 
      }
      
      friend undirected_incident_edge_iterator 
      operator+(difference_type n, undirected_incident_edge_iterator i) 
      { 
        return i += n; 
      }

      undirected_incident_edge_iterator operator--(int)
      {
        undirected_incident_edge_iterator tmp{*this}; 
        this->operator--();
        return tmp;
      }

      friend undirected_incident_edge_iterator 
      operator-(undirected_incident_edge_iterator i, difference_type n)
      {
        return i -= n;
      }

      // Random_access_iterator: distance
      friend difference_type 
      operator-(undirected_incident_edge_iterator const& a, 
                undirected_incident_edge_iterator const& b) 
      { 
        return a.value() - b.value();
      }

    private:
      undirected_edge_t get_edge() const
      {
        return graph->get_incident_edge(source, index);
      }

    private:
      Graph* graph;
      vertex_t source;
      Size index;
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
