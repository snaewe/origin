# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.

add_subdirectory(dynarray)
# add_subdirectory(heap)

# origin_run_test(stack stack.cpp)
# origin_run_test(queue queue.cpp)

origin_run_test(ordinal ordinal.cpp)
origin_run_test(ordinal_map ordinal_map.cpp)

# add_subdirectory(perf)
