# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.

origin_run_test(range_concepts concepts.cpp)
origin_run_test(range_function range.cpp)
origin_run_test(range_bounded bounded.cpp)
origin_run_test(range_stride stride.cpp)

origin_run_test(range_reverse reverse.cpp)
origin_run_test(range_filter filter.cpp)
origin_run_test(range_until until.cpp)
origin_run_test(range_transform transform.cpp)

origin_run_test(range_zip zip.cpp)

origin_run_test(range_permutation permutation.cpp)
origin_run_test(range_combination combination.cpp)


# origin_executable(range_perf_counting perf_counting.cpp)
