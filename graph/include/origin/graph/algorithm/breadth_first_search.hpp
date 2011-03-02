// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_BREADTH_FIRST_SEARCH_HPP
#define ORIGIN_GRAPH_BREADTH_FIRST_SEARCH_HPP

#include <queue>
#include <unordered_map>

#include <origin/iterator/facades.hpp>

#include <origin/graph/color.hpp>
#include <origin/graph/label.hpp>
#include <origin/graph/edge.hpp>

namespace origin
{
  // What do users want to get from a BFS?
  //  - a vertex fortest (links between vertices)
  //  - an edge forest (links between edges)
  //  - distance from root
  //  - some subset of the above
  //  - just a traversal order
  // The first four are trivially solved with the visitor pattern and some
  // clever algorithmic specifications. 
  //
  // The last, we should expose as a range whose iterators yield either vertices
  // or edges. I'm not sure what the syntax should look like for this yet. What
  // about:
  // 
  //    bfs<vertex>(g, [v])
  //    bfs<edge>(g, [v])
  //
  // where vertex/edge selects the result type of the iterator? 

  // What are the use cases for providing your own color label?
  //
  // - You want the colors to persist beyond the algorithm
  // - You want to modify the colors
  // - The user wants to external data to denote color (e.g., a data member)
  //
  // In general, the first two requirements are generally solved by the
  // current implemention. The latter, however, is not. In fact, I think it's
  // the only motivating argument for supporting parameterization over a color
  // map.
    
  // FIXME: Should visitor functions be postfixed with _vertex and _edge?
  // I don't think so, but others certainly will...
  //
  // If the visitor uses template visit functions (a la BGL), then we have
  // to append _vertex and _edge. If the visitor does not (as per the current
  // implementation), then we don't need to.
  //
  // Question: is it better to let the graph and visitor type "bubble" up to
  // the BGL or to "fix" those types before calling the function. I personally
  // prefer the latter, but I can see arguments for the former.
  
  // FIXME: I don't like the visit/search distinction, here. They're both
  // searches, its just that one visits all vertices, and the other is rooted
  // and a single vertex. Other viable name pairs are:
  //
  //   - search, search_all
  //   - search_from, search

  /**
   * @ingroup graph_bfs
   *
   * @note The visitor uses polymorphic visit functions to accomodate both
   * const and non-const visit strategies. If the visiting algorithm is 
   * instatiated over a constant graph type, then the graph, vertex, and
   * edge parameters will also be constant.
   */
  struct bfs_visitor
  {
    // Called after a vertex has been initialized
    template<typename Graph, typename Vertex>
    void initialized_vertex(Graph& g, Vertex v) { }
    
    // Called after a vertex has been discovered
    template<typename Graph, typename Vertex>
    void discovered_vertex(Graph& g, Vertex v) { }
    
    // Called after a vertex has been popped from the queue and before its
    // incident edges have been examined.
    template<typename Graph, typename Vertex>
    void started_vertex(Graph& g, Vertex v) { }
    
    // Called after the vertex has been examined.
    template<typename Graph, typename Vertex>
    void finished_vertex(Graph& g, Vertex v) { }
    
    // Called when a discovered vertex is the root of a search tree in the
    // search forest.
    template<typename Graph, typename Vertex>
    void root_vertex(Graph& g, Vertex v) { }
    
    // Called before an incident edge is examined.
    template<typename Graph, typename Edge>
    void started_edge(Graph& g, Edge e) { }

    // Called when an edge is determined to be in the search tree. Occurs 
    // just before the target vertex is discovered.
    template<typename Graph, typename Edge>
    void tree_edge(Graph& g, Edge e) { }
    
    // Called when an edge is determined to not be in the search tree.
    template<typename Graph, typename Edge>
    void nontree_edge(Graph& g, Edge e) { }
  };
  
  /**
   * The breadth first visit algorithm object performs a breadth first 
   * traversal on all vertices connected to a single starting vertex.
   *
   *
   * @tparam Color_Label If given, a label for a coloring with at least three
   *         values: white, black, and gray.
   */
  template<typename Graph, 
           typename Visitor,
           typename Color_Label = internal_label<Graph, basic_color_t>>
  class rooted_bfs_algo
  {
  public:
    typedef Visitor visitor_type;

