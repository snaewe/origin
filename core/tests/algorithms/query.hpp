
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



  
  // FIXME: Write specs for find_next_if, find_nth_if.


	// Count if (property)
  // The semantics of count if can be specified in terms of a reduction.
  template <typename R, typename P>
	  struct count_if_equiv
	  {
	  	bool operator()(const R& range, P pred) const
	  	{
	  		auto f = [pred](const Value_type<R>& x, Distance_type<R> n)
	  		{
	  			return (pred(x)) ? n + 1 : n;
	  		};

	  		return count_if(range, pred) == reduce(range, 0, f);
	  	}
	  };


	// Count if not (property)
	template <typename R, typename P>
	  struct count_ifnt_equiv
	  {
	    bool operator()(const R& range, P pred) const
	    {
	      return count_if_not(range, pred) == count_if(range, negation(pred));
	    }
	  };



  // Count equal (property)
	template <typename R, typename T>
	  struct count_eq_equiv
	  {
	  	bool operator()(const R& range, const T& value) const
	  	{
	  		return count(range, value) == count_if(range, eq(value));
	  	}
	  };



  // Count not equal (property)
	template <typename R, typename T>
	  struct count_neq_equiv
	  {
	  	bool operator()(const R& range, const T& value) const
	  	{
	  		return count_not_equal(range, value) == count_if_not(range, eq(value));
	  	}
	  };



	// All of/find if (property)
	template <typename R, typename P>
		struct all_equiv_find
		{
			bool operator()(const R& range, P pred) const
			{
				return all_of(range, pred) == (find_if_not(range, pred) == end(range));
			}
		};



	// Some of/find if (property)
	template <typename R, typename P>
		struct some_equiv_find
		{
			bool operator()(const R& range, P pred) const
			{
				return some_of(range, pred) == (find_if(range, pred) != end(range));
			}
		};



	// Not all of (property)
	template <typename R, typename P>
		struct nall_neg_all
		{
			bool operator()(const R& range, P pred) const
			{
				return not_all_of(range, pred) == !all_of(range, pred);
			}
		};



	// None of (property)
	template <typename R, typename P>
		struct none_neg_some
		{
			bool operator()(const R& range, P pred) const
			{
				return none_of(range, pred) == !some_of(range, pred);
			}
		};



	// All of/count if (property)
	template <typename R, typename P>
		struct all_equiv_count
		{
			bool operator()(const R& range, P pred) const
			{
				return all_of(range, pred) == (count_if(range, pred) == distance(range));
			}
		};



	// Some of/count if (property)
	template <typename R, typename P>
		struct some_equiv_count
		{
			bool operator()(const R& range, P pred) const
			{
				return some_of(range, pred) == (count_if(range, pred) != 0);
			}
		};



	// Not all of/count if (property)
	template <typename R, typename P>
		struct nall_equiv_count
		{
			bool operator()(const R& range, P pred) const
			{
				return not_all_of(range, pred) == (count_if(range, pred) != distance(range));
			}
		};



	// None of/count if (property)
	template <typename R, typename P>
		struct none_equiv_count
		{
			bool operator()(const R& range, P pred) const
			{
				return none_of(range, pred) == (count_if(range, pred) == 0);
			}
		};



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

