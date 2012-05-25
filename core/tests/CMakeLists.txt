# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011-2012 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.

# FIXME: Move these into separate subdirectories.
origin_run_test(tuple tuple.cpp)
origin_run_test(optional optional.cpp)
origin_run_test(functional functional.cpp)
origin_run_test(memory memory.cpp)

# FIXME: The testing directory is temporarily broken.

# add_subdirectory(testing)
add_subdirectory(traits)
add_subdirectory(concepts)

add_subdirectory(iterator)
add_subdirectory(ranges)
add_subdirectory(algorithms)