    typedef Graph graph_type;
    typedef typename graph_traits<graph_type>::vertex vertex;
    typedef typename graph_traits<graph_type>::edge edge;

    typedef vertex_label<Graph, Color_Label> color_label;
    typedef color_traits<typename color_label::value_type> colors;

    typedef std::queue<vertex> search_queue;
    
    rooted_bfs_algo(Graph& g, Visitor vis)
      : graph(g), visitor(vis), queue(), color(g)
    { }
    
    rooted_bfs_algo(Graph& g, Visitor vis, Color_Label color)
      : graph(g), visitor(vis), queue(), color(color)
    { }

    void init()
    {
      for(auto v : graph.vertices()) {
        color(v) = colors::white();
        visitor.initialized_vertex(graph, v);
      }
    }
    
    // Perform search
    void operator()(vertex v)
    {
      color(v) = colors::gray();
      queue.push(v);
      visitor.discovered_vertex(graph, v);
      visitor.root_vertex(graph, v);

      while(!queue.empty()) {
        vertex u = queue.front();
        queue.pop();
        visitor.started_vertex(graph, u);
        
        for(edge e : out_edges(graph, u)) {
          visitor.started_edge(graph, e);
          vertex v = graph.target(e);
          
          if(color(v) == colors::white()) {
            color(v) = colors::gray();
            queue.push(v);
            visitor.tree_edge(graph, e);
            visitor.discovered_vertex(graph, v);
          } else {
            visitor.nontree_edge(graph, e);
          }
        }
        
        color(v) = colors::black();
        visitor.finished_vertex(graph, u);
      }
    }
    
    graph_type& graph;
    visitor_type visitor;
    search_queue queue;
    color_label color;    
  };
  
  /**
   * The breadth first search algorithm performs a breadth first search on
   * the entire graph. All vertices are visited.
   */
  template<typename Graph, 
           typename Visitor,
           typename Color_Label = internal_label<Graph, basic_color_t>>
  struct bfs_algo 
    : private rooted_bfs_algo<Graph, Visitor, Color_Label>
  {
    typedef rooted_bfs_algo<Graph, Visitor, Color_Label> base_type;
    
    typedef Visitor visitor_type;
    typedef Graph graph_type;
    typedef typename base_type::type vertex;
    typedef typename base_type::type edge;
    typedef typename base_type::colors colors;
    
    bfs_algo(Graph& g, Visitor vis)
      : base_type(g, vis)
    { }

    bfs_algo(Graph& g, Visitor vis, Color_Label label)
      : base_type(g, vis, label)
    { }

    void operator()()
    {
      for(auto v : this->graph.vertices()) {
        if(this->color(v) == colors::white()) {
          this->search(v);
        }
      }
    }
  };
  
  /**
   * Perform a breadth first search on the graph starting from the given
   * vertex. This is an overloaded name.
   */
  //@{
  template<typename Graph, typename Vertex, typename Visitor>
  void breadth_first_search_from(Graph& g, Vertex v, Visitor vis)
  {
    rooted_bfs_algo<Graph, Visitor> algo(g, vis);
    algo(v);
  }

  // Const version of above.
  template<typename Graph, typename Vertex, typename Visitor>
  void breadth_first_search_from(Graph const& g, Vertex v, Visitor vis)
  {
    rooted_bfs_algo<Graph const, Visitor> algo(g, vis);
    algo(v);
  }
  
  // This variant takes an additional color label parameter.
  template<typename Graph, 
           typename Vertex, 
           typename Visitor, 
           typename Color_Label>
  void breadth_first_search_from(Graph& g, 
                                 Vertex v, 
                                 Visitor vis,
                                 Color_Label color)
  {
    rooted_bfs_algo<Graph, Visitor, Color_Label> algo(g, vis, color);
    algo(v);
  }

  // Const version of above.
  template<typename Graph, 
           typename Vertex, 
           typename Visitor, 
           typename Color_Label>
  void breadth_first_search_from(Graph const& g, 
                                 Vertex v, 
                                 Visitor vis,
                                 Color_Label color)
  {
    rooted_bfs_algo<Graph const, Visitor, Color_Label> algo(g, vis, color);
    algo(v);
  }
  //@}

