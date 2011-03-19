# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.


# Add a new test run test for a given source code file. This adds a target for
# building the executable and a test target for the CTest.
#
#   origin_run_test(target src [args])
#
# Target is the name of the target, and src is the name of the source file.
# Command-line arguments to the test can also be specified.
macro(origin_run_test target src)
  origin_executable(${target} ${src})
  add_test(test_${target} ${target} ${ARGN})
endmacro()

# Turn on testing.
enable_testing()

