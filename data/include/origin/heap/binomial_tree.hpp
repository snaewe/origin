// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_HEAP_BINOMIAL_TREE_HPP
#define ORIGIN_HEAP_BINOMIAL_TREE_HPP

#include <cassert>
#include <vector>

namespace origin 
{
  template<typename> struct binomial_tree_node;
  template<typename> struct mutable_binomial_tree_node;

  /**
   * This type trait return true if the given type is a kind of binomial tree 
   * node, false otherwise.
   */
  template<typename T>
    struct is_binomial_tree_node : std::false_type { };
    
  template<typename T>
    struct is_binomial_tree_node<binomial_tree_node<T>> : std::true_type { };
    
  template<typename T>
    struct is_binomial_tree_node<mutable_binomial_tree_node<T>> : std::true_type { };

  
  // FIXME: Refactor into dependent/non-dependent types.
  /**
   * @internal
   * 
   * The binomial tree node is a node in a binomial tree. The order of a 
   * binomial tree is related to the number of nodes it contains such that
   * a binomial tree of order n contains 2^n nodes. Conceptually, a binomial
   * tree is either a single node with order 0 or a node with order k whose
   * children are binomial subtrees of order k-1, k-2, ..., 0 in that order.
   * 
   * The node is implemented as a "forward tree" where each node is comprised
   * of a link to its left-most child and its own right sibling. Note that 
   * child of a subtree with order k has order k - 1. Also the right sibling
   * of a subtree with order k has order k - 1, but only if it is not the
   * root of a binomial tree.
   * 
   * A binomial tree node is "rooted" if it is the root of a binomial tree.
   * A rooted node is one that can claim no siblings; no other node must point 
   * to it.
   * 
   * Binomial trees can be linked with other binomial trees of the same order.
   * Linking two binomial trees of order k produces a new binomial tree of
   * order k+1.
   */
  template<typename T> 
    struct binomial_tree_node
    {
      typedef T value_type;
      typedef std::size_t size_type;
      
      // Forwarding constructor. Links are nullptr-initialized. 
      // NOTE: The only way to set the should be by calling link or unlink.
      template<typename... Args>
        binomial_tree_node(Args&&... args) 
          : value(std::forward<Args>(args)...), right{}, child{}
        { }
       
      // Return the order of this subtree. It is computed recursively.
      size_type order() const
      {
        return child ? 1 + child->order() : 0;
      }
       
      // Link this rooted node with the given rooted node such that this tree
      // represented by this node becomes the root of the joined trees. The
      // order of this node is increased by 1.
      // 
      // Note that both nodes must be roots of binomial trees. However, we
      // cannot enforce this precondition since we cannot adequately enforce
      // the constraint.
      void link(binomial_tree_node* p)
      {
        assert(( order() == p->order() ));
        
        // Extend the set of children by attaching the given node to the left
        // of the left-most child.
        p->right = child;
        child = p;
      }
      
      // Detach this node from its child and sibling. Note that this operation 
      // can corrupt a well-formed binomial tree.
      void unlink()
      {
        right = child = nullptr;
      }

      // Make this node act as the root of a binomial subtree by ensuring that
      // it has no siblings.
      void relink()
      {
        right = nullptr;
      }

      value_type value;
      binomial_tree_node* right;  // Right sibling of this node
      binomial_tree_node* child;  // Left-most child node
    };

  // Clone the given subtree. Alloc must be an Allocator of the Node type.
  template<typename T, typename Alloc>
    binomial_tree_node<T>* clone(binomial_tree_node<T> const* p, Alloc alloc) 
    {
      assert(( p ));

      // FIXME: Actually use the allocator
      // FIXME: Make this non-recursive if we can do so efficiently.
      binomial_tree_node<T>* ret = new binomial_tree_node<T>{p->value};
      if(p->right)
        ret->right = clone(p->right, alloc);
      if(p->child)
        ret->child = clone(p->child, alloc);
      return ret;
    }


  // FIXME: Is there a better name for this?
  /**
   * A mutable binomial tree node is a binomial tree node that supports the
   * implementation of a mutable binomial heap by storing a link to its
   * parent node. Note that we can also use this information to determine if 
   * the node is a root of a binomial tree.
   */
  template<typename T>
    struct mutable_binomial_tree_node
    {
      typedef T value_type;
      typedef std::size_t size_type;

      // Forwarding constructor. Links are nullptr-initialized.
      // NOTE: The only way to set the should be by calling link or unlink.
      template<typename... Args>
        mutable_binomial_tree_node(Args&&... args) 
          : value(std::forward<Args>(args)...), parent{}, right{}, child{}
        { }

      // Return true if the node is the root of a binomial tree.
      bool is_root() const
      {
        return !parent;
      }

      // Return the order of tree
      size_type order() const
      {
        return child ? 1 + child->order() : 0;
      }
      
      // Link this rooted node with the given rooted node such that this tree
      // represented by this node becomes the root of the joined trees. The
      // order of this node is increased by 1.
      void link(mutable_binomial_tree_node* p)
      {
        assert(( is_root() && p->is_root() ));
        assert(( order() == p->order() ));
        
        // Extend the set of children by attaching the given node to the left
        // of the left-most child.
        p->right = child;
        child = p;
        p->parent = this;
      }
      
      // Detach this node from its child and sibling. Note that this operation 
      // can corrupt a well-formed binomial tree.
      void unlink()
      {
        parent = right = child = nullptr;
      }

      // Make this node act as the root of a binomial subtree by ensuring that
      // it has neither siblings nor parent.
      void relink()
      {
        parent = right = nullptr;
      }


      value_type value;
      mutable_binomial_tree_node* parent;  // Parent of this node or nullptr
      mutable_binomial_tree_node* right;   // Right sibling of this node
      mutable_binomial_tree_node* child;   // Left-most child node
    };

