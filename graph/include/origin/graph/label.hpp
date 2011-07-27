// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_LABEL_HPP
#define ORIGIN_GRAPH_LABEL_HPP

#include <unordered_map>

#include <origin/ordinal_map.hpp>
#include <origin/graph/traits.hpp>

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
      // The result type is the deduced result of applying the label.
      typedef typename std::result_of<Label(Arg)>::type result_type;
      
      // The value type is the underlying type accessed by the label. It is
      // not a reference or otherwise qualified.
      typedef typename std::decay<result_type>::type value_type;
    };

  // Depending on the type of key, choose a map data structure that optimally
  // implements the association. For ordinal types, this is an ordinal map.
  // For all other types, this is an unordered map (hash table).
  //
  // FIXME: This isn't quite right. We basically have to "partially" instantiate
  // the map and then rebind that instantiation with the supplied allocator.
  // Otherwise, the allocator won't be correct.
  template<typename Key, typename Value, typename Alloc>
    struct choose_graph_map
      : std::conditional<
          is_ordinal<Key>::value,
          ordinal_map<Key, Value>,
          std::unordered_map<Key, Value>
        >
    { };
    
  // The handle map defines an association between a graph handle (a vertex or
  // edge) and another set of values.
  //
  // FIXME: Finish implementing the the map interface.
  template<typename Key, typename Value, typename Alloc>
    class handle_map
    {
      typedef typename choose_graph_map<Key, Value, Alloc>::type map_type;
    public:
      typedef typename map_type::key_type key_type;
      typedef typename map_type::mapped_type mapped_type;
      typedef typename map_type::value_type value_type;
      typedef typename map_type::reference reference;
      typedef typename map_type::const_reference const_reference;
      typedef typename map_type::size_type size_type;
      
      typedef typename map_type::iterator iterator;
      typedef typename map_type::const_iterator const_iterator;

      handle_map()
        : data{}
      { }
      
      handle_map(size_type n)
        : data(n)
      { }
      
      // Properties
      bool empty() const
      {
        return data.empty();
      }
      
      size_type size() const
      {
        return data.size();
      }
      
      // Map operations
      iterator find(key_type const& k)
      {
        return data.find(k);
      }
      
      const_iterator find(key_type const& k) const
      {
        return data.find(k);
      }
      
      mapped_type& operator[](key_type const& k)
      {
        return data[k];
      }
      
    private:
      map_type data;    
    };
    
  /**
   * @ingroup label
   * 
   * The vertex_map defines an association between the vertices of a graph and
   * a set of values. 
   * 
   * The underlying implementation of the association is determined by the
   * properties of the Graph's vertex type. Ordinal vertex types result in
   * ordinal vertex maps. All other types default to hash tables.
   *
   * @tparam Graph    A Graph type
   * @tparam Value    An Object type
   * @tparam Alloc    An Allocator (unused for now)
   */
  template<typename Graph, typename Value, typename Alloc = std::allocator<Value>>
    class vertex_map 
      : private handle_map<typename Graph::const_vertex, Value, Alloc>
    {
      typedef handle_map<typename Graph::const_vertex, Value, Alloc> base_type;
    public:
      typedef typename base_type::key_type key_type;
      typedef typename base_type::mapped_type mapped_type;
      typedef typename base_type::value_type value_type;
      typedef typename base_type::reference reference;
      typedef typename base_type::const_reference const_reference;
      typedef typename base_type::iterator iterator;
      typedef typename base_type::const_iterator const_iterator;
      typedef typename base_type::size_type size_type;
      
      vertex_map()
        : base_type{}
      { }

      /**
       * @brief Bucket initialization
       * Initialize the map with n buckets.
       */
      explicit vertex_map(size_type n)
        : base_type(n)
      { }
      
      using base_type::empty;
      using base_type::size;
      using base_type::find;
      using base_type::operator[];
    };

  /**
   * @ingroup 
   * 
   * The edge_map defines an association between the edges of graph and a set
   * of values.
   * 
   * The underlying implementation of the association is determined by the
   * properties of the Graph's edge type. Ordinal edge types result in ordinal
   * edge maps. All other types default to hash tables.
   *
   * @tparam Graph    A Graph type
   * @tparam Value    An Object type
   * @tparam Alloc    An Allocator (unused for now)
   */
  template<typename Graph, typename Value, typename Alloc = std::allocator<Value>>
    class edge_map 
      : private handle_map<typename Graph::const_edge, Value, Alloc>
    {
      typedef handle_map<typename Graph::const_edge, Value, Alloc> base_type;
    public:
      typedef typename base_type::key_type key_type;
      typedef typename base_type::mapped_type mapped_type;
      typedef typename base_type::value_type value_type;
      typedef typename base_type::reference reference;
      typedef typename base_type::const_reference const_reference;
      typedef typename base_type::iterator iterator;
      typedef typename base_type::const_iterator const_iterator;
      typedef typename base_type::size_type size_type;

      edge_map()
        : base_type{}
      { }
      
      /**
       * @brief Bucket initialization
       * Initialize the map with n buckets.
       */
      explicit edge_map(size_type n)
        : base_type(n)
      { }
      
      using base_type::empty;
      using base_type::size;
      using base_type::find;
      using base_type::operator[];
    };
    
  /**
   * @ingroup label
   * 
   * The mapped label structure transforms a Map into an Input_label or
   * Mutable_label. The label takes an argument of the map's key type/
   */
  template<typename Map>
    struct map_label
    {
      typedef typename Map::key_type key_type;
      typedef typename Map::mapped_type mapped_type;

      map_label(Map& m)
        : map(map)
      { }
      
      mapped_type& operator()(key_type const& k) const
      {
        return map[k];
      }
      
      Map& map;
    };
    
  // A specialization for constant maps uses find instead of subscripting.
  template<typename Map>
    struct map_label<Map const>
    {
      typedef typename Map::key_type key_type;
      typedef typename Map::mapped_type mapped_type;

      map_label(Map const& m)
        : map(map)
      { }
      
      mapped_type const& operator()(key_type const& k) const
      {
        return *map.find(k);
      }
      
      Map const& map;
    };
    
  /**
   * @ingroup label
   * 
   * Construct a label over the given map.
   */
  template<typename Map>
    map_label<Map> label(Map& m)
    {
      return map_label<Map>{m};
    }
    
  template<typename Map>
    map_label<Map const> label(Map const& m)
    {
      return map_label<Map const>{m};
    }


} // namespace origin

#endif
