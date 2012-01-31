// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <iostream>
#include <vector>

#include <origin/algorithm.hpp>

using namespace std;
using namespace origin;


template<typename R>
  void print(const R& range)
  {
    for(auto x : range)
      cout << x << ' ';
    cout << '\n';
  }

  
template<typename S, typename T>
  void push(S&& seq, const T& value)
  {
    seq.push_back(value);
    push_d_heap<2>(seq);
  }
  
template<typename S>
  void pop(S&& seq)
  {
    pop_d_heap<2>(seq);
//     seq.pop_back();
  }
  
int main()
{
  vector<int> v;
  
  push(v, 54); print(v);
  push(v, 87); print(v);
  push(v, 27); print(v);
  push(v, 67); print(v);
  push(v, 19); print(v);
  push(v, 31); print(v);
  push(v, 29); print(v);
  
  pop(v); print(v);
}
