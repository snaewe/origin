// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>

#include <origin/functional.hpp>

using namespace std;
using namespace origin;

struct fail { };

void check_eq()
{
	auto r_eq = eq();
	assert(r_eq(3, 3));
	assert(!r_eq(3, 4));
	
	auto p_eq = eq(3);
	assert(p_eq(3));
	assert(!p_eq(4));

	auto f_eq = eq(3, 3);
	assert(f_eq());

	// These must result in lookup failure.
	// auto r_eq_bad = eq() (3, fail {});
	// auto p_eq_bad = eq(3) (fail{});
	// auto f_eq_bad = eq(3, fail {});
}

void check_neq()
{
	auto r_neq = neq();
	assert(!r_neq(3, 3));
	assert(r_neq(3, 4));
	
	auto p_neq = neq(3);
	assert(!p_neq(3));
	assert(p_neq(4));

	auto f_neq = neq(3, 3);
	assert(!f_neq());

	// These must result in lookup failure.
	// auto r_neq_bad = neq() (3, fail {});
	// auto p_neq_bad = neq(3) (fail{});
	// auto f_neq_bad = neq(3, fail {});
}

void check_lt()
{
	auto r_lt = lt();
	assert(!r_lt(3, 3));
	assert(r_lt(3, 4));
	
	auto p_lt = lt(3);
	assert(!p_lt(3));
	assert(p_lt(4));

	auto f_lt = lt(3, 3);
	assert(!f_lt());

	// These must result in lookup failure.
	// auto r_lt_bad = lt() (3, fail {});
	// auto p_lt_bad = lt(3) (fail{});
	// auto f_lt_bad = lt(3, fail {});
}

void check_gt()
{
	auto r_gt = gt();
	assert(!r_gt(3, 3));
	assert(r_gt(3, 2));
	
	auto p_gt = gt(3);
	assert(!p_gt(3));
	assert(p_gt(2));

	auto f_gt = gt(3, 3);
	assert(!f_gt());

	// These must resugt in lookup failure.
	// auto r_gt_bad = gt() (3, fail {});
	// auto p_gt_bad = gt(3) (fail{});
	// auto f_gt_bad = gt(3, fail {});
}

void check_lte()
{
	auto r_lte = lte();
	assert(r_lte(3, 3));
	assert(!r_lte(3, 2));
	
	auto p_lte = lte(3);
	assert(p_lte(3));
	assert(!p_lte(2));

	auto f_lte = lte(3, 3);
	assert(f_lte());

	// These must resulte in lookup failure.
	// auto r_lte_bad = lte() (3, fail {});
	// auto p_lte_bad = lte(3) (fail{});
	// auto f_lte_bad = lte(3, fail {});
}

void check_gte()
{
	auto r_gte = gte();
	assert(r_gte(3, 3));
	assert(!r_gte(3, 4));
	
	auto p_gte = gte(3);
	assert(p_gte(3));
	assert(!p_gte(4));

	auto f_gte = gte(3, 3);
	assert(f_gte());

	// These must resugte in lookup failure.
	// auto r_gte_bad = gte() (3, fail {});
	// auto p_gte_bad = gte(3) (fail{});
	// auto f_gte_bad = gte(3, fail {});
}

int main()
{
	// FIXME: Make each of these its own test file? Perhaps.

	check_eq();
	check_neq();
	check_lt();
	check_gt();
	check_lte();
	check_gte();

	// FIXME: Finish testing the other function objects in functional.
}
