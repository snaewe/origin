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
   * @internal
   * A placeholder type that is used to indicate that the use of an internal
   * label.
   */
  template<typename Graph, typename Value>
  struct internal_label { };
  
  // FIXME: I really dislike the name of this type. It's not a property. It
  // wraps a lapel, possibly adding data to it.

  /**
   * The vertex property class provides a uniform interface for internally
   * and externally defined labels. For labels that refer to data stored
   * externally, this class simply wraps the label. For internal properties
   * a specialization also declares storage for the property.
   *
   * The label, the actual data accessor, is available via a member variable
   * named label.
   */
  template<typename Graph, typename Label>
  struct vertex_property
  {
    typedef Label label_type;
  
    // FIXME: This should be the same as decay<T>, I think. Basically, we're
    // just getting the value type with cv-qualifiers removed. Also note that
    // we're explicitly generating the result type over the non-const vertex
    // type. The const and non-const result types should only vary by a const
    // and maybe a reference, both of which are removed here.
    typedef typename std::remove_reference<
      typename std::remove_const<
        typename std::result_of<label_type(typename Graph::vertex)>::type
      >::type
    >::type value_type;
  
    // Explicitly suppress copy construction. This vertex need to be wrapped
    // with a functor for use as a real label.
    vertex_property(vertex_property const&) = delete;
    vertex_property& operator=(vertex_property const&) = delete;
  
    vertex_property(Label l)
      : label(l)
    { }
    
    label_type label;
  };
  
  // Specialization over internal labels. This actually owns the data that
  // the label references.
  template<typename Graph, typename Value>
  struct vertex_property<Graph, internal_label<Graph, Value>>
  {
    typedef typename graph_traits<Graph>::vertex vertex;
    typedef Value value_type;
    typedef Value& reference;
    typedef Value const& const_reference;
    
    // FIXME: Select an optional mapping type based on the graph kind. 
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
    vertex_property(vertex_property const&) = delete;
    vertex_property& operator=(vertex_property const&) = delete;
    
    vertex_property(Graph const& g)
      : data(g.order()), label(data)
    { }
    
    mapping_type data;
    label_type label;
  };


} // namespace origin

#endif
