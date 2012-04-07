# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.

# origin_blas_run_test 
# Creates a run test for a blas file with the correct simd flags enabled.
# To test without SIMD flags use the regular origin_run_test
macro(origin_blas_run_test progname)
    origin_run_test(${progname} ${ARGN})
    set_target_properties( ${target} PROPERTIES COMPILE_FLAGS "-msse -msse2")
    # TODO: Add compiler definitions later on at somepoint.
    #COMPILE_DEFINITIONS 
endmacro(origin_blas_run_test)


#set_target_properties(
#    ${target}
#    PROPERTIES
#    COMPILE_FLAGS /Yu"${hdrfile}" /Fp"${pchpath}"
#)

origin_blas_run_test(matrix_alloc_base matrix_alloc_base_test.cpp)

