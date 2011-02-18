// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef CHECK_HPP
#define CHECK_HPP

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

#endif
