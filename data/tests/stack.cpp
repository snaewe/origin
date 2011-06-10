// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <iostream>

#include <origin/stack.hpp>

using namespace std;
using namespace origin;

// FIXME: Move this into a top-level testing framework.

// Some kind of vague resource
struct Resource { };

// This is just a test harness to study move semantics.
struct Moveable {
  Moveable()
    : rc{nullptr}
  { }
  
  Moveable(Resource* rc)
    : rc{rc}
  { }
  
  ~Moveable()
  {
    delete rc;
  }
  
  Moveable(Moveable const&) = delete;
  
  Moveable(Moveable&& x)
    : rc{x.rc}
  {
    x.rc = nullptr;
  }
  
  Resource* rc;
};

int main()
{
  Moveable x{new Resource{}};
  assert(( x.rc ));
  Moveable y{move(x)};
  assert(( y.rc && !x.rc ));
  
  {
    stack<Moveable> s;
    s.push(new Resource{});
    assert(( !s.empty() ));
    assert(( s.size() == 1));
    Moveable x = s.displace();
    assert(( x.rc && !s.top().rc ));
  }
}
