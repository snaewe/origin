// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>
#include <vector>
#include <set>

#include <origin/algorithm.hpp>
#include <origin/container.hpp>
#include <origin/testing.hpp>

using namespace std;
using namespace origin;


// This is a recursive formulation of the find_if algorithm. The purpose of
// using a recursive specification is to connect the algorithm with a more
// mathematical model of the computation. Since the underlying abstraction is
// a sequence, we can effectively describe the computation as a mathematical
// function.
//
// Although there are 3 branches through the list, we only need two values to
// fully test those paths: a sequence with an element satisfying pred, and one
// without.
template <typename I, typename P>
  I find_if_rec(I first, I last, P pred)
  {
    if (first == last) {
      return last;
    } else {
      if (pred(*first))
        return first;
      else
        return find_if_rec(next(first), last, pred);
    }
  }

// Range-based version of the algorithm above.
template <typename R, typename P>
  auto find_if_rec(R&& range, P pred) -> decltype(o_begin(range))
  {
    return find_if_rec(o_begin(range), o_end(range), pred);
  }



// Find if is equivalent to its recursive specification.
template <typename R, typename P>
  struct find_if_spec
  {
    P pred;

    find_if_spec(P pred) : pred(pred) { }

    bool operator()(const R& range) const
    {
      return find_if(range, pred) == find_if_rec(range, pred);
    }
  };



template <typename Env, typename R, typename P>
  void check_find_if(Env& env, const R& range, P pred)
  {
    find_if_spec<Forwarded<R>, P> spec;
    check(env, spec, range, pred);
  }


int main()
{
  std::minstd_rand eng(time(0));
  assert_checker<std::minstd_rand> env{eng};

  using V = vector<int>;
  using P = non_zero_pred<int>;
  find_if_spec<V, P> spec {P {}};

  // Non-random testing
  V v1 = {0, 0, 0, 0};
  V v2 = {0, 0, 0, 1};
  check(env, spec, v1);  
  check(env, spec, v2);  

  auto var = make_random<V>(eng);
  quick_check(env, spec, var);

}


