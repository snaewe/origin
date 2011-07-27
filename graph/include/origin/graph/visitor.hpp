// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_VISITOR_HPP
#define ORIGIN_GRAPH_VISITOR_HPP

namespace origin
{
  /**
   * The action enumeration defines a set of actions that can be returned by
   * observers in order to influence the control of an algorithm.
   */
  enum class action 
  {
    handle,   ///< Process the vertex or edge in the usual way
    ignore,   ///< Ignore the vertex or edge, but continue processing
    accept    ///< Accept the vertex or edge and stop processing
  };
  
  /**
   * The distance visitor assigns an a distance to each vertex in the graph. 
   * For every tree edge, the distance to the target vertex is 1 greater than 
   * the distance to the source vertex.
   */
  template<typename Graph, typename Distance_Label, typename Base>
    struct distance_visitor : Base
    {
      typedef typename graph_traits<Graph>::vertex Vertex;
      typedef typename graph_traits<Graph>::edge Edge;
      typedef typename label_traits<Distance_Label, Vertex>::value_type Distance;

      distance_visitor(Distance_Label d, Distance z, Distance i)
        : dist{d}, zero{z}, inf{i}
      { }

      // Initialize all distances to infinity.
      void initialized_vertex(Graph& g, Vertex v)
      {
        dist(v) = inf;
      }
        
      // For the root vertex, make sure it's distance is 0.
      void root_vertex(Graph& g, Vertex v)
      {
        dist(v) = zero;
      }

      // On tree edges, the distance of the target is one more than that of
      // the source.
      // NOTE: The distance type needs to be Constructible<int>
      // FIXME: Should we abstract the increment so that we call next() or
      // somethng like that? That would support an interestingly broad
      // range of abstractions.
      void tree_edge(Graph& g, Edge e)
      {
        // FIXME: Clamp the accumulation.
        dist(g.target(e)) = dist(g.source(e)) + Distance{1};
      }
      
      Distance_Label dist;
      Distance zero;
      Distance inf;
    };
    
  /**
   * The predecessor visitor assigns a predecessor (or parent) vertex to each
   * vertex in the graph. Initially, all vertices are their own predecessor.
   * For every discovered tree edge, the predecessor of the target vertex is 
   * the source vertex. The root of a search tree is its own predecessor.
   */
  template<typename Graph, typename Parent_Label, typename Base>
    struct parent_visitor : Base
    {
      typedef typename graph_traits<Graph>::vertex Vertex;
      typedef typename graph_traits<Graph>::edge Edge;

      parent_visitor(Parent_Label p)
        : pred{p}
      { }
      
      // Initially, every vertex is its own parent.
      void initialized_vertex(Graph& g, Vertex v)
      {
        pred(v) = v;
      }

      // For a tree edge, the parent of the target is the source.
      void tree_edge(Graph& g, Edge e)
      {
        pred(g.target(e)) = g.source(e);
      }
      
      Parent_Label pred;
    };

} // namespace origin

#endif
