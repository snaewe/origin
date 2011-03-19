// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_SEARCH_BREADTH_FIRST_RANGE_HPP
#define ORIGIN_GRAPH_ALGORITHM_SEARCH_BREADTH_FIRST_RANGE_HPP

#include <origin/iterator/facades.hpp>
#include <origin/graph/algorithm/search/breadth_first_common.hpp>

namespace origin
{
  // NOTE: There seem to be some issues with this iterator. I think that
  // it's only really easy to implement input iterators. Even the concept of
  // equality is extremely weak for these iterators. Two iterator referring to
  // the same range are equivalent, but that's about it. Since you can't
  // actually have two active BFS iterators, equality comparison probably
  // doesn't need to be defined too strictly.
  //
  // What if you have 2 BFS ranges? Can you compare iterators drawn from
  // different ranges? It's technically possible, but I'm not sure if its
  // sound. You don't compare iterators from different containers...

  // FIXME: This should be able applicable to DFS ranges also. It might be
  // worthwhile to rewrite this as just a search_iterator.

  /**
   * @internal
   * @ingroup graph_bfs
   *
   * The bfs_iterator provides an Input_Iterator abstraction for breadth-first
   * search ranges.
   */
  template<typename Range>
  class bfs_iterator
    : public input_iterator_facade<
        bfs_iterator<Range>,
        typename Range::vertex,
        typename Range::vertex,
        typename Range::vertex
      >
  {
  public:
    typedef typename Range::graph_type graph_type;
    typedef typename Range::vertex vertex;

    // Initialize the iterator to PTE
    bfs_iterator()
      : range_(0)
    { }

    // Initialze over a range. Must be non-null.
    bfs_iterator(Range* rng)
      : range_(rng)
    {
      assert(( range_ ));

      // Start by moving to the next (first) vertex.
      if(!range_->empty()) {
        range_->next();
      }
    }

    bool equal(bfs_iterator const& x) const
    { return range_ == x.range_; }

    vertex dereference() const
    {
      assert(( range_ ));
      return range_->current;
    }

    void increment()
    {
      // If we're already PTE or about to be PTE, make sure that we don't
      // move to a bad state.
      if(!range_) {
        return;
      } else if(range_->empty()) {
        range_ = nullptr;
        return;
      }

      // Update the state of the search.
      range_->next();
    }

  private:
    Range* range_;
  };

  // FIXME: These guys should also be parameterized over visitors. It would
  // probably be useful to do some of the same things with the range-based
  // traversal as with the standard algorithms.

  // NOTE: Range-based traversal should be slower than a basic algorithmic
  // traversal. Write some performance tests that confirm this and demonstrate
  // the difference in performance.

  /**
   * @internal
   * @ingroup graph_bfs
   *
   * Implementation of the BFS range.
   *
   * @tparam Graph        An Outward_Graph
   * @tparam Color_Label  If given, a Vertex_Label<Graph, Color>
   */
  template<typename Graph, typename Color_Label>
  class bfs_range_impl
  {
    typedef bfs_range_impl<Graph, Color_Label> this_type;
  public:
    typedef Graph graph_type;
    typedef typename graph_traits<graph_type>::vertex vertex;
    typedef typename graph_traits<graph_type>::edge edge;

    typedef Color_Label color_label;
    typedef typename label_traits<color_label, vertex>::value_type color_type;
    typedef origin::color_traits<color_type> color_traits;

    typedef std::queue<vertex> search_queue;

    typedef bfs_iterator<this_type> iterator;

    bfs_range_impl(Graph& g, vertex v, Color_Label label)
      : graph(g), current(v), color(label)
    { init(v); }

    // Return true if the search queue is empty.
    bool empty() const
    { return queue.empty(); }

    // Initialize the traversal by marking all vertices as unvisited.
    void init(vertex start)
    {
      for(auto v : graph.vertices()) {
        color(v) = color_traits::white();
      }
      search(start);
    }

    // Enqueue the given vertex so that it will be searched later.
    void search(vertex v)
    {
      queue.push(v);
      color(v) = color_traits::gray();
    }

    // Move to the next vertex in the search buffer and search its incident
    // edges for undiscovered vertices.
    void next()
    {
      current = queue.front();
      queue.pop();
      for(auto e : out_edges(graph, current)) {
        vertex v = graph.target(e);
        if(color(v) == color_traits::white()) {
          search(v);
        }
      }
      color(current) = color_traits::black();
    }

    Graph& graph;
    vertex current;
    search_queue queue;
    color_label color;
  };

