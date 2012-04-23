# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011-2012 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.

# origin_run_test(test_archetypes archetypes.cpp)
origin_run_test(test_check check.cpp)
origin_run_test(test_concepts concepts.cpp)
origin_run_test(test_iterators iterators.cpp)
origin_run_test(test_archetypes archetypes.cpp)
origin_run_test(test_prototypes prototypes.cpp)
origin_run_test(test_stability stability.cpp)

origin_executable(test_performance performance.cpp)
