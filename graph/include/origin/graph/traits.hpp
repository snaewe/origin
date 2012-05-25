// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_TRAITS_HPP
#define ORIGIN_GRAPH_TRAITS_HPP

#include <origin/container.hpp>

// FIXMEs
//  - make Vertex, Vertex

namespace origin
{

#if 0
  // FIXME: What are the different kinds of graph mutability:
  // Static Vertex Graph - The graph is initialized over a set of vertices
  // Static Graph - The graph is initialized over a set of vertices and edges
  // Buildable Vertex Graph - Vertices can be added to the graph
  // Buildable Edge Graph - Edges can be added to the graph.
  // Dynamic Vertex Graph - Vertices can be added and removed.
  // Dynamic Edge Graph - Edges can be added and removed.
  // Buildable Graph - Vertices and edges can be added to the graph.
  // Dynamic Graph - Vertices and edges can be added and removed.

  // the adjacency vector is buildable
  // the adjacency list is dynamic
  // the adjacency matrix is static vertex/dynamic edge
  // the static graph is static.

  // Directionality
  struct directed_graph_tag { };
  struct undirected_graph_tag { };
  
  // Mutability
  struct vertex_initialized_graph_tag { };
  struct initialized_graph_tag : vertex_initialized_graph_tag { };

  // FIXME: Is a buildable graph also initialized?
  struct vertex_buildable_graph_tag : vertex_initialized_graph_tag { };
  struct edge_buildable_graph_tag { };
  struct buildable_graph_tag : vertex_buildable_graph_tag, edge_buildable_graph_tag { };

  struct vertex_dynamic_graph_tag : vertex_buildable_graph_tag { };
  struct edge_dynamic_graph_tag : edge_buildable_graph_tag { };
  struct dynamic_graph_tag : vertex_dynamic_graph_tag, edge_dynamic_graph_tag { };
  

  // Safely get the nested type declation G::graph_category.
  template<typename G>
    struct graph_category
    {
    private:
      template<typename X>
        static typename X::graph_category check(X const&);
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<G>()));
    };
    
  // An alias for the graph category.
  template<typename G>
    using Graph_category = typename graph_category<G>::type;

  // Returns true if G has a graph category.
  template<typename G>
    constexpr bool Has_graph_category()
    {
      return Subst_succeeded<Graph_category<G>>();
    }
    


    
  // FIXME: Many of these can be syntactically evaluated without tag classes.
  // For example, vertex buildable graphs have add_vertex, dynamic vertex
  // graphs have remove_vertex. Implement this.
  
  // Return true if G can be initialized over a vertex set. Vertices may not
  // be added or removed.
  template<typename G>
    struct is_vertex_initialized_graph
      : std::is_base_of<vertex_initialized_graph_tag, typename G::graph_category>::type
    { };

  // Return true if G can be initialized over a vertex and edge set. Vertices
  // and edges may not be removed.
  template<typename G>
    struct is_initialized_graph
      : std::is_base_of<initialized_graph_tag, typename G::graph_category>::type
    { };

  // Return true if G supports incremental construction of the vertex set.
  // Vertices can be added but not removed.
  template<typename G>
    struct is_vertex_buildable_graph
      : std::is_base_of<vertex_buildable_graph_tag, typename G::graph_category>::type
    { };

  // Return true if G supports incremental construction of the edge set. Edges
  // can be added but not removed.
  template<typename G>
    struct is_edge_buildable_graph
      : std::is_base_of<edge_buildable_graph_tag, typename G::graph_category>::type
    { };
    
  // Return true if G is incremtnally constructible. Vertices and edges can 
  // added but not removed.
  template<typename G>
    struct is_buildable_graph
      : std::is_base_of<buildable_graph_tag, typename G::graph_category>::type
    { };

  // Return true if G supports incremental construction and destruction of the
  // vertex set. Vertices can be added and removed.
  template<typename G>
    struct is_vertex_dynamic_graph
      : std::is_base_of<vertex_dynamic_graph_tag, typename G::graph_category>::type
    { };

  // Return true if G supports incremental consruction and destruction of the
  // edge set. Edges can be added and removed.
  template<typename G>
    struct is_edge_dynamic_graph
      : std::is_base_of<edge_dynamic_graph_tag, typename G::graph_category>::type
    { };

  // Return true if G supports incremental construction and destruction. 
  // Vertices and edges can be added and removed.
  template<typename G>
    struct is_dynamic_graph 
      : std::is_base_of<dynamic_graph_tag, typename G::graph_category>::type
    { };
    
  /**
   * Return true if the graph G is directed.
   */
  template<typename G>
  struct is_directed_graph
    : std::is_base_of<directed_graph_tag, typename G::graph_category>::type
  { };

  /**
   * Return true if the graph G is undirected.
   */
  template<typename G>
  struct is_undirected_graph
    : std::is_base_of<undirected_graph_tag, typename G::graph_category>::type
  { };
  //@}
