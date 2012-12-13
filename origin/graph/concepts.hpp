// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef GRAPH_CONCEPTS_HPP
#define GRAPH_CONCEPTS_HPP

#include <origin/type/concepts.hpp>

namespace origin
{
  // TODO: Lots and lots of traits for graphs!

  namespace graph_impl
  {
    template<typename G>
      struct get_out_edges
      {
      private:
        template<typename X>
          static auto check(const X& x) -> decltype(x.out_edges());

        static subst_failure check(...);
      public:
        using type = decltype(check(std::declval<G>()));
      };

  } // namsepace graph_impl

  // Returns true if g.out_edges() is a valid expression.
  template<typename G>
    constexpr bool Has_out_edges()
    {
      return Subst_succeeded<typename graph_impl::get_out_edges<G>::type>();
    }

  // Returns true if G is a directed graph.
  template<typename G>
    constexpr bool Directed_graph()
    {
      return Has_out_edges<G>();
    }

} // namespace origin

#endif
