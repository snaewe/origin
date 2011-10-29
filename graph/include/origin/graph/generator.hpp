// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_GENERATOR_HPP
#define ORIGIN_GRAPH_GENERATOR_HPP

#include <utility>
#include <algorithm>
#include <unordered_map>

#include <origin/graph/traits.hpp>

namespace origin
{
  // Add a vertex to the graph labeled with the given value, using map to
  // ensure that each vertex is uniquely labeled. If the given value is already
  // in the graph, the associated vertex is added.
  //
  // requires UniqueMap<VertexType<G>, T> && Convertible<T, VertexValueType<G>>
  //
  // TODO: Move to traits?
  template<typename G, typename Map, typename T>
    typename graph_traits<G>::vertex 
    add_labeled_vertex(G& g, Map& map, T const& value)
    {
      typedef typename graph_traits<G>::vertex Vertex;
      auto x = map.insert({value, Vertex{}});
      if(x.second)
        x.first->second = g.add_vertex(value);
      return x.first->second;
    }
  
  // Return the edge label from the edge triple t.
  template<typename G, typename Tuple>
    inline auto 
    edge_label_from_tuple(G const& g, Tuple const& t) -> decltype(std::get<2>(t))
    {
      return std::get<2>(t);
    }
  
  // Return the default edge label when given an edge pair.
  template<typename G, typename V>
    inline typename graph_traits<G>::edge_value_type
    edge_from_tuple(G const& g, std::pair<V, V> const& p)
    {
      return typename graph_traits<G>::edge_value_type{};
    }

  // Build a graph from the given sequence of edge tuples, using the given map 
  // to uniquely associate each vertex with its given label.
  template<typename G, typename Map, typename Iter>
    void build_edge_graph(G& g, Map& map, Iter first, Iter last)
    {
      typedef typename graph_traits<G>::vertex Vertex;
      while(first != last) {
        auto const& t = *first;
        Vertex u = add_labeled_vertex(g, map, std::get<0>(t));
        Vertex v = add_labeled_vertex(g, map, std::get<1>(t));
        add_edge(g, u, v, edge_from_tuple(g, t));
        ++first;
      }
    }

  // Given a sequence of edge tuples build a graph. Note that the tuple can be 
  // an edge pair (u, v) or a triple (u, v, e). If given a sequence of pairs,
  // edges are assigned default labels.
  template<typename G, typename Iter>
    void build_edge_graph(G& g, Iter first, Iter last)
    {
      typedef typename graph_traits<G>::vertex Vertex;
      typedef typename graph_traits<G>::vertex_value_type Value;
      std::unordered_map<Value, Vertex> map;
      build_edge_graph(g, map, first, last);
    }
  

  // TODO: There's an EdgeFunction concept, which is a function with the
  // syntax f(g, u, v).
  
  
  // An edge function that adds an edge between two vertices.
  //
  // requires EdgeBuildableGraph<G>
  template<typename G>
    struct make_edge
    {
      typedef typename graph_traits<G>::vertex Vertex;
      typedef typename graph_traits<G>::edge Edge;
      
      void operator()(G& g, Vertex u, Vertex v) const
      {
        add_edge(g, u, v);
      }
    };

  // An edge function that adds an edge between two vertices, assigning it
  // the a value specified by an increasing iterator.
  //
  // requires EdgeBuildableGraph<G>
  //       && WeakInputIterator<Iter>
  //       && Same<VertexValueType<G>, ValueType<Iter>>
  template<typename G, typename Iter>
    struct copy_edge
    {
      typedef typename graph_traits<G>::vertex Vertex;
      typedef typename graph_traits<G>::edge Edge;
      
      copy_edge(Iter i)
        : iter{i}
      { }
      
      void operator()(G& g, Vertex u, Vertex v)
      {
        add_edge(g, u, v, *iter++);
      }
      
      Iter iter;
    };
  
  // An edge function that adds an edge between two vertices, assigning
  // it the specified property.
  //
  // requires EdgeBuildableGraph<G> && Convertible<T, VertexValueType<G>>
  // FIXME: Remove T?
  template<typename G, typename T>
    struct fill_edge
    {
      typedef typename graph_traits<G>::vertex Vertex;
      typedef typename graph_traits<G>::edge Edge;
      
