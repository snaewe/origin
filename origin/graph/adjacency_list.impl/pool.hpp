// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.


namespace origin
{
  namespace adjacency_list_impl
  {
    // Explcitily indicates an invalid index in the pool.
    static constexpr std::size_t invalid_index = -1;

    // A pool node is a mix between an optional type and a linked list node.
    // The prev and next fields refer to the corresponding "live" nodes in a
    // pool. The data buffer stores a possibly initialized value.
    template<typename T>
      struct pool_node
      {
        static constexpr std::size_t limit = -1;

        pool_node();

        // Forwarding constructor
        template<typename... Args>
          pool_node(std::size_t p, std::size_t n, Args&&... args);

        ~pool_node();

        explicit operator bool() const { return valid(); }

        // Returns true if the object is initialized.
        bool valid() const;

        // Returns a pointer to the stored object.
        T*       ptr();
        const T* ptr() const;

        // Returns a reference to the stored object.
        T&       get();
        const T& get() const;

        std::size_t prev;
        std::size_t next;
        Aligned_storage<sizeof(T), alignof(T)> data;
      };

    template<typename T>
      pool_node<T>::pool_node() : prev(limit), next(limit) { }

    template<typename T>
      template<typename... Args>
        pool_node<T>::pool_node(std::size_t p, std::size_t n, Args&&... args)
          : prev(p), next(n)
        {
          new (&data) T(std::forward<Args>(args)...);
        }

    template<typename T>
      pool_node<T>::~pool_node()
      {
        if (valid())
          get().~T();
      }

    template<typename T>
      inline bool
      pool_node<T>::valid() const { return next != limit; }

    template<typename T>
      inline T*
      pool_node<T>::ptr() 
      { 
        assert(valid());
        return static_cast<T*>(&data); 
      }

    template<typename T>
      inline const T*
      pool_node<T>::ptr() const 
      { 
        assert(valid());
        return static_cast<const T*>(&data); 
      }

    template<typename T>
      inline T&
      pool_node<T>::get() { return *ptr(); }

    template<typename T>
      inline const T&
      pool_node<T>::get() const { return *ptr(); }


    // ---------------------------------------------------------------------- //
    //                              Node Pool
    //
    // The node pool is the basis for the vertex and edge sets in the adjacency
    // list data structure. The data structure is a cross between a vector and
    // an object pool in that removing elements from the pool do not cause any
    // additional data movements. Insertions into the pool always reuse a free
    // (previously erased) index in the pool.
    //
    // The pool provides one other useful feature: iteration. The elements of
    // the pool can be traversed in time proportional to the number of elements.
    // Traversal skips deleted elements by jumping over the "dead" locations in
    // the pool. This is done by creating a linked list rather than using a
    // filtering technique.
    //
    // The data structure functions like normal vector until an object is
    // erased. When erased, the object is cleared, and its index is added to the
    // free index list, which is actually a min-queue. When a new object is
    // inserted, the least index is taken from the queue and used as the
    // location for the new object. The new object is woven into the linked
    // list of live nodes in constant time. Re-linking list to incorporate the
    // new node is done in constant time.
    //
    // Because the free index list is a min-queue, we always return the first
    // unoccuped index. Unless that index is 0, we are guaranteed that the
    // next lower index is occupied, and we can thus re-link the list based on
    // its pointers. If the returned index is 0, we rely simply use the head
    // pointer to find the first non-empty index and re-link the list with the
    // new element at the head.
    //
    // Performance properties:
    //    - Insertion: O(log2 d)
    //    - Erasure: O(log2 d)
    // Where d is the number of deleted nodes in the pool.
    //
    // This data structure has some similarity to conventional object pools
    // except that it doesn't really allocate memory, and it has additional
    // requirements. In particular, it must maintain the correspondence between
    // indices and the objects that they are mapped to. We also have to
    // provide efficient iteration over elements in the pool.
    template<typename T>
      struct pool
      {
        std::size_t insert(const T& x);

      private:
        using list_type = std::vector<T*>;
        using queue_type = std::priority_queue<std::size_t, 
                                               std::vector<std::size_t>, 
                                               std::greater<size_t>>;

        list_type  nodes; // The actual node vector
        queue_type free;  // The free index list
      };

    // Insert the node p into the vector. If there are dead indices, reuse
    // one. Otherwise, append the vertex.
    template<typename T>
      inline size_t
      pool<T>::insert(const T& x)
      {
        return -1;
      }

  } // adjacency_list_impl
}
