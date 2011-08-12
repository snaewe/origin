# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.

origin_executable(graph graph.cpp)

# origin_executable(adjacency_matrix adjacency_matrix.cpp)
origin_executable(adjacency_vector adjacency_vector.cpp)

origin_executable(undirected undirected.cpp)

origin_run_test(generator generator.cpp)

origin_run_test(algorithm algorithm.cpp)
origin_run_test(bfs bfs.cpp)
origin_run_test(dfs dfs.cpp)
origin_run_test(dfs_check dfs_check.cpp)
origin_run_test(bipartite bipartite.cpp)

# Shortest path Tests
origin_run_test(dijkstra_shortest_path dijkstra_shortest_path.cpp)
origin_run_test(floyd_warshall floyd_warshall.cpp)
origin_run_test(bellman_ford bellman_ford.cpp)

add_subdirectory(io)