      fill_edge(T const& x)
        : value(x)
      { }
      
      void operator()(G& g, Vertex u, Vertex v) const
      {
        add_edge(g, u, v, value);
      }
      
      T const& value;
    };

  // An edge function that adds an edge between two vertices, assigning it the
  // result of the configured edge function.
  //
  // requires EdgeBuildableGraph<G>
  //       && EdgeFunction<Gen, G>
  //       && Convertible<EdgeFunctionResultType<Gen, G>, VertexValueType<G>>
  template<typename G, typename Gen>
    struct generate_edge
    {
      typedef typename graph_traits<G>::vertex Vertex;
      typedef typename graph_traits<G>::edge Edge;
      
      generate_edge(Gen g)
        : gen{g}
      { }
      
      void operator()(G& g, Vertex u, Vertex v) const
      {
        add_edge(g, u, v, gen(g, u, v));
      }
      
      Gen gen;
    };

  // An edge function that adds an edge between two vertices, assigning it
  // a value that increases with each subsequent edge addition.
  // requires EdgeBuildableGraph<G> && Same<VertexType<G>, Num>
  template<typename G, typename Num>
    struct iota_edge
    {
      typedef typename graph_traits<G>::vertex Vertex;
      typedef typename graph_traits<G>::edge Edge;
      
      iota_edge(Num& n)
        : num(n)
      { }
      
      void operator()(G& g, Vertex u, Vertex v)
      {
        add_edge(g, u, v, num++);
      }
      
      Num& num;
    };


  // Graph generators
    
  // requires Graph<G>
  template<typename G>
    inline G make_null_graph()
    {
      return std::move(G{});
    }

  // Make a trivial graph with 1 vertex and no edges.
  //
  // requires VertexInitializedGraph<G>
  template<typename G>
    inline G make_trivial_graph()
    {
      G g(1);
      return std::move(g);
    }
    
  // Make a trivial graph with 1 vertex and no edges. Initalize the label of 
  // the vertex to the given value.
  //
  // requires VertexInitializedGraph<G>
  //
  // FIXME: Replace V with VertexType<G>
  template<typename G, typename V>
    inline G make_trivial_graph(V const& value)
    {
      // Kind of tricky, but &x and &x+1 define a range of 1 element.
      G g(&value, &value + 1);
      return std::move(g);
    }

  // Path graphs have n vertices and n - 1 edges.
  // P_1 - singleton graph, no edges
  // P_2 - two vertices, 1 edge (u -- v)
  // P_n - n vertices n - 1 edges (v1 -- v2 -- ... -- vn)
  
  
  // Traverse the given range of vertices as a path, calling f for every
  // consecutive pair of vertices.
  //
  // requires Graph<G>
  //       && InputIterator<Iter>
  //       && Same<VertexType<G>, ValueType<Iter>>
  //       && EdgeFunction<F, G>
  //
  // TODO: Is there a better name for this algorith? for_each_in_path?
  template<typename G, typename Iter, typename F>
    F for_path(G& g, Iter first, Iter last, F f) 
    {
      typedef typename graph_traits<G>::vertex Vertex;
      assert(( first != last ));

      Vertex u = *first++;
      while(first != last) {
        Vertex v = *first++;
        f(g, u, v);
        u = v;
      }
      return f;
    }
  
  // Induce the graph Pn on the graph g over the specified vertices in the 
  // range [first, last) such that the first refers to the the head of the 
  // path, and prev(last) refers to the tail, and the value associated with
  // each edge is specified by the given value.. Edges (u, v) are added between
  // subsequent iterators. That is, for iterators i and j in [first, last) 
  // where j == next(i), the action taken is g.add_edge(*i, *j, value);
  //
  // requires EdgeBuildable<G> && InputIterator<Iter>
  template<typename G, typename Iter>
    void make_path(G& g, Iter first, Iter last)
    {
      for_path(g, first, last, make_edge<G>{});
    }

  template<typename G, typename Iter1, typename Iter2>
    void make_path(G& g, Iter1 first1, Iter1 last1, Iter2 first2)
    {
      for_path(g, first1, last1, copy_edge<G, Iter2>{first2});
    }

  // Create a path as per make_path, assigning each edge an the given value.
  template<typename G, typename Iter, typename T>
    void fill_path(G& g, Iter first, Iter last, T const& value)
    {
      for_path(g, first, last, fill_edge<G, T>{value});
    }

