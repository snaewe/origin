# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.

origin_run_test(tuple tuple.cpp)
origin_run_test(optional optional.cpp)

add_subdirectory(iterator)
add_subdirectory(concepts)
