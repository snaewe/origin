// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ADJACENCY_VECTOR_UNDIRECTED_HPP
#define ORIGIN_GRAPH_ADJACENCY_VECTOR_UNDIRECTED_HPP

#include <vector>

#include <origin/utility/facades.hpp>
#include <origin/graph/adjacency_vector/directed.hpp>

namespace origin
{
  // An undirected edge is a triple containing an edge handle, and two
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

  // The undirected adjacency vector implements an Undirected Adjacency_list.
  // It is implemented as a directed graph where the in-edges of a vertex are
  // considered to also be incident. In this implementation, an undirected edge 
  //is represented by a single directed edge.
  template<typename Vertex = empty_t,
           typename Edge = empty_t,
            typename Alloc = std::allocator<void>>
    class undirected_adjacency_vector
    {
      typedef directed_adjacency_vector<Vertex, Edge, Alloc> base_type;
      typedef undirected_adjacency_vector<Vertex, Edge, Alloc> this_type;
    public:
      typedef typename base_type::vertex_allocator_type vertex_allocator_type;
      typedef typename base_type::edge_allocator_type   edge_allocator_type;

      typedef typename base_type::size_type       size_type;
      typedef typename base_type::difference_type difference_type;
      
      typedef Vertex vertex_value_type;
      typedef Edge   edge_value_type;
      
      typedef vertex_t vertex;
      typedef vertex_t const_vertex;
      
      typedef undirected_edge_t edge;
      typedef undirected_edge_t const_edge;
    private:
      typedef undirected_edge_iterator<this_type> edge_iterator;
      typedef undirected_edge_iterator<this_type const> const_edge_iterator;
      
      typedef undirected_incident_edge_iterator<this_type> incident_edge_iterator;
      typedef undirected_incident_edge_iterator<this_type const> const_incident_edge_iterator;
    public:
      typedef iterator_range<vertex_iterator> vertex_range;
      typedef iterator_range<vertex_iterator> const_vertex_range;
      
      typedef iterator_range<edge_iterator>       edge_range;
      typedef iterator_range<const_edge_iterator> const_edge_range;
      
      typedef iterator_range<incident_edge_iterator>       incident_edge_range;
      typedef iterator_range<const_incident_edge_iterator> const_incident_edge_range;
      
      struct graph_category : undirected_graph_tag, buildable_graph_tag { };

      // Initialization
      
      // Semiregular.
      // Copy and move are implicitly generated.
      undirected_adjacency_vector()
        : base_{}
      { }

      undirected_adjacency_vector(size_type n, 
                                  vertex_value_type const& value = vertex_value_type{})
        : base_(n, value)
      { }

      template<typename Iter>
        undirected_adjacency_vector(Iter first, Iter last)
          : base_(first, last)
        { }
        
      undirected_adjacency_vector(std::initializer_list<vertex_value_type> list)
        : base_(list)
      { }

      // Container properties
      constexpr size_type max_order() const { return base_.max_order(); }
      constexpr size_type max_size() const  { return base_.max_size(); }
      
      vertex_allocator_type get_vertex_allocator() const { return base_.get_vertex_allocator(); }
      edge_allocator_type   get_edge_allocator() const { return base_.get_edge_allocator(); }

      // Implementation properties
      base_type&        base()       { return base_; }
      base_type const&  base() const { return base_; } 

      // Graph properties
      bool null() const { return base_.null(); }
      size_type order() const{ return base_.order(); }
      
      bool empty() const { return base_.empty(); }
      size_type size() const { return base_.size(); }
      
      vertex_value_type&       operator[](vertex v)       { return base_[v]; }
      vertex_value_type const& operator[](vertex v) const { return base_[v]; }
      
      edge_value_type&       operator[](edge e)       { return base_[e.edge]; }
      edge_value_type const& operator[](edge e) const { return base_[e.edge]; }
      
      // Graph operations
      vertex       get_vertex(size_type n)       { return n; }
      const_vertex get_vertex(size_type n) const { return n; }
    
      size_type degree(vertex v) const { return base_.degree(v); }

      edge       get_edge(size_type n)       { return make_edge(edge_t{n}); }
      const_edge get_edge(size_type n) const { return make_edge(edge_t{n}); }

      edge get_edge(vertex u, vertex v);
      const_edge get_edge(vertex u, vertex v) const;
      
      edge       get_incident_edge(vertex v, size_type n);
      const_edge get_incident_edge(vertex v, size_type n) const;
          
      vertex       source(edge e)       { return e.source; } 
      const_vertex source(edge e) const { return e.source; }
      
