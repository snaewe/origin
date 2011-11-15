// Copyright (c) 2008-2011 Kent State University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef TEST_ADJACENCY_MATRIX_DATA_ACCESS_GRAPH_HPP
#define TEST_ADJACENCY_MATRIX_DATA_ACCESS_GRAPH_HPP

#include <iostream>
#include <limits>
#include <origin/graph/edge.hpp>

//#include <random>

/**
 * Test the construction and copy semantics of graph types.
 *
 * Tests:
 *    boolean_adjacency_matrix(n)
 *    add_edge(v, u)
 *    g(i,j)
 *    g[e]
 *
 * @tparam Graph A directed adjacency matrix.
 */
template<typename Graph>
struct graph_data_access_bool
{
  typedef typename Graph::vertex Vertex;
  typedef typename Graph::edge Edge;
  typedef typename Graph::matrix_type Matrix;
  typedef typename Graph::size_type size_type;

  void test()
  {
    using namespace std;

    Graph g(order);

    

    // Generate edges from random binary data

    bool data[order][order] = {
      {0,1,0,1,0,1,0,1},
      {1,1,0,0,1,1,0,0},
      {0,0,0,1,0,1,1,1},
      {1,1,0,1,0,1,0,0},
      {0,0,0,1,0,0,0,1},
      {1,0,0,0,1,1,0,0},
      {0,0,1,0,1,1,1,0},
      {1,0,1,0,0,0,0,1}};

    // count degrees of vertices
    size_type in_deg[order], out_deg[order], deg[order];
    for(auto i = 0u; i < order; ++i) {
      in_deg[i] = out_deg[i] = deg[i] = 0;
      for(auto j = 0u; j < order; ++j) {
        if(data[i][j])
        { ++out_deg[i]; ++deg[i]; }
        if(data[j][i]) {
          ++in_deg[i];
          ++deg[i];
          if(i == j) --deg[i];
        }
      }
    }

    // Add vertices
    for(auto i = 0u; i < order; ++i)
      for(auto j = 0u; j < order; ++j)
        if(data[i][j]) 
          g.add_edge(Vertex(i), Vertex(j), true);

    // Verify vertices
    for(auto i = 0u; i < order; ++i)
      for(auto j = 0u; j < order; ++j) {
        assert(( g(i,j) == data[i][j] ));
        Edge e(i,j);
        assert(( g[e] == data[i][j] ));
      }

    // Check edge iterators
    for(auto ev : g.edges()) {
      assert(( data[g.source(ev).value][g.target(ev).value] ));
    }

    // Check in/out edges and in/out degrees
    for(auto i = 0u; i < order; ++i) {
      assert(( g.degree(Vertex(i)) == deg[i] ));
      for(auto out_e : out_edges(g, Vertex(i))) {
        assert(( data[i][g.target(out_e).value] ));
        assert(( g.out_degree(Vertex(i)) == out_deg[i] ));
      }
      for(auto in_e : g.in_edges(Vertex(i))) {
        assert(( data[g.source(in_e).value][i] ));
        assert(( g.in_degree(Vertex(i)) == in_deg[i] ));
      }
    }

    // Assert total degree of vertices
    for(auto i = 0u; i < order; ++i) {
      assert(( g.degree(Vertex(i)) == deg[i] ));
    }
  }

  private:
    static const size_type order = 8;

    // Helper function returns an array of boolean data.
    void set_random(char* p)
    {
      /*std::uniform_int_distribution unif(-127, 128);
      for(auto i = 0u; i < order; ++i) {
        p[i] = unif();
      }*/
    }
};

template<typename Graph>
struct graph_data_access_dist
{
  typedef typename Graph::vertex Vertex;
  typedef typename Graph::edge Edge;
  typedef typename Graph::matrix_type Matrix;
  typedef typename Graph::size_type size_type;

  void test()
  {
    using namespace std;

    Graph g(order);

    

    // Generate edges from random binary data

    float data[order][order] = {
      {0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f},
      {1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f},
      {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f},
      {1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f},
      {0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f},
      {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f},
      {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f},
      {1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f}};

    // count degrees of vertices
    size_type in_deg[order], out_deg[order], deg[order];
    for(auto i = 0u; i < order; ++i) {
      in_deg[i] = out_deg[i] = deg[i] = 0;
      for(auto j = 0u; j < order; ++j) {
        if(data[i][j] == 1.0f)
        { ++out_deg[i]; ++deg[i]; }
        if(data[j][i] == 1.0f) {
          ++in_deg[i];
          ++deg[i];
          if(i == j) --deg[i];
        }
      }
    }

    // Add vertices
    for(auto i = 0u; i < order; ++i)
      for(auto j = 0u; j < order; ++j)
        if(data[i][j] == 1.0f) 
          g.add_edge(Vertex(i), Vertex(j), 1.0f);


    // Verify vertices
    for(auto i = 0u; i < order; ++i)
      for(auto j = 0u; j < order; ++j) {
        if(data[i][j] == 1.0f) {
          assert(( g(i,j) == data[i][j] ));
          Edge e(i,j);
          assert(( g[e] == data[i][j] ));
        }
        else {
          assert(( g(i,j) == std::numeric_limits<float>::infinity() ));
          Edge e(i,j);
          assert(( g[e] == std::numeric_limits<float>::infinity() ));
        }
      }

    // Check edge iterators
    for(auto ev : g.edges()) {
      assert(( data[g.source(ev).value][g.target(ev).value] ));
    }

    // Check in/out edges and in/out degrees
    for(auto i = 0u; i < order; ++i) {
      assert(( g.degree(Vertex(i)) == deg[i] ));
      for(auto out_e : g.out_edges(Vertex(i))) {
        assert(( data[i][g.target(out_e).value] ));
        assert(( g.out_degree(Vertex(i)) == out_deg[i] ));
      }
      for(auto in_e : g.in_edges(Vertex(i))) {
        assert(( data[g.source(in_e).value][i] ));
        assert(( g.in_degree(Vertex(i)) == in_deg[i] ));
      }
    }

    // Assert total degree of vertices
    for(auto i = 0u; i < order; ++i) {
      assert(( g.degree(Vertex(i)) == deg[i] ));
    }
  }

  private:
    static const size_type order = 8;

    // Helper function returns an array of boolean data.
    void set_random(char* p)
    {
      /*std::uniform_int_distribution unif(-127, 128);
      for(auto i = 0u; i < order; ++i) {
        p[i] = unif();
      }*/
    }
};
#endif
