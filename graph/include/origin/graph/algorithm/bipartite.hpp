// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_BIPARTITE_HPP
#define ORIGIN_GRAPH_ALGORITHM_BIPARTITE_HPP

#include <origin/graph/algorithm/search/breadth_first.hpp>
#include <origin/graph/algorithm/coloring/two_coloring.hpp>

namespace origin
{
  // FIXME: Write a coloring visitor that invokes a callback when a vertex
  // is assigned a color! Consider specialized visitors such as two-color
  // visitors, etc. Note that this probably applies to the coloring module
  // rahter than the search module.

  /**
   * @internal
   *
   * Implementation of the is_bipartite algorithm.
   *
   * @tparam Graph        An Outward_Graph type
   * @tparam Color_Label  A Vertex_Label associating vertices with a Two_Color,
   *                      minimally. Higher-valued color values may also be
   *                      used.
   */
  template<typename Graph, typename Color_Label>
  struct is_bipartite_impl
  {
    typedef Graph graph_type;
    typedef Color_Label color_label;

    typedef two_color_visitor<graph_type, color_label> visitor_type;

    // FIXME: The BFS uses the default color label for its internal state.
    // Is there any graceful way to override this? Yes... another color
    // label parameter at the top-level. This is actually quite easy to
    // implement.
    //
    // Why would you want to do this? Your graph's vertex type has a state
    // member that defines a bitfield, recording 3 bits of state for the color
    // in the underlying BFS and 1 bit of state for the two-coloring.
    //
    // FIXME: Would it be worthwhile to parameterize the search over the
    // the traversal?
    typedef bft_algo<graph_type, visitor_type> search_type;

    is_bipartite_impl(Graph& g, Color_Label label)
      : visitor(label), bfs(g, visitor)
    { }

    bool run()
    {
      try {
        bfs();
      } catch(...) {
        return false;
      }
      return true;
    }

    visitor_type visitor;
    search_type bfs;
  };

  /**
   * @ingroup graph_algo
   *
   * The is_bipartite algoirthm object determines if a graph is bipartite. A
   * bipartite graph can be colored using only two colors. Note that any graph
   * containing an odd cycle is not a biparatite graph.
   *
   * @tparam Graph        An Outward_Graph type
   * @tparam Color_Label  A Vertex_Label associating vertices with a Two_Color
   */
  //@{
  template<typename Graph, typename Color_Label = default_t>
  struct is_bipartite_algo
  {
    typedef Graph graph_type;
    typedef Color_Label color_label;

    typedef is_bipartite_impl<graph_type, color_label> impl_type;

    // Construct the algorithm with a custom two-color label.
    is_bipartite_algo(Graph& g, Color_Label label)
      : impl(g, label)
    { }

    bool operator()()
    { return impl.run(); }

    impl_type impl;
  };

  // Specialization for a default color map.
  template<typename Graph>
  struct is_bipartite_algo<Graph, default_t>
  {
    typedef Graph graph_type;
    typedef vertex_map<Graph, bool> color_map;
    typedef typename color_map::label_type color_label;

    typedef is_bipartite_impl<graph_type, color_label> impl_type;

    // Construct the algorithm with a custom two-color label.
    is_bipartite_algo(Graph& g)
      : colors(g), impl(g, colors.label)
    { }

    bool operator()()
    { return impl.run(); }

    color_map colors;
    impl_type impl;
  };
  //@}

  /**
   * @fn is_biparite(g)
   * @fn is_biparite(g, color)
   *
   * Return true if the graph is bipartite.
   *
   * @tparam Graph        An Outward_Graph type
   * @tparam Color_Label  If present, a Vertex_Label associating vertices with
   *                      a Two_Color (minimally).
   *
   * @param g       A Graph object
   * @param color   If specified, a Color_Label
   */
  //@{
  template<typename Graph>
  bool is_bipartite(Graph& g)
  {
    is_bipartite_algo<Graph> algo(g);
    return algo();
  }

  template<typename Graph>
  bool is_bipartite(Graph const& g)
  {
    is_bipartite_algo<Graph> algo(g);
    return algo();
  }
  //@}

  /**
   * @function is_bipartite(g, color)
   *
   * Return true if the graph is bipartite. Store the results of the mapping
   * in the given color mapping.
   */
  //@{
  template<typename Graph, typename Color_Label>
  bool is_bipartite(Graph& g, Color_Label label)
  {
    is_bipartite_algo<Graph, Color_Label> algo(g, label);
    return algo();
  }

  template<typename Graph, typename Color_Label>
  bool is_bipartite(Graph const& g, Color_Label label)
  {
    is_bipartite_algo<Graph const, Color_Label> algo(g, label);
    return algo();
  }
  //@}

  // HOWTO: How do I get the two-coloring of the graph when using is_bipartite
  // with an color label?
  //
  // Instead of using the is_bipartite algorithm, create an is_bipartite_algo
  // object and call it as a function. For example::
  //
  //    is_bipartite_algo<Graph> algo(g);
  //    algo();
  //    ... algo.color(v)
  //
  // The color member of the algorithm object is a label that desribes the
  // two-coloring.
  //
  // Interestingly, the algo.color is also effectively a Predicate on the
  // vertices of g that classifies them into the bipartite sets described by
  // the graph.


  // HOWTO: How do I access the color labels of the underlying breadth-first
  // search used by the is_bipartite algorithm?
  //
  // The breadth-first search used by the is_bipartite algorithm object is
  // stored as a member of the is_bipartite_algo object. It can be accessed
  // in the following manner::
  //
  //    is_bipartite<Graph> algo(g);
  //    algo();
  //    ... algo.bfs.color(v)
  //
  // The bfs member provides access to the underlying search object. It's
  // color member is a label that describes the states of vertices resulting
  // from the breadth first search.


  // HOWTO: Can I use a custom color label for the underlying breadth-first
  // search used by the is_bipartite algorith?
  //
  // FIXME: This is not currently implemented, but it shouldn't be terribly
  // difficult.


  // HOWTO: How do I find out which edge caused the graph to be non-bipartite?
  //
  // Use the two_coloring algorithm like so::
  //
  //    try {
  //      two_coloring(g, color);
  //    } catch(coloring_error& err) {
  //      // FIXME: You can't actually do anything here.
  //    }
  //
  // FIXME: This is the basic strategy, but it's incomplete snce the two
  // coloring algrithm loses all of its state when an exception is thrown.
  // The best answer is to probably rewrite the two-coloring algorithm to use
  // an algorithm object.

} // namespace origin

#endif

