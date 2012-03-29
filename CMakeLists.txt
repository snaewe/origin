# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011-2012 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.

cmake_minimum_required(VERSION 2.8)

if(DEFINED $ENV{ORIGIN_ROOT})
  set(ORIGIN_ROOT $ENV{ORIGIN_ROOT})
else()
  set(ORIGIN_ROOT ${CMAKE_SOURCE_DIR})
endif(DEFINED $ENV{ORIGIN_ROOT})
list(APPEND CMAKE_MODULE_PATH ${ORIGIN_ROOT}/cmake)
include(Origin)


add_subdirectory(core)
add_subdirectory(range)
add_subdirectory(data)
add_subdirectory(graph)
