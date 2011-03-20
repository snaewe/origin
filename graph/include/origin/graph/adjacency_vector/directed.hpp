// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ADJACENCY_VECTOR_DIRECTED_HPP
#define ORIGIN_GRAPH_ADJACENCY_VECTOR_DIRECTED_HPP

#include <algorithm>
#include <vector>

#include <origin/utility/empty.hpp>
#include <origin/range/iterator_range.hpp>
#include <origin/graph/vertex.hpp>
#include <origin/graph/edge.hpp>

namespace origin
{
  /** @internal */
  namespace adcacency_vector_
  {
    // FIXME: Consider splitting on dependent, non-dependent features and 
    // optimize using EBO.

    /**
     * The edge node for a directed adjacency list stores the source and vertex
     * indices of the graph.
     */
    template<typename E>
    struct edge_node
    {
      typedef E value_type;
      
      edge_node(vertex_t src, vertex_t tgt, value_type const& x = value_type{})
        : source(src), target(tgt), value{x}
      { }

      vertex_t source;
      vertex_t target;
      value_type value;
    };
    
    // FIXME: Split dependent and non-dependent features. Optimize using EBO.
    
    /**
     * The vertex node of an adjacency vector stores the out edge and in edge
     * lists.
     */
    template<typename V>
    struct vertex_node
    {
      typedef V value_type;
      typedef std::vector<edge_t> edge_list;
      
      vertex_node(value_type const& x = value_type{})
        : value{x}
      { }
      
      std::size_t degree() const
      { return out.size() + in.size(); }
      
      std::size_t out_degree() const
      { return out.size(); }
      
      std::size_t in_degree() const
      { return in.size(); }
      
      void add_out(edge_t e)
      { out.push_back(e); }
      
      void add_in(edge_t e)
      { in.push_back(e); }

      edge_list out;
      edge_list in;
      value_type value;
    };
  } // namespace adjacency_vector
  
  /** 
   * The directed adjacency vector implements an Adjacency List. The vertex
   * and edge sets are implemented using vectors.
   */
  template<typename Vertex = empty_t, 
           typename Edge = empty_t, 
           typename Alloc = std::allocator<Vertex>>
  class directed_adjacency_vector
  {
  public:
    // FIXME: Clean these guys up...
    typedef Alloc allocator_type;
    typedef std::size_t size_type;
    
    typedef Vertex vertex_value_type;
    typedef Edge edge_value_type;

    typedef adcacency_vector_::vertex_node<vertex_value_type> vertex_type;
    typedef adcacency_vector_::edge_node<edge_value_type> edge_type;

    typedef std::vector<vertex_type> vertex_list;
    typedef std::vector<edge_type> edge_list;
    
    typedef vertex_t vertex;
    typedef vertex_t const_vertex;
    typedef edge_t edge;
    typedef edge_t const_edge;
    
  public:
    typedef iterator_range<vertex_iterator> vertex_range;
    typedef iterator_range<vertex_iterator> const_vertex_range;
    
    typedef iterator_range<edge_iterator> edge_range;
    typedef iterator_range<edge_iterator> const_edge_range;
    
    typedef iterator_range<edge_iterator> out_edge_range;
    typedef iterator_range<edge_iterator> const_out_edge_range;
    typedef iterator_range<edge_iterator> in_edge_range;
    typedef iterator_range<edge_iterator> const_in_edge_range;
    
    /** @name Initialization and Assignment. */
    //@{
    directed_adjacency_vector()
      : vertices_(), edges_()
    { }
    //@}
    
    size_type order() const
    { return vertices_.size(); }
    
    bool null() const
    { return !order(); }
    
    size_type size() const
    { return edges_.size(); }
    
    bool empty() const
    { return !size(); }
    
    vertex_value_type& operator[](vertex v)
    { return get(v).value; }
    
    vertex_value_type const& operator[](vertex v) const
    { return get(v).value; }
    
