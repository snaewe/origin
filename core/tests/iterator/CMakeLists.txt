# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.


origin_run_test(iter_concepts concepts.cpp)

origin_run_test(iter_counter counter.cpp)
# origin_run_test(enumerator enumerator.cpp)

origin_run_test(iter_filter filter_iterator.cpp)
origin_run_test(iter_transform transform_iterator.cpp)
origin_run_test(iter_stride stride_iterator.cpp)
origin_run_test(iter_zip zip_iterator.cpp)

