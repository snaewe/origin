// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <string>
#include <iostream>

#include <origin/utility/typestr.hpp>
#include <origin/concepts.hpp>

#include <boost/proto/proto.hpp>

#include "check.hpp"

using namespace std;
using namespace origin;
using namespace boost::proto;

// The purpose of this program is to test concepts against a DSEL, that I've
// chosen to write using Boost.Proto.

// FIXME: It would be even nicer to find domain-specific ET systems (e.g.,
// Eigen, or POOMA) rather than Proto's fully general DSEL framework. For
// example, if the domain only overloads mathematical operators, how can we
// validate things like equivalence? Will those expression trees combine under
// relational operators?

int main()
{
  terminal<bool>::type true_{true};
  terminal<bool>::type false_{false};

  // Yeah! I've found a model of types that are Boolean, but not Bool.
  check<Boolean<decltype(true_)>>(true);
  check<Bool<decltype(true_)>>(false);

}
