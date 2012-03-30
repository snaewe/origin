// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <vector>

#include <origin/algorithm.hpp>
#include <origin/testing.hpp>

using namespace std;
using namespace origin;


// The all of quantifier is true when no element can be found that does not
// satsify the given predicate. That is, the following expression must be
// true:
//
// 		all_of(range, pred) <=> find_if_not(range, pred) == end(range)
//
// for all ranges and predicates.
template <typename R, typename P>
	struct all_of_equiv_find_if
	{
		P pred;

		all_of_equiv_find_if(P pred) : pred(pred) { }

		bool operator()(const R& range) const
		{
			return all_of(range, pred) == (find_if_not(range, pred) == end(range));
		}
	};


// The all of quantifier is true when the number of elements in the range
// that satisfy the predicate is equal to the size of the range. That is, the 
// is true:
//
// 		all_of(range, pred) <=> count_if(range, pred) == distance(range)
//
// for all ranges and predicates.
template <typename R, typename P>
	struct all_of_equiv_count_if
	{
		P pred;

		all_of_equiv_count_if(P pred) : pred(pred) { }

		bool operator()(const R& range) const
		{
			return all_of(range, pred) == (count_if(range, pred) == distance(range));
		}
	};


// Check the specification of find_if.
// FIXME: Should the specification encompass all Query-like quantifiers?
template <typename R, typename P>
	struct all_of_spec
	{
		static_assert(Range_query<R, P>(), "");

		all_of_equiv_find_if<R, P> all_find_if;
		all_of_equiv_count_if<R, P> all_count_if;

		explicit all_of_spec(P pred) 
			: all_find_if(pred)
			, all_count_if(pred)
		{ }

		template <typename Env, typename Var>
			void operator()(Env& env, Var&& var) const
			{
				static_assert(Same<Result_type<Forwarded<Var>>, R>(), "");
				check(env, all_find_if, var);
				check(env, all_count_if, var);
			}
	};


// template <typename Env, typename R, typename P>
//   void check_all_of(Env& env, const R& range, P pred)
//   {
//     all_of_equiv_find_if<Forwarded<R>, P> find_if;
//     all_of_equiv_count_if<Forwarded<R>, P> count_if;
//     check(env, find_if, spec, range, pred);
//     check(env, find_if, spec, range, pred);
//   }

int main()
{
  assert_checker<> env;

	using V = vector<int>;
	using P = non_zero_pred<int>;

	auto var = checkable_var<V>(env);

	all_of_spec<V, P> spec {P {}};
	quick_check(env, spec, var);
}
