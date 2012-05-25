// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>

#include <origin/algorithm.hpp>

int main()
{
	
}

/*

using namespace std;
using namespace origin;


// Visited (adaptor)
// Associates the given value type with a value indicating that it has been
// visited by a function. The value -1 corresponds to a non-visited state.
// Any value greater than or equal to 0 indicates the order in which the
// object has been visited.
//
// TODO: Allow an object to be visited multiple times. This should record
// each visit.
//
// TODO: Move this into a testing tools facility and make it more fully
// featured (i.e., make it model a variety of basic concepts).
template <typename T = empty_t>
	struct visited_type
	{
		T value;
		int visited;

		visited_type(const T& x = {}) : value(x), visited(-1) { }
	};



// Visit (function)
// Visit an element x, marking it as visited.
template <typename T>
	struct visit
	{
		int counter;

		visit() : counter(0) { }

		void operator()(T& x) { x.visited = counter++; }
	};



// Visited (predicate)
// Returns true if an element x has been visited.
template <typename T>
	struct visited
	{
		bool operator()(const T& x) const { return x.visited != -1; }
	};



// Compare visitation order
// Returns true if a is visited before b.
//
// TODO: How does this work for multiple visitations?
template <typename T, typename R = less_relation>
	struct visitation_order_comparison
	{
		R comp;

		visitation_order_comparison(R comp = {}) : comp(comp) { }

		bool operator()(const T& a, const T& b) const
		{
			return comp(a.visited, b.visited);
		}
	};



// Check for each (test)
// For each applies a function f to each element in order, from left to
// right.
template <typename Env, typename Seq>
	void check_for_each(Env& env, Seq& seq)
	{
		using T = Value_type<Seq>;

		for_each(seq, visit<T> {});

		// All elements have been visited.
		assert(all_of(seq, visited<T> {}));

		// The elements are visited left-to-right.
		assert(is_sorted(seq, visitation_order_comparison<T> {}));
	}


int main()
{
  assert_checker<> env;

	using T = visited_type<int>;
	using V = vector<T>;

	V v0;
	check_for_each(env, v0);

	V v1 {1, 2, 3, 4, 5};
	check_for_each(env, v1);
}

*/