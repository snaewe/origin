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
   * Clamps the return value of an accumulator to max.
   *
   * @tparam Graph  A Graph
   * 
   * @tparam Weight_Label   A Readable edge Label that assocates a weight
   * with every edge.
   * 
   * @tparam Weight_Accum   An associative (commutative?) binary operation 
   * that accumulates edge weights.
   * 
   * @tparam Weight_Comp    A Strict_Weak_Order over the graph's edge weights
   *
   * @tparam Distance_Label A Writable vertex Label that records the cumulative
   * edge weight from a start vertex to every other vertex.
   * 
   * 
   * @tparam Visitor        A Dijkstra_Visitor
   *
   * TODO Put in static asserts!
   */
  template<typename Accumulator
           typename Compare = std::less<Accumulator::first_argument_type>>
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
