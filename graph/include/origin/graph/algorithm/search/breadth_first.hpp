// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ALGORITHM_SEARCH_BREADTH_FIRST_HPP
#define ORIGIN_GRAPH_ALGORITHM_SEARCH_BREADTH_FIRST_HPP

#include <queue>
#include <unordered_map>

#include <origin/iterator/facades.hpp>

#include <origin/graph/color.hpp>
#include <origin/graph/label.hpp>
#include <origin/graph/edge.hpp>

namespace origin
{
  /**
   * @defgroup graph_bfs Breadth First Search
   */

  /**
   * @ingroup graph_bfs
   *
   * @note The visitor uses polymorphic visit functions to accomodate both
   * const and non-const visit instantiations. If the visiting algorithm is
   * instatiated over a const graph type, then the graph, vertex, and edge
   * parameters to each visit function will be const.
   */
  struct bfs_visitor
  {
    /**
     * Called after a vertex has been initialized.
     */
    template<typename Graph, typename Vertex>
    void initialized_vertex(Graph& g, Vertex v) { }

    /**
     * Called after a vertex has been discovered.
     */
    template<typename Graph, typename Vertex>
    void discovered_vertex(Graph& g, Vertex v) { }

    /**
     * Called after a vertex has been popped from the queue and before its
     * incident edges have been examined.
     */
    template<typename Graph, typename Vertex>
    void started_vertex(Graph& g, Vertex v) { }

    /**
     * Called after the vertex has been examined.
     */
    template<typename Graph, typename Vertex>
    void finished_vertex(Graph& g, Vertex v) { }

    /**
     * Called when a new search tree root is encountered. This is called
     * before the vertex is discovered, allowing derived visitors to perform
     * search-tree initialization before the discovery of the root vertex.
     */
    template<typename Graph, typename Vertex>
    void root_vertex(Graph& g, Vertex v) { }

    /**
     * Called before an incident edge is examined.
     */
    template<typename Graph, typename Edge>
    void started_edge(Graph& g, Edge e) { }

    /**
     * Called when an edge is determined to be in the search tree. Occurs
     * just before the target vertex is discovered.
     */
    template<typename Graph, typename Edge>
    void tree_edge(Graph& g, Edge e) { }

    /**
     * Called when an edge is determined to not be in the search tree.
     */
    template<typename Graph, typename Edge>
    void nontree_edge(Graph& g, Edge e) { }
  };

  /**
   * @ingroup graph_bfs
   * The breadth first visit algorithm object performs a breadth first
   * traversal on all vertices connected to a single starting vertex.
   *
   * @tparam Graph        A Graph type.
   * @tparam Visitor      A Search_Visitor type.
   * @tparam Color_Label  If given, a label for a coloring with at least three
   *                      values: white, black, and gray.
   */
  template<typename Graph,
           typename Visitor,
           typename Color_Label = internal_label<Graph, basic_color_t>>
  class bf_search_algo
  {
  public:
    typedef Visitor visitor_type;

    typedef Graph graph_type;
    typedef typename graph_traits<graph_type>::vertex vertex;
    typedef typename graph_traits<graph_type>::edge edge;

    typedef vertex_property<Graph, Color_Label> color_property;
    typedef color_traits<typename color_property::value_type> colors;

    typedef std::queue<vertex> search_queue;

    bf_search_algo(Graph& g, Visitor vis)
      : graph(g), visitor(vis), queue(), color(g)
    { }

    bf_search_algo(Graph& g, Visitor vis, Color_Label color)
      : graph(g), visitor(vis), queue(), color(color)
    { }

    void init()
    {
      for(auto v : graph.vertices()) {
        color.label(v) = colors::white();
        visitor.initialized_vertex(graph, v);
      }
    }

    // Perform search
    void operator()(vertex v)
    {
      color.label(v) = colors::gray();
      queue.push(v);
      visitor.root_vertex(graph, v);
      visitor.discovered_vertex(graph, v);

      while(!queue.empty()) {
        vertex u = queue.front();
        queue.pop();
        visitor.started_vertex(graph, u);

        for(edge e : out_edges(graph, u)) {
          visitor.started_edge(graph, e);
          vertex v = graph.target(e);

          if(color.label(v) == colors::white()) {
            color.label(v) = colors::gray();
            queue.push(v);
            visitor.tree_edge(graph, e);
            visitor.discovered_vertex(graph, v);
          } else {
            visitor.nontree_edge(graph, e);
          }
        }

        color.label(v) = colors::black();
        visitor.finished_vertex(graph, u);
      }
    }