  // Create a path as per make_path, assigning each edge the result of the
  // given edge value generator.
  template<typename G, typename Iter, typename Gen>
    void generate_path(G& g, Iter first, Iter last, Gen gen)
    {
      for_path(g, first, last, fill_edge<G, Gen>{gen});
    }

  // Create a path as per make_path, assigning each edge an increasing value.
  template<typename G, typename Iter, typename Num>
    void iota_path(G& g, Iter first, Iter last, Num num)
    {
      for_path(g, first, last, iota_edge<G, Num>{num});
    }

  // These might also be nice? Maybe...
  // make_path_n - use a weak range
  // make_path_n_fill - assign edge labels to a constant, using weak range
  // make_path_n_generate - assign edge labels to f(u, v), using weak range
  // make_path_n_iota - assign edge labels to increasing value, using weak range

  // Create a path graph with n vertices and n - 1 edges. Vertex and edge labels
  // are default initialized.
  //
  // requires VertexInitializedGraph<G> && EdgeBuildableGraph<G>
  template<typename G>
    inline G make_path_graph(typename G::size_type n)
    {
      assert(( n != 0 ));

      G g(n);
      make_path(g, begin(vertices(g)), end(vertices(g)));
      return std::move(g);
    }

  template<typename G, typename Iter>
    inline G make_path_graph(typename G::size_type n, Iter first)
    {
      // assert(( readable_weak_range(first, n - 1) ));
      assert(( n != 0 ));

      G g(n);
      make_path(g, std::begin(vertices(g)), std::end(vertices(g)), first);
      return std::move(g);
    }


  // Create a path graph using the values in the range [first, last) as the
  // labels of vertices to be added. The resulting graph will have last - first
  // vertices.
  template<typename G, typename Iter>
    inline G make_path_graph(Iter first, Iter last)
    {
      // assert(( readable_bounded_range(first1, last1) ));
      assert(( first != last ));

      G g(first, last);
      make_path(g, std::begin(vertices(g)), std::end(vertices(g)));
      return std::move(g);
    }

  template<typename G, typename Iter1, typename Iter2>
    inline G make_path_graph(Iter1 first1, Iter1 last1, Iter2 first2)
    {
      // assert(( readable_bounded_range(first1, last1) ));
      // assert(( readable_weak_range(first, distance(first1, last1)) ));
      assert(( first1 != last1 ));

      G g(first1, last1);
      make_path(g, std::begin(vertices(g)), std::end(vertices(g)), first2);
      return std::move(g);
    }

  template<typename G, typename V>
    inline G make_path_graph(std::initializer_list<V> list)
    {
      assert(( list.size() != 0 ));
      return std::move(make_path_graph<G>(list.begin(), list.end()));
    }


  template<typename G, typename V, typename E>
    inline G make_path_graph(std::initializer_list<V> list1, 
                             std::initializer_list<E> list2)
    {
      assert(( list1.size() > 0 ));
      assert(( list2.size() == list1.size() - 1 ));
      return std::move(make_path_graph<G>(list1.begin(), list1.end(), list2.begin()));
    }

  // TODO: Add overloads for fill/generate path graphs
  
  template<typename G, typename Num>
    inline G iota_path_graph(typename G::size_type n, Num num)
    {
      assert(( n != 0 ));
      
      G g(n);
      iota_path(g, std::begin(vertices(g)), std::end(vertices(g)), num);
      return std::move(g);
    }
  
  template<typename G, typename Iter, typename Num>
    inline G iota_path_graph(Iter first, Iter last, Num num)
    {
      assert(( first != last ));
      
      G g(first, last);
      iota_path(g, std::begin(vertices(g)), std::end(vertices(g)), num);
      return std::move(g);
    }
  
  template<typename G, typename V, typename Num>
    inline G iota_path_graph(std::initializer_list<V> list, Num num)
    {
      assert(( list.size() != 0 ));
      return std::move(iota_path_graph<G>(list.begin(), list.end(), num));
    }



  // Cycle graph are path graphs where the tail is connected to the head.
  // C1: v--v
  // C2: u--v--u
  // Cn: v1--v2--...--vn--v1
  // Cycle have n vertices and edges.
  
