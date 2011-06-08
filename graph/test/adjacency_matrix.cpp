// Copyright (c) 2008-2011 Kent State University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.


#include <iostream>
#include <cassert>

#include <origin/range/zip_range.hpp>
#include <origin/graph/adjacency_matrix.hpp>
#include <origin/dynarray/square_dynarray.hpp>

// Test suite
#include "test_adjacency_matrix/test_create.hpp"
#include "test_adjacency_matrix/test_size.hpp"
#include "test_adjacency_matrix/test_data_access.hpp"

/**
 * A simple harness for running a test model. Note that setup and teardown are
 * performed in the (default) constructors of the test model.
 */
void test_copy_create() {
  using namespace origin;

  typedef square_dynarray<bool> Boolean_Matrix;
  typedef square_dynarray<float> Distance_Matrix;

  create_copy_graph_bool<adjacency_matrix<char, Boolean_Matrix>> t_bool;
  create_copy_graph_dist<distance_matrix<char, Distance_Matrix>> t_dist;

  t_bool.test();
  t_dist.test();
};

void test_sizes() {
  using namespace origin;

  typedef square_dynarray<bool> Boolean_Matrix;
  typedef square_dynarray<float> Distance_Matrix;

  graph_sizes_bool<adjacency_matrix<char, Boolean_Matrix>> t_bool;
  graph_sizes_dist<distance_matrix<char, Distance_Matrix>> t_dist;

  t_bool.test();
  t_dist.test();
};

template<typename Matrix = origin::square_dynarray<bool>>
void test_data_access() {
  using namespace origin;

  typedef square_dynarray<bool> Boolean_Matrix;
  typedef square_dynarray<float> Distance_Matrix;

  graph_data_access_bool<adjacency_matrix<char, Boolean_Matrix>> t_bool;
  graph_data_access_dist<distance_matrix<char, Distance_Matrix>> t_dist;

  t_bool.test();
  t_dist.test();
};

int main()
{
  test_copy_create();
  test_sizes();
  test_data_access();

  return 0;
}