#endif

  // Vertex set
  // All graphs provide access to a vertex set, which is represented by a
  // range of vertices. All graphs provide the following operations related
  // to their vertex sets and vertices.
  //
  //    vertices(g) Returns a range over the vertices in g.
  //    order(g)    Returns the size of vertices(g)
  //    null(g)     Returns true when order(g) == 0
  //    g[v]        Returns user-defined data associated with v.
  //
  // The following types are associated with these operations:
  //
  //    Vertex<G>        The type of a vertex handle for G.
  //    Vertex_range<G>       The range type representing the vertex set
  //    Vertex_value_type<G>  The user-defined vertex data type
    


  // Vertices
  // Return a range over the verticess of g.
  template<typename G>
    inline auto vertices(G const& g) -> decltype(g.vertices())
    {
      return g.vertices();
    }
    
  // Graph order
  // Return the number of vertices in a graph.
  template<typename G>
    inline auto order(G const& g) -> decltype(g.order())
    {
      return g.order();
    }

  // Null graph
  // Returns true if the graph has no vertices.
  template<typename G>
    inline auto null(G const& g) -> decltype(g.null())
    {
      return g.null();
    }

    

  // Safely deduce the result of the expression vertices(g).
  template<typename G>
    struct vertices_result
    {
    private:
      template<typename X>
        static auto check(const X x) -> decltype(vertices(x));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<G>()));
    };

  // Safely deduce the result of the expression order(g)
  template<typename G>
    struct order_result
    {
    private:
      template<typename X>
        static auto check(const X& x) -> decltype(order(x));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<G>()));
    };
    
  // Safely deduce the result of the expression null(g).
  template<typename G>
    struct null_result
    {
    private:
      template<typename X>
        static auto check(const X& x) -> decltype(null(x));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<G>()));
    };
    
  // Safely deduce the result of the expression g[v].
  template<typename G, typename V>
    struct vertex_data
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y y) -> decltype(x[y]);
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<G>(), std::declval<V>()));
    };
    
    
    
  // Vertex range
  // An alias for the vertex range associated with the graph G. Note that
  // different ranges may be associated with G and const G.
  template<typename G>
    using Vertex_range = typename vertices_result<G>::type;
    
  // Vertex type
  // An alias for the vertex handle associated with G.
  template<typename G>
    using Vertex = Value_type<Vertex_range<G>>;
    
  // Vertex value type.
  // An alias for the vertex value type of the graph. 
  template<typename G>
    using Vertex_value_type = Decay<typename vertex_data<G, Vertex<G>>::type>;
    
  // An alias for the result of the expression order(g).
  template<typename G>
    using Order_result = typename order_result<G>::type;
    
  // An alias for the result of the expression null(g).
  template<typename G>
    using Null_result = typename null_result<G>::type;

    
    
  // Returns true if vertices(g) is a valid expression.
  template<typename G>
    constexpr bool Has_vertices()
    {
      return Subst_succeeded<Vertex_range<G>>();
    }

  // Returns true if order(g) is a valid expression.
  template<typename G>
    constexpr bool Has_order()
    {
      return Subst_succeeded<Order_result<G>>();
    }

    // Returns true if null(g) is a valid expression.
  template<typename G>
    constexpr bool Has_null()
    {
      return Subst_succeeded<Null_result<G>>();
    }
    
  // Returns true if G has an associated vertex data type.
  template<typename G>
    static constexpr bool Has_vertex_data()
    {
      return Subst_succeeded<Vertex_value_type<G>>();
    }
    
  // Return true if G has an empty vertex data. If a graph has an empty value
  // type, we can sometimes optimize space overhead by eliminating the need
  // to store or refer to individual vertices.
  // optimizing 
  template<typename G>
    static constexpr bool Has_empty_vertex_data()
    {
      return Has_vertex_data<G> && Empty<Vertex_value_type<G>>();
    }

    
    
  // Edge set
  // In addition to a vertex set, all graphs provide access to an edge set. The 
  // following operations are defined on that edge set:
  //
  //    edges(g)     Returns a range over the edges of g.
  //    size(g)      Returns the size of edges(g).
  //    empty(g)     Returns true if size(g) == 0.
  //    source(g, e) Returns the source vertex of e
  //    target(g, e) Returns the target vertex of e
  //    g[e]         Returns user-defined data associated with e
  //
  // The following types are associated with those operations.
  //
  //    - Edge_range<G> -- the range type describing the edge set
  //    - Edge<G> -- an edge handle type
  //    - Edge_value_type<G> -- a user-defined edge data type
  
  
  
  // Edges
  // Returns the edge set of a graph.
  template<typename G>
    inline auto edges(const G& g) -> decltype(g.edges())
    {
      return g.edges();
    }
    
  // NOTE: The size(g) and empty(g) operations are commonplace. They are 
  // defined in <origin/traits.hpp> along with the Size_type alias.
    
   // Safely deduce the result of the expression edges(g).
  template<typename G>
    struct get_edges_result
    {
    private:
      template<typename X>
        static auto check(const X& x) -> decltype(edges(x));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<G>()));
    };
    
  // Safely deduce the result of the expression g[e].
  template<typename G, typename V>
    struct get_edge_data
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y y) -> decltype(x[y]);
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<G>(), std::declval<V>()));
    }; 
  

    
  // Edge range
  // An alias for the vertex range associated with the graph G. Note that
  // different ranges may be associated with G and const G.
  template<typename G>
    using Edge_range = typename get_edges_result<G>::type;
    
  // Edge type
  // An alias for the vertex handle associated with G. Note that different
  // handle types may be associated with G and const G.
  template<typename G>
    using Edge = Value_type<Edge_range<G>>;
    
  // Edge value type
  // An alias for the edge data type of the graph.
  template<typename G>
    using Edge_value_type = Decay<typename get_edge_data<G, Edge<G>>::type>;
    
    

    // Returns true if edges(g) is a valid expression.
  template<typename G>
    constexpr bool Has_edges()
    {
      return Subst_succeeded<Edge_range<G>>();
    }
    
  // Returns true if g[e] is a valid expression.
  template<typename G>
    static constexpr bool Has_edge_data()
    {
      return Subst_succeeded<Edge_value_type<G>>();
    }
    
  // Returns true if G has empty edge data.
  template<typename G>
    static constexpr bool Has_empty_edge_data()
    {
      return Has_edge_data<G>() && Empty<Edge_value_type<G>>();
    }

    
    
  // Source vertex
  // Return the source vertex of the given edge.
  template<typename G>
    inline auto source(const G& g, Edge<G> e) -> decltype(g.source(e))
    {
      assert(e);
      return g.source(e);
    }

  // Target vertex
  // Return the target vertex of the given edge.
  template<typename G>
    inline auto target(const G& g, Edge<G> e) -> decltype(g.target(e))
    {
      assert(e);
      return g.target(e);
    }
    
    
    
  // Safely deduce the result of the expression source(g, e).
  template<typename G , typename E>
    struct get_source_result
    {
    private:
      template<typename X, typename Y>
        static auto check(const X& x, Y y) -> decltype(source(x, y));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<G>(), std::declval<E>()));
    };    

  // Safely deduce the result of the expression target(g, e).
  template<typename G , typename E>
    struct get_target_result
    {
    private:
      template<typename X, typename Y>
        static auto check(const X& x, Y y) -> decltype(target(x, y));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<G>(), std::declval<E>()));
    };    

    

  // An alias for the result of source(g, e).
  template<typename G>
    using Source_result = typename get_source_result<G, Edge<G>>::type;
    
  // An alias for the result of target(g, e).
  template<typename G>
    using Target_result = typename get_target_result<G, Edge<G>>::type;

    

  // Returns true if source(g, e) is a valid expression.
  template<typename G>
    constexpr bool Has_source()
    {
      return Subst_succeeded<Source_result<G>>();
    }

  // Returns true if target(g, e) is a valid expression.
  template<typename G>
    constexpr bool Has_target()
    {
      return Subst_succeeded<Target_result<G>>();
    }
    
    

  // Graph (concept)
  // A graph is mathematically defined as a pair of sets G = (V, E) where
  // V indicates a set of vertices, and E a set of pairs (u, v) with u and v
  // representing vertices in V.
  //
  // In origin a Graph provides access to the vertex set and edge sets and
  // some common operations. In particular, it requires the operations given
  // above for vertex and edge sets.
  //
  // FIXME: Should the order and size types be the same?
  template<typename G>
    constexpr bool Graph()
    {
        return Has_null<G>() &&     Boolean<Null_result<G>>()
            && Has_empty<G>() &&    Boolean<Empty_result<G>>()
            
            && Has_order<G>() &&    Unsigned<Order_result<G>>()
            && Has_size<G>() &&     Unsigned<Size_type<G>>()
            && Has_vertices<G>() && Input_range<Vertex_range<G>>()
            && Has_edges<G>() &&    Input_range<Edge_range<G>>();
    }

    
    
  // Graph structure
  // The following operations query the graph's structure: incident edges and
  // adjacent vertices within the graph. The graph structure differentiates
  // directed and undirected graphs. 
  
  // A directed graph provides access to its successor and predecessor vertices
  // and the incident edges that lead to them. The operations are:
  //
  //    out_edges(g, v)
  //    in_edges(g, v)
  //
  // The adjacent vertices of those incident edges are accessible through the
  // following operations:
  //
  //    successors(g, v)
  //    predecessors(g, v)
  //
  // The size of the successor and predecessor sets is measured by these
  // operations:
  //
  //    out_degree(g, v)  Return the size of the successor set
  //    in_degree(g, v)   Return the size of the predecessor set
  //    degree(g, v)      Return the number of edges incident to g.
  //
  // The following associated types can be used with directed graphs:
  //
  //    Out_edge_range<G>
  //    In_edge_range<G>


  // Out edges
  // Return the out edges incident to the vertex v in the directed graph g.
  template<typename G>
    inline auto out_edges(const G& g, Vertex<G> v) -> decltype(g.out_edges(v))
    {
      return g.out_edges(v); 
    }
    
  // In edges
  // Return the in edges incident to the vertex v in the directed graph g.
  template<typename G>
    inline auto in_edges(const G& g, Vertex<G> v) -> decltype(g.in_edges(v))
    {
      return g.in_edges(v);
    }
    
  // Out degree
  // Return the number of out edges that are incident to the vertex v in the 
  // directed graph g.
  template<typename G>
    inline auto out_degree(const G& g, Vertex<G> v) -> decltype(g.out_degree(v))
    {
      return g.out_degree(v);
    }    
    
  // In degree
  // Return the number of in edges that are incident to the vertex v in the 
  // directed graph g.
  template<typename G>
    inline auto in_degree(const G& g, Vertex<G> v) -> decltype(g.in_degree(v))
    {
      return g.in_degree(v);
    }
    
  // Return the number of edges in g that are incident to the vertex v.
  template<typename G, typename V>
    inline auto degree(G const& g, V v) -> decltype(g.degree(v))
    {
      return g.degree(v);
    }


    
  // Safely get the result of the expression out_edges(g, v).
  template<typename G, typename V>
    struct out_edges_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(out_edges(x, y));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<G>(), std::declval<V>()));
    };
    
  // Safely deduce the result of the expression in_edges(g, v).
  template<typename G, typename V>
    struct in_edges_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(in_edges(x, y));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<G>(), std::declval<V>()));
    };
    
  // Safely deduce the result of the expression out_degree(g, v).
  template<typename G, typename V>
    struct out_degree_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(out_degree(x, y));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<G>(), std::declval<V>()));
    };

  // Safely deduce the result of the expression in_degree(g, v).
  template<typename G, typename V>
    struct in_degree_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(in_degree(x, y));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<G>(), std::declval<V>()));
    };
    
  // Safely deduce the result of the expression degree(g, v).
  //
  // FIXME; This is going to overlap with other structrures that might have
  // a degree property. I can immediately think of Polynomial function types.
  template<typename G, typename V>
    struct degree_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(degree(x, y));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<G>(), std::declval<V>()));
    };
  
    
    
  // Out edge range
  // An alias for the result of the expression out_edges(g, v).
  template<typename G>
    using Out_edge_range = typename out_edges_result<G, Vertex<G>>::type;
    
  // In edge range
  // An alias for the result of the expression in_edges(g, v).
  template<typename G>
    using In_edge_range = typename in_edges_result<G, Vertex<G>>::type;
    
  // An alias for the result of the expression out_degree(g, v).
  template<typename G>
    using Out_degree_result = typename out_degree_result<G, Vertex<G>>::type;

  // An alias for the result of the expression in_degree(g, v).
  template<typename G>
    using In_degree_result = typename in_degree_result<G, Vertex<G>>::type;

  // An alias for the result of the expression degree(g, v).
  template<typename G>
    using Degree_result = typename degree_result<G, Vertex<G>>::type;


    
  // Returns true if out_edges(g, v) is a valid expression.
  template<typename G>
    constexpr bool Has_out_edges() { return Subst_succeeded<Out_edge_range<G>>(); }

  // Returns true if in_edges(g, v) is a valid expression.
  template<typename G>
    constexpr bool Has_in_edges() { return Subst_succeeded<In_edge_range<G>>(); }

  // Returns true if out_degree(g, v) is a valid expression.
  template<typename G>
    constexpr bool Has_out_degree() { return Subst_succeeded<Out_degree_result<G>>(); }
  
  // Returns true if in_degree(g, v) is a valid expression.
  template<typename G>
    constexpr bool Has_in_degree() { return Subst_succeeded<In_degree_result<G>>(); }

  template<typename G>
    constexpr bool Has_degree() { return Subst_succeeded<Degree_result<G>>(); }
    
    
  // Semidirected graph (concept)
  // A semidirected graph is one that only provides access to its out edges
  // and successor vertices. This is a common requirement for algorithms that
  // do not require access to the in edges or predecessors of the graph.
  //
  // NOTE: We don't need to check successors(g, v) because its implementation
  // is trivially derived from out_edges(g, v).
  template<typename G>
    constexpr bool Semidirected_graph()
    {
      return Graph<G>()
          && Has_out_edges<G>() &&  Input_range<Out_edge_range<G>>()
          && Has_out_degree<G>() && Unsigned<Out_degree_result<G>>();
    }
    
  // Directed Graph (concept)
  // A directed graph is a Semidirected graph that provides access to the in
  // edges and predecessor vertices of a vertex.
  //
  // NOTE: We don't need to check predecessors(g, v) because its implementation
  // is trivially derived from out_edges(g, v).
  //
  // FIXME: Should the types of in and out degrees be the same?
  template<typename G>
    constexpr bool Directed_graph()
    {
      return Semidirected_graph<G>()
          && Has_in_edges<G>() &&   Input_range<In_edge_range<G>>()
          && Has_in_degree<G>() &&  Unsigned<In_degree_result<G>>()
          && Has_degree<G>() &&     Unsigned<Degree_result<G>>();
    };
    

    
  // An undirected graph provides the following operations on its structure:
  //
  //    edges(g, v)     Returns the incident edges of v
  //    neighbors(g, v) Returns the neighboring (adjacent) vertices of v
  //    degree(g, v)    Returns size(edges(g, v)).
  //
  // The following alias are provided for use with these operations:
  //
  //    Incident_edge_range<G>

    
    
  // Incident edges
  // Return the incident edges of the vertex v in g.
  template<typename G>
    inline auto edges(const G& g, Vertex<G> v) -> decltype(g.edges(v))
    {
      return g.edges(v);
    }

  // NOTE: The degree(g, v) function is defiend above.

  

  // Safely get the result of the expression edges(g, v). 
  template<typename G, typename V>
    struct incident_edges_result
    {
    private:
      template<typename X, typename Y>
        static auto check(const X& x, Y y) -> decltype(edges(x, y));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<G>(), std::declval<V>()));
    };

    

  // An alias for the result of the expression undirected_edges(g, v).
  template<typename G>
    using Incident_edge_range = typename incident_edges_result<G, Vertex<G>>::type;
    
    
    
  // Returns true if undirected_edges(g, v) is a valid expression.
  template<typename G>
    constexpr bool Has_incident_edges()
    {
      return Subst_succeeded<Incident_edge_range<G>>();
    }


  
  // Undircted_graph (concept)
  // Returns true if G is a undirected graph.
  template<typename G>
    constexpr bool Undirected_graph()
    {
      return Graph<G>()
          && Has_incident_edges<G>() && Input_range<Incident_edge_range<G>>()
          && Has_degree<G>() &&         Unsigned<Degree_result<G>>();
    };
    
    
  // Mutable graphs
  // Mutable graphs support the ability to add and remove vertices and edges.
  // There are several operations:
  //
  //    add_vertex(g)             Add a vertex to g
  //    add_vertex(g, value)      Add a vertex with value x to g
  //
  //    remove_vertex(g, v)       Remove the vertex v from g
  //    remove_vertices(g)        Remove all vertices from g
  //
  //    add_edge(g, u, v)         Add an edge connecting u to v
  //    add_edge(g, u, v, value)  Add an edge with value x connecting u to v
  //
  //    remove_edge(g, e)         Remove the edge e from g
  //    remove_edges(g, u, v)     Remove all edges connecting u to v
  //    remove_edges(g, v)        Remove all edges incident to v
  //    remove_edges(g)           Remove all edges from v.
  //
  // FIXME: Build traits for these operations.

    
  // Add vertex
  // Add a vertex to the graph
  template<typename G>
    inline auto add_vertex(G& g) -> decltype(g.add_vertex())
    {
      return g.add_vertex();
    }

  // Add a vertex to the graph the specified value.
  template<typename G>
    inline auto add_vertex(G& g, const Vertex_value_type<G>& value) 
      -> decltype(g.add_vertex(value))
    {
      return g.add_vertex(value);
    }

  // Remove the given vertex from the graph. Removing a vertex will also
  // remove its incdient edges.
  //
  // The result type is expected to be void.
  template<typename G>
    inline auto remove_vertex(G& g, Vertex<G> v)
      -> decltype(g.remove_vertex(v))
    {
      return g.remove_vertex(v);
    }

  // Remove all vertices (and also all edges), from the graph. 
  //
  // The result is expected to be void.
  template<typename G>
    inline auto remove_vertices(G& g) -> decltype(g.remove_vertices(g))
    {
      return g.remove_vertices();
    }
    
  // Add an edge connecting the vertices u and v.
  template<typename G>
    inline auto add_edge(G& g, Vertex<G> u, Vertex<G> v)
      -> decltype(g.add_edge(u, v))
    {
      return g.add_edge(u, v);
    }

  // Add an edge connecting the vertices u and v, having the specified value.
  template<typename G>
    inline auto add_edge(G& g, 
                         Vertex<G> u, 
                         Vertex<G> v, 
                         const Edge_value_type<G>& value)
      -> decltype(g.add_edge(u, v, value))
    {
      return g.add_edge(u, v, value);
    }

  // Remove the edge e in g.
  template<typename G>
    inline auto remove_edge(G& g, Edge<G> e) -> decltype(g.remove_edge(e))
    {
      return g.remove_edge(e);
    }
    
  // Remove all edges connecting vertices u and v in the graph g.
  template<typename G>
    inline auto remove_edges(G& g, Vertex<G> u, Vertex<G> v)
      -> decltype(g.remove_edges(u, v))
    {
      return g.remove_edge(u, v);
    }
    
  // Remove all edges in g.
  template<typename G>
    inline auto remove_edges(G& g) -> decltype(g.remove_edges())
    {
      return g.remove_edges();
    }
    

    
 #if 0
   
  // FIXME: Define this class!
  template<typename R> struct adjacency_range;
  
  // Return the successors of v, the set of vertices reachable from the out
  // edges of v.
  template<typename G>
    inline adjacency_range<Out_edge_range<G>> successors(G& g, Vertex<G> v)
    {
      return out_edges(g, v);
    }
  
  // A const version of the function above.
  template<typename G>
    inline adjacency_range<Out_edge_range<G const>>
    successors(G const& g, Vertex<G const> v)
    {
      return out_edges(g, v);
    }
    
  // Neighbors
  // Return the neighbors of v in g, the set of vertices reachable from v's
  // incident edges.
  template<typename G>
    inline auto neighbors(const G& g, Vertex<G> v)
      -> adjacency_range<Incident_edge_range<G>>
    {
      return edges(g, v);
    }
    
    
    
  // Return the predecessors of v, the set of vertices reachable from the
  // in edges of v.
  template<typename G>
    inline adjacency_range<In_edge_range<G>> predecessors(G& g, Vertex<G> v)
    {
      return in_edges(g, v);
    }

  template<typename G>
    inline adjacency_range<In_edge_range<G const>> 
    predecessors(G const& g, Vertex<G const> v)
    {
      return in_edges(g, v);
    }
    
    

    
    
    
  // The two concepts are unified by overloads that select a "dominant 
  // traversal" strategy based on the modeled concept. For directed graphs, 
  // out edges are most commonly used for traversal. For undirected graphs
  // it is all incident edges. The corresponding operations are:
  //    - incident_edges(g, v)
  //    - adjacent_vertices(g, v)
  //    - degree(g, v)
  //
  // Technically, degree is a requirement of both directed and undirected
  // graphs, but I'm re-listing it here because it makes sense to do so. The
  // definition is above.
    
  // Incident edges
  // The incident_edges(g, v) method returns the set of edges incident to v
  // according to the dominant traversal method of the graph.
    
  // Specialization for directed graphs. The set of incident edges is the
  // same as the out edges of v.
  template<typename G>
    inline auto incident_edges(G& g, Vertex<G> v) -> decltype(out_edges(g, v))
    {
      return out_edges(g);
    }
    
  // A const version of the function above.
  template<typename G>
    inline auto incident_edges(G const& g, Vertex<G const> v) -> decltype(out_edges(g, v))
    {
      return out_edges(g);
    }

  // Specialization for undirected graphs. The set of incident edges is just
  // the edges of v.
  template<typename G>
    inline auto incident_edges(G& g, Vertex<G> v) -> decltype(edges(g, v))
    {
      return edges(g, v);
    }
    
  // A const version of the function above.
  template<typename G>
    inline auto incident_edges(G const& g, Vertex<G const> v) -> decltype(edges(g, v))
    {
      return edges(g, v);
    }
  
  
  // Adjacent vertices
  // The adjacent_vertices(g, v) operation returns the set of vertices opposite
  // to the incident edges of v. The resulting set of vertices depends on the
  // dominant traversal method of graph.
    
  // Specialization for directed graphs. The set of adjacent vertices is the
  // same as the successors.
  template<typename G>
    inline auto adjacent_vertices(G& g, Vertex<G> v) -> decltype(successors(g))
    {
      return successors(g, v);
    }
  
  // A const version of the function above.
  template<typename G>
    inline auto adjacent_vertices(G const& g, Vertex<G const> v) -> decltype(successors(g))
    {
      return successors(g, v);
    }

  // Specialization for undirected graphs. The set of adjacent vertices is the
  // same as the neighbors.
  template<typename G>
    inline auto adjacent_vertices(G& g, Vertex<G> v) -> decltype(neighbors(g))
    {
      return neighbors(g, v);
    }
    
  // A const version of the function above.
  template<typename G>
    inline auto adjacent_vertices(G const& g, Vertex<G const> v) -> decltype(neighbors(g))
    {
      return neighbors(g, v);
    }

    
  // Opposite vertex
  // Return the opposite end of the given edge.
  template<typename G>
    inline Vertex<G> opposite(G& g, Edge<G> e, Vertex<G> v)
    {
      return source(g, e) == v ? target(g, e) : v; 
    }

  template<typename G>
    inline Vertex<G const> opposite(G const& g, Edge<G const> e, Vertex<G const> v)
    {
      return source(g, e) == v ? target(g, e) : v; 
    }

    
  
  // Miscellaneous graph concepts
  
  // Returns true if F is an edge function over a Graph G. An edge function is 
  // a ternary function with the syntax f(g, u, v) where g has type G, and
  // u and v are vertices of g.
  template<typename F, typename G>
    constexpr bool Edge_function()
    {
      return Graph<G>() && Function<F, G, Vertex<G>, Vertex<G>>();
    }
    
  template<typename F, typename G>
    using Edge_function_result = Result_type<F, G, Vertex<G>, Vertex<G>>;
    
  // Returns true if G is an edge data generator. An edge data generator is an 
  // edge function whose result type is convertible to the graph's edge data
  // type.
  template<typename F, typename G>
    constexpr bool Edge_data_generator()
    {
      return Edge_function<F, G> 
          && Convertible<Edge_function_result<F, G>, Edge_data<G>>();
    }
#endif
} // namesapce origin

#endif
