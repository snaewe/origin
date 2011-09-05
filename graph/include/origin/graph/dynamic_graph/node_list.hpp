// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_ADJACENCY_LIST_NODE_LIST_HPP
#define ORIGIN_GRAPH_ADJACENCY_LIST_NODE_LIST_HPP

#include <cassert>
#include <cstdint>

#include <origin/exception.hpp>
#include <origin/iterator/facades.hpp>

namespace origin {

  // FIXME: Consider rewriting all the linkage stuff in terms of pairs of
  // pointers rather than two possibly disjoint member variables.

  /** @internal */
  namespace adjacency_list_
  {
    /**
     * @internal The basic_linkage class associates accessor functions with a
     * node type. The default accessor functions return the next and prev
     * members of the Node type.
     */
    template<typename Node>
    struct basic_linkage
    {
      static Node* next(Node* p)
      { return p->next; }

      static void set_next(Node* p, Node* q)
      { p->next = q; }

      static Node* prev(Node* p)
      { return p->prev; }

      static void set_prev(Node* p, Node* q)
      { p->prev = q; }
    };

    // NOTE: It does not seem to be possible to implement a generic form of
    // iteration with a doubly linked list and a single node header. Even GCC
    // and libcxx use a node-base as the header, which still gives two pointers
    // and a nicely described "end" position. Note that this strategy would
    // also waste 2 pointers per list, which grows rapidly for graphs. Here,
    // we implement a non-cyclic linear list with a two-pointer header and
    // nullptr being the logical choice for past the end. Also note that the
    // list knows its own size.

    /** @internal
     * The node list class implements a linked list that is parameterized over
     * a node structure rather than the more traditional value type. The intent
     * of this data structure is to provide a facility of adapting arbitrarily
     * linked pointers into list-like abstractions. This satisfies a number of
     * requirements for the graph data structures.
     *
     * Note that the node list is not a Container in conceptual sense. It does
     * not own its member nodes, it simply provides a method for linearizing
     * their traversal.
     *
     * This data structure currently only supports appending node to the end
     * of the list and removing arbitrary nodes. More complex operations have
     * not been considered.
     */
    template<typename Node, typename Linkage = basic_linkage<Node>>
    class node_list
    {
    public:
      typedef std::size_t size_type;

      typedef Node node_type;
      typedef Node* node_pointer;
      typedef Node const* const_node_pointer;

      node_list()
        : head_{}, tail_{}, size_{}
      { }

      // Node lists are not copyable.
      node_list(node_list const&) = delete;
      node_list& operator=(node_list const&) = delete;

      bool empty() const
      { return head_ == 0; }

      size_type size() const
      { return size_; }

      // Front Access and modification.
      node_pointer head()
      { return head_; }

      const_node_pointer head() const
      { return head_; }

      // FIXME: Rename these functions.
      void push_front(node_pointer p);
      void pop_front();

      // Back access and modification.
      node_pointer tail()
      { return tail_; }

      const_node_pointer tail() const
      { return tail_; }

      // FIXME: Rename these functions
      void push_back(node_pointer p);
      void pop_back();

      // Insert/Erase
      void insert(node_pointer p, node_pointer q);
      node_pointer erase(node_pointer p);

      // Next and previous nodes
      static node_pointer next(node_pointer p)
      { return Linkage::next(p); }

      // FIXME: We need the const cast because the linkage interface doesn't
      // define const methods either. It could, but it doesn't for now.
      static const_node_pointer next(const_node_pointer p)
      { return Linkage::next(const_cast<node_pointer>(p)); }

      static void set_next(node_pointer p, node_pointer q)
      { Linkage::set_next(p, q); }

      static node_pointer prev(node_pointer p)
      { return Linkage::prev(p); }

      static const_node_pointer prev(const_node_pointer p)
      { return Linkage::prev(const_cast<node_pointer>(p)); }

      static void set_prev(node_pointer p, node_pointer q)
      { Linkage::set_prev(p, q); }

      // Special functions
      void swap(node_list& x);

      void clear()
      {
        head_ = nullptr;
        size_ = 0;
      }

    private:
      node_pointer head_;
      node_pointer tail_;
      size_type size_;
    };

    // FIXME: It might be nice to rewrite all of these functions in terms
    // of hook or link operations (as with GCC or libcxx).

    /**
     * Push the given node into the front of the list.
     *
     * @pre p != nullptr
     */
    template<typename Node, typename Linkage>
    void node_list<Node, Linkage>::push_front(node_pointer p)
    {
      assert(( p != nullptr ));
      set_prev(p, nullptr);
      if(empty()) {
        set_next(p, nullptr);
        tail_ = p;
      } else {
        set_next(p, head_);
        set_prev(head_, p);
      }
      head_ = p;
      ++size_;
    }

    /**
     * Pop a node from the front of the list. Note that this simply unlinks
     * the front node. It does not erase it.
     *
     * @pre !empty()
     */
    template<typename Node, typename Linkage>
    void node_list<Node, Linkage>::pop_front()
    {
      assert(( !empty() ));
      --size_;
      head_ = next(head_);
      if(head_) {
        set_prev(head_, nullptr);
      } else {
        tail_ = nullptr;
      }
    }

