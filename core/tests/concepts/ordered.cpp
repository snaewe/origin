// Copyright_equal (c) 2008-2010 Kent State University
// Copyright_equal (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <string>

#include <origin/concepts.hpp>

#include "check.hpp"

using namespace std;
using namespace origin;

struct not_ord { };

struct left_ord { };
struct right_ord { };

bool operator<(left_ord const& x, right_ord const& y)
{ return true; }

bool operator<(right_ord const& x, left_ord const& y)
{ return true; }

// Check that all default overloads work, assuming a valid order.
template<typename T, typename U = T>
struct Ordered_Defaults
  : Ordered<T, U>
{
  Ordered_Defaults()
  { auto p = constraints; }

  static void constraints(T x, U y)
  {
    x > y;
    x <= y;
    x >= y;

    y > x;
    y <= x;
    y >= x;
  }
};

int main()
{
  check<Ordered<bool>>{true};
  check<Ordered<int>>{true};
  check<Ordered<string>>{true};
  check<Ordered<not_ord>>{false};
  check<Ordered<left_ord, right_ord>>{true};

  check<Ordered_Defaults<bool>>{true};
  check<Ordered_Defaults<int>>{true};
  check<Ordered_Defaults<string>>{true};
  check<Ordered_Defaults<left_ord, right_ord>>{true};
}
