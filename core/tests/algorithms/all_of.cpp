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


// Quantifiers in terms of find algorithms.

template <typename R, typename P>
	struct all_equiv_find
	{
		bool operator()(const R& range, P pred) const
		{
			return all_of(range, pred) == (find_if_not(range, pred) == end(range));
		}
	};

template <typename R, typename P>
	struct some_equiv_find
	{
		bool operator()(const R& range, P pred) const
		{
			return some_of(range, pred) == (find_if(range, pred) != end(range));
		}
	};

template <typename R, typename P>
	struct nall_equiv_all
	{
		bool operator()(const R& range, P pred) const
		{
			return not_all_of(range, pred) == !all_of(range, pred);
		}
	};

template <typename R, typename P>
	struct none_equiv_some
	{
		bool operator()(const R& range, P pred) const
		{
			return none_of(range, pred) == !some_of(range, pred);
		}
	};


// Quantifiers in terms of counting algorithms. Because a range has size
// 0 to n, a distance n != 0 means implies strictly positive, and n != distance
// means strictly less than distance.

template <typename R, typename P>
	struct all_equiv_count
	{
		bool operator()(const R& range, P pred) const
		{
			return all_of(range, pred) == (count_if(range, pred) == distance(range));
		}
	};

template <typename R, typename P>
	struct some_equiv_count
	{
		bool operator()(const R& range, P pred) const
		{
			return some_of(range, pred) == (count_if(range, pred) != 0);
		}
	};

template <typename R, typename P>
	struct nall_equiv_count
	{
		bool operator()(const R& range, P pred) const
		{
			return not_all_of(range, pred) == (count_if(range, pred) != distance(range));
		}
	};

template <typename R, typename P>
	struct none_equiv_count
	{
		bool operator()(const R& range, P pred) const
		{
			return none_of(range, pred) == (count_if(range, pred) == 0);
		}
	};

// Check the specification of query-based qualifier predicates.
//
// FIXME: Finish writing qualifier requirements in terms of count.
template <typename R, typename P>
	struct quantifier_spec
	{
		static_assert(Range_query<R, P>(), "");

		// Positive queries in terms of find_if
		all_equiv_find<R, P> all_find;
		some_equiv_find<R, P> some_find;

		// Negations of positive queries.
		nall_equiv_all<R, P> nall;
		none_equiv_some<R, P> none;
		
		// Queries in terms of counting.
		all_equiv_count<R, P> all_count;
		some_equiv_count<R, P> some_count;
		nall_equiv_count<R, P> nall_count;
		none_equiv_count<R, P> none_count;

		single_value_distribution<P> pred;

		explicit quantifier_spec(P pred) : pred(pred) { }

		template <typename Env, typename Var>
			void operator()(Env& env, Var&& var) const
			{
				static_assert(Same<Result_type<Forwarded<Var>>, R>(), "");

				auto pvar = make_random(env.random_engine(), pred);
				check(env, all_find, var, pvar);
				check(env, some_find, var, pvar);
				check(env, nall, var, pvar);
				check(env, none, var, pvar);
				check(env, all_count, var, pvar);
				check(env, some_count, var, pvar);
				check(env, nall_count, var, pvar);
				check(env, none_count, var, pvar);
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

	quantifier_spec<V, P> spec {P {}};
	quick_check(env, spec, var, 1000);
}
