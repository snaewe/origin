// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_GENERATOR_HPP
#define ORIGIN_GRAPH_GENERATOR_HPP

#include <algorithm>

#include <origin/graph/traits.hpp>

namespace origin
{
  // TODO: There's an EdgeFunction concept, which is a function with the
  // syntax f(g, u, v).
  
  
  // An edge function that adds an edge between two vertices.
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
    
  // requires Graph<G>;
  template<typename G>
    inline G make_null_graph()
    {
      return std::move(G{});
    }

  // Make a trivial graph with 1 vertex and no edges.
  // requires VertexInitializedGraph<G>
  template<typename G>
    inline G make_trivial_graph()
    {
      return std::move(G(1));
    }
    
  // Make a trivial graph with 1 vertex and no edges. Initalize the label of 
  // the vertex to the given value.
  // requires VertexInitializedGraph<G>
  template<typename G, typename V>
    inline G make_trivial_graph(V const& value)
    {
      // Kind of tricky, but &x and &x+1 define a range of 1 element.
      return std::move(&value, &value + 1);
    }

  // Path graph: A graph  om
  // P_0 - undefined
  // P_1 - singleton graph, no edges
  // P_2 - two vertices, 1 edge (u -- v)
  // P_n - n vertices n - 1 edges (v1 -- v2 -- ... -- vn)