  // Traverse the given range of vertices as a cycle, calling f for every
  // consecutive pair of vertices and lastly for the vertices 
  // (*(last - 1), *first).
  // 
  // requires Graph<Graph> && InputIterator<Iter> && EdgeFunction<F, G>
  template<typename G, typename Iter, typename F>
    F for_cycle(G& g, Iter first, Iter last, F f)
    {
      typedef typename graph_traits<G>::vertex Vertex;
      assert(( first != last ));

      Vertex u = *first++;
      Vertex h = u;
      while(first != last) {
        Vertex v = *first++;
        f(g, u, v);
        u = v;
      }
      f(g, u, h);
      return f;
    }
  
  // Create cycle on the vertices in the range [first, last), that is a path
  // on [first, last) and *(last - 1) is also connected to *first.
  // is connected to *first.
  // requires: EdgeBuildable<Graph> && InputIterator<Iter>
  template<typename G, typename Iter>
    void make_cycle(G& g, Iter first, Iter last)
    {
      for_cycle(g, first, last, make_edge<G>{});
    }

  // Create a cycle
  template<typename G, typename Iter, typename T>
    void fill_cycle(G& g, Iter first, Iter last, T const& value)
    {
      for_cycle(g, first, last, fill_edge<G, T>{value});
    }

  template<typename G, typename Iter, typename Gen>
    void generate_cycle(G& g, Iter first, Iter last, Gen gen)
    {
      for_cycle(g, first, last, generate_edge<G, Gen>{gen});
    }

  template<typename G, typename Iter, typename Num>
    void iota_cycle(G& g, Iter first, Iter last, Num num)
    {
      for_cycle(g, first, last, iota_edge<G, Num>{num});
    }

  // Make a cycle graph with n vertices and edges. Vertex and edge labels are 
  // default initialized.
  template<typename G>
    G make_cycle_graph(typename graph_traits<G>::size_type n)
    {
      assert((n != 0 ));

      G g(n);
      make_cycle(g, begin(vertices(g)), end(vertices(g)));
      return std::move(g);
    }

  // Make a cycle graph with n vertices and edges. Vertex labels are default
  // constructed and edge labels are assigned consecutive values in the weak 
  // range [first, n). 
  template<typename G, typename Iter>
    inline G make_cycle_graph(typename G::size_type n, Iter first)
    {
      // assert(( pre: readable_weak_range(first, n) ));
      assert(( n != 0 ));

      G g(n);
      make_cycle(g, begin(vertices(g)), end(vertices(g)), first);
      return std::move(g);
    }

  template<typename G, typename Iter>
    G make_cycle_graph(Iter first, Iter last)
    {
      G g(first, last);
      make_cycle(g, begin(vertices(g)), end(vertices(g)));
      return std::move(g);
    }

  // Make a cycle graph with last1 - first1 vertices and edges. Vertex labels 
  // are assigned consecutive values in the range [first1, last1), and edge 
  // labels are assigned consecutive values in the weak range 
  // [|first2, last1 - first1|).
  template<typename G, typename Iter1, typename Iter2>
    inline G make_cycle_graph(Iter1 first1, Iter1 last1, Iter2 first2)
    {
      // assert(( readable_bounded_range(first1, last1) ));
      // assert(( readable_weak_range(first, distance(first1, last1)) ));
      assert(( first1 != last1 ));

      G g(first1, last1);
      make_cycle(g, begin(vertices(g)), end(vertices(g)), first2);
      return std::move(g);
    }


  template<typename G, typename V>
    G make_cycle_graph(std::initializer_list<V> list)
    {
      assert(( list.size() != 0 ));
      return std::move(make_cycle_graph<G>(list.begin(), list.end()));
    }

  // Make a cycle graph with list.size() vertices and edges. Vertex and edge
  // labels are assigned values the initializer lists list1 and list2, 
  // respectively.
  template<typename G, typename V, typename E>
    inline G make_cycle_graph(std::initializer_list<V> list1, 
                             std::initializer_list<E> list2)
    {
      assert(( list1.size() == list2.size() ));
      return std::move(make_cycle_graph<G>(list1.begin(), list1.end(), list2.begin()));
    }

  // TODO: Add overloads for fill/generate path graph.