    edge_value_type& operator[](edge e)
    { return get(e).value; }
    
    edge_value_type const& operator[](edge e) const
    { return get(e).value; }
    
    vertex add_vertex(vertex_value_type const& x = vertex_value_type{})
    {
      vertices_.push_back(vertex_type{x});
      return vertices_.size() - 1;
    }

    vertex get_vertex(std::size_t n)
    { return n; }
    
    const_vertex get_vertex(std::size_t n) const
    { return n; }
   
    size_type degree(const_vertex v) const
    { return get(v).degree(); }

    size_type out_degree(const_vertex v) const
    { return get(v).out_degree(); }

    size_type in_degree(const_vertex v) const
    { return get(v).in_degree(); }    
      
    /** @name Edge Operations */
    //@{
    edge add_edge(vertex u, vertex v, edge_value_type const& x = edge_value_type{})
    {
      edges_.push_back(edge_type{u, v, x});
      edge e = edges_.size() - 1;
      get(u).add_out(e);
      get(v).add_in(e);
      return e;
    }
    
    edge get_edge(size_type e)
    { return e; }
    
    const_edge get_edge(size_type e) const
    { return e; }

    edge get_edge(vertex u, vertex v);    
    const_edge get_edge(vertex u, vertex v) const;
        
    vertex source(edge e)
    { return get(e).source; }
    
    const_vertex source(const_edge e) const
    { return get(e).source; }
    
    vertex target(edge e)
    { return get(e).target; }
    
    const_vertex target(edge e) const
    { return get(e).target; }
    //@}

    /** @name Vertex and Edge Ranges */
    //@{
    vertex_range vertices()
    { 
      vertex_iterator f{0}, l{order()};
      return {f, l};
    }

    const_vertex_range vertices() const
    { 
      vertex_iterator f{0}, l{order()};
      return {f, l};
    }
    
    edge_range edges()
    { 
      edge_iterator f{0}, l{size()};
      return {f, l};
    }
    
    const_edge_range edges() const
    { 
      edge_iterator f{0}, l{size()};
      return {f, l};
    }
    
    out_edge_range out_edges(vertex v)
    { 
      edge_iterator f{0}, l{get(v).out_degree()};
      return {f, l};
    }
    
    const_out_edge_range out_edges(vertex v) const
    { 
      edge_iterator f{0}, l{get(v).out_degree()};
      return {f, l};
    }
    
    in_edge_range in_edges(vertex v)
    { 
      edge_iterator f{0}, l{get(v).in_degree()};
      return {f, l};
    }

    const_in_edge_range in_edges(vertex v) const
    { 
      edge_iterator f{0}, l{get(v).in_degree()};
      return {f, l};
    }
    //@}

  private:
    vertex_type& get(vertex v)
    { return vertices_[v.value]; }
    
    vertex_type const& get(const_vertex v) const
    { return vertices_[v.value]; }
    
    edge_type& get(edge e)
    { return edges_[e.value]; }
    
    edge_type const& get(const_edge e) const
    { return edges_[e.value]; }
  
  private:
    vertex_list vertices_;
    edge_list edges_;
  };

  template<typename V, typename E, typename A>
  inline auto directed_adjacency_vector<V, E, A>::get_edge(vertex u, vertex v)
    -> edge
  {
    auto& out = get(u).out;
    auto iter = find_if(out.begin(), out.end(), [v, this](edge e) { 
      return this->target(e) == v; 
    });
    return iter == out.end() ? edge{} : *iter;
  }

  template<typename V, typename E, typename A>
  inline auto 
  directed_adjacency_vector<V, E, A>::get_edge(const_vertex u, 
                                               const_vertex v) const
    -> const_edge
  {
    auto& out = get(u).out;
    auto iter = find_if(out.begin(), out.end(), [v, this](edge e) { 
      return this->target(e) == v; 
    });
    return iter == out.end() ? edge{} : *iter;
  }

} // namespace origin

#endif
