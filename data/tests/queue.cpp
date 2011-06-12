// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>

#include <origin/queue.hpp>

using namespace std;
using namespace origin;

// FIXME: Parameterize over the queue type?
template<typename T>
  void check_queue(std::initializer_list<T> list)
  {
    queue<T> q;
    assert(( q.empty() ));
    assert(( q.size() == 0 ));
    for(auto& x : list) {
      q.push(x);
      assert(( q.back() == x ));
    }
    assert(( q.size() == list.size() ));
    
    auto i = list.begin();
    while(!q.empty()) {
      assert(( q.front() == *i ));
      q.pop();
      ++i;
    }
  }

int main()
{
  check_queue({1, 2, 3, 4, 5});
}