  template<typename G, typename Num>
    inline G iota_cycle_graph(typename G::size_type n, Num num)
    {
      assert(( n != 0 ));
      
      G g(n);
      iota_cycle(g, std::begin(vertices(g)), std::end(vertices(g)), num);
      return std::move(g);
    }
  
  template<typename G, typename Iter, typename Num>
    inline G iota_cycle_graph(Iter first, Iter last, Num num)
    {
      assert(( first != last ));
      
      G g(first, last);
      iota_cycle(g, std::begin(vertices(g)), std::end(vertices(g)), num);
      return std::move(g);
    }
  
  template<typename G, typename V, typename Num>
    inline G iota_cycle_graph(std::initializer_list<V> list, Num num)
    {
      assert(( list.size() != 0 ));
      return std::move(iota_cycle_graph<G>(list.begin(), list.end(), num));
    }

  // Complete graph: A fully connected graph. Every vertex is connected to
  // every other vertex. A complete graph Kn has n vertices and 
  // binomial_coefficient(n, 2).
  // K1 - the trivial graph
  // K2 - the same as P2
  // K3 - a triangle
  // Kn - ...
  //
  // A complete directed graph has arcs between every pair of vertices, 
  // including the arc (v, v). From the perspective of an adjacency matrix, it
  // generates the "matrix of 1s", which is often called Jn. This definition
  // appears in Knuth's The Art of Computer Programming vol. 4.
  //
  // A complete directed graph has n^2 edges.
  //
  // Another common interpretation of complete digraphs is that loops are not
  // included. Such graphs would have n^2 - n edges. This definition is the same
  // as that given on the Wolfram site.
  //
  // FIXME: Rethink the design of complete digraphs. If we choose Knuth, we're
  // consistent with the adjacency matrix representations. If we choose the
  // Wolfram definition, then we're consistent with the more common notion of
  // simple graphs.

  // Traverse the vertices in the range  [first, last) in such a way that a
  // complete graph is constructed. The algorithm calls f for every combination
  // of vertices in the range. If n == last - first, then f is called exactly
  // n(n - 1)/2 times.
  //
  // requires Graph<G> && ForwardIterator<Iter> && EdgeFunction<F, G>.
  template<typename G, typename Iter, typename F>
    auto for_clique(G& g, Iter first, Iter last, F f)
      -> typename std::enable_if<is_undirected_graph<G>::value, F>::type
    {
      for( ; first != last; ++first) {
        for(Iter i = std::next(first); i != last; ++i) {
          f(g, *first, *i);
        }
      }
      return f;
    }

  // A specialization for directed graphs generates all pairs of vertices.
  // FIXME: See comments above about possible interpretations of this
  // graph generator.
  template<typename G, typename Iter, typename F>
    auto for_clique(G& g, Iter first, Iter last, F f)
      -> typename std::enable_if<is_directed_graph<G>::value, F>::type
    {
      for(Iter i = first ; i != last; ++i) {
        for(Iter j = first; j != last; ++j)
          f(g, *i, *j);
      }
      return f;
    }

  template<typename G, typename Iter>
    void make_clique(G& g, Iter first, Iter last)
    {
      for_clique(g, first, last, make_edge<G>{});
    }
    
  template<typename G, typename Iter1, typename Iter2>
    void make_clique(G& g, Iter1 first1, Iter1 last1, Iter2 first2)
    {
      for_clique(g, first1, last1, copy_edge<G, Iter2>{first2});
    }
    
  template<typename G, typename Iter, typename T>
    void fill_clique(G& g, Iter first, Iter last, T const& value)
    {
      for_clique(g, first, last, fill_edge<G, T>{value});
    }

  template<typename G, typename Iter, typename Gen>
    void generate_clique(G& g, Iter first, Iter last, Gen gen)
    {
      for_clique(g, first, last, generate_edge<G, Gen>{gen});
    }
    
  template<typename G, typename Iter, typename Num>
    void iota_clique(G& g, Iter first, Iter last, Num num)
    {
      for_clique(g, first, last, iota_edge<G, Num>{num});
    }