    graph_type& graph;
    visitor_type visitor;
    search_queue queue;
    color_property color;
  };

  /**
   * @
   * The breadth first search algorithm performs a breadth first search on
   * the entire graph. All vertices are visited.
   */
  template<typename Graph,
           typename Visitor,
           typename Color_Label = internal_label<Graph, basic_color_t>>
  struct bf_traversal_algo
    : private bf_search_algo<Graph, Visitor, Color_Label>
  {
    typedef bf_search_algo<Graph, Visitor, Color_Label> base_type;

    typedef Visitor visitor_type;
    typedef Graph graph_type;
    typedef typename base_type::vertex vertex;
    typedef typename base_type::edge edge;
    typedef typename base_type::colors colors;

    bf_traversal_algo(Graph& g, Visitor vis)
      : base_type(g, vis)
    { }

    bf_traversal_algo(Graph& g, Visitor vis, Color_Label label)
      : base_type(g, vis, label)
    { }

    void operator()()
    {
      for(auto v : this->graph.vertices()) {
        if(this->color.label(v) == colors::white()) {
          base_type::operator()(v);
        }
      }
    }
  };

  // NOTE: Why are there overloads for const and non-const algorithms? Isn't
  // it always the case that algorithms don't modify the input? Generally, but
  // it's actually really easy to write labels that actually modify the graph
  // during execution (e.g., by storing values on a vertex or edge). Because
  // our graphs data structures are const-correct, you can't actually call an
  // algorithm taking a const graph with labels that modify the value.
  //
  // FIXME: This needs to be documented.

  // FIXME: Consider using enable_if to provide a single set of overloads
  // for breadth_first_search. I really only need to specialize on the three
  // parameter versions of bfs_from(g, v, vis) and bfs(g, vis, color). It
  // should be relatively easy.

  /**
   * @fn breadth_first_search(g, v, vis)
   * @fn breadth_first_search(g, v, vis, color)
   *
   * Perform a breadth-first search on the graph starting from the given
   * vertex and using the given visitor. The color label, if specified records
   * the states of vertices during * traversal.
   *
   * @tparam Graph        A Graph type.
   * @tparam Visitor      A Breadth_First_Visitor type.
   * @tparam Color_Label  A Read_Write_Label that maps vertices to a Color
   *                      type supporting at least three colors.
   *
   * @param g       A Graph object.
   * @param v       The vertex from which the search begins.
   * @param vis     A visitor.
   * @param color   A color label.
   */
  //@{
  template<typename Graph, typename Visitor>
  inline void
  breadth_first_search(Graph& g, typename Graph::vertex v, Visitor vis)
  {
    bf_search_algo<Graph, Visitor> algo(g, vis);
    algo(v);
  }

  // Const version of above.
  // FIXME: Should I be using graph_traits?
  template<typename Graph, typename Visitor>
  inline void
  breadth_first_search(Graph const& g,
                       typename Graph::const_vertex v,
                       Visitor vis)
  {
    bf_search_algo<Graph const, Visitor> algo(g, vis);
    algo(v);
  }

  // Color label version
  template<typename Graph, typename Visitor, typename Color_Label>
  inline void
  breadth_first_search(Graph& g,
                      typename Graph::vertex v,
                      Visitor vis,
                      Color_Label color)
  {
    bf_search_algo<Graph, Visitor, Color_Label> algo(g, vis, color);
    algo(v);
  }

  // Const version of above.
  template<typename Graph,
           typename Visitor,
           typename Color_Label>
  inline void
  breadth_first_search(Graph const& g,
                       typename Graph::const_vertex v,
                       Visitor vis,
                       Color_Label color)
  {
    bf_search_algo<Graph const, Visitor, Color_Label> algo(g, vis, color);
    algo(v);
  }
  //@}

