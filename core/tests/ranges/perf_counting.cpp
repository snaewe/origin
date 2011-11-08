// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>
#include <chrono>

#include <origin/range.hpp>

using namespace std;
using namespace std::chrono;
using namespace origin;

// This is a simple performance comparison test that sums over the 1st n
// numbers using a hand-built for-loop and the range() function for counting.

int main()
{
  constexpr int N = 1000000;
  long long t1, t2;

  {
    auto start = system_clock::now();

    double sum = 0;
    for(auto x : range(0, N))
      sum += rand() % 100;
    sum /= N;
    
    auto stop = system_clock::now();
    t1 = (stop - start).count();
  }

  {
    auto start = system_clock::now();

    double sum = 0;
    for(int i = 0; i < N; ++i)
      sum += rand() % 100;
    sum /= N;

    auto stop = system_clock::now();
    t2 = (stop - start).count();
  }
  cout << double(t1) / double(t2) << "\n";
}

