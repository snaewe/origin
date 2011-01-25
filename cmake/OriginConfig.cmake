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
set(CMAKE_CXX_FLAGS "-Wall -Wno-unused-variable -std=c++0x")

# NOTE: This was deprecated in favor of simply snatching the BoostUtils modul
# into the Origin source tree
# Grab BOOST_ROOT from the environment and put the Boost CMake modules in the
# search path. We use this to import BoostUtils, which defines some useful
# macros like parse_arguments.
set(BOOST_ROOT $ENV{BOOST_ROOT})
if(NOT BOOST_ROOT)
    message(FATAL_ERROR "Set $BOOST_ROOT before building")
endif()

# FIXME: We shouldn't need this because we've directly imported
# the util files.
# list(APPEND CMAKE_MODULE_PATH ${BOOST_ROOT}/tools/build/CMake)

# Define the BOOST_INCLUDE_DIR, which will probably be used by just about
# every library out there. Also, automatically add Boost to the include path
# for all our nested projects.
set(BOOST_INCLUDE_DIR ${BOOST_ROOT})
include_directories(${BOOST_INCLUDE_DIR})

