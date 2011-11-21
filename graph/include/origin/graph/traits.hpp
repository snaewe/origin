// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_TRAITS_HPP
#define ORIGIN_GRAPH_TRAITS_HPP

#include <origin/container.hpp>

namespace origin
{
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

  
  // Graph interface
  // There are 3 primary features of every graph type:
  //    - The vertex set
  //    - The edge set
  //    - User-defined vertex and edge properties

  
  
  // Vertex set
  // All graphs provide access to a vertex set, which is represented by a
  // range of vertices. A graph associates the following types and operations
  // with the vertex set:
  //    - Vertex_range<G> -- the range type representing the vertex set
  //    - Vertex<G> -- a vertex handle type
  //    - Vertex_value_type<G> -- User-defined vertex data
  //    - order(g) -- the size of the vertex set
  //    - null(g) -- true when order(g) == 0
  //    - g[v] -- User-defined data associated with v.
  
  // Return the vertex set of a graph
  template<typename G>
    inline auto vertices(G& g) -> decltype(g.vertices())
    {
      return g.vertices();
    }
  
  // Const version of the function above.
  template<typename G>
    inline auto vertices(G const& g) -> decltype(g.vertices())
    {
      return g.vertices();
    }

  // Safely get the result of the expression vertices(g).
  template<typename G>
    struct vertices_result
    {
    private:
      template<typename X>
        static auto check(X&& x) -> decltype(vertices(x));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<G>()));
    };
    
  // An alias for the vertex range associated with the graph G. Note that
  // different ranges may be associated with G and const G.
  template<typename G>
    using Vertex_range = typename vertices_result<G>::type;
    
  template<typename G>
    constexpr bool Has_vertices()
    {
      return Subst_succeeded<Vertex_range<G>>();
    }
    
  // An alias for the vertex handle associated with G. Note that different
  // handle types may be associated with G and const G.
  template<typename G>
    using Vertex = Value_type<Vertex_range<G>>;
    
    
  // Returns true if the graph has no vertices.
  template<typename G>
    inline auto null(G const& g) -> decltype(g.null())
    {
      return g.null();
    }
    
  // FIXME: Where can I put general definitions of null(x) and order(x). It's
  // probably that null(x) will be sufficiently graph-specific, but order(x)
  // has meaning for matrices (and probably other mathematical concepts) too.

    // Safely get the result of the expression null(g)
  template<typename G>
    struct null_result
    {
    private:
      template<typename X>
        static auto check(X const& x) -> decltype(null(x));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<G>()));
    };
    
  // An alias for the result of the expression null(g).
  template<typename G>
    using Null_result = typename null_result<G>::type;
    
  template<typename G>
    constexpr bool Has_null()
    {
      return Subst_succeeded<Null_result<G>>();
    }
    
    
    
  // Return the number of vertices in a graph.
  template<typename G>
    inline auto order(G const& g) -> decltype(g.order())
    {
      return g.order();
    }

  // Safely get the result of the expression order(g)
  template<typename G>
    struct order_result
    {
    private:
      template<typename X>
        static auto check(X const& x) -> decltype(order(x));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<G>()));
    };
    
  // An alias for the result of the expression order(g).
  template<typename G>
    using Order_result = typename order_result<G>::type;
    
  template<typename G>
    constexpr bool Has_order()
    {
      return Subst_succeeded<Order_result<G>>();
    }

    
    
  // Vertex data
  // Vertex data is the user-defined data type associated with each vertex. 
  // User-defined vertex data is accessed by the expression g[v] where v is a 
  // vertex in g.
  
  // Safely get the vertex data type
  template<typename G, typename V>
    struct vertex_data
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x[y]);
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<G>(), std::declval<V>()));
    };

  // An alias for the vertex data type of the graph. Note that the vertex data
  // is a value type: non-reference and unqualified. It does not depend on
  // the const-ness of G.
  template<typename G>
    using Vertex_data = Decay<typename vertex_data<G, Vertex<G>>::type>;
    
  // Returns true if G has an associated vertex data type.
  template<typename G>
    static constexpr bool Has_vertex_data()
    {
      return Subst_succeeded<Vertex_data<G>>();
    }
    
  // Return true if G has an empty vertex data.
  template<typename G>
    static constexpr bool Has_empty_vertex_data()
    {
      return Convertible<Vertex_data<G>, empty_t>();
    }

  // Mutable graph operations
  // Mutable graphs support the ability to add and remove vertices.

  // FIXME: Clean up the add_vertex operations.
  // Add a vertex to the graph
  template<typename G>
    inline auto add_vertex(G& g) -> decltype(g.add_vertex())
    {
      return g.add_vertex();
    }

  // Add a vertex to the graph the specified value.
  template<typename G>
    inline auto add_vertex(G& g, Vertex_data<G> value) 
      -> decltype(g.add_vertex(value))
    {
      return g.add_vertex(value);
    }
    
  // Remove the given vertex from the graph. Removing a vertex will also
  // remove its incdient edges.
  //
  // The result type is usually expected to be void.
  template<typename G>
    inline auto remove_vertex(G& g, Vertex<G> v)
      -> decltype(g.remove_vertex(v))
    {
      return g.remove_vertex(v);
    }
    
  // Remove all vertices (and also all edges), from the graph.
  template<typename G>
    inline auto remove_vertices(G& g) -> decltype(g.remove_vertices(g))
    {
      return g.remove_vertices();
    }
    
    

  // Edge set
  // All graphs provide access to an edge set, which is represented by a
  // range of edges. The edge set has the following associated types and
  // operations:
  //    - Edge_range<G> -- the range type describing the edge set
  //    - Edge<G> -- an edge handle type
  //    - Edge_value_type<G> -- a user-defined edge data type
  //    - size(g) -- the number of edges in g
  //    - empty(g) -- true if size(g) == 0
  //    - get_edge(g, u, v) -- return the edge connecting u and v
  //    - source(g, e) -- return the source vertex of e
  //    - target(g, e) -- return the target vertex of e
  //    - g[e] -- access user-defined data associated with e
    
  // Return the edge set of a graph.
  template<typename G>
    inline auto edges(G& g) -> decltype(g.edges())
    {
      return g.edges();
    }

  // A const version of the function above.
  template<typename G>
    inline auto edges(G const& g) -> decltype(g.edges())
    {
      return g.edges();
    }
    
  // Safely get the result of the expression edges(g).
  template<typename G>
    struct edges_result
    {
    private:
      template<typename X>
        static auto check(X&& x) -> decltype(edges(x));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<G>()));
    };
    
  // An alias for the vertex range associated with the graph G. Note that
  // different ranges may be associated with G and const G.
  template<typename G>
    using Edge_range = typename edges_result<G>::type;
    
  template<typename G>
    constexpr bool Has_edges()
    {
      return Subst_succeeded<Edge_range<G>>();
    }
    
  // An alias for the vertex handle associated with G. Note that different
  // handle types may be associated with G and const G.
  template<typename G>
    using Edge = Value_type<Edge_range<G>>;
    
    
    
  // NOTE: The empty() and size() operations are defined for the 
  // Container concept. In a sense, a Graph is a Container of edges.

    
  // Return an edge descriptor connecting the vertices u and v, if one exists.
  // If not, return a null edge descriptor.
  template<typename G>
    inline auto get_edge(G& g, Vertex<G> u, Vertex<G> v) 
      -> decltype(g.get_edge(u, v))
    {
      return g.get_edge(u, v);
    }

  // Const version of the previous function.
  template<typename G>
    inline auto get_edge(G const& g, Vertex<G const> u, Vertex<G const> v) 
      -> decltype(g.get_edge(u, v))
    {
      return g.get_edge(u, v);
    }
    
  // FIXME: Source and target could easily be defined in the context of other
  // concepts.
    
  // Return the source vertex of the given edge.
  template<typename G>
    inline auto source(G const& g, Edge<G const> e) -> decltype(g.source(e))
    {
      return g.source(e);
    }

  // Return the target vertex of the given edge.
  template<typename G>
    inline auto target(G const& g, Edge<G const> e) -> decltype(g.target(e))
    {
      return g.target(e);
    }
    
    
    
  // Edge data
  // Edge data is a user-defined data type associated with each edge. User-
  // defined edge data is accessed by the expression g[e] where e is an
  // edge in g.

  // Safely get the edge data type
  template<typename G, typename V>
    struct edge_data
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(x[y]);
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<G>(), std::declval<V>()));
    };

  // An alias for the edge data type of the graph. Note that edge data is
  // neither a reference, nor qualified. It does not depend on the const-ness
  // of G.
  template<typename G>
    using Edge_data = Decay<typename edge_data<G, Edge<G>>::type>;
    
  // Returns true if G has an associated edge data type. 
  template<typename G>
    static constexpr bool Has_edge_data()
    {
      return Subst_succeeded<Edge_data<G>>();
    }
    
  // Returns true if G has empty edge data.
  template<typename G>
    static constexpr bool Has_empty_edge_data()
    {
      return Convertible<Edge_data<G>, empty_t>();
    }
  
  
  // Mutable edge set operations
  // The following operations might be used to modify an edge set.
  //    - add_edge(g, u, v)
  //    - add_edge(g, u, v, x)
  //    - remove_edge(g, e)
  //    - remove_edge(g, u, v)
  //    - remove_edges(g)
  
  // Add an edge to the graph.
  //
  // FIXME: Should these be Vertex<G const>?
  template<typename G>
    inline auto add_edge(G& g, Vertex<G> u, Vertex<G> v)
      -> decltype(g.add_edge(u, v))
    {
      return g.add_edge(u, v);
    }

  // Add an edge to the graph with the specified property.
  template<typename G>
    inline auto add_edge(G& g, Vertex<G> u, Vertex<G> v, Edge_data<G> const& value)
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
    inline auto remove_edge(G& g, Vertex<G> u, Vertex<G> v)
      -> decltype(g.remove_edge(u, v))
    {
      return g.remove_edge(u, v);
    }
    
  // Remove all edges in g.
  template<typename G>
    inline auto remove_edges(G& g) -> decltype(g.remove_edges())
    {
      return g.remove_edges();
    }
    

    
  // Graph concept
  
  // A helper class for cheking syntactic requirements
  template<bool Prereqs, typename G>
    struct Graph_requirements
    {
      static constexpr bool check() { return false; }
    };
    
  template<typename G>
    struct Graph_requirements<true, G>
    {
      static constexpr bool check()
      {
        return Has_null<G>() &&    Boolean<Null_result<G>>()
            && Has_empty<G>() &&   Boolean<Empty_result<G>>()
            && Has_order<G>() &&   Unsigned<Order_result<G>>()
            && Has_size<G>() &&    Unsigned<Size_type<G>>();
      }
    };
    
  // The graph concept specification.
  template<typename G>
    struct Graph_concept
    {
      static constexpr bool check()
      {
        return Graph_requirements<Has_graph_category<G>(), G>::check();
      }
    };    
    
  template<typename G>
    constexpr bool Graph()
    {
      return Graph_concept<G>::check();
    }
    
    
  // Graph structure
  // The following operations query the graph's structure: incident edges and
  // adjacent vertices within the graph. The graph structure differentiates
  // directed and undirected graphs. 
  
  // For directed graphs, we have:
  //    - Out_edge_range<G>
  //    - out_edges(g, v)
  //    - out_degree(g, v)
  //    - In_edge_range<G>
  //    - in_edges(g, v)
  //    - in_degree(g, v)
  //    - successors(g, v)
  //    - predecessors(g, v)
  

  // Out edges

  // Return the out edges of the vertex v in g.
  template<typename G>
    inline auto out_edges(G& g, Vertex<G> v) -> decltype(g.out_edges(v))
    {
      return g.out_edges(v); 
    }
    
  // A const version of the function above.
  template<typename G>
    inline auto out_edges(G const& g, Vertex<G const> v) -> decltype(g.out_edges(v))
    {
      return g.out_edges(v); 
    }
    
  // Return the out degree of the vertex v in the directed graph g.
  template<typename G>
    inline auto out_degree(G const& g, Vertex<G const> v) -> decltype(g.out_degree(v))
    {
      return g.out_degree(v);
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
    
  // An alias for the result of the expression out_edges(g, v).
  template<typename G>
    using Out_edge_range = typename out_edges_result<G, Vertex<G>>::type;
    
  // Returns true if out_edges(g, v) is a valid expression.
  template<typename G>
    constexpr bool Has_out_edges()
    {
      return Subst_succeeded<Out_edge_range<G>>();
    }

    
    
  // In edges

  // Return the in edges of the vertex v in g.
  template<typename G>
    inline auto in_edges(G& g, Vertex<G> v) -> decltype(g.in_edges(v))
    {
      return g.in_edges(v);
    }
  
  // A const version of the function above.
  template<typename G>
    inline auto in_edges(G const& g, Vertex<G const> v) -> decltype(g.in_edges(v))
    {
      return g.in_edges(v);
    }

  // Return the in degree of the vertex v in the directed graph g.
  template<typename G>
    inline auto in_degree(G const& g, Vertex<G const> v) -> decltype(g.in_degree(v))
    {
      return g.in_degree(v);
    }
    
  // Safely get the result of the expression in_edges(g, v).
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
    
  // An alias for the result of the expression in_edges(g, v).
  template<typename G>
    using In_edge_range = typename in_edges_result<G, Vertex<G>>::type;
    
  // Returns true if in_edges(g, v) is a valid expression.
  template<typename G>
    constexpr bool Has_in_edges()
    {
      return Subst_succeeded<In_edge_range<G>>();
    }
  
  
  
  // Return the degree of the vertex v. This is the total number of edges
  // incident to the vertex. This is defined for both directed and undirected
  // graphs.
  template<typename G, typename V>
    inline auto degree(G const& g, V v) -> decltype(g.degree(v))
    {
      return g.degree(v);
    }
    
    
    
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
    
    

  // Directed Graphs
  
  // A helper class for checking syntactic requirements.
  //
  // FIXME: This is incomplete. I need to write checks for a lot of other 
  // required features.
  template<bool Prereqs, typename G>
    struct Directed_graph_requirements
    {
      static constexpr bool check() { return false; }
    };
    
  template<typename G>
    struct Directed_graph_requirements<true, G>
    {
      static constexpr bool check()
      {
        return Has_out_edges<G>() && Range<Out_edge_range<G>>()
            && Has_in_edges<G>() && Range<In_edge_range<G>>();
      }
    };
  
  // The directed graph concept specification.
  template<typename G>
    struct Directed_graph_concept
    {
      static constexpr bool check()
      {
        return Directed_graph_requirements<Graph<G>(), G>::check();
      }
    };
    
  // Returns true if G is a directed graph.
  template<typename G>
    constexpr bool Directed_graph()
    {
      return Directed_graph_concept<G>::check();
    };

    
  // For undirected graphs we have:
  //    - Incident_edge_range<G>
  //    - edges(g, v)
  //    - degree(g, v)
  //    - neighbors(g, v)

  // Incident edges
    
  // Return the incident edges of the vertex v in g.
  template<typename G>
    inline auto edges(G& g, Vertex<G> v) -> decltype(g.edges(v))
    {
      return g.edges(v);
    }
    
  // A const version of the function above.
  template<typename G>
    inline auto incident_edges(G const& g, Vertex<G const> v) -> decltype(g.edges(v))
    {
      return g.edges(v);
    }

  // Safely get the result of the expression undirected_edges(g, v). 
  template<typename G, typename V>
    struct undirected_edges_result
    {
    private:
      template<typename X, typename Y>
        static auto check(X&& x, Y&& y) -> decltype(edges(x, y));
      static subst_failure check(...);
    public:
      using type = decltype(check(std::declval<G>(), std::declval<V>()));
    };
    
  // An alias for the result of the expression undirected_edges(g, v).
  template<typename G>
    using Undirected_edge_range = typename undirected_edges_result<G, Vertex<G>>::type;
    
  // Returns true if undirected_edges(g, v) is a valid expression.
  template<typename G>
    constexpr bool Has_undirected_edges()
    {
      return Subst_succeeded<Undirected_edge_range<G>>();
    }

  // Return the neighbors of v in g, the set of vertices reachable from v's
  // incident edges.
  template<typename G>
    inline auto neighbors(G const& g, Vertex<G const> v)
      -> adjacency_range<Undirected_edge_range<G>>
    {
      return edges(g, v);
    }

    
  
  // Undirected Graphs
  
  // A helper class for checking syntactic requirements.
  //
  // FIXME: This is incomplete. I need to write checks for a lot of other 
  // required features.
  template<bool Prereqs, typename G>
    struct Undirected_graph_requirements
    {
      static constexpr bool check() { return false; }
    };
    
  template<typename G>
    struct Undirected_graph_requirements<true, G>
    {
      static constexpr bool check()
      {
        return Has_undirected_edges<G>() && Range<Undirected_edge_range<G>>();
      }
    };
  
  // The undirected graph concept specification.
  template<typename G>
    struct Undirected_graph_concept
    {
      static constexpr bool check()
      {
        return Undirected_graph_requirements<Graph<G>(), G>::check();
      }
    };
    
  // Returns true if G is a undirected graph.
  template<typename G>
    constexpr bool Undirected_graph()
    {
      return Undirected_graph_concept<G>::check();
    };

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
    inline auto incident_edgse(G& g, Vertex<G> v) -> decltype(out_edges(g, v))
    {
      return out_edges(g);
    }
    
  // A const version of the function above.
  template<typename G>
    inline auto incident_edgse(G const& g, Vertex<G const> v) -> decltype(out_edges(g, v))
    {
      return out_edges(g);
    }

  // Specialization for undirected graphs. The set of incident edges is just
  // the edges of v.
  template<typename G>
    inline auto incident_edgse(G& g, Vertex<G> v) -> decltype(edges(g, v))
    {
      return edges(g, v);
    }
    
  // A const version of the function above.
  template<typename G>
    inline auto incident_edgse(G const& g, Vertex<G const> v) -> decltype(edges(g, v))
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
    
} // namesapce origin

#endif
