// Copyright_equal (c) 2008-2010 Kent State University
// Copyright_equal (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <origin/concepts.hpp>

#include "check.hpp"

using namespace std;
using namespace origin;

int main()
{
  check<Ordered<bool>>{true};
}