  /**
   * @internal
   * @ingroup graph_bfs
   *
   * The breadth-first range class abstracts a rooted breadth-first search as a
   * range, allowing iteration via the range-based for loop.
   *
   * An optional color label may be specified as the template parameter of the
   * algorithm object. If given, this type must be a label function associating
   * each vertex in the graph with a Color value. If the label is ommitted,
   * then the algorithm object will internally allocate its own association
   * and label function.
   *
   * @tparam Graph        An Outward_Graph
   * @tparam Color_Label  If given, a Vertex_Label<Graph, Color>
   */
  template<typename Graph, typename Color_Label = default_t>
  class bfs_range
  {
    typedef typename graph_traits<Graph>::vertex vertex;
    typedef bfs_range_impl<Graph, Color_Label> range_impl;
  public:
    typedef bfs_iterator<range_impl> iterator;

    bfs_range(Graph& g, vertex v, Color_Label label)
      : impl(g, v, label)
    { }

    iterator begin()
    { return {&impl}; }

    iterator end() const
    { return {}; }

  private:
    range_impl impl;
  };

  // The default specialization of the search range implements an internal
  // color map.
  template<typename Graph>
  class bfs_range<Graph, default_t>
  {
    typedef typename graph_traits<Graph>::vertex vertex;

    typedef vertex_map<Graph, basic_color_t> color_map;
    typedef typename color_map::label_type color_label;

    typedef bfs_range_impl<Graph, color_label> range_impl;
  public:
    typedef bfs_iterator<range_impl> iterator;

    bfs_range(Graph& g, vertex v)
      : colors(g), impl(g, v, colors.label)
    { }

    iterator begin()
    { return {&impl}; }

    iterator end() const
    { return {}; }

  private:
    color_map colors;
    range_impl impl;
  };

  /**
   * @internal
   * @ingroup graph_bfs
   *
   * Implements the breadth-first traversal range.
   *
   * @tparam Graph        An Outward_Graph
   * @tparam Color_Label  A Vertex_Label<Graph, Color>
   */
  template<typename Graph, typename Color_Label>
  class bft_range_impl
    : private bfs_range_impl<Graph, Color_Label>
  {
    typedef bfs_range_impl<Graph, Color_Label> base_type;
    typedef bft_range_impl<Graph, Color_Label> this_type;
  public:
    typedef Graph graph_type;
    typedef typename graph_traits<graph_type>::vertex vertex;
    typedef typename graph_traits<graph_type>::edge edge;
    typedef typename graph_traits<graph_type>::vertex_iterator vertex_iterator;
    typedef typename base_type::color_traits color_traits;

    typedef bfs_iterator<this_type> iterator;

    // NOTE: This is not a valid constructor for empty graphs. I'd like
    // for this to yield an empty range if constructed over a null graph.
    bft_range_impl(Graph& g)
      : base_type(g, first_vertex(g))
      , iter(begin_vertex(g))
      , last(end_vertex(g))
    { init(current); }

    bft_range_impl(Graph& g, Color_Label color)
      : base_type(g, first_vertex(g), color)
      , iter(begin_vertex(g))
      , last(end_vertex(g))
    { this->init(current); }

    using base_type::empty;
    using base_type::init;
    using base_type::search;

    // Move to the next vertex in the search buffer and search its incident
    // edges for undiscovered vertices.
    void next() {
      // Perform the standard BFS search activity.
      current = queue.front();
      queue.pop();

      for(auto e : out_edges(graph, current)) {
        vertex v = graph.target(e);
        if(color(v) == color_traits::white()) {
          search(v);
        }
      }
      color(current) = color_traits::black();

      // Move to the next undiscovered root.
      while(iter != last && color(*iter) != color_traits::white()) {
        ++iter;
      }

      // If we haven't reached the end, then we should queue up the vertex for
      // the next round of searching.
      if(iter != last) {
        search(*iter);
      }
    }

  private:
    // Return either the first vertex or a default initialized vertex. Note
    // that default initialized vertices are required to be invalid. This
    // allows the range to be constructed on a null graph.
    vertex first_vertex(Graph& g)
    { return g.null() ? vertex() : *begin_vertex(g); }

  public:

    using base_type::graph;
    using base_type::current;
    using base_type::queue;
    using base_type::color;

    vertex_iterator iter;   // Points to the current search tree root
    vertex_iterator last;   // PTE of the vertex range
  };