  /**
   * @fn breadth_first_traverse(g, vis)
   * @fn breadth_first_traverse(g, vis, color)
   *
   * Perform a breadth-first traversal on the graph, visiting all vertices.
   * The color label, if specified, records the states of vertices during
   * traversal.
   *
   * @tparam Graph        A Graph type.
   * @tparam Visitor      A Breadth_First_Visitor type.
   * @tparam Color_Label  A Read_Write_Label that maps vertices to a Color
   *                      type supporting at least three colors.
   *
   * @param g       A Graph object.
   * @param v       The vertex from which the search begins.
   * @param vis     A visitor.
   * @param color   A color label.
   */
  //@{
  template<typename Graph, typename Visitor>
  inline void
  breadth_first_traverse(Graph& g, Visitor vis)
  {
    bf_traversal_algo<Graph, Visitor> algo(g, vis);
    algo();
  }

  // Const version of above.
  template<typename Graph, typename Visitor>
  inline void
  breadth_first_traverse(Graph const& g, Visitor vis)
  {
    bf_traversal_algo<Graph const, Visitor> algo(g, vis);
    algo();
  }

  // Color label variant
  template<typename Graph, typename Visitor, typename Color_Label>
  inline void
  breadth_first_traverse(Graph& g, Visitor vis, Color_Label color)
  {
    bf_traversal_algo<Graph, Visitor, Color_Label> algo(g, vis, color);
    algo();
  }

  // Const version of above.
  template<typename Graph, typename Visitor, typename Color_Label>
  inline void
  breadth_first_traverse(Graph const& g, Visitor vis, Color_Label color)
  {
    bf_traversal_algo<Graph const, Visitor, Color_Label> algo(g, vis, color);
    algo();
  }
  //@}

  // FIXME: There seem to be a lot of issues with this iterator... Namely that
  // it's only really easy to implement input iterators. Even the concept of
  // equality is extremely weak. Two iterator referring to the same state
  // are equivalent, but that's about it.

  // FIXME: This should be able applicable to DFS ranges also. It might be
  // worthwhile to rewrite this as just a search_iterator.

  /**
   * @internal
   * @ingroup graph_bfs
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

  // FIXME: Make Do a better job with access protection.

  // FIXME: These guys should also be parameterized over visitors. It would
  // probably be useful to do some of the same things with the range-based
  // traversal as with the standard algorithms.

  // NOTE: Range-based traversal should be slower than a basic algorithmic
  // traversal. Write some performance tests that confirm this and demonstrate
  // the difference in performance.

  /**
   * @internal
   * @ingroup graph_bfs
   * The breadth-first range class abstracts a rooted breadth-first search as a
   * range, allowing iteration.
   */
  template<typename Graph,
           typename Color_Label = internal_label<Graph, basic_color_t>>
  class bf_search_range
  {
    typedef bf_search_range<Graph, Color_Label> this_type;
  public:
    typedef Graph graph_type;
    typedef typename graph_traits<graph_type>::vertex vertex;
    typedef typename graph_traits<graph_type>::edge edge;

    typedef vertex_property<Graph, Color_Label> color_property;
    typedef color_traits<typename color_property::value_type> colors;

    typedef std::queue<vertex> search_queue;

    typedef bfs_iterator<this_type> iterator;

    bf_search_range(Graph& g, vertex v)
      : graph(g), current(v), color(g)
    { init(v); }

    bf_search_range(Graph& g, vertex v, Color_Label label)
      : graph(g), current(v), color(label)
    { init(v); }

    iterator begin()
    { return {this}; }

    iterator end() const
    { return {}; }

    /**
     * Return true if the search queue is empty.
     */
    bool empty() const
    { return queue.empty(); }

    /**
     * Initialize the traversal by marking all vertices as unvisited.
     */
    void init(vertex start)
    {
      for(auto v : graph.vertices()) {
        color.label(v) = colors::white();
      }
      search(start);
    }

    /**
     * Enqueue the given vertex so that it will be searched later.
     */
    void search(vertex v)
    {
      queue.push(v);
      color.label(v) = colors::gray();
    }

    /**
     * Move to the next vertex in the search buffer and search its incident
     * edges for undiscovered vertices.
     */
    void next()
    {
      current = queue.front();
      queue.pop();
      for(auto e : out_edges(graph, current)) {
        vertex v = graph.target(e);
        if(color.label(v) == colors::white()) {
          search(v);
        }
      }
      color.label(current) = colors::black();
    }

    Graph& graph;
    vertex current;
    search_queue queue;
    color_property color;
  };


