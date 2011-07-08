// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_CLAMPED_ACCUM
#define ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_CLAMPED_ACCUM

#include <functional>


namespace origin {

  /**
   * The edge_weight function object abstracts the weight operation on a
   * Weighted graph.
   * 
   * @tparam Graph  A Weighted Graph
   */
  template<typename Graph>
    struct edge_weight
    {
      // FIXME: Do I need to formally specify the result type? I think the
      // idiom to do so is a nested result struct that is partially specialized
      // over the different function signatures.

      
      // FIXME: This needs to result in a reference. Does it?
      auto operator()(Graph& g, typename Graph::edge e) 
        -> decltype(weight(g, e))
      {
        return weight(g, e);
      }
      
      auto operator()(Graph const& g, typename Graph::const_edge e) const
        -> const decltype(weight(g, e))
      {
        return weight(g, e);
      }
    };

  /**
   * A wrapper that limits that clamps the return value of an accumulator.
   *
   * @tparam Accumulator  An accumulator
   * 
   * @tparam Compare      An order on the value type.
   */
  template<typename Accumulator,
           typename Compare>
    struct clamped_accumulate
      : std::binary_function<typename Accumulator::first_argument_type,
                             typename Accumulator::second_argument_type,
                             typename Accumulator::result_type>
    {
    private:
      typedef typename Accumulator::first_argument_type val_type;
      typedef typename Accumulator::first_argument_type res_type;

    public:
      clamped_accumulate(Accumulator a, val_type m, Compare c = Compare())
        : accum(a), max(m), cmp(c)
      { }

      // Should this also prevent overflow?
      res_type operator()(val_type const& a, val_type const& b)
      {
        val_type t = accum(a,b);
        if(cmp(t, max))
          return t;
        else
          return max;
      }

      Accumulator accum;
      val_type max;
      Compare cmp;
    };

} // namespace origin

#endif // ORIGIN_GRAPH_ALGORITHM_SHORTEST_PATH_CLAMPED_ACCUM
