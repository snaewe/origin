// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_SPANNING_TREE_PRIM_HPP
#define ORIGIN_GRAPH_SPANNING_TREE_PRIM_HPP

// FIXMEs
//  - need to fix vertex.hpp, or just make own iterator

#include <limits>
#include <functional>
#include <iostream>

#include <origin/heap/binary_heap.hpp>
#include <origin/graph/traits.hpp>
#include <origin/graph/label.hpp>



/*============================================================================*/
enum class tri_color : char { black, gray, white };

template <typename T>
  constexpr T max_val() { return std::numeric_limits<T>::max(); }

//template <typename T>  using Label_range = 

namespace origin
{
  namespace
  {
    template <typename Vertex, typename Labeling, typename Relation>
      struct label_compare
      {
        label_compare(Labeling & l, Relation r)
          : l_(l), r_(r)
        { }

        bool operator()(Vertex a, Vertex b) const {return r_(l_(b),l_(a)); }

        const Labeling & l_;
        Relation r_;
      };
  }


/*============================================================================*/

  template <typename G, typename Edge_weight>
    vertex_labeling<typename G::vertex>
    prim(const G& g, typename G::vertex s, Edge_weight & w)
    {
      using Vertex = typename G::vertex;
      using Edge = typename G::edge;
      using weight_type = typename Edge_weight::value_type;
      using weight_compare = label_compare<
        Vertex, vertex_labeling<weight_type>, std::less<weight_type>>;
      // make pred map, color map, vertex weight
      auto pred = make_vertex_labeling<G, Vertex>(g, Vertex(99999));
      auto color = make_vertex_labeling(g, tri_color::white);
      auto v_w = make_vertex_labeling(g,max_val<weight_type>());
      
      // Prepare the queue and start with the source vertex s
      mutable_binary_heap<Vertex, weight_compare> q (
        weight_compare(v_w, std::less<weight_type>())
        );
      q.push(s);
      color(s) = tri_color::gray;

      while (!q.empty()) {
        Vertex u = q.top();
        std::cerr << "popped " << u.value() << '\n';
        q.pop();
        for (auto e_ : g.incident_edges(u)) {
          Edge e =*e_;
          Vertex v = opposite(e,u);
          if (w(e) < v_w(v)) {
            std::cerr << "Working on " << v.value() << '\n';
            std::cerr << w(e) << " < " << v_w(v) << '\n';
            v_w(v) = w(e);
            pred(v) = u;
            if (color(v) == tri_color::white) {
              std::cerr << v.value() << " is new" << '\n';
              q.push(v);
              color(v) = tri_color::gray;
            } else if (color(v) == tri_color::gray) {
              std::cerr << v.value() << " is already queued" << '\n';
              q.decrease(v);
            }
          }
        }
        std::cerr << u.value() << " is finished" << '\n';
        color(u) = tri_color::black;
      }
      return pred;
    }

} // namespace origin

#endif //ORIGIN_GRAPH_SPANNING_TREE_PRIM_HPP