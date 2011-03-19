# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.

# Note that this is only ever included once. It prevents multiple inclusions
# and checks when building from the top-level.
if(NOT ORIGIN_INCLUDED)
  set(ORIGIN_INCLUDED TRUE)

  # The ORIGIN_CMAKE_DIR is the path to the directory containing the Origin
  # CMake build extensions.
  get_filename_component(ORIGIN_CMAKE_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)

  # Define ORIGIN_ROOT as either the current build, or the user has specified
  # an alternative in the environemnt. This variable is used in various
  # macros to affect the build environment.
  set(root $ENV{ORIGIN_ROOT})
  if(root)
      set(ORIGIN_ROOT ${root})
  else()
      set(ORIGIN_ROOT ${CMAKE_SOURCE_DIR})
  endif()

  # This file contains all of the build macros necessary to build, test, and
  # install Origin libraries. Note that much of this is actually based on the
  # Boost.CMake environment.

  # Include Origin-specific macros.
  include(BoostUtils)
  include(OriginVersion)
  include(OriginConfig)
  include(OriginProject)
  include(OriginDepend)
  include(OriginExecutable)
  include(OriginTest)
  include(OriginDocument)
  include(OriginInstall)
  include(OriginPackage)
endif()
