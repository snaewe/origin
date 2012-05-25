// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_LABEL_HPP
#define ORIGIN_GRAPH_LABEL_HPP



#include <vector>

#include <origin/ordinal_map.hpp>
#include <origin/graph/traits.hpp>
#include <origin/graph/vertex.hpp>
#include <origin/graph/edge.hpp>



// FIXMEs
//  - consider other desings for label, label_wrapper and label refernce count
//  - weight queue does not belong here

// This file defines graph labels. In the origin graph library, vertices are
// required to model ordinal. Meaning they map to integral values and the
// mapping is an injective function (mapping is unique). This allows origin to
// provide simple mappings of data onto vertices in the form of Labelings.

// As stated above, a labeling is a unique association of vertices to data.
// Because of function call style interface, everything from user data, weight,
// color, and even graph metrics like vertex degree can be expressed as a
// labeling.

namespace origin
{
  template <typename T, typename Label_type>
    class labeling
    {
      // static_assert(Ordinal<T>() , "");

      using size_type = std::size_t;
    public:
      // Types
      using key_type = T;
      using value_type = Label_type;

      // Initializers
      labeling(size_type n, const Label_type & x = Label_type()) : map (n,x) { }
      labeling(const labeling & l) : map (l.map) { }
      labeling(labeling && l) : map (/*move*/ l.map) { }
      template <typename Iter>
        labeling(Iter f, Iter l) : map (f,l) { }
      // FIXME range-based intializer

      // Accessors
      // FIXME add safe accessor support?
      inline Label_type & operator() (T t) { return map[ord(t)]; }
      inline const Label_type & operator() (T t) const  { return map[ord(t)]; }

    private:
      std::vector<Label_type> map;
    };

  // FIXME move this into a better position
  template <typename T>
    using Key_type = typename T::key_type;


/*============================================================================*/
  // Helper functions for labels

  template <typename G, typename T>
    labeling <typename G::vertex, T>
    label_vertices (const G & g, const T& t = T())
    { return labeling <typename G::vertex, T>(g.order(), t); }

  template <typename G, typename T>
    labeling <typename G::edge, T>
    label_edges (const G & g, const T & t = T())
    { return labeling <typename G::edge, T>(g.size(), t); }



  // Helper definitions for a weight queue
  // FIXME this really needs better treatment
  template <typename L, typename Relation>
    struct label_relation
    {
      label_relation(const L & l, Relation r)
        : l_(l), r_(r)
      { }

      bool operator()(Key_type<L> a, Key_type<L> b) const
      { return r_(l_(a),l_(b)); }

      const L & l_;
      Relation r_;
    };

  template <typename Weight_label>
    mutable_binary_heap<
      Key_type<Weight_label>,
      label_relation <Weight_label, std::greater <Value_type <Weight_label>>>
    >
    make_weight_queue(const Weight_label & w)
    {
      using weight_compare =
        label_relation <Weight_label, std::greater <Value_type <Weight_label>>>;

      return
        mutable_binary_heap<Key_type<Weight_label>, weight_compare>
          (weight_compare(w, std::greater<Value_type<Weight_label>>()));
    }


#if 0
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
          Ordinal<Key>(),
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
        : map(m)
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
        : map(m)
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
#endif
/*============================================================================*/
  template <typename T>
    class vertex_labeling
    {
      using size_type = std::size_t;
      using vertex = vertex_handle<size_type>;
      using map_type = std::vector<T>;
      using ref = typename map_type::reference;
      using const_ref = typename map_type::const_reference;
    public:
      // Types
      using value_type = T;


      // Initializers
      vertex_labeling(size_type n, const_ref x = T()) : map_(n, x) { }
      template <typename Iterator>
      vertex_labeling(Iterator f, Iterator l) : map_(f, l) { }
      /*template<typename Range>
      vertex_labeling(Range rng) : map_(begin(rng), rng.end()) { }*/
      vertex_labeling(vertex_labeling const& l) : map_(l.map_) { }

      // Accessors
      ref operator() (vertex v) { return map_[v.value()]; }
      const_ref operator() (vertex v) const { return map_[v.value()]; }

    private:
      map_type map_;
    };
  template <typename T>
    class edge_labeling
    {
      using size_type = std::size_t;
      using edge = undirected_edge_handle<size_type>;
      using map_type = std::vector<T>;
      using ref = typename map_type::reference;
      using const_ref = typename map_type::const_reference;
    public:
      // Types
      using value_type = T;


      // Initializers
      edge_labeling(size_type n, const_ref x = T()) : map_(n, x) { }
      template <typename Iterator>
      edge_labeling(Iterator f, Iterator l) : map_(f, l) { }
      /*template<typename Range>
      edge_labeling(Range rng) : map_(rng.begin(), rng.end()) { }*/
      edge_labeling(edge_labeling const& l) : map_(l.map_) { }

      // Accessors
      ref operator() (edge e) { return map_[e.edge.value()]; }
      const_ref operator() (edge e) const { return map_[e.edge.value()]; }

    private:
      map_type map_;
    };

  template <typename G, typename T>
    vertex_labeling <T>
    make_vertex_labeling(const G & g, const T& x = T())
    { return vertex_labeling<T>(g.order(), x); }

  template <typename G, typename T>
    edge_labeling <T>
    make_edge_labeling(const G & g, const T& x = T())
    { return edge_labeling<T>(g.size(), x); }

} // namespace origin

#endif
