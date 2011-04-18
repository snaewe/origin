// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONCEPTS_ALGEBRAIC_HPP
#define ORIGIN_CONCEPTS_ALGEBRAIC_HPP

#include <origin/concepts/impl.hpp>
#include <origin/concepts/fwd.hpp>

namespace origin
{
  /**
   * @defgroup concepts_algebraic
   *
   * Algebraic concepts are constraints that define semantic relationships
   * between types, operations, and relations.
   */

  /**
   * @ingroup concepts_algebraic
   * A boolean algebra...
   */
  template<typename T,
           typename And,
           typename Or,
           typename Not,
           typename True,
           typename False>
  struct Boolean_Algebra
    : std::true_type
  { };

} // namespace origin

#endif