  // Make a complete graph with n vertices and binomial_coefficient(n, 2) edges.
  // Vertex and edge labels are default initialized.
  //
  // requires: Graph<G>
  // precondition: n >= 0
  template<typename G>
    G make_complete_graph(typename graph_traits<G>::size_type n)
    {
      assert(( n >= 0 ));
      G g(n);
      make_clique(g, begin(vertices(g)), end(verices(g)));
      return std::move(g);
    }

  // Make a complete graph with n vertices and binomial_coefficient(n, 2) edges.
  // Vertex labels are default initialized and edge labels are assigned
  // consecutive values from the weak range [|first, n|).
  template<typename G, typename Iter>
    G make_complete_graph(typename graph_traits<G>::size_type n, Iter first)
    {
      // assert((  ));
      assert(( n >= 0 ));
      G g(n);
      make_clique(g, std::begin(vertices(g)), std::end(verices(g)), first);
      return std::move(g);
    }

  // precondition: first != last
  // precondition: readable_range(first, n)
  template<typename G, typename Iter>
    G make_complete_graph(Iter first, Iter last)
    {
      assert(( first != last ));

      G g(first, last);
      make_clique(g, std::begin(vertices(g)), std::end(verices(g)));
      return std::move(g);
    }

  // precondition: first != last
  // precondition: readable_range(first1, last1)
  // precondition: readable_range(firast2, binomial_coefficient(last1 - first1, 2))
  template<typename G, typename Iter1, typename Iter2>
    G make_complete_graph(Iter1 first1, Iter1 last1, Iter2 first2)
    {
      assert(( first1 != last1 ));

      G g(first1, last1);
      make_clique(g, std::begin(vertices(g)), std::end(verices(g)), first2);
      return std::move(g);
    }

  template<typename G, typename V>
    G make_complete_graph(std::initializer_list<V> list)
    {
      assert(( list.size() != 0 ));
      return std::move(make_complete_graph<G>(list.begin(), list.end()));
    }

  template<typename G, typename V, typename E>
    G make_complete_graph(std::initializer_list<V> list1,
                          std::initializer_list<E> list2)
    {
      assert(( list1.size() != 0 ));
      // assert(( list2.size() == binomial_coefficient(list1.size(), 2) ));

      return std::move(make_complete_graph<G>(list1.begin(), list1.end(), list2.begin()));
    }

  // TODO: Add overloads for fill/generate path graph.

  template<typename G, typename Num>
    inline G iota_complete_graph(typename G::size_type n, Num num)
    {
      assert(( n != 0 ));
      
      G g(n);
      iota_clique(g, std::begin(vertices(g)), std::end(vertices(g)), num);
      return std::move(g);
    }
  
  template<typename G, typename Iter, typename Num>
    inline G iota_complete_graph(Iter first, Iter last, Num num)
    {
      assert(( first != last ));
      
      G g(first, last);
      iota_clique(g, std::begin(vertices(g)), std::end(vertices(g)), num);
      return std::move(g);
    }
  
  template<typename G, typename V, typename Num>
    inline G iota_complete_graph(std::initializer_list<V> list, Num num)
    {
      assert(( list.size() != 0 ));
      return std::move(iota_complete_graph<G>(list.begin(), list.end(), num));
    }


  // A star graph, Sn is a set of n vertices with the first being a "hub"
  // vertex with the remainder being connected to the hub by n - 1 edges.
  // S1 - The trivial graph
  // S2 - Isomorphic to P2
  // S3 - Isomorphic to P3
  // S4 - Isomorphic to the claw graph
  
  // Traverse the sequence of vertices in the range [first, last) such that
  // for each iterator i != first, f is called for f(*first, *i).
  //
  // requires Graph<G> && InputIterator<Iter> && EdgeFunction<F>
  template<typename G, typename Iter, typename F>
    F for_star(G& g, Iter first, Iter last, F f)
    {
      typedef typename graph_traits<G>::vertex Vertex;
      // assert(( readable_bounded_range(first, last) ));
      assert(( first != last ));
      
      Vertex u = *first++;
      while(first != last) {
        Vertex v = *first++;
        f(g, u, v);
      }
      return f;
    }

  // Make a star graph on the vertices in the range [first, last) where
  // first indicates the hub and [first + 1, last) are the vertices on the
  // exterior. Edges are constructed, for each distance i in last - (first + 1)
  // as [first, first + i).
  //
  // requires InputIterator<Iter>
  template<typename G, typename Iter>
    void make_star(G& g, Iter first, Iter last) 
    {
      for_star(g, first, last, make_edge<G>{});
    }
    
