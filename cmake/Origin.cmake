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

  # Set basic tree information. The project root is the top-level directory
  # that includes the build system and the module root. The module root
  # is the top-level of all modules.
  set(ORIGIN_PROJECT_ROOT ${CMAKE_SOURCE_DIR})
  set(ORIGIN_MODULE_ROOT ${CMAKE_SOURCE_DIR}/origin)


  # Set the directory containing Origin's CMake modules: the build system.
  get_filename_component(ORIGIN_CMAKE_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)

  # Include some useful packages.
  include(CMakeDependentOption)
  include(BoostUtils)


  # Be sure to compile in C++11 mode!
  # FIXME: Move the C++ configuration stuff into a separate config module.
  set(CMAKE_CXX_FLAGS "-std=c++11")

  # Make sure that we can include files as <origin/xxx>.
  include_directories(${ORIGIN_PROJECT_ROOT})  


  # Include Origin-specific macros
  include(OriginVersion)
  include(OriginModule)
  include(OriginTest)

endif()

