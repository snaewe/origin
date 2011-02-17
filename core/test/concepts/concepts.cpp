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

using namespace std;
using namespace origin;

// Failable archetypes
struct not_equal { };
struct not_ordered { };

struct not_default { not_default() = delete; };
struct not_copyable { not_copyable(not_copyable const&) = delete; };

/**
 * The check class is used to evaluate both the compile-time and run-time
 * checks of a concept. The boolean value passed to the constructor specifies
 * the test frameworks expectation about the validity of the model.
 *
 * This framework is also valid for testing type traits. Note that a type
 * predicates. A type predicate (boolean metafunction) is a concept that the
 * does not enforce any static assertions when instantiated. Type traits also
 * have weaker guarantees about never generating compiler errors.
 */
template<typename Model, bool valid = Model::value>
struct check
{
  check(bool result)
  { assert(( result == false )); }
};

template<typename Model>
struct check<Model, true>
{
  check(bool result)
  {
    Model{};
    assert(( result == true ));
  }
};


int main()
{
  // FIXME: Test exhuastively. I should Make sure that these are valid for all
  // builtin types, standard types, containers, etc.

  // Equality tests
  {
    check<Equal<bool>>{true};
    check<Equal<string>>{true};
    check<Equal<not_equal>>{false};

    // Spot-checking with axioms
    assert(( Equal<int>::axioms::Reflexive(1) ));
    assert(( Equal<int>::axioms::Symmetric(1, 2) ));
    assert(( Equal<int>::axioms::Transitive(1, 1, 1) ));
  }

  // Order tests
  {
    check<Ordered<bool>>{true};
    check<Ordered<int>>{true};
    check<Ordered<string>>{true};
    check<Ordered<not_ordered>>{false};
  }

  // Constructability
  {
    check<Default<int>>{true};
    check<Default<string>>{true};
    check<Default<not_default>>{false};

    check<Copyable<int>>{true};
    check<Copyable<string>>{true};
    check<Copyable<not_copyable>>{false};
  }

  // Regular types
  {
    check<Regular<int>>{true};
    check<Regular<string>>{true};

    check<Regular<not_default>>{false};
    check<Regular<not_copyable>>{false};
  }
}