  template<typename G, typename Iter1, typename Iter2>
    void make_star(G& g, Iter1 first1, Iter1 last1, Iter2 first2)
    {
      for_star(g, first1, last1, copy_edge<G, Iter2>{first2});
    }

  template<typename G, typename Iter, typename T>
    void fill_star(G& g, Iter first, Iter last, T const& value)
    {
      for_star(g, first, last, fill_edge<G, T>{value});
    }
    
  template<typename G, typename Iter, typename Gen>
    void generate_star(G& g, Iter first, Iter last, Gen gen)
    {
      for_star(g, first, last, generate_edge<G, Gen>{gen});
    }
    
  template<typename G, typename Iter, typename Num>
    void iota_star(G& g, Iter first, Iter last, Num num)
    {
      for_star(g, first, last, iota_edge<G, Num>{num});
    }

  // Make a star graph with n vertices and n - 1 edges. Vertex and edge labels
  // are default initialized.
  template<typename G>
    G make_star_graph(typename graph_traits<G>::size_type n)
    {
      assert(( n != 0 ));

      G g(n);
      make_star(g, std::begin(vertices(g)), std::end(verices(g)));
      return std::move(g);
    }

  template<typename G, typename Iter>
    G make_star_graph(typename graph_traits<G>::size_type n, Iter first)
    {
      // assert(( readable_weak_range(first, n) ));
      assert(( n != 0 ));

      G g(n);
      make_star(g, std::begin(vertices(g)), std::end(verices(g)), first);
      return std::move(g);
    }

  template<typename G, typename Iter>
    G make_star_graph(Iter first, Iter last)
    {
      // assert(( readable_bounded_range(first, last) ));
      assert(( first != last ));

      G g(first, last);
      make_star(g, std::begin(vertices(g)), std::end(verices(g)));
      return std::move(g);
    }

  template<typename G, typename Iter1, typename Iter2>
    G make_star_graph(Iter1 first1, Iter1 last1, Iter2 first2)
    {
      // assert(( readable_bounded_range(first1, last1) ));
      // assert(( readable_weak_range(first2, last1 - first1) ));
      assert(( first1 != last1 ));
      
      G g(first1, last1);
      make_star(g, std::begin(vertices(g)), std::end(verices(g)), first2);
      return std::move(g);
    }

  template<typename G, typename V>
    G make_star_graph(std::initializer_list<V> list)
    {
      assert(( list.size() != 0 ));

      return std::move(make_star_graph<G>(list.begin(), list.end()));
    }

  template<typename G, typename V, typename E>
    G make_star_graph(std::initializer_list<V> list1, std::initializer_list<E> list2)
    {
      assert(( list1.size() != 0 ));
      assert(( list2.size() == list1.size() - 1 ));

      return std::move(make_star_graph<G>(list1.begin(), list1.end(), list2.begin()));
    }

  // TODO: Write fill/generate overloads.

  template<typename G, typename Num>
    inline G iota_star_graph(typename G::size_type n, Num num)
    {
      assert(( n != 0 ));
      
      G g(n);
      iota_star(g, std::begin(vertices(g)), std::end(vertices(g)), num);
      return std::move(g);
    }
  
  template<typename G, typename Iter, typename Num>
    inline G iota_star_graph(Iter first, Iter last, Num num)
    {
      assert(( first != last ));
      
      G g(first, last);
      iota_star(g, std::begin(vertices(g)), std::end(vertices(g)), num);
      return std::move(g);
    }
  
  template<typename G, typename V, typename Num>
    inline G iota_star_graph(std::initializer_list<V> list, Num num)
    {
      assert(( list.size() != 0 ));
      return std::move(iota_star_graph<G>(list.begin(), list.end(), num));
    }

  // A wheel graph Wn is comprised of a star graph Sn and a cycle graph C(n-1)
  // over the "spoke" vertices of the star. A star graph Sn has n vertices
  // and 2(n - 1) vertices.

  template<typename G, typename Iter, typename F>
    F for_wheel(G& g, Iter first, Iter last, F f)
    {
      for_star(g, first, last, f);
      for_cycle(g, std::next(first), last, f);
      return f;
    }