  template<typename G, typename Iter, typename F>
    void for_path(G& g, Iter first, Iter last, F f) 
    {
      typedef typename graph_traits<G>::vertex Vertex;
      Vertex u = *first++;
      while(first != last) {
        Vertex v = *first++;
        f(g, u, v);
        u = v;
      }
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

  // Create a path graph with n vertices, all default initialized.
  // requires VertexInitializedGraph<G> && EdgeBuildableGraph<G>
  template<typename G>
    inline G make_path_graph(typename G::size_type n)
    {
      G g(n);
      make_path(g, begin(vertices(g)), end(vertices(g)));
      return std::move(g);
    }

  // Create a path graph using the values in the range [first, last) as the
  // labels of vertices to be added. The resulting graph will have last - first
  // vertices.
  template<typename G, typename Iter>
    inline G make_path_graph(Iter first, Iter last)
    {
      G g(first, last);
      make_path(g, begin(vertices(g)), end(vertices(g)));
      return std::move(g);
    }

  // Create a path graph using the values in the specified initializer list
  // as the labels of vertices to be added. The resulting graph will have
  // list.size() vertices.
  template<typename G, typename V>
    inline G make_path_graph(std::initializer_list<V> list)
    {
      return std::move(make_path_graph<G>(list.begin(), list.end()));
    }

  // Make with edge-specified values

  template<typename G, typename Iter>
    inline G make_path_graph(typename G::size_type n, Iter first)
    {
      // pre: readable_weak_range(first, n)
      G g(n);
      make_path(g, begin(vertices(g)), end(vertices(g)), first);
      return std::move(g);
    }

  template<typename G, typename Iter1, typename Iter2>
    inline G make_path_graph(Iter1 first1, Iter1 last1, Iter2 first2)
    {
      G g(first1, last1);
      make_path(g, begin(vertices(g)), end(vertices(g)), first2);
      return std::move(g);
    }

  template<typename G, typename V, typename Iter>
    inline G make_path_graph(std::initializer_list<V> list, Iter first)
    {
      return std::move(make_path_graph<G>(list.begin(), list.end()), first);
    }

  template<typename G, typename V, typename E>
    inline G make_path_graph(std::initializer_list<V> list1, 
                             std::initializer_list<E> list2)
    {
      return std::move(make_path_graph<G>(list1.begin(), list1.end(), 
                                          list2.begin()));
    }
  
  // Fill path graph
  
  template<typename G, typename T>
    inline G fill_path_graph(typename G::size_type n, T const& value)
    {
      G g(n);
      fill_path(g, begin(vertices(g)), end(vertices(g)), value);
      return std::move(g);
    }

  template<typename G, typename Iter, typename T>
    inline G fill_path_graph(Iter first, Iter last, T const& value)
    {
      G g(first, last);
      fill_path(g, begin(vertices(g)), end(vertices(g)), value);
      return std::move(g);
    }

  template<typename G, typename V, typename T>
    inline G fill_path_graph(std::initializer_list<V> list, T const& value)
    {
      return std::move(fill_path_graph<G>(list.begin(), list.end()), value);
    }

  // Generate Path Graph

  template<typename G, typename Gen>
    inline G generate_path_graph(typename G::size_type n, Gen gen)
    {
      G g(n);
      generate_path(g, begin(vertices(g)), end(vertices(g)), gen);
      return std::move(g);
    }

  template<typename G, typename Iter, typename Gen>
    inline G generate_path_graph(Iter first, Iter last, Gen gen)
    {
      G g(first, last);
      generate_path(g, begin(vertices(g)), end(vertices(g)), gen);
      return std::move(g);
    }

  template<typename G, typename V, typename Gen>
    inline G generate_path_graph(std::initializer_list<V> list, Gen gen)
    {
      return std::move(generate_path_graph<G>(list.begin(), list.end()), gen);
    }

  // Iota Path Graph

  template<typename G, typename Num>
    inline G iota_path_graph(typename G::size_type n, Num num)
    {
      G g(n);
      iota_path(g, begin(vertices(g)), end(vertices(g)), num);
      return std::move(g);
    }

  // Create a path graph using the values in the range [first, last) as the
  // labels of vertices to be added. The resulting graph will have last - first
  // vertices.
  template<typename G, typename Iter, typename Num>
    inline G iota_path_graph(Iter first, Iter last, Num n)
    {
      G g(first, last);
      iota_path(g, begin(vertices(g)), end(vertices(g)), n);
      return std::move(g);
    }

  // Create a path graph using the values in the specified initializer list
  // as the labels of vertices to be added. The resulting graph will have
  // list.size() vertices.
  template<typename G, typename V, typename Num>
    inline G iota_path_graph(std::initializer_list<V> list, Num n)
    {
      return std::move(iota_path_graph<G>(list.begin(), list.end(), n));
    }
  
  // Cycle graph: A path graph where the tail is adjacent to the head.
  // C1: v--v
  // C2: u--v--u
  // Cn: v1--v2--...--vn--v1
  // Cycle have n vertices and edgse.
  
  template<typename G, typename Iter, typename F>
    void for_cycle(G& g, Iter first, Iter last, F f)
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

  // Make cycle graph
  template<typename G>
    G make_cycle_graph(typename graph_traits<G>::size_type n)
    {
      G g(n);
      make_cycle(g, begin(vertices(g)), end(vertices(g)));
      return std::move(g);
    }
    
  template<typename G, typename Iter>
    G make_cycle_graph(Iter first, Iter last)
    {
      G g(first, last);
      make_cycle(g, begin(vertices(g)), end(vertices(g)));
      return std::move(g);
    }

  template<typename G, typename V>
    G make_cycle_graph(std::initializer_list<V> list)
    {
      return std::move(make_cycle_graph<G>(list.begin(), list.end()));
    }

  // TODO: Write fill_cycle_graph and generate_cycle_graph.

  // Iota cycle graph
  template<typename G, typename Num>
    G iota_cycle_graph(typename graph_traits<G>::size_type n, Num num)
    {
      G g(n);
      iota_cycle(g, begin(vertices(g)), end(vertices(g)), num);
      return std::move(g);
    }
    
  template<typename G, typename Iter, typename Num>
    G iota_cycle_graph(Iter first, Iter last, Num num)
    {
      G g(first, last);
      iota_cycle(g, begin(vertices(g)), end(vertices(g)), num);
      return std::move(g);
    }
    
  template<typename G, typename V, typename Num>
    G iota_cycle_graph(std::initializer_list<V> list, Num num)
    {
      return std::move(iota_cycle_graph<G>(list.begin(), list.end(), num));
    }


  // Complete graph: A fully connected graph. Every vertex is connected to
  // every other vertex. A complete graph Kn has n vertices and nC2 edges.

  // FIXME: Specialize make_clique (fill_clique?) for directed graphs. It
  // should add edges from (u, v) and (v, u).

  // The algorithm first connects each vertex referenced by the iterators in
  // [i, last) to every vertex in the range [next(i), last). 
  //
  // requires UndirectedGraph<G> ForwardIterator<Iter>
  template<typename G, typename Iter>
    void make_clique(G& g, Iter first, Iter last)
    {
      for( ; first != last; ++first) {
        for(Iter i = next(first); i != last; ++i) {
          add_edge(*first, *i);
        }
      }
    }
    
  template<typename G>
    G make_complete_graph(typename graph_traits<G>::size_type n)
    {
      G g(n);
      make_clique(g, begin(vertices(g)), end(verices(g)));
      return std::move(g);
    }

  template<typename G, typename Iter>
    G make_complete_graph(Iter first, Iter last)
    {
      G g(first, last);
      make_clique(g, begin(vertices(g)), end(verices(g)));
      return std::move(g);
    }

  template<typename G, typename V>
    G make_complete_graph(std::initializer_list<V> list)
    {
      return std::move(make_complete_graph<G>(list.begin(), list.end()));
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
      typedef typename graph_traits<G>::vertex Vertex;
      
      Vertex u = *first++;
      while(first != last) {
        Vertex v = *first++;
        add_edge(g, u, v);
      }
    }
    
  template<typename G>
    G make_star_graph(typename graph_traits<G>::size_type n)
    {
      G g(n);
      make_star(g, begin(vertices(g)), end(verices(g)));
      return std::move(g);
    }

  template<typename G, typename Iter>
    G make_star_graph(Iter first, Iter last)
    {
      G g(first, last);
      make_star(g, begin(vertices(g)), end(verices(g)));
      return std::move(g);
    }

  template<typename G, typename V>
    G make_star_graph(std::initializer_list<V> list)
    {
      return std::move(make_star_graph<G>(list.begin(), list.end()));
    }
    
  // Make a wheel graph on the vertices in the range [first, last). A wheel
  // graph is constructed as a star graph on [first, last) and a cycle on 
  // [first + 1, last). Wheel graphs can only be constructed on 4 or more 
  // vertices.
  template<typename G, typename Iter>
    void make_wheel(G& g, Iter first, Iter last)
    {
      assert(( distance(first, last) > 3 ));
      make_start(first, last);
      make_cycle(++first, last);
    }

  template<typename G>
    G make_wheel_graph(typename graph_traits<G>::size_type n)
    {
      G g(n);
      make_wheel(g, begin(vertices(g)), end(verices(g)));
      return std::move(g);
    }

  template<typename G, typename Iter>
    G make_wheel_graph(Iter first, Iter last)
    {
      G g(first, last);
      make_wheel(g, begin(vertices(g)), end(verices(g)));
      return std::move(g);
    }

  template<typename G, typename V>
    G make_wheel_graph(std::initializer_list<V> list)
    {
      return std::move(make_wheel_graph<G>(list.begin(), list.end()));
    }
    
} // namespace origin

#endif
