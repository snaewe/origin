// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_VISITOR_HPP
#define ORIGIN_GRAPH_VISITOR_HPP

#include <iosfwd>

#include <origin/graph/label.hpp>

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
  
  // An event visitor is a visitor parameterized over a function that 
  // specializes a single operation.
  
  template<typename Func, typename Base>
    struct discovered_vertex_visitor : Base
    {
      discovered_vertex_visitor(Func f)
        : fn{f}
      { }
      
      template<typename Graph, typename Vertex>
        void discovered_vertex(Graph& g, Vertex v)
        {
          fn(g, v);
        }
        
        Func fn;
    };
    
  template<typename Func, typename Base>
    inline discovered_vertex_visitor<Func, Base>
    on_discover_vertex(Func fn, Base const&)
    {
      return discovered_vertex_visitor<Func, Base>{fn};
    }

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

  template<typename Graph, typename Distance_Label, typename Distance, typename Base>
    inline distance_visitor<Graph, Distance_Label, Base> 
    visit_distance(Graph& g, 
                   Distance_Label d, 
                   Distance zero, 
                   Distance inf, 
                   Base const&)
    {
      return distance_visitor<Graph, Distance_Label, Base>(d, zero, inf);
    }

  template<typename Graph, typename Distance_Label, typename Base>
    inline distance_visitor<Graph, Distance_Label, Base> 
    visit_distance(Graph& g, Distance_Label d, Base const& base)
    {
      typedef typename graph_traits<Graph>::vertex Vertex;
      typedef typename label_traits<Distance_Label, Vertex>::value_type Distance;
      Distance zero = 0;
      Distance inf = std::numeric_limits<Distance>::max();
      return visit_distance(g, d, zero, inf, base);
    }

  /**
   * The predecessor visitor assigns a predecessor (or parent) vertex to each
   * vertex in the graph. Initially, all vertices are their own predecessor.
   * For every discovered tree edge, the predecessor of the target vertex is 
   * the source vertex. The root of a search tree is its own predecessor.
   */
  template<typename Graph, typename Predecessor_Label, typename Base>
    struct predecessor_visitor : Base
    {
      typedef typename graph_traits<Graph>::vertex Vertex;
      typedef typename graph_traits<Graph>::edge Edge;

      predecessor_visitor(Predecessor_Label p)
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
      
      Predecessor_Label pred;
    };
    
  template<typename Graph, typename Predecessor_Label, typename Base>
    inline predecessor_visitor<Graph, Predecessor_Label, Base>
    visit_predecessor(Graph& g, Predecessor_Label p, Base const&)
    {
      return predecessor_visitor<Graph, Predecessor_Label, Base>(p);
    }

  // FIXME: It might be better if this sat on an output iterator rather than
  // an output stream. Think about insertion iterators.
  /**
   * The ostream visitor is an Event Visitor that streams the value associated
   * with the visited handle to an output stream. For example, visiting
   * discovered vertices will case vertex properties to streamed out.
   */
  template<typename Output_Stream>
    struct ostream_visitor
    {
      typedef typename Output_Stream::char_type Char;

      ostream_visitor(Output_Stream& os, Char const* sep = " ")
        : os(os), sep{sep}
      { }

      template<typename Graph, typename Handle>
        void operator()(Graph& g, Handle x)
        {
          os << g[x] << sep;
        }
      
      Output_Stream& os;
      Char const* sep;
    };

  // Return an ostream visitor on the given stream.
  template<typename Output_Stream>
    inline ostream_visitor<Output_Stream> 
    ostream_visit(Output_Stream& os,
                  typename Output_Stream::char_type const* sep = " ")
    {
      return ostream_visitor<Output_Stream>{os, sep};
    }
    
} // namespace origin

#endif
