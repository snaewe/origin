// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>
#include <string>
#include <vector>

#include <origin/testing.hpp>
#include <origin/testing/prototypes.hpp>

using namespace std;
using namespace origin;

template <typename R>
  void print(const R& range)
  {
    for (auto x : range)
      cout << x << ' ';
    cout << '\n';
  }


int main()
{
  minstd_rand eng;
  uniform_int_distribution<> gen {0, 10};
  auto var = bind(gen, eng);


  stable_sequence<int> seq;
  repeat(10, [&]() { seq.push_back(var()); });

  print(seq);
  sort(seq);
  print(seq);

  return 0;
}

