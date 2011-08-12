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
  // TODO: Implement real generators that create and return graphs. How do
  // we specify the value of vertices? Do we initialize over a range of vertex
  // values?

  
  // Path graph: A graph  om
  // P_0 - undefined
  // P_1 - singleton graph, no edges
  // P_2 - two vertices, 1 edge (u -- v)
  // P_n - n vertices n - 1 edges (v1 -- v2 -- ... -- vn)

  // Induce the graph Pn on the graph g over the specified vertices in the 
  // range [first, last) such that the first refers to the the head of the 
  // path, and prev(last) refers to the tail, and the value associated with
  // each edge is specified by the given value.. Edges (u, v) are added between
  // subsequent iterators. That is, for iterators i and j in [first, last) 
  // where j == next(i), the action taken is g.add_edge(*i, *j, value);
  //
  // requires EdgeBuildable<Graph> && InputIterator<Iter> &&
  //   Same<Vertex<Graph>, ValueType<Iter>>
  template<typename Graph, typename Iter, typename T>
    void fill_path(Graph& g, Iter first, Iter last, T const& value)
    {
      typedef typename graph_traits<Graph>::vertex Vertex;
      assert(( first != last ));
      Vertex u = *first++;
      while(first != last) {
        Vertex v = *first++;
        g.add_edge(u, v, value);
        u = v;
      }
    }

  // Induce the graph Pn on the first n vertices of g.
  template<typename Graph, typename T>
    void fill_path_n(Graph& g, typename graph_traits<Graph>::size_type n, T const& value)
    {
      assert(( n > 0 ));
      auto i = begin(g.vertices());
      fill_path(g, i, std::next(i, n), value);
    }
    
  // Return a new path graph whose vertices are initialized over the values
  // in the range [first, last) and whose edges are filled with the given
  // value.
  template<typename Graph, typename Iter, typename T>
    Graph make_filled_path(Iter first, Iter last, T const& value)
    {
      assert(( first != last ));
      Graph g(first, last);
      fill_path_n(g, g.order(), value);
      return std::move(g);
    }

  // Return a new path graph whose vertices are initialized over the values in
  // the given initializer list and whose edges are filled with the given
  // edge value.
  template<typename Graph, typename Vertex, typename Edge>
    Graph make_filled_path(std::initializer_list<Vertex> list, Edge const& value)
    {
      return std::move(make_filled_path<Graph>(list.begin(), list.end(), value));
    }

  // Return a new path graph whose edges are filled with the given value. The
  // vertices of the graph are default initialized.
  template<typename Graph, typename T>
    Graph make_filled_path_n(typename graph_traits<Graph>::size_type n, T const& value)
    {
      assert(( n > 0 ));
      Graph g(n);
      fill_path_n(g, g.order(), value);
      return std::move(g);
    }


  // Induce a path over the vertices in the range [first, last).
  //
  // The value assigned to added edge each edge is the result of the gen
  // function, which is called over the graph and the ends of the added edge.
  template<typename Graph, typename Iter, typename Gen>
    void generate_path(Graph& g, Iter first, Iter last, Gen gen)
    {
      typedef typename graph_traits<Graph>::vertex Vertex;
      assert(( first != last ));
      Vertex u = *first++;
      while(first != last) {
        Vertex v = *first++;
        g.add_edge(u, v, gen(g, u, v));
        u = v;
      }
    }
  
  // Induce the graph Pn on the first n vertices of g.
  template<typename Graph, typename Gen>
    void generate_path_n(Graph& g, typename graph_traits<Graph>::size_type n, Gen gen)
    {
      assert(( n >= 1 ));
      auto i = begin(g.vertices());
      generate_path(g, i, next(i, n), gen);
    }

  // FIXME: Write make_generated_path functions.

  // Cycle graph: A path graph where the tail is adjacent to the head.
  // C1: v--v
  // C2: u--v--u
  // Cn: v1--v2--...--vn--v1
  // Cycle have n vertices and edgse.
  
  // Induce a cycle on the vertices in the range [first, last).
  // requires: EdgeBuildable<Graph> && InputIterator<Iter>
  template<typename Graph, typename Iter, typename T>
    void fill_cycle(Graph& g, Iter first, Iter last, T const& value)
    {
      typedef typename graph_traits<Graph>::vertex Vertex;
      assert(( first != last ));
      
      Vertex u = *first++;
      Vertex h = u;
      while(first != last) {
        Vertex v = *first++;
        g.add_edge(u, v, value);
        u = v;
      }
      g.add_edge(u, h);
    }
    
  template<typename Graph, typename T>
    void fill_cycle_n(Graph& g, typename graph_traits<Graph>::size_type n, T const& value)
    {
      assert(( n >= 1 ));
      auto i = begin(g.vertices());
      fill_cycle(g, i, std::next(i, n), value);
    }
    
  // FIXME: Write generate_cycle functions
  
  // FIXME: Write make_filled_cycle functions
  
  // FIXME: Write make_generated_cycle
    
  // Complete graph: A fully connected graph. Every vertex is connected to
  // every other vertex. A complete graph Kn has n vertices and nC2 edges.

  // FIXME: Specialize fill_complete (fill_clique?) for directed graphs. It
  // should add edges from (u, v) and (v, u).

  // The algorithm first connects each vertex referenced by the iterators in
  // [i, last) to every vertex in the range [next(i), last). 
  //
  // requires ForwardIterator<Iter>
  template<typename Graph, typename Iter, typename T>
    void fill_complete(Graph& g, Iter first, Iter last, T const& value)
    {
      for( ; first != last; ++first) {
        for(Iter i = next(first); i != last; ++i) {
          add_edge(*first, *i, value);
        }
      }
    }
}

#endif
