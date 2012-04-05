
// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef TESTING_QUERY_HPP
#define TESTING_QUERY_HPP

#include <vector>

#include <origin/algorithm.hpp>
#include <origin/container.hpp>
#include <origin/testing.hpp>

namespace testing
{
	using namespace std;
	using namespace origin;










	// Find if (specification)
	// Check additional algorithms and properties affiliated with the find_if 
	// algorithm.
	template <typename R, typename P>
	  struct find_if_specs
	  {
	    static_assert(Range_query<R, P>(), "");

	    find_if_results<R, P> find_if;
	    find_ifnt_equiv<R, P> find_ifnt;

	    // The default predicate.
	    P pred;

	    find_if_specs(P p = {}) : pred(p) { }
	    
	    // Value checking.
	    template <typename Env>
	      void operator()(Env& env, const R& range, P pred) const
	      {
	      	check(env, find_if, range, pred);
	        check(env, find_ifnt, range, pred);
	      }

	    // Random checking.
	    template <typename Env, typename Rgen, typename Pgen>
	      auto operator()(Env& env, Rgen& range, Pgen& pred) const
	        -> Requires<Random_variable<Rgen>() && Random_variable<Pgen>()>
	      {
	        operator()(env, range(), pred());
	      }
	    
	    // Default random checking.
      template <typename Env>
	      void operator()(Env& env) const
	      {
	      	auto pdist = single_value_distribution<P> {pred};
	      	auto pgen = checkable_var(env, pdist);
	      	auto rgen = checkable_var<R>(env);
	      	operator()(env, rgen, pgen);
	      }	  
		};



	// Find if (specification)
	// Check additional algorithms and properties affiliated with the find_if 
	// algorithm.
	template <typename R, typename T = Value_type<R>>
	  struct find_specs
	  {
	    static_assert(Range_search<R, T>(), "");

	    find_eq_equiv<R, T> find_eq;
	    find_neq_equiv<R, T> find_neq;

	    // Value checking.
	    template <typename Env>
	      void operator()(Env& env, const R& range, const T& value) const
	      {
	      	check(env, find_eq, range, value);
	        check(env, find_neq, range, value);
	      }

	    // Random checking.
	    template <typename Env, typename Rgen, typename Tgen>
	      auto operator()(Env& env, Rgen& range, Tgen& value) const
	        -> Requires<Random_variable<Rgen>() && Random_variable<Tgen>()>
	      {
	        operator()(env, range(), value());
	      }
	    
	    // Default random checking.
      template <typename Env>
	      void operator()(Env& env) const
	      {
	      	auto rgen = checkable_var<R>(env);
	      	auto tgen = checkable_var<T>(env);
	      	operator()(env, rgen, tgen);
	      }
		};



	// Count if (specification)
	// Check additional algorithms and properties affiliated with the count_if 
	// algorithm.
	template <typename R, typename P>
	  struct count_if_specs
	  {
	    static_assert(Range_query<R, P>(), "");

	    count_if_equiv<R, P> count_if;
	    count_ifnt_equiv<R, P> count_ifnt;

	    // The default predicate.
	    P pred;

	    count_if_specs(P p = {}) : pred(p) { }
	    
	    // Value checking.
	    template <typename Env>
	      void operator()(Env& env, const R& range, P pred) const
	      {
	      	check(env, count_if, range, pred);
	        check(env, count_ifnt, range, pred);
	      }

	    // Random checking.
	    template <typename Env, typename Rgen, typename Pgen>
	      auto operator()(Env& env, Rgen& range, Pgen& pred) const
	        -> Requires<Random_variable<Rgen>() && Random_variable<Pgen>()>
	      {
	        operator()(env, range(), pred());
	      }


	    // Default random checking.
      template <typename Env>
	      void operator()(Env& env) const
	      {
	      	auto pdist = single_value_distribution<P> {pred};
	      	auto pgen = checkable_var(env, pdist);
	      	auto rgen = checkable_var<R>(env);
	      	operator()(env, rgen, pgen);
	      }
	  };



	// Count if (specification)
	// Check additional algorithms and properties affiliated with the count_if 
	// algorithm.
	template <typename R, typename T = Value_type<R>>
	  struct count_specs
	  {
	    static_assert(Range_search<R, T>(), "");

	    count_eq_equiv<R, T> count_eq;
	    count_neq_equiv<R, T> count_neq;

	    // Value checking.
	    template <typename Env>
	      void operator()(Env& env, const R& range, const T& value) const
	      {
	      	check(env, count_eq, range, value);
	        check(env, count_neq, range, value);
	      }

	    // Random checking.
	    template <typename Env, typename Rgen, typename Tgen>
	      auto operator()(Env& env, Rgen& range, Tgen& value) const
	        -> Requires<Random_variable<Rgen>() && Random_variable<Tgen>()>
	      {
	        operator()(env, range(), value());
	      }


	    // Default random checking.
      template <typename Env>
	      void operator()(Env& env) const
	      {
	      	auto rgen = checkable_var<R>(env);
	      	auto tgen = checkable_var<T>(env);
	      	operator()(env, rgen, tgen);
	      }
	  };



	// Quantifiers (specification)
	template <typename R, typename P>
		struct quant_of_specs
		{
			static_assert(Range_query<R, P>(), "");

			// Positive queries in terms of find_if
			all_equiv_find<R, P> all_find;
			some_equiv_find<R, P> some_find;

			// Negations of positive queries.
			nall_neg_all<R, P> nall;
			none_neg_some<R, P> none;
			
			// Queries in terms of counting.
			all_equiv_count<R, P> all_count;
			some_equiv_count<R, P> some_count;
			nall_equiv_count<R, P> nall_count;
			none_equiv_count<R, P> none_count;

			// The default predicate.
			P pred;

			quant_of_specs(P p = {}) : pred(p) { }

			template <typename Env>
				void operator()(Env& env, const R& range, P pred) const
				{
					check(env, all_find, range, pred);
					check(env, some_find, range, pred);
					check(env, nall, range, pred);
					check(env, none, range, pred);
					check(env, all_count, range, pred);
					check(env, some_count, range, pred);
					check(env, nall_count, range, pred);
					check(env, none_count, range, pred);
				}

	    template <typename Env, typename Rgen, typename Pgen>
	      auto operator()(Env& env, Rgen& range, Pgen& pred) const
	        -> Requires<Random_variable<Rgen>() && Random_variable<Pgen>()>
				{
					operator()(env, range(), pred());
				}

			template <typename Env>
				void operator()(Env& env) const
				{
					auto pdist = single_value_distribution<P> {pred};
					auto pgen = checkable_var(env, pdist);
					auto rgen = checkable_var<R>(env);
					operator()(env, rgen, pgen);
				}
		};



} // namespace origin

#endif