  /**
   * @ingroup graph_bfs
   * @class bft_range<Graph, Color_Label>
   * @class bft_range<Graph>
   *
   * The breadth-first traveral range describes a breadth-first ordering of
   * the vertices of a graph. All vertices in the graph are visited by the
   * traversal.
   *
   * @tparam Graph        An Outward_Graph
   * @tparam Color_Label  A Vertex_Label<Graph, Color>
   */
  //@{
  template<typename Graph, typename Color_Label = default_t>
  class bft_range
  {
    typedef bft_range_impl<Graph, Color_Label> range_impl;
  public:
    typedef bfs_iterator<range_impl> iterator;

    bft_range(Graph& g)
      : impl(g)
    { }

    bft_range(Graph& g, Color_Label color)
      : impl(g, color)
    { }

    iterator begin()
    { return {&impl}; }

    iterator end()
    { return {}; }

  private:
    range_impl impl;
  };

  // Specialization for the default color map
  template<typename Graph>
  class bft_range<Graph, default_t>
  {
    typedef typename graph_traits<Graph>::vertex vertex;

    typedef vertex_map<Graph, basic_color_t> color_map;
    typedef typename color_map::label_type color_label;

    typedef bft_range_impl<Graph, color_label> range_impl;
  public:
    typedef bfs_iterator<range_impl> iterator;

    bft_range(Graph& g)
      : colors(g), impl(g, colors.label)
    { }

    iterator begin()
    { return {&impl}; }

    iterator end() const
    { return {}; }

  private:
    color_map colors;
    range_impl impl;
  };
  //@}

  // FIXME: Write overloads that accept a visitor for the bfs_range objects.

  /**
   * @fn bfs_from(g, v)
   * @fn bfs_from(g, v, color)
   *
   * Construct an iterable breadth-first search range on the graph, starting
   * from the given vertex. Only vertices in the same connected component are
   * visited by the search.
   *
   * The color label, if given, records the states of vertices during the
   * traversal.
   *
   * @tparam Graph        A Graph type.
   * @tparam Color_Label  A Read_Write_Label that maps vertices to a Color
   *                      type supporting at least three colors.
   *
   * @param g       A Graph object.
   * @param vis     A visitor.
   * @param color   A color label.
   */
  //@{
  template<typename Graph>
  inline bfs_range<Graph> bfs(Graph& g, typename Graph::vertex v)
  { return {g, v}; }

  template<typename Graph>
  bfs_range<Graph const> bfs(Graph const& g, typename Graph::const_vertex v)
  { return {g, v}; }

  template<typename Graph, typename Color_Label>
  inline bfs_range<Graph, Color_Label>
  bfs(Graph& g, typename Graph::vertex v, Color_Label color)
  { return {g, v, color}; }

  template<typename Graph, typename Color_Label>
  inline bfs_range<Graph const, Color_Label>
  bfs(Graph const& g, typename Graph::const_vertex v, Color_Label color)
  { return {g, v, color}; }
  //@}

  /**
   * @fn bfs(g)
   * @fn bfs(g, color)
   *
   * Construct an iterable breadth-first search range on the graph. All
   * vertices in the graph are visited.
   *
   * The color label, if given, records the states of vertices during the
   * traversal. The type of the color label must be Vertex_Label<Graph, Color>.
   *
   * @tparam Graph        An Outward_Graph type.
   * @tparam Color_Label  A Vertex_Label<Graph, Color>.
   *
   * @param g       A Graph object.
   * @param color   A color label.
   */
  //@{
  template<typename Graph>
  inline bft_range<Graph> bfs(Graph& g)
  { return {g}; }

  template<typename Graph>
  inline bft_range<Graph const> bfs(Graph const& g)
  { return {g}; }

  // Construct a complete BFS range with a custom color label. This collides
  // with bfs(g, v) and needs to be explicitly specialized.
  // FIXME: The correct specialization is to check that Color_Label is a
  // Read/Write label, not that it's not the same as the vertex type. Update
  // this to use a real concept check when I have real concepts.
  template<typename Graph, typename Color_Label>
  inline typename std::enable_if<
    !std::is_same<Color_Label, typename Graph::vertex>::value,
    bft_range<Graph, Color_Label>
  >::type
  bfs(Graph& g, Color_Label color)
  { return {g, color}; }

  template<typename Graph, typename Color_Label>
  inline typename std::enable_if<
    !std::is_same<Color_Label, typename Graph::const_vertex>::value,
    bft_range<Graph const, Color_Label>
  >::type
  bfs(Graph const& g, Color_Label color)
  { return {g, color}; }
  //@}

} // namespace origin

#endif