  /**
   * Perform a breadth first search on the graph. All vertices in the graph
   * are visited.
   */
  //@{
  template<typename Graph, typename Visitor>
  void breadth_first_search(Graph& g, Visitor vis)
  {
    bfs_algo<Graph, Visitor> algo(g, vis);
    algo();
  }

  // Const version of above.
  template<typename Graph, typename Visitor>
  void breadth_first_search(Graph const& g, Visitor vis)
  {
    bfs_algo<Graph const, Visitor> algo(g, vis);
    algo();
  }
  
  // This variant takes an additional color label parameter.
  template<typename Graph, typename Visitor, typename Color_Label>
  void breadth_first_search(Graph& g, Visitor vis, Color_Label color)
  {
    bfs_algo<Graph, Visitor, Color_Label> algo(g, vis, color);
    algo();
  }

  // Const version of above.
  template<typename Graph, typename Visitor, typename Color_Label>
  void breadth_first_search(Graph const& g, Visitor vis, Color_Label color)
  {
    bfs_algo<Graph const, Visitor, Color_Label> algo(g, vis, color);
    algo();
  }
  //@}
  
  // FIXME: This should be able to work for DFS ranges also.
  // FIXME: There seem to be a lot of issues with this concept... Namely that
  // it's only really easy to implement input iterators. Even the concept of
  // equality is extremely weak. Two iterator referring to the same state
  // are equivalent, but that's about it.
  template<typename Range>
  struct bfs_iterator
    : input_iterator_facade<bfs_iterator<Range>, typename Range::vertex>
  {
    typedef typename Range::graph_type graph_type;
    typedef typename Range::vertex vertex;
    
    // Initialize the iterator to PTE
    bfs_iterator()
      : range_(0)
    { }
    
    // Initialze over a range. Must be non-null.
    explicit bfs_iterator(Range* rng)
      : range_(rng)
    {
      assert(( range_ ));
      
      // Start by moving to the next (first) vertex.
      if(!range_->empty()) {
        range_->next_vertex();
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
      range_->next_vertex();
    }
    
  private:
    Range* range_;
  };


  // FIXME: Make Do a better job with access protection.
  /**
   * The breadth-first range class abstracts a rooted breadth-first search as a
   * range, allowing iteration.
   */
  template<typename Graph, 
           typename Color_Label = internal_label<Graph, basic_color_t>>
  struct rooted_bfs_range
  {
    typedef Graph graph_type;
    typedef typename graph_traits<graph_type>::vertex vertex;
    typedef typename graph_traits<graph_type>::edge edge;

    typedef vertex_label<Graph, Color_Label> color_label;
    typedef color_traits<typename color_label::value_type> colors;

    typedef std::queue<vertex> search_queue;
    
    typedef bfs_iterator<rooted_bfs_range<Graph, Color_Label>> iterator;

    rooted_bfs_range(Graph& g, vertex v)
      : graph(g), current(v), color(g)
    { init(v); }

    rooted_bfs_range(Graph& g, vertex v, Color_Label label)
      : graph(g), current(v), color(label)
    { init(v); }
    
    iterator begin()
    { return iterator{this}; }
    
    iterator end()
    { return iterator{}; }


    /** Initialize the traversal by marking all vertices as unvisited. */
    void init(vertex start) 
    {
      for(auto v : graph.vertices()) {
        color(v) = colors::white();
      }
      search_vertex(start);
    }

    /** 
     * Return true if the search queue is empty. 
     */
    bool empty() const {
      return queue.empty();
    }

    /** 
     * Enqueue the given vertex so that it will be searched later.
     */
    void search_vertex(vertex v) 
    {
      queue.push(v);
      color(v) = colors::gray();
    }

    /** 
     * Move to the next vertex in the search buffer and search its incident
     * edges for undiscovered vertices.
     */
    void next_vertex() {
      current = queue.front();
      queue.pop();
      for(auto e : out_edges(graph, current)) {
        vertex v = graph.target(e);
        if(color(v) == colors::white()) {
          search_vertex(v);
        }
      }
      color(current) = colors::black();
    }

    Graph& graph;
    vertex current;
    search_queue queue;
    color_label color;
  };

  // FIXME: Rewrite this in terms of the previous rooted range.

  /**
   * Constructs a traversable range over a graph. All vertices are visited.
   */
  template<typename Graph, 
           typename Color_Label = internal_label<Graph, basic_color_t>>
  struct bfs_range
  {
    typedef Graph graph_type;
    typedef typename graph_traits<graph_type>::vertex vertex;
    typedef typename graph_traits<graph_type>::edge edge;
    typedef typename graph_traits<graph_type>::vertex_iterator vertex_iterator;    
    
    typedef vertex_label<Graph, Color_Label> color_label;
    typedef color_traits<typename color_label::value_type> colors;

    typedef std::queue<vertex> search_queue;
    
    typedef bfs_iterator<bfs_range<Graph, Color_Label>> iterator;

    // FIXME: This is not a valid constructor for empty graphs.
    bfs_range(Graph& g)
      : graph(g), current(*begin_vertex(g)), color(g)
      , iter(begin_vertex(g)), fini(end_vertex(g))
    { init(current); }

    bfs_range(Graph& g, Color_Label label)
      : graph(g), current(*begin_vertex(g)), color(label)
      , iter(begin_vertex(g)), fini(end_vertex(g))
    { init(current); }
    
    iterator begin()
    { return iterator{this}; }
    
    iterator end()
    { return iterator{}; }

    /** Initialize the traversal by marking all vertices as unvisited. */
    void init(vertex start) 
    {
      for(auto v : graph.vertices()) {
        color(v) = colors::white();
      }
      search_vertex(start);
    }

    /** 
     * Return true if the search queue is empty. 
     */
    bool empty() const {
      return queue.empty();
    }

    /** 
     * Enqueue the given vertex so that it will be searched later.
     */
    void search_vertex(vertex v) 
    {
      queue.push(v);
      color(v) = colors::gray();
    }

    /** 
     * Move to the next vertex in the search buffer and search its incident
     * edges for undiscovered vertices.
     */
    void next_vertex() {
      // Perform the standard BFS search activity.
      current = queue.front();
      queue.pop();
      for(auto e : out_edges(graph, current)) {
        vertex v = graph.target(e);
        if(color(v) == colors::white()) {
          search_vertex(v);
        }
      }
      color(current) = colors::black();
      
      // Move to the next undiscovered root.
      while(iter != fini && color(*iter) != colors::white()) {
        ++iter;
      }
      
      // If we haven't reached the end, then we should queue up the vertex for
      // the next round of searching.
      if(iter != fini) {
        search_vertex(*iter);
      }
    }
    
    Graph& graph;
    vertex current;
    search_queue queue;
    color_label color;
    vertex_iterator iter;   // Points to the current search tree root
    vertex_iterator fini;   // PTE of the vertex range
  };

  /**
   * Construct a BFS range, supporting iterative traversal of the graph. This 
   * is an overloaded name.
   */
  // Construct a rooted BFS range
  template<typename Graph, typename Vertex>
  rooted_bfs_range<Graph> bfs(Graph& g, Vertex v)
  { return {g, v}; }
  
  template<typename Graph, typename Vertex>
  rooted_bfs_range<Graph const> bfs(Graph const& g, Vertex v)
  { return {g, v}; }

  // Construct a rooted BFS range with a custom color label.
  template<typename Graph, typename Vertex, typename Color_Label>
  rooted_bfs_range<Graph> bfs(Graph& g, Vertex v, Color_Label color)
  { return {g, v, color}; }
  
  template<typename Graph, typename Vertex, typename Color_Label>
  rooted_bfs_range<Graph const> bfs(Graph const& g, Vertex v, Color_Label label)
  { return {g, v, label}; }
  
  // Construct a complete BFS range
  template<typename Graph>
  bfs_range<Graph> bfs(Graph& g)
  { return {g}; }
  
  template<typename Graph>
  bfs_range<Graph const> bfs(Graph const& g)
  { return {g}; }

  // Construct a complete BFS range with a custom color label.
  template<typename Graph, typename Vertex, typename Color_Label>
  bfs_range<Graph> bfs(Graph& g, Color_Label color)
  { return {g, color}; }
  
  template<typename Graph, typename Vertex, typename Color_Label>
  bfs_range<Graph const> bfs(Graph const& g, Color_Label label)
  { return {g, label}; }


} // namespace origin

#endif