  // Make a wheel graph on the vertices in the range [first, last). A wheel
  // graph is constructed as a star graph on [first, last) and a cycle on 
  // [first + 1, last). Wheel graphs can only be constructed on 4 or more 
  // vertices.
  template<typename G, typename Iter>
    void make_wheel(G& g, Iter first, Iter last)
    {
      for_wheel(g, first, last, make_edge<G>{});
    }
    
  template<typename G, typename Iter1, typename Iter2>
    void make_wheel(G& g, Iter1 first1, Iter1 last1, Iter2 first2)
    {
      for_wheel(g, first1, last1, copy_edge<G, Iter2>{first2});
    }
    
  template<typename G, typename Iter, typename T>
    void fill_wheel(G& g, Iter first, Iter last, T const& value)
    {
      for_wheel(g, first, last, fill_edge<G, T>{value});
    }
    
  template<typename G, typename Iter, typename Gen>
    void generate_wheel(G& g, Iter first, Iter last, Gen gen)
    {
      for_wheel(g, first, last, generate_edge<G, Gen>{gen});
    }
    
  template<typename G, typename Iter, typename Num>
    void iota_wheel(G& g, Iter first, Iter last, Num num)
    {
      for_wheel(g, first, last, iota_edge<G, Num>{num});
    }

  template<typename G>
    G make_wheel_graph(typename graph_traits<G>::size_type n)
    {
      assert(( n >= 4 ));

      G g(n);
      make_wheel(g, std::begin(vertices(g)), std::end(verices(g)));
      return std::move(g);
    }

  template<typename G, typename Iter>
    G make_wheel_graph(typename graph_traits<G>::size_type n, Iter first)
    {
      // assert(( readable_weak_range(first, 2 * (n - 1)) ));
      assert(( n >= 4 ));

      G g(n);
      make_wheel(g, std::begin(vertices(g)), std::end(verices(g)));
      return std::move(g);
    }

  template<typename G, typename Iter>
    G make_wheel_graph(Iter first, Iter last)
    {
      // assert(( readable_bounded_range(first, last) ));
      // assert(( last - first >= 4 ));

      G g(first, last);
      make_wheel(g, std::begin(vertices(g)), std::end(verices(g)));
      return std::move(g);
    }

  template<typename G, typename Iter1, typename Iter2>
    G make_wheel_graph(Iter1 first1, Iter1 last1, Iter2 first2)
    {
      // assert(( readable_bounded_range(first1, last1) ));
      // assert(( readable_weak_range(first2, 2 * (last1 - first1 - 1) ));
      // assert(( last - first >= 4 ));

      G g(first1, last1);
      make_wheel(g, std::begin(vertices(g)), std::end(verices(g)));
      return std::move(g);
    }

  template<typename G, typename V>
    G make_wheel_graph(std::initializer_list<V> list)
    {
      assert(( list.size() >= 4 ));

      return std::move(make_wheel_graph<G>(list.begin(), list.end()));
    }

  template<typename G, typename V, typename E>
    G make_wheel_graph(std::initializer_list<V> list1, std::initializer_list<E> list2)
    {
      assert(( list1.size() >= 4 ));
      assert(( list2.size() == 2 * (list1.size() - 1) ));

      return std::move(make_wheel_graph<G>(list1.begin(), list1.end(), list2.begin()));
    }

  // TODO: Write fill/generate overloads

  template<typename G, typename Num>
    inline G iota_wheel_graph(typename G::size_type n, Num num)
    {
      assert(( n != 0 ));
      
      G g(n);
      iota_wheel(g, std::begin(vertices(g)), std::end(vertices(g)), num);
      return std::move(g);
    }
  
  template<typename G, typename Iter, typename Num>
    inline G iota_wheel_graph(Iter first, Iter last, Num num)
    {
      assert(( first != last ));
      
      G g(first, last);
      iota_wheel(g, std::begin(vertices(g)), std::end(vertices(g)), num);
      return std::move(g);
    }
  
  template<typename G, typename V, typename Num>
    inline G iota_wheel_graph(std::initializer_list<V> list, Num num)
    {
      assert(( list.size() != 0 ));

      return std::move(iota_wheel_graph<G>(list.begin(), list.end(), num));
    }


} // namespace origin

#endif