      vertex       target(edge e)       { return e.target; }
      const_vertex target(edge e) const { return e.target; }

      vertex add_vertex(vertex_value_type const& x = vertex_value_type{});
      edge add_edge(vertex u, vertex v, edge_value_type const& x = edge_value_type{});

      // Ranges 
      vertex_range       vertices()       { return base_.vertices(); }
      const_vertex_range vertices() const { return base_.vertices(); }

      edge_range       edges()       { return {begin_edges(), end_edges()}; }
      const_edge_range edges() const { return {begin_edges(), end_edges()}; }
      
      incident_edge_range       incident_edges(vertex v);
      const_incident_edge_range incident_edges(vertex v) const;

    private:
      // Create an undirected edge using the underlying edge and source vertex.
      edge       make_edge(edge_t e, vertex_t v)       { return {e, v, base_.target(e)}; }
      const_edge make_edge(edge_t e, vertex_t v) const { return {e, v, base_.target(e)}; }

      // Create an undirected edge over the underlying edge type. Use underlying
      // source of the edge as the source vertex.
      edge       make_edge(edge_t e)       { return make_edge(e, base_.source(e)); }
      const_edge make_edge(edge_t e) const { return make_edge(e, base_.source(e)); }

      edge_iterator       begin_edges()       { return {*this, get_edge(0)}; }
      const_edge_iterator begin_edges() const { return {*this, get_edge(0)}; }
      
      edge_iterator       end_edges()       { return {*this, get_edge(order())}; }
      const_edge_iterator end_edges() const { return {*this, get_edge(order())}; }
      
      incident_edge_iterator       begin_incident_edges(vertex v)       { return {*this, v, 0}; }
      const_incident_edge_iterator begin_incident_edges(vertex v) const { return {*this, v, 0}; }
      
      incident_edge_iterator       end_incident_edges(vertex v)       { return {*this, v, degree(v)}; }
      const_incident_edge_iterator end_incident_edges(vertex v) const { return {*this, v, degree(v)}; }
    private:
      base_type base_;
    };

  // FIXME: I think that this might be optimized by searching the vertex
  // with the smaller degree first.
  template<typename V, typename E, typename A>
    auto undirected_adjacency_vector<V, E, A>::
      get_edge(vertex u, vertex v) -> edge
      {
        typename base_type::edge e = base_.get_edge(u, v);
        return e ? make_edge(e, u) : make_edge(base_.get_edge(v, u), v);
      }
    
  template<typename V, typename E, typename A>
    auto undirected_adjacency_vector<V, E, A>::
      get_edge(vertex u, vertex v) const -> const_edge
      {
        typename base_type::edge e = base_.get_edge(u, v);
        return e ? make_edge(e, u) : make_edge(base_.get_edge(v, u), v);
      }


  template<typename V, typename E, typename A>
    auto undirected_adjacency_vector<V, E, A>::
      get_incident_edge(vertex v, size_type n) -> edge
      {
        size_type d = base_.out_degree(v);
        edge_t e = n < d ? base_.get_out_edge(v, n) : base_.get_in_edge(v, n - d);
        return {e, v, target(make_edge(e, v))};
      }

  template<typename V, typename E, typename A>
    auto undirected_adjacency_vector<V, E, A>::
      get_incident_edge(vertex v, size_type n) const -> const_edge
      {
        size_type d = base_.out_degree(v);
        edge_t e = n < d ? base_.get_out_edge(v, n) : base_.get_in_edge(v, n - d);
        return {e, v, target(make_edge(e, v))};
      }


  template<typename V, typename E, typename A>
    auto undirected_adjacency_vector<V, E, A>::
      add_vertex(vertex_value_type const& x) -> vertex
      { 
        return base_.add_vertex(x);
      }

  template<typename V, typename E, typename A>
    auto undirected_adjacency_vector<V, E, A>::
      add_edge(vertex u, vertex v, edge_value_type const& x) -> edge
      { 
        return make_edge(base_.add_edge(u, v, x), u); 
      }

  template<typename V, typename E, typename A>
    auto undirected_adjacency_vector<V, E, A>::
      incident_edges(vertex v) -> incident_edge_range
      {
        return {begin_incident_edges(v), end_incident_edges(v)};
      }

  template<typename V, typename E, typename A>
    auto undirected_adjacency_vector<V, E, A>::
      incident_edges(vertex v) const -> const_incident_edge_range
      {
        return {begin_incident_edges(v), end_incident_edges(v)};
      }

} // namespace origin

#endif
