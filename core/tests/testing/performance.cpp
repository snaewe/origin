// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <vector>
#include <list>

#include <origin/functional.hpp>
#include <origin/testing.hpp>
#include <origin/testing/performance.hpp>

using namespace std;
using namespace origin;


template <typename R>
  void print(const R& range)
  {
    for (auto x : range)
      cout << x << ' ';
    cout << '\n';
  }


// Insert an element into the sequence so that it is sorted after insertion.
template <typename Seq, typename T>
  void insert_sorted(Seq& seq, const T& value)
  {
    auto i = find_if(seq, gte(value));
    seq.insert(i, value);
  }

template <typename Seq, typename Eng>
  void build_sorted(Seq& seq, Eng& eng, int size)
  {
    using Dist = uniform_int_distribution<int>;

    Dist dist(0, size);
    while (size != 0) {
      int n = dist(eng);
      insert_sorted(seq, n);
      --size;
    }
  }


// The timing environment
timing_environment<> env {time(0)};


// Test the construction of a sorted vector.
struct test_vector
{
  bool operator()(size_t n) const
  {
    vector<int> v;
    build_sorted(v, env.random_engine(), n);
    return true;
  }
};

// Test the construction of a sorted linked list.
struct test_list
{
  bool operator()(size_t n) const
  {
    list<int> l;
    build_sorted(l, env.random_engine(), n);
    return true;
  }
};


int main()
{
  // Burn a test so that everything is initialized up front.
  test_vector{}(5);

  constexpr int N = 4;

  // Test the construction of sorted vectors.
  {
    int p = 1;
    for(int n = 1; n <= N; ++n, p *= 10) {
      int i = p;
      for ( ; i < 10 * p; i += p)
        check(env, test_vector {}, i);
    }
    check(env, test_vector {}, p);
  }

  // Test the construction of sorted lists.
  {
    int p = 1;
    for(int n = 1; n <= N; ++n, p *= 10) {
      int i = p;
      for ( ; i < 10 * p; i += p)
        check(env, test_list {}, i);
    }
    check(env, test_list {}, p);
  }

  cout << typestr(chrono::system_clock::now()) << '\n';

}
