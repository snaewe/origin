# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.

find_package(PythonInterp)

# Add a top-level check target.
# FIXME: How can I have the check target propagated to subdirectories. I'd
# really like this to execute 'make check' in each subdir rather then have
# all tests hooked into this target.
add_custom_target(check)
# FIXME: It would be great if we could summarize the test results... how many
# executed, how many passed, how many failed, etc.

# Set a variable "includes" to be the set of include flags for the current
# directory.
macro(get_include_flags)
  set(incs)
  get_directory_property(dirs INCLUDE_DIRECTORIES)
  foreach(dir ${dirs})
    set(incs "${incs} -I${dir}")
  endforeach()
  separate_arguments(incs UNIX_COMMAND "${incs}")
  set(includes ${incs})
endmacro()

# Analyze the test input and set a number of variables that are used to
# configure test information. Variables configured by this macro are:
#   name - the name of the test
#   module - the module in which the test resides (relative path)
#   test - the namae of the test target
#   path - the absoluate path to the input file
#   bindir - the binary directory where the test output is written
#   includes - Compiler include flags
#   flags - Compiler flags
macro(get_test_info)
  # Get the full "path" to the input file.
  # The "name" is the name of the file without its extension
  # The module is is the relative path to the directory being tested.
  set(path ${CMAKE_CURRENT_SOURCE_DIR}/${file})
  get_filename_component(name ${file} NAME_WE)
  file(RELATIVE_PATH module ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

  string(REPLACE "/" "-" tmp ${module})
  set(test test-${tmp}-${name})

  # Make sure that the binary directory for the test suite actually exists
  # before we try to build anything.
  set(bindir ${CMAKE_CURRENT_BINARY_DIR}/${name})
  if(NOT EXISTS ${bindir})
    file(MAKE_DIRECTORY ${bindir})
  endif()

  # Get some compiler flags.
  get_include_flags(includes)
  separate_arguments(flags UNIX_COMMAND "-g ${CMAKE_CXX_FLAGS}")
endmacro()

##
## origin_add_run_test
##
function(origin_add_run_test file)
  get_test_info()

  # NOTE There's really no need to name the executable anything other than
  # "run", but for completeness, we'll just name it after the input file.
  set(exec ${bindir}/${name}${CMAKE_EXECUTABLE_SUFFIX})

  # Add a command for building the output executable. This depends on
  # (minimally) the input file and produces a symbolic file, test_success in
  # the output directory.
  add_custom_command(
    OUTPUT ${bindir}/test-success
    IMPLICIT_DEPENDS CXX ${path}
    WORKING_DIRECTORY ${bindir}
    COMMAND
      ${PYTHON_EXECUTABLE} ${ORIGIN_ROOT}/cmake/test/run.py
        ${CMAKE_CXX_COMPILER}
        ${flags} ${includes}
        -o${exec}
        ${path}
    COMMENT "Testing ${name}")

  # Generate a target name for the test and hook it into the check target.
  add_custom_target(${test} DEPENDS ${bindir}/test-success)
  add_dependencies(check ${test})
endfunction()

##
## origin_add_compile_test
##
function(origin_add_compile_test file)
  get_test_info()

  # Create an output object name.
  set(target ${bindir}/${name}.o)

  # Add a command for building the output executable. This depends on
  # (minimally) the input file and produces a symbolic file, test_success in
  # the output directory.
  add_custom_command(
    OUTPUT ${bindir}/test-success
    IMPLICIT_DEPENDS CXX ${path}
    WORKING_DIRECTORY ${bindir}
    COMMAND
      ${PYTHON_EXECUTABLE} ${ORIGIN_ROOT}/cmake/test/compile.py
        ${CMAKE_CXX_COMPILER}
        ${flags} ${includes}
        -c -o${target}    # Note the "compile only" flag
        ${path}
    COMMENT "Testing ${name}")

  # Generate a target name for the test and hook it into the check target.
  add_custom_target(${test} DEPENDS ${bindir}/test-success)
  add_dependencies(check ${test})
endfunction()

##
## origin_add_fail_test
##
function(origin_add_fail_test file)
  get_test_info()

  # Create an output object name. This file should never actually be created.
  set(target ${bindir}/${name}.o)

  # Add a command for building the output executable. This depends on
  # (minimally) the input file and produces a symbolic file, test_success in
  # the output directory.
  add_custom_command(
    OUTPUT ${bindir}/test-success
    IMPLICIT_DEPENDS CXX ${path}
    WORKING_DIRECTORY ${bindir}
    COMMAND
      ${PYTHON_EXECUTABLE} ${ORIGIN_ROOT}/cmake/test/fail.py
        ${CMAKE_CXX_COMPILER}
        ${flags} ${includes}
        -o${target}
        ${path}
    COMMENT "Testing ${name}")

  # Generate a target name for the test and hook it into the check target.
  add_custom_target(${test} DEPENDS ${bindir}/test-success)
  add_dependencies(check ${test})
endfunction()

##
## origin_add_run_test
##
## Add a test case for the given file. Files must be prefixed with an
## instruction: one of "run", "compile", or "fail". This indicates the kind
## of test that will be executed when "make test" is run. A run test is required
## to compile and execute successfully. A compile test is only required to
## compile successfully. A fail test is required to fail compilation.
function(origin_add_test file)
  set(path )
  if(file MATCHES "run-*")
    origin_add_run_test(${file})
  elseif(file MATCHES "compile-*")
    origin_add_compile_test(${file})
  elseif(file MATCHES "fail-*")
    origin_add_fail_test(${file})
  else()
    # Oops, we can't deduce the test instruction.
    message(
      WARNING
      "No instructions for test ${CMAKE_CURRENT_SOURCE_DIR}/${file}")
  endif()
endfunction()

function(origin_test_run name file)
  message("Deprecated 'origin_run_test' at "
           "${CMAKE_CURRENT_LIST_FILE}:${CMAKE_CURRENT_LIST_LINE}")
  origin_add_run_test(${file})
endfunction()