    /**
     * Push a node into the back of the list.
     *
     * @pre p != nullptr
     */
    template<typename Node, typename Linkage>
    void node_list<Node, Linkage>::push_back(node_pointer p)
    {
      assert(( p != nullptr ));
      set_next(p, nullptr);
      if(empty()) {
        set_prev(p, nullptr);
        head_ = p;
      } else {
        set_next(tail_, p);
        set_prev(p, tail_);
      }
      tail_ = p;
      ++size_;
    }

    /**
     * Pop a node from the back of the list.
     *
     * @pre !empty()
     */
    template<typename Node, typename Linkage>
    void node_list<Node, Linkage>::pop_back()
    {
      assert(( !empty() ));
      --size_;
      tail_ = prev(tail_);
      if(tail_) {
        set_next(tail_, nullptr);
      } else {
        head_ = nullptr;
      }
    }

    /**
     * Insert the node into the list at the given position.
     *
     * @todo Implement me!
     */
    template<typename Node, typename Linkage>
    void node_list<Node, Linkage>::insert(node_pointer p, node_pointer q)
    { throw not_implemented("node_list::insert(p, q)"); }

    /**
     * Remove the given node from the list. Note that this operation simply
     * unlinks the node from the list. It does not reclaim any memory.
     *
     * @pre !empty() and p != nullptr
     */
    template<typename Node, typename Linkage>
    auto node_list<Node, Linkage>::erase(node_pointer p) -> node_pointer
    {
      assert(( p != nullptr ));
      assert(( !empty() ));
      if(p == head_) {
        pop_front();
        return head_;
      } else if(p == tail_) {
        pop_back();
        return nullptr;
      } else {
        --size_;
        set_next(prev(p), next(p));
        set_prev(next(p), prev(p));
        return next(p);
      }
    }

    template<typename Node, typename Linkage>
    void node_list<Node, Linkage>::swap(node_list& x)
    {
      swap(head_, x.head_);
      swap(tail_, x.tail_);
      swap(size_, x.size_);
    }

    // Specialize functions.
    template<typename N, typename L>
    void swap(node_list<N, L>& x, node_list<N, L>& y)
    { x.swap(y); }


    template<typename, typename> class node_iterator;
    template<typename, typename> class const_node_iterator;

    /**
     * The node iterator adapts a node pointer into a bidirectional iterator.
     * Dereferencing a node list iterator results in a node pointer.
     *
     * @tparam List An instance of the node_list type.
     * @tparam Node The most derived type of node in the list.
     *
     * Note that Node must be derived from List::node_type, but we can't
     * statically enforce this requirement in the body of the class because it
     * can introduce a requirement for a complete type.
     *
     * @note Deducing the value type from the node type requires the node type
     * to be complete, which inhibits certain kinds of graph declarations.
     */
    template<typename List, typename Node>
    class node_iterator
      : public bidirectional_iterator_facade<
          node_iterator<List, Node>, Node*, Node*, Node*
        >
    {
      friend class const_node_iterator<List, Node>;
    public:
      typedef Node* value_type;
      typedef Node* reference;
      typedef Node* pointer;
      typedef std::ptrdiff_t difference_type;

      node_iterator(pointer n = nullptr)
        : node_{n}
      { }

      node_iterator& operator=(pointer n)
      { node_ = n; return *this; }

      // FIXME: You should be able to compare const/non-const iterators, right?
      // How is this best done?
      bool equal(node_iterator const& x) const
      { return node_ == x.node_; }

      reference dereference() const
      { return node_; }

      void increment()
      { node_ = static_cast<pointer>(List::next(node_)); }

      void decrement()
      { node_ = static_cast<pointer>(List::prev(node_)); }

    private:
      pointer node_;
    };

    // The constant version of the iterator above.
    template<typename List, typename Node>
    class const_node_iterator
      : public bidirectional_iterator_facade<
          node_iterator<List, Node>, Node*, Node*, Node*
        >
    {
    public:
      typedef Node* value_type;
      typedef Node const* reference;
      typedef Node const* pointer;
      typedef std::ptrdiff_t difference_type;

      // List/Node initialization
      const_node_iterator(pointer n = nullptr)
        : node_{n}
      { }

      const_node_iterator& operator=(pointer n)
      { node_ = n; return *this; }

      // Interop initialization
      const_node_iterator(node_iterator<List, Node> const& x)
        : node_{x.node_}
      { }

      const_node_iterator operator=(node_iterator<List, Node> const& x)
      { node_ = x.node_; return *this; }

      bool equal(const_node_iterator const& x) const
      { return node_ == x.node_; }

      reference dereference() const
      { return node_; }

      void increment()
      { node_ = static_cast<pointer>(List::next(node_)); }

      void decrement()
      { node_ = static_cast<pointer>(List::prev(node_)); }

    private:
      pointer node_;
    };
  } // namespace adjacency_list_
} // namesapce origin

#endif