  // Clone the given subtree. Alloc must be an Allocator of the Node type.
  template<typename T, typename Alloc>
    mutable_binomial_tree_node<T>* 
    clone(mutable_binomial_tree_node<T> const* p, Alloc alloc) 
    {
      assert(( p ));
      // FIXME: Actually use the allocator
      // FIXME: Make this non-recursive if we can do so efficiently.
      mutable_binomial_tree_node<T>* ret = new mutable_binomial_tree_node<T>{p->value};
      if(p->right)
        ret->right = clone(p->right, alloc);
      if(p->child) {
        ret->child = clone(p->child, alloc);
        ret->child->parent = ret;
      }
      return ret;
    }

  // Destroy the given subtree. Alloc must be an Allocator of the Node type.
  // This applies to both tree node types.
  template<typename T, typename Alloc>
    typename std::enable_if<is_binomial_tree_node<T>::value, void>::type
    destroy(T* p, Alloc alloc) 
    {
      if(!p) return;
      // FIXME: Make this non-recursive if we can do so efficiently.
      destroy(p->child, alloc);
      destroy(p->right, alloc);
      delete p;
    }

  // FIXME: Actually use the allocator in a meaningful way.
  /**
   * @internal
   * 
   * The binomial tree class wraps a rooted binomial tree node and provides
   * regularity for the entire tree structure, especially for initialization, 
   * copying, and destruction.
   * 
   * Note that the binomial tree does not provide a size operation because
   * it cannot be constructed in constant time.
   * 
   * The binomial tree node can be parameterized by any Binomial_tree_node
   * type. There are currently two. The typical "forward" tree node and the
   * "bidirectional" version.
   * 
   * @tparam T      A Regular type; the value type
   * @tparam Node   A Binomial_tree_node containing T
   * @tparam Alloc  An Allocator of T
   */
  template<typename T, 
           typename Node = binomial_tree_node<T>, 
           typename Alloc = std::allocator<T>>
    class binomial_tree
    {
    public:
      typedef Alloc allocator_type;
      typedef T value_type;
      typedef Node node_type;
      typedef typename allocator_type::size_type size_type;
    private:
      typedef typename Alloc::template rebind<node_type> node_allocator;

      /**
       * @brief Node constructor.
       * Create a binomial tree over the given node. The new tree takes 
       * ownership of the others. This is used internally to explode a binomial
       * tree into its root and child subtrees. Note that the right pointer
       * of the new root is reset.
       */
      explicit binomial_tree(node_type* p)
        : root_{p}
      {
        root_->relink();
      }
    public:
      
      /** @name Initialization */
      //@{
      binomial_tree(allocator_type const& alloc = allocator_type{})
        : root_{nullptr}
      { }
      
      /**
       * @brief Copy constructor
       */
      binomial_tree(binomial_tree const& x)
        : root_{x.root_ ? clone(x.root_, get_allocator()) : nullptr}
      { }
      
      binomial_tree& operator=(binomial_tree const& x)
      {
        binomial_tree tmp{x};
        swap(tmp);
        return *this;
      }
      
      /**
       * @brief Move constructor
       */
      binomial_tree(binomial_tree&& x)
        : root_{x.root_}
      { 
        x.root_ = nullptr;
      }
      
      binomial_tree& operator=(binomial_tree&& x)
      {
        binomial_tree tmp{std::move(x)};
        swap(tmp);
        return *this;
      }
      
      // FIXME: Use the allocator
      /**
       * @brief Singleton value constructor
       * Create a binomial tree containing a single value.
       */
      binomial_tree(value_type const& x)
        : root_{new node_type{x}}
      { }
      
      binomial_tree(value_type&& x)
        : root_{new node_type{std::move(x)}}
      { }
     
      // FIXME: How do we support a forwarding constructor in a class that
      // already overloads a number of constructors. We could use a dummy
      // argument (e.g., emplace_t) to indicate the requested operation.
      
      ~binomial_tree()
      {
        clear();
      }
      //@}
      
      /** @name Properties */
      //@{
      /**
       * Return true if the tree has no nodes.
       */
      bool empty() const
      {
        return !root_;
      }
      
      /**
       * Return the data structure's allocator.
       */
      allocator_type get_allocator() const
      {
        return allocator_type{};
      }
      //@}
      
      /** @name Tree properties */
      //@{
      /**
       * Return the root node of the tree.
       */
      node_type* root()
      {
        return root_;
      }
      
      node_type const* root() const
      {
        return root_;
      }
      
      size_type order() const
      {
        return root_->order();
      }
      //@}
      
      /** @name Binomial tree operations */
      //@{
      /**
       * Link the given binomial tree as a child of this, increasing the order
       * of the tree. The given tree is empty after the link operation.
       */
      void link(binomial_tree& x)
      {
        root_->link(x.root_);
        x.root_ = nullptr;
      }
      
      /**
       * Return a sequence containing the individual subtrees of this one.
       * This tree is reduced to a binomial tree of order 0.
       */
      std::vector<binomial_tree> explode()
      {
        std::vector<binomial_tree> result;
        node_type* p = root_->child;
        while(p != nullptr) {
          node_type* q = p->right;
          result.push_back(binomial_tree(p));
          p = q;
        }
        root_->unlink();
        return std::move(result);
      }
      //@}
      
      /** @name Operations */
      //@{
      void swap(binomial_tree& x)
      {
        std::swap(root_, x.root_);
      }
      
      /**
       * Remove all elements from the tree.
       */
      void clear()
      {
        destroy(root_, get_allocator());
        root_ = nullptr;
      }
      //@}
      
    private:
      node_type* root_;
    };

} // namespace origin

#endif
