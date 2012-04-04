
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

	// Find if results (property)
	template <typename R, typename P>
		struct find_if_results
		{
			bool operator()(const R& range, P pred) const
			{
				auto first = begin(range);
				auto last = end(range);
				auto i = find_if(first, last, pred);
				if (i != last)
					return pred(*i) && find_if(first, i, pred) == i;
				else
					return true;
			}
		};

	// Find if not (property)
	template <typename R, typename P>
	  struct find_if_not_equiv
	  {
	    bool operator()(const R& range, P pred) const
	    {
	      return find_if_not(range, pred) == find_if(range, negation(pred));
	    }
	  };



	// Find equivalence (property)
	template <typename R, typename T>
		struct find_equiv
		{
			bool operator()(const R& range, const T& value)
			{
				return find(range, value) == find_if(range, eq(value));
			}
		};


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
	  struct count_if_not_equiv
	  {
	    bool operator()(const R& range, P pred) const
	    {
	      return count_if_not(range, pred) == count_if(range, negation(pred));
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
	    find_if_not_equiv<R, P> find_if_not;
	    
	    // Check with specific values.
	    template <typename Env>
	      void operator()(Env& env, const R& range, P pred) const
	      {
	      	check(env, find_if, range, pred);
	        check(env, find_if_not, range, pred);
	      }

	    // Check randomly.
	    template <typename Env, typename Rgen, typename Pgen>
	      auto operator()(Env& env, Rgen& range, Pgen& pred) const
	        -> Requires<Random_variable<Rgen>() && Random_variable<Pgen>()>
	      {
	        operator()(env, range(), pred());
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
	    count_if_not_equiv<R, P> count_if_not;
	    
	    // Check with specific values.
	    // Note that the same value will be used for each check.
	    template <typename Env>
	      void operator()(Env& env, const R& range, P pred) const
	      {
	      	check(env, count_if, range, pred);
	        check(env, count_if_not, range, pred);
	      }

	    // Check randomly.
	    template <typename Env, typename Rgen, typename Pgen>
	      auto operator()(Env& env, Rgen& range, Pgen& pred) const
	        -> Requires<Random_variable<Rgen>() && Random_variable<Pgen>()>
	      {
	        operator()(env, range(), pred());
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
		};



	// Find if (check)
	// Check find_if against prototypical inputs: boolean sequences.
	struct find_if_check
	{
	  using V = vector<bool>;
	  using P = to_bool_function;

	  P pred;
	  V v0;
	  V v1;
	  V v2;

	  find_if_check() 
	    : pred()
	    , v0 {}     // Empty sequence
	    , v1 {0}    // No such element
	    , v2 {1, 1} // Returns the first such element
	  { }

	  // Check the default property
	  template <typename Env>
	    void operator()(Env& env) const
	    {
	      // An empty sequence has no such element.
	      check(env, eq(), find_if(v0, pred), end(v0));

	      // No such element exists in the sequence.
	      check(env, eq(), find_if(v1, pred), end(v1));

	      // Returns the first such element.
	      check(env, eq(), find_if(v2, pred), begin(v2));
	    }

	  // Test the given specification using these inputs.
	  template <typename Env, typename Spec>
	    void operator()(Env& env, const Spec& spec) const
	    {
	      spec(env, v0, pred);
	      spec(env, v1, pred);
	      spec(env, v2, pred);
	    }
	};


	// Check count_if against prototypical inputs: boolean sequences. This in
	// turn checks against the derived specifiations of find_if_not.
	struct count_if_check
	{
	  using V = vector<bool>;
	  using P = to_bool_function;

	  P pred;
	  V v0;
	  V v1;
	  V v2;

	  count_if_check() 
	    : pred()
	    , v0 {}     // Empty sequence
	    , v1 {0}    // No such element
	    , v2 {0, 1} // At least one element.
	  { }

	  // Check the default property
	  template <typename Env>
	    void operator()(Env& env) const
	    {
	      // An empty list has no matching elements.
	      check(env, eq(), count_if(v0, pred), 0);

	      // There are no matching elements.
	      check(env, eq(), count_if(v1, pred), 0);

	      // Returns the number of matching elements.
	      check(env, eq(), count_if(v2, pred), 1);
	    }

	  // Test the given specification using these inputs.
	  template <typename Env, typename Spec>
	    void operator()(Env& env, const Spec& spec) const
	    {
	      spec(env, v0, pred);
	      spec(env, v1, pred);
	      spec(env, v2, pred);
	    }
	};

} // namespace origin

#endif

