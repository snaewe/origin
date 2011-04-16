# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.

# FIXME: Make these real tests!
add_executable(tuple tuple.cpp)
add_executable(optional optional.cpp)

add_subdirectory(iterator)

# FIXME: Re-enable concept testing when I backport the concepts
add_subdirectory(concepts)
