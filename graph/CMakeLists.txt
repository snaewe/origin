# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.

cmake_minimum_required(VERSION 2.8)

set(ORIGIN_ROOT $ENV{ORIGIN_ROOT})
list(APPEND CMAKE_MODULE_PATH ${ORIGIN_ROOT}/cmake)
include(Origin)

origin_component(
  graph
  VERSION 0.1.0
  DEPENDS core range data math
  AUTHORS
    Andrew Sutton <andrew.n.sutton -at- gmail.com>
    Michael Lopez <mlopez7 -at- kent.edu>
)
add_subdirectory(test)
