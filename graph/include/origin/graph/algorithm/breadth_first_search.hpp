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

#include <origin/graph/color.hpp>

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

  /**
   * @ingroup graph_bfs
   */
  template<typename Graph>
  struct breadth_first_search_visitor
  {
    typedef Graph graph_type;    
    typedef typename graph_type::vertex vertex;
    typedef typename graph_type::edge edge;
    
    // Called after a vertex has been initialized
    void initialized(graph_type const& g, vertex v) { }
    
    // Called after a vertex has been discovered
    void discovered(graph_type const& g, vertex v) { }
    
    // Called after a vertex has been popped from the queue and before its
    // incident edges have been examined.
    void started(graph_type const& g, vertex v) { }
    
    // Called after the vertex has been examined.
    void finished(graph_type const& g, vertex v) { }
    
    // Called when a discovered vertex is the root of a search tree in the
    // search forest.
    void root(graph_type const& g, vertex v) { }
    
    // Called before an incident edge is examined.
    void started(graph_type const& g, edge e) { }

    // Called when an edge is determined to be in the search tree. Occurs 
    // just before the target vertex is discovered.
    void tree(graph_type const& g, edge e) { }
    
    // Called when an edge is determined to not be in the search tree.
    void nontree(graph_type const& g, edge e) { }
  };

  // FIXME: Parameterize over a color label. Use a default argument to select
  // between one or more base classes that supply labeling functions.

  /**
   * The breadth first visit algorithm object performs a breadth first 
   * traversal on all vertices connected to a single starting vertex.
   */
  template<typename Graph, typename Visitor>
  class breadth_first_visit_algorithm
  {
  public:
    typedef Visitor visitor_type;
    typedef Graph graph_type;
    typedef typename graph_type::vertex vertex;
    typedef typename graph_type::edge edge;
    
    // FIXME: Generate an optimal mapping based on the indexing properties
    // of the graph type.
    typedef std::unordered_map<vertex, color_t> color_map;
    
    typedef std::queue<vertex> search_queue;
    
    breadth_first_visit_algorithm(Graph const& g, Visitor vis)
      : graph(g), visitor(vis)
    { 
      for(vertex v : graph.vertices()) {
        color(v) = white;
        visitor.initialized(graph, v);
      }
    }
    
    // Perform search
    void search(vertex v)
    {
      color(v) = gray;
      queue.push(v);
      visitor.discovered(graph, v);
      visitor.root(graph, v);

      while(!queue.empty()) {
        vertex u = queue.front();
        queue.pop();
        visitor.started(u);
        
        for(edge e : search_edges(graph)) {
          visitor.started(e);
          vertex v = graph.target(e);
          
          if(color(v) == white) {
            color(v) = gray;
            queue.push(v);
            visitor.tree(e);
            visitor.discovered(v);
          } else {
            visitor.nontree(e);
          }
        }
        
        color(v) = black;
        visitor.finished(u);
      }
    }
    
    color_t& color(vertex v)
    { return colors[v]; }
    
    color_t color(vertex v) const
    { return colors[v]; }
    
    graph_type const& graph;
    visitor_type visitor;
    search_queue queue;
    color_map colors;    
  };
  
  /**
   * The breadth first search algorithm performs a breadth first search on
   * the entire graph. All vertices are visited.
   */
  template<typename Graph, typename Visitor>
  struct breadth_first_search_algorithm 
    : private breadth_first_visit_algorithm<Graph, Visitor>
  {
    typedef breadth_first_visit_algorithm<Graph, Visitor> base_type;
    typedef Graph graph_type;
    typedef typename graph_type::vertex vertex;
    typedef typename graph_type::edge edge;
    
    breadth_first_search_algorithm(Graph const& g, Visitor vis)
      : base_type(g, vis)
    { }

    void search()
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

} // namespace origin

#endif