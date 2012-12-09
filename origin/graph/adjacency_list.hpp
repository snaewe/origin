// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ADJACENCY_LIST_HPP
#define ORIGIN_GRAPH_ADJACENCY_LIST_HPP

#include <vector>
#include <queue>

#include <origin/type/concepts.hpp>
#include <origin/type/empty.hpp>
#include <origin/graph/handle.hpp>

namespace origin
{
  // ------------------------------------------------------------------------ //
  //                                Empty Base
  //
  // The empty base facility is used to compress a possibly empty type out
  // of a data structure. This is useful when a class has a non-dependent
  // component that can be fully separated from a single dependent element
  // (e.g., a linked list node).
  template<typename T, bool = Empty<T>()>
    struct empty_base;

  template<typename T>
    struct empty_base<T, true> : T
    {
      template<typename... Args>
        empty_base(Args&&... args) : T(std::forward<Args>()...) { }

      T&       data()       { return *this; }
      const T& data() const { return *this; }
    };

  template<typename T>
    struct empty_base<T, false>
    {
      template<typename... Args>
        empty_base(Args&&... args) : data_(std::forward<Args>()...) { }

      T&       data()       { return data_; }
      const T& data() const { return data_; }
      
      T data_;
    };



  namespace adjacency_list_impl
  {
    struct vertex_base;
    struct edge_base;

    using edge_list = std::vector<edge_handle>;

    // ---------------------------------------------------------------------- //
    //                           Vertex Representation
    
    struct vertex_base
    {
      vertex_base* next;
      vertex_base* prev;
      edge_list out;     // Out edges
      edge_list in;      // In edges
    };

    template<typename V>
      struct vertex : vertex_base, empty_base<V>
      {
        template<typename... Args>
          vertex(Args&&... args) 
            : empty_base<V>(std::forward<Args>(args)...) 
          { }
      };


    // ---------------------------------------------------------------------- //
    //                            Edge Representation

    struct edge_base
    {
      vertex_handle source;
      vertex_handle target;
    };

    template<typename E>
      struct edge : edge_base
      {
        E data;
      };

    // ---------------------------------------------------------------------- //
    //                              Node Pool
    //
    // The node pool is the basis for the vertex and edge sets in the adjacency
    // list data structure. It is, first and foremost, a vector of pointers to
    // nodes. The node vector has the special property that when an element is
    // erased, the index of the erased object is saved in a dead index queue.
    // Any insertion into that list will reuse dead indices before increasing
    // the size of the vertex set.
    //
    //
    // This data structure has some similarity to conventional object pools
    // except that it doesn't really allocate memory, and it has additional
    // requirements. In particular, it must maintain the correspondence between
    // indices and the objects that they are mapped to. We also have to
    // provide efficient iteration over elements in the pool.
    template<typename T>
      struct node_pool
      {
        size_t insert(T* p);

        T* erase(std::size_t n);

      private:
        std::size_t append(T* p);
        std::size_t reuse(T* p);

        std::size_t take();

        T* find_prev(std::size_t n) const;
        T* find_next(std::size_t n) const;

        void link_prev(T* p, T* prev);
        void link_next(T* p, T* next);
        void link_tail(T* p);
      public:

        std::vector<T*>    nodes; // The actual node vector
        std::queue<size_t> dead;  // The dead vertex list
      };

    // Insert the node p into the vector. If there are dead indices, reuse
    // one. Otherwise, append the vertex.
    template<typename T>
      inline size_t
      node_pool<T>::insert(T* p)
      {
        if (dead.empty())
          return append(p);
        else
          return reuse(p);
      }

    // Erase the node at the specified index, and adding the index to the dead
    // queue. Return a pointer to the node being erased. If the node was already
    // erased, no action is taken, and nullptr is returned.
    template<typename T>
      inline T*
      node_pool<T>::erase(size_t n)
      {
        if (nodes[n]) {
          dead.push(n);
          T* p = nodes[n];
          nodes[n] = nullptr;
          return p;
        }
        return nullptr;
      }

    // Append the ndoe to the vector.
    template<typename T>
      inline std::size_t
      node_pool<T>::append(T* p)
      {
        link_tail(p);
        nodes.push_back(p);
        return --nodes.size();
      }

    // Reuse the next index from the dead list.
    template<typename T>
      inline std::size_t
      node_pool<T>::reuse(T* p)
      {
        size_t n = take();
        link_prev(p, find_prev(n));
        link_next(p, find_next(n));
        nodes[n] = p;
        return n;
      }

    // Take the next index out of the dead list.
    template<typename T>
      inline std::size_t
      node_pool<T>::take()
      {
        size_t n = dead.front();
        dead.pop();
        return n;
      }

    // Return a pointer to the previous live node or nullptr if there are none.
    template<typename T>
      inline T*
      node_pool<T>::find_prev(size_t n) const
      {
        const T** first = &nodes[n] - 1;
        const T** last = &nodes.front() - 1;
        while (first != last && !*first)
          --first;
        return first != last ? *first : nullptr;
      }

    // Return a pointer to the next live node or nullptr if there are none.
    template<typename T>
      inline T*
      node_pool<T>::find_next(size_t n) const
      {
        const T** first = &nodes[n] + 1;
        const T** last = &nodes.back() + 1;
        while (first != last && !*first)
          ++first;
        return first != last ? *first : nullptr;
      }

    // Link p so that its previous node is prev.
    template<typename T>
      inline void
      node_pool<T>::link_prev(T* p, T* prev)
      {
        p->prev = prev;
        if (prev)
          prev->next = p;      
      }

    // Link p so that its next node is next.
    template<typename T>
      inline void
      node_pool<T>::link_next(T* p, T* next)
      {
        p->next = next;
        if (next)
          next->prev = p;
      }

    // Link p to the tail of the pool.
    template<typename T>
      inline void
      node_pool<T>::link_tail(T* p)
      {
        T* tail = nodes.back();
        tail->next = p;
        p->prev = tail;
      }

    // ---------------------------------------------------------------------- //
    //                                  Aliases

    template<typename V>
      using vertex_pool = node_pool<vertex<V>>;

  } // adjacency_list_impl


  // ------------------------------------------------------------------------ //
  //                        Directed Adjacency List
  //
  // Blah blah blah
  template<typename V = empty_t, typename E = empty_t>
    class directed_adjacency_list
    {
      using vertex_type = adjacency_list_impl::vertex<V>;
      using vertex_set = adjacency_list_impl::vertex_pool<V>;
    public:
      using vertex = vertex_handle;

      vertex add_vertex();
      vertex add_vertex(V&& x);
      vertex add_vertex(const V& x);

    private:
      vertex_set verts_;
    };

  template<typename V, typename E>
    inline auto
    directed_adjacency_list<V, E>::add_vertex() -> vertex
    {
      return add_vertex(V{});
    }

  template<typename V, typename E>
    auto
    directed_adjacency_list<V, E>::add_vertex(V&& x) -> vertex
    {
      vertex_type* v = new_vertex(std::move(x));
      return verts_.insert(v);
    }

  template<typename V, typename E>
    auto
    directed_adjacency_list<V, E>::add_vertex(const V& x) -> vertex
    {
      vertex_type* v = new_vertex(x);
      return verts_.insert(v);
    }
};


#endif