  /**
   * @internal
   * @ingroup graph_bfs.
   * Constructs a traversable range over a graph. All vertices are visited.
   */
  template<typename Graph,
           typename Color_Label = internal_label<Graph, basic_color_t>>
  class bf_traversal_range
    : private bf_search_range<Graph, Color_Label>
  {
    typedef bf_search_range<Graph, Color_Label> base_type;
    typedef bf_traversal_range<Graph, Color_Label> this_type;
  public:
    typedef Graph graph_type;
    typedef typename graph_traits<graph_type>::vertex vertex;
    typedef typename graph_traits<graph_type>::edge edge;
    typedef typename graph_traits<graph_type>::vertex_iterator vertex_iterator;
    typedef typename base_type::colors colors;

    typedef bfs_iterator<this_type> iterator;

    // FIXME: This is not a valid constructor for empty graphs.
    bf_traversal_range(Graph& g)
      : base_type(g, *begin_vertex(g))
      , iter(begin_vertex(g))
      , last(end_vertex(g))
    { init(current); }

    bf_traversal_range(Graph& g, Color_Label color)
      : base_type(g, *begin_vertex(g), color)
      , iter(begin_vertex(g))
      , last(end_vertex(g))
    { this->init(current); }

    iterator begin()
    { return {this}; }

    iterator end()
    { return {}; }

    using base_type::empty;
    using base_type::init;
    using base_type::search;

    /**
     * Move to the next vertex in the search buffer and search its incident
     * edges for undiscovered vertices.
     */
    void next() {
      // Perform the standard BFS search activity.
      current = queue.front();
      queue.pop();
      for(auto e : out_edges(graph, current)) {
        vertex v = graph.target(e);
        if(color.label(v) == colors::white()) {
          search(v);
        }
      }
      color.label(current) = colors::black();

      // Move to the next undiscovered root.
      while(iter != last && color.label(*iter) != colors::white()) {
        ++iter;
      }

      // If we haven't reached the end, then we should queue up the vertex for
      // the next round of searching.
      if(iter != last) {
        search(*iter);
      }
    }

    using base_type::graph;
    using base_type::current;
    using base_type::queue;
    using base_type::color;

    vertex_iterator iter;  // Points to the current search tree root
    vertex_iterator last;   // PTE of the vertex range
  };

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
  inline bf_search_range<Graph>
  bfs(Graph& g, typename Graph::vertex v)
  { return {g, v}; }

  template<typename Graph>
  bf_search_range<Graph const>
  bfs(Graph const& g, typename Graph::const_vertex v)
  { return {g, v}; }

  template<typename Graph, typename Color_Label>
  inline bf_search_range<Graph, Color_Label>
  bfs(Graph& g, typename Graph::vertex v, Color_Label color)
  { return {g, v, color}; }

  template<typename Graph, typename Color_Label>
  inline bf_search_range<Graph const, Color_Label>
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
   * traversal.
   *
   * @tparam Graph        A Graph type.
   * @tparam Color_Label  A Read_Write_Label that maps vertices to a Color
   *                      type supporting at least three colors.
   *
   * @param g       A Graph object.
   * @param color   A color label.
   */
  //@{
  template<typename Graph>
  inline bf_traversal_range<Graph> bfs(Graph& g)
  { return {g}; }

  template<typename Graph>
  inline bf_traversal_range<Graph const> bfs(Graph const& g)
  { return {g}; }

  // Construct a complete BFS range with a custom color label. This collides
  // with bfs(g, v) and needs to be explicitly specialized.
  // FIXME: The correct specialization is to check that Color_Label is a
  // Read/Write label, not that it's not the same as the vertex type. Update
  // this to use a real concept check when I have real concepts.
  template<typename Graph, typename Color_Label>
  inline typename std::enable_if<
    !std::is_same<Color_Label, typename Graph::vertex>::value,
    bf_traversal_range<Graph, Color_Label>
  >::type
  bfs(Graph& g, Color_Label color)
  { return {g, color}; }

  template<typename Graph, typename Color_Label>
  inline typename std::enable_if<
    !std::is_same<Color_Label, typename Graph::const_vertex>::value,
    bf_traversal_range<Graph const, Color_Label>
  >::type
  bfs(Graph const& g, Color_Label color)
  { return {g, color}; }
  //@}

} // namespace origin

#endif