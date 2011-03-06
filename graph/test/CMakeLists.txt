# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.

origin_executable(graph graph.cpp)

# Scratch Work. This will not be integrated into the library

origin_executable(adjacency_graph adjacency_matrix.cpp)
origin_executable(bfs bfs.cpp)

origin_executable(bipartite bipartite.cpp)

add_subdirectory(io)