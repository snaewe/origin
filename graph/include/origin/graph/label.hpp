// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_LABEL_HPP
#define ORIGIN_GRAPH_LABEL_HPP

/**
 * @defgroup graph_label
 * Types and traits related to the definition and use of labels. A label is
 * a function maps vertices and edges to a value type, supporting both reading
 * and writing.
 */

namespace origin
{
  /**
   * The label traits class provides access to the result type of the label
   * function and its associated value type. The value type is typically the
   * result type with any reference qualifier removed.
   *
   * @tparam Label    A function moeling Callable<Label, Arg>
   * @tparam Arg      A graph vertex or edge handle
   */
  template<typename Label, typename Arg>
  struct label_traits
  {
    typedef typename std::result_of<Label(Arg)>::type result_type;
    typedef typename std::remove_reference<result_type>::type value_type;
  };


  /**
   * The vertex map implements an association between a graph's vertices
   * and some value type. A label function, accessible via the label member
   * variable, provides access to the map.
   *
   * Note that vertex maps are not copyable.
   *
   * @tparam Graph    A Graph type
   * @tparam Value    An Object type
   */
  template<typename Graph, typename Value>
  struct vertex_map
  {
    typedef typename graph_traits<Graph>::vertex vertex;
    typedef Value value_type;
    typedef Value& reference;
    typedef Value const& const_reference;

    // FIXME: Select an optimal mapping type based on the graph kind.
    typedef std::unordered_map<vertex, value_type> mapping_type;

    // The label is a function object that abstracts access to the data.
    struct label_type
    {
      label_type(mapping_type& map)
        : map(map)
      { }

      reference operator()(vertex v)
      { return map[v]; }

      const_reference operator()(vertex v) const
      { return map[v]; }

      mapping_type& map;
    };

    // Explicitly suppress copy construction. This vertex need to be wrapped
    // with a functor for use as a real label.
    vertex_map(vertex_map const&) = delete;
    vertex_map& operator=(vertex_map const&) = delete;

    vertex_map(Graph const& g)
      : data(g.order()), label(data)
    { }

    mapping_type data;
    label_type label;
  };


} // namespace origin

#endif
