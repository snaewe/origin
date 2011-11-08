# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.


# This module is responsible for the configuration of the build system with
# respect to the local environment.


# Define a Profile build mode.
# TODO: Support for profiling is not very good right now. This should be
# improved substantially.
# set(CMAKE_C_FLAGS_PROFILE "-pg" CACHE STRING "Profile")
# set(CMAKE_CXX_FLAGS_PROFILE "-pg" CACHE STRING "Profile")


# Define C++0x flags - these get used in macros for creating binary libraries
# and executables.
# Explicitly disable unused variables, which causes concept code to become
# quite noisy.
# FIXME: Disable unused parameters also?
set(ORIGIN_IGNORED_WARNINGS "-Wno-unused-variable -Wno-unused-value")
set(CMAKE_CXX_FLAGS "-Wall ${ORIGIN_IGNORED_WARNINGS} -std=c++0x")

