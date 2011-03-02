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
  struct breadth_first_search_visitor
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
   */
  template<typename Graph, 
           typename Visitor, 
           typename Color_Label = internal_label<Graph, color_t>>
  class breadth_first_search_from_algorithm
  {
  public:
    typedef Visitor visitor_type;
    typedef Graph graph_type;
    typedef typename vertex_type<graph_type>::type vertex;
    typedef typename edge_type<graph_type>::type edge;
    typedef std::queue<vertex> search_queue;
    typedef vertex_label<Graph, Color_Label> color_label;
    
    breadth_first_search_from_algorithm(Graph& g, Visitor vis)
      : graph(g), visitor(vis), queue(), color(g)
    { }
    
    breadth_first_search_from_algorithm(Graph& g, Visitor vis, Color_Label color)
      : graph(g), visitor(vis), queue(), color(color)
    { }

    void init()
    {
      for(auto v : graph.vertices()) {
        color(v) = white;
        visitor.initialized_vertex(graph, v);
      }
    }
    
    // Perform search
    void operator()(vertex v)
    {
      color(v) = gray;
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
          
          if(color(v) == white) {
            color(v) = gray;
            queue.push(v);
            visitor.tree_edge(graph, e);
            visitor.discovered_vertex(graph, v);
          } else {
            visitor.nontree_edge(graph, e);
          }
        }
        
        color(v) = black;
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
  template<typename Graph, typename Visitor>
  struct breadth_first_search_algorithm 
    : private breadth_first_search_from_algorithm<Graph, Visitor>
  {
    typedef breadth_first_search_from_algorithm<Graph, Visitor> base_type;
    
    typedef Visitor visitor_type;
    typedef Graph graph_type;
    typedef typename base_type::type vertex;
    typedef typename base_type::type edge;
    
    breadth_first_search_algorithm(Graph& g, Visitor vis)
      : base_type(g, vis)
    { }

    void operator()()
    {
      for(auto v : graph.vertices()) {
        if(color(v) == white) {
          base_type::search(v);
        }
      }
    }
    
    using base_type::color;
    using base_type::colors;
    using base_type::graph;
  };
  
  /**
   * Perform a breadth first search on the graph, starting at the given vertex.
   */
  //@{
  template<typename Graph, typename Vertex, typename Visitor>
  void breadth_first_search_from(Graph& g,  Vertex v,  Visitor vis)
  {
    breadth_first_search_from_algorithm<Graph, Visitor> algo(g, vis);
    algo(v);
  }

  // Const version of above.
  template<typename Graph, typename Vertex, typename Visitor>
  void breadth_first_search_from(Graph const& g,  Vertex v,  Visitor vis)
  {
    breadth_first_search_from_algorithm<Graph const, Visitor> algo(g, vis);
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
    breadth_first_search_from_algorithm<Graph, Visitor, Color_Label> 
      algo(g, vis, color);
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
    breadth_first_search_from_algorithm<Graph const, Visitor, Color_Label> 
      algo(g, vis, color);
    algo(v);
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
           typename Color_Label = internal_label<Graph, color_t>>
  struct rooted_bfs_range
  {
    typedef Graph graph_type;
    typedef typename vertex_type<graph_type>::type vertex;
    typedef typename edge_type<graph_type>::type edge;
    typedef vertex_label<Graph, Color_Label> color_label;
    typedef std::queue<vertex> search_queue;
    
    typedef bfs_iterator<rooted_bfs_range<Graph, Color_Label>> iterator;

    rooted_bfs_range(Graph& g, vertex v)
      : graph(g), current(v), colors(g)
    { init(v); }

    rooted_bfs_range(Graph& g, vertex v, Color_Label label)
      : graph(g), current(v), colors(label)
    { init(v); }
    
    iterator begin()
    { return iterator{this}; }
    
    iterator end()
    { return iterator{}; }


    /** Initialize the traversal by marking all vertices as unvisited. */
    void init(vertex start) 
    {
      for(auto v : graph.vertices()) {
        colors(v) = white;
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
      colors(v) = gray;
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
        if(colors(v) == white) {
          search_vertex(v);
        }
      }
      colors(current) = black;
    }

    Graph& graph;
    vertex current;
    search_queue queue;
    color_label colors;
  };

  template<typename Graph, typename Vertex>
  rooted_bfs_range<Graph> rooted_bfs(Graph& g, Vertex v)
  { return {g, v}; }
  
  template<typename Graph, typename Vertex>
  rooted_bfs_range<Graph const> rooted_bfs(Graph const& g, Vertex v)
  { return {g, v}; }
  

} // namespace origin

#endif