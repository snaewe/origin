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

template<typename S, typename T>
  void push(S& seq, const T& value)
  {
    seq.push_back(value);
    push_heap(seq);
  }
  
template<typename S>
  void pop(S& seq, S& result)
  {
    result.push_back(seq.front());
    pop_heap(seq);
    seq.pop_back();
  }

template<typename R>
  void print(const R& range)
  {
    for(auto x : range)
      cout << x << ' ';
    cout << '\n';
  }

int main()
{
  vector<int> v1;
  for(auto n : {1, 2, 3, 4, 5})
    push(v1, n);
  
  vector<int> v2;
  while(!v1.empty())
    pop(v1, v2);
  print(v2);
}


// FIXME: Test d-ary heaps.
#if 0

  
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
    seq.pop_back();
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
  pop(v); print(v);
  pop(v); print(v);
  pop(v); print(v);
  pop(v); print(v);
  pop(v); print(v);
}
#endif