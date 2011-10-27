# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.

origin_executable(adjacency_list adjacency_list.cpp)

origin_run_test(generator generator.cpp)
origin_run_test(breadth_first breadth_first.cpp)
origin_run_test(depth_first depth_first.cpp)
origin_run_test(bipartite bipartite.cpp)
origin_run_test(topological_sort topological_sort.cpp)

# Shortest path Tests
# origin_run_test(dijkstra_shortest_path dijkstra_shortest_path.cpp)
# origin_run_test(floyd_warshall floyd_warshall.cpp)
# origin_run_test(bellman_ford bellman_ford.cpp)

add_subdirectory(operators)
