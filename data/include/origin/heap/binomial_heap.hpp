// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_HEAP_BINOMIAL_HEAP_HPP
#define ORIGIN_HEAP_BINOMIAL_HEAP_HPP

#include <cassert>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include <origin/functional.hpp>
#include <origin/heap/binomial_tree.hpp>

namespace origin 
{
  // FIXME: I shouldn't have to invert the order here. I should be able to
  // implement the data structure directly in terms of the base comparison.
  // For some reason changing the comparison in the link function gives
  // incorrect orders, and I can't figure out why. This works for now.
  /**
   * @internal
   * 
   * Compare two non-null binomial nodes. Note that the comparison is inverted
   * in order to preserve the heap ordering semantic.
   */
  template<typename T, typename Comp>
    struct binomial_node_compare : inverse_compare<Comp>
    {
      typedef inverse_compare<Comp> base_comp;

      binomial_node_compare(Comp c)
        : base_comp{c}
      { }

      bool operator()(T const* p, T const* q) const
      {
        // FIXME: Would this work if I just swapped p and q without relying
        // on inverse_compare?
        return base_comp::operator()(p->value, q->value);
      }
    };

  /**
   * @internal
   * 
   * An extended comparison two binomial trees for empty trees. An empty tree
   * is treated as an extreme value under this comparison. For any tree t, and 
   * an empty tree e, comp(t, e) == true and comp(e, t) == false.
   * 
   * @tparam T      A binomial tree
   * @tparam Comp   A Strict_weak_order on node pointers
   */
  template<typename T, typename Comp>
    struct binomial_tree_compare : private Comp
    {
      typedef Comp base_comp;

      binomial_tree_compare(Comp c)
        : base_comp{c}
      { }

      bool operator()(T const& p, T const& q) const
      {
        // Note that the first condition covers the case when both p and q
        // are nullptr. That's also false in order to preserve the strict weak
        // ordering.
        if(p.empty()) 
          return false;   // comp(nullptr, *) == false
        else if(q.empty()) 
          return true;    // comp(*, nullptr) == true
        else 
          return base_comp::operator()(p.root(), q.root());
      }
    };

  // FIXME: Actually use the allocator
  /**
   * The binomial heap data structure is a well-know implementation of a 
   * mergeable heap; a heap that can be merged efficiently (in O(lg n) time)
   * with other heaps.
   * 
   * @tparam T      A Regular type; the value type
   * @tparam Comp   A Strict_weak_order on T
   * @tparam Alloc  An allocator of T
   */
  template<typename T, 
           typename Comp = std::less<T>, 
           typename Alloc = std::allocator<T>> 
    class binomial_heap 
    {
      typedef binomial_tree_node<T> node_type;
      typedef binomial_tree<T, node_type, Alloc> tree_type;
      typedef std::vector<tree_type> tree_list;
    public:
      typedef Alloc allocator_type;
      
      typedef T value_type;
      typedef typename Alloc::reference reference;
      typedef typename Alloc::const_reference const_reference;
      typedef typename Alloc::size_type size_type;
      
      typedef Comp value_compare;
    private:
      // We use two additional comparison operators. The root compare compares
      // two non-empty trees. The tree compare extends that ordering to empty
      // trees.
      typedef binomial_node_compare<node_type, value_compare> node_compare;
      typedef binomial_tree_compare<tree_type, node_compare> tree_compare;
    public:

      /** @name Initialization */
      //@{
      /**
       * @brief Default constructor
       */
      binomial_heap(value_compare const& comp = value_compare{},
                    allocator_type const& alloc = allocator_type{})
        : trees_{alloc}, size_{0}, comp_{comp}
      { }

      /**
       * @brief Copy constructor
       */
      binomial_heap(binomial_heap const& x)
        : trees_{x.trees_}, size_{x.size_}, comp_{x.value_comp()}
      { }

      binomial_heap& operator=(binomial_heap const& x)
      {
        binomial_heap tmp{x};
        swap(x);
        return *this;
      }
      
      /**
       * @brief Move constructor
       */
      binomial_heap(binomial_heap&& x)
        : trees_{std::move(x.trees_)}, size_{x.size_}, comp_{x.comp_}
      {
        x.size_ = 0;  // x should be empty after moving
      }

      binomial_heap& operator=(binomial_heap&& x)
      {
        binomial_heap tmp{std::move(x)};
        swap(tmp);
        return *this;
      }
      //@}

      /** @name Properties */
      //@{
      /**
       * Return true if there are no elements in the heap.
       */
      bool empty() const
      {
        return size_ == 0;
      }

      /**
       * Return the number of elements in the heap.
       */
      size_type size() const
      {
        return size_;
      }
      
      /**
       * Return the function used to compare heap values.
       */
      value_compare value_comp() const
      {
        return comp_;
      }
      
      allocator_type get_allocator() const
      {
        return allocator_type{};
      }
      //@}

      /** @name Heap operations */
      //@{
      /**
       * Return the value at the top of the heap.
       */
      const_reference top() const
      {
        return search()->root()->value;
      }

      /**
       * Insert a new value into the heap.
       */
      void push(value_type const&);
      void push(value_type&&);
    
      template<typename... Args>
        void emplace(Args&&... x);
      
      /**
       * Remove the top element from the heap.
       */
      void pop();
      value_type displace();

      void merge(binomial_heap& x);
      //@}

      /** @name Object operations */
      //@{
      void clear()
      {
        trees_.clear();
      }
      
      void swap(binomial_heap& x)
      {
        std::swap(trees_, x.trees_);
        std::swap(size_, x.size_);
        std::swap(comp_, x.comp_);
      }
      //@}

    private:
      // Return the root node comparison function
      node_compare get_node_compare() const
      {
        return node_compare{value_comp()};
      }
      
      // Return the extended tree comparison function
      tree_compare get_tree_compare() const
      {
        return tree_compare{value_comp()};
      }
      
      // Compare two tree nodes.
      bool compare(node_type const* p, node_type const* q) const
      {
        assert(( p && q ));
        return get_node_compare()(p, q);
      }
      
      // Search for the top element, returning an iterator.
      typename tree_list::iterator search()
      {
        assert(( !empty() ));
        return std::min_element(begin(trees_), end(trees_), get_tree_compare());
      }

      typename tree_list::const_iterator search() const
      {
        assert(( !empty() ));
        return std::min_element(begin(trees_), end(trees_), get_tree_compare());
      }

      void link(tree_type& x, tree_type& y);
      void merge(tree_list& x, tree_list& y);

    private:
      tree_list trees_;     // A vector of trees indexed by tree order
      size_type size_;      // Number of objects in the heap
      value_compare comp_;  // The heap-ordering function
    };

  // Link the trees l and r such that l becomes the new root and that r is 
  // empty after the operation.
  template<typename T, typename Comp, typename Alloc>
    void binomial_heap<T, Comp, Alloc>::link(tree_type& l, tree_type& r) 
    {
      using std::swap;

      assert(( !l.empty() && !r.empty() ));

      if(compare(r.root(), l.root()))
        swap(l, r);
      l.link(r);
    }
  
  // Merge the root lists of two different trees. The tree list r should
  // contain no trees after merging.
  template<typename T, typename Comp, typename Alloc>
    void binomial_heap<T, Comp, Alloc>::merge(tree_list& l, tree_list& r) 
    {
      // Results are computed out-of-place and will be moved into the l.
      tree_list result;

      // Simplify the computation by ensuring the two forest have the same
      // order.
      // FIXME: Don't resize existing heaps! It shouldn't be too hard write
      // without resizing.
      size_t n = std::max(l.size(), r.size());
      l.resize(n);
      r.resize(n);

      // Accumulate addends. Initially reserve all the capacity it's going to
      // use so we don't repeatedly allocate memory.
      tree_list tmp;
      tmp.reserve(4);

      tree_type carry;
      for(size_t i = 0; i < n; ++i) {
        // Create a list of all non-null addends by actually moving them from
        // their source trees.
        tmp.clear();
        if(!carry.empty())
          tmp.push_back(std::move(carry));
        if(!l[i].empty())
          tmp.push_back(std::move(l[i]));
        if(!r[i].empty())
          tmp.push_back(std::move(r[i]));
      
        // Add the trees in the temporary buffer, possibly carrying any left
        // over. There are four cases.
        switch(tmp.size()) {
          case 0:
            // With 0 addends, push an empty tree at order i
            result.push_back(tree_type{});
            break;
          case 1:
            // With a single addend, the result is that tree at order i
            result.push_back(std::move(tmp[0]));
            break;
          case 2:
            // With two addends, the result is a null tree and a we carry the
            // linked result
            result.push_back(tree_type{});
            link(tmp[0], tmp[1]);
            carry = std::move(tmp[0]);
            break;
          case 3:
            // With three addends, the result is (any) one, and we carry the
            // merged remaining trees.
            result.push_back(std::move(tmp[0]));
            link(tmp[1], tmp[2]);
            carry = std::move(tmp[1]);
            break;
        }
      }

      // Finally, add the carry to the result.
      if(!carry.empty())
        result.push_back(std::move(carry));

      // Reset the previous r (we've moved everything out) and set the
      // the l to the new result.
      r.clear();
      l = std::move(result);
    }

  template<typename T, typename C, typename A>
    void binomial_heap<T, C, A>::push(value_type const& value) 
    {
      // Create a singleton forest and merge it into the heap.
      tree_list tmp = {tree_type{value}};
      merge(trees_, tmp);
      ++size_;
    }

  template<typename T, typename C, typename A>
    void binomial_heap<T, C, A>::push(value_type&& value) 
    {
      // Create a singleton forest and merge it into the heap.
      tree_list tmp = {tree_type{std::move(value)}};
      merge(trees_, tmp);
      ++size_;
    }

  template<typename T, typename C, typename A>
    template<typename... Args>
      void binomial_heap<T, C, A>::emplace(Args&&... args) 
      {
        // FIXME: This is currently broken, pending a fix in binomial_tree.
        assert(( false ));
      }

  template<typename T, typename C, typename A>
    void binomial_heap<T, C, A>::merge(binomial_heap& x) 
    {
      merge(trees_, x.trees_);
      size_ += x.size_;
      x.size_ = 0;
    }

  template<typename T, typename C, typename A>
    void binomial_heap<T, C, A>::pop() 
    {
      // Find the top of the heap and explode the tree into a list of children.
      // Note that we have to reverse the list in order to have it in the
      // right order. Then, reset the tree at this level.
      auto iter = search();
      auto kids = iter->explode();
      std::reverse(begin(kids), end(kids));
      iter->clear();

      // FIXME: Note we aren't actually reducing memory usage here because
      // a) shrinking the vector won't actually release used memory, and b)
      // it's a relatively small amount of memory compared to the number of
      // objects that could otherwise be contained (i.e., 2^n). If the vector
      // is 32 elements wide (32 pointers), it could hold 4 billion objects.

      // Merge the children back into the heap and decrement the size.
      merge(trees_, kids);
      --size_;
    }

  // FIXME: Actually use the allocator
  /**
   * The mutable binomial heap...
   * 
   * @tparam T      A Regular type; the value type
   * @tparam Comp   A Strict_weak_order on T
   * @tparam Alloc  An allocator of T
   */
  template<typename T, 
           typename Comp = std::less<T>, 
           typename Alloc = std::allocator<T>,
           typename Map = std::unordered_map<T, mutable_binomial_tree_node<T>*>>
    class mutable_binomial_heap 
    {
      typedef mutable_binomial_tree_node<T> node_type;
      typedef binomial_tree<T, node_type, Alloc> tree_type;
      typedef std::vector<tree_type> tree_list;
      typedef Map map_type;
    public:
      typedef Alloc allocator_type;
      
      typedef T value_type;
      typedef typename Alloc::reference reference;
      typedef typename Alloc::const_reference const_reference;
      typedef typename Alloc::size_type size_type;
      
      typedef Comp value_compare;
    private:
      // We use two additional comparison operators. The root compare compares
      // two non-empty trees. The tree compare extends that ordering to empty
      // trees.
      typedef binomial_node_compare<node_type, value_compare> node_compare;
      typedef binomial_tree_compare<tree_type, node_compare> tree_compare;
    public:

      /** @name Initialization */
      //@{
      /**
       * @brief Default constructor
       */
      mutable_binomial_heap(value_compare const& comp = value_compare{},
                    allocator_type const& alloc = allocator_type{})
        : trees_{alloc}, size_{0}, comp_{comp}
      { }

      /**
       * @brief Copy constructor
       */
      mutable_binomial_heap(mutable_binomial_heap const& x)
        : trees_{x.trees_}, size_{x.size_}, comp_{x.value_comp()}
      { }

      mutable_binomial_heap& operator=(mutable_binomial_heap const& x)
      {
        mutable_binomial_heap tmp{x};
        swap(x);
        return *this;
      }
      
      /**
       * @brief Move constructor
       */
      mutable_binomial_heap(mutable_binomial_heap&& x)
        : trees_{std::move(x.trees_)}, size_{x.size_}, comp_{x.comp_}
      {
        x.size_ = 0;  // x should be empty after moving
      }

      mutable_binomial_heap& operator=(mutable_binomial_heap&& x)
      {
        mutable_binomial_heap tmp{std::move(x)};
        swap(tmp);
        return *this;
      }
      //@}

      /** @name Properties */
      //@{
      /**
       * Return true if there are no elements in the heap.
       */
      bool empty() const
      {
        return size_ == 0;
      }

      /**
       * Return the number of elements in the heap.
       */
      size_type size() const
      {
        return size_;
      }
      
      /**
       * Return the function used to compare heap values.
       */
      value_compare value_comp() const
      {
        return comp_;
      }
      
      allocator_type get_allocator() const
      {
        return allocator_type{};
      }
      //@}

      /** @name Heap operations */
      //@{
      /**
       * Return the value at the top of the heap.
       */
      const_reference top() const
      {
        return search()->root()->value;
      }

      /**
       * Insert a new value into the heap.
       */
      void push(value_type const&);
      void push(value_type&&);
    
      template<typename... Args>
        void emplace(Args&&... x);
      
      /**
       * Remove the top element from the heap.
       */
      void pop();
      value_type displace();

      void merge(mutable_binomial_heap& x);
      //@}

    
      /** @name Mutable heap operations */
      //@{
      /**
       * Update the position of the given value within the heap.
       */
      void update(const value_type& x);
      
      /**
       * Indicate that the given value should move toward the top of the heap,
       * that its values has increased (w.r.t., the heap order).
       */
      void increase(const value_type& x)
      {
        assert(( get(x)->value == x ));
        up_heap(get(x));
      }

      /**
       * Indicate that the given value should move away from the top of the
       * heap, that its value has decreased (w.r.t., the heap order).
       */
      void decrease(const value_type& x)
      {
        assert(( get(x)->value == x ));
        down_heap(get(x));
      }
      //@}
    
      /** @name Object operations */
      //@{
      void clear()
      {
        trees_.clear();
      }
      
      void swap(mutable_binomial_heap& x)
      {
        std::swap(trees_, x.trees_);
        std::swap(size_, x.size_);
        std::swap(comp_, x.comp_);
      }
      //@}

    private:
      // Return the node comparison function
      node_compare get_node_compare() const
      {
        return node_compare{value_comp()};
      }
      
      // Return the extended tree comparison function
      tree_compare get_tree_compare() const
      {
        return tree_compare{value_comp()};
      }
      
      // Compare two tree nodes using the node comparator
      bool compare(node_type const* p, node_type const* q) const
      {
        return get_node_compare()(p, q);
      }
      
      // Search for the top element, returning an iterator.
      typename tree_list::iterator search()
      {
        assert(( !empty() ));
        return std::min_element(begin(trees_), end(trees_), get_tree_compare());
      }

      typename tree_list::const_iterator search() const
      {
        assert(( !empty() ));
        return std::min_element(begin(trees_), end(trees_), get_tree_compare());
      }
      
      void link(tree_type& x, tree_type& y);
      void merge(tree_list& x, tree_list& y);

      // Return the node associated with the value.
      node_type *get(value_type const& x)
      {
        assert(( map_.count(x) != 0 ));
        return map_[x];
      }
      
      // Bubble a value up or down the heap. Return true if the indicated
      // value changed nodes.
      bool up_heap(node_type* p);
      bool down_heap(node_type* p);
      
      // Swap the values of the given nodes and update their positions in the
      // map.
      void exchange(node_type* p, node_type* q)
      {
        using std::swap;
        swap(p->value, q->value);
        map_[p->value] = p;
        map_[q->value] = q;
      }

    private:
      tree_list trees_;     // A vector of trees indexed by tree order
      size_type size_;      // Number of objects in the heap
      value_compare comp_;  // The heap-ordering function
      map_type map_;        // Mapping of values to nodes
    };

  // Link the trees l and r such that l becomes the new root and that r is 
  // empty after the operation.
  template<typename T, typename Comp, typename Alloc, typename Map>
    void mutable_binomial_heap<T, Comp, Alloc, Map>::link(tree_type& l, tree_type& r) 
    {
      using std::swap;

      assert(( !l.empty() && !r.empty() ));

      if(compare(r.root(), l.root()))
        swap(l, r);
      l.link(r);
    }
  
  // Merge the root lists of two different trees. The tree list r should
  // contain no trees after merging.
  template<typename T, typename Comp, typename Alloc, typename Map>
    void mutable_binomial_heap<T, Comp, Alloc, Map>::merge(tree_list& l, tree_list& r) 
    {
      // Results are computed out-of-place and will be moved into the l.
      tree_list result;

      // Simplify the computation by ensuring the two forest have the same
      // order.
      // FIXME: Don't resize existing heaps! It shouldn't be too hard write
      // without resizing.
      size_t n = std::max(l.size(), r.size());
      l.resize(n);
      r.resize(n);

      // Accumulate addends. Initially reserve all the capacity it's going to
      // use so we don't repeatedly allocate memory.
      tree_list tmp;
      tmp.reserve(4);

      tree_type carry;
      for(size_t i = 0; i < n; ++i) {
        // Create a list of all non-null addends by actually moving them from
        // their source trees.
        tmp.clear();
        if(!carry.empty())
          tmp.push_back(std::move(carry));
        if(!l[i].empty())
          tmp.push_back(std::move(l[i]));
        if(!r[i].empty())
          tmp.push_back(std::move(r[i]));
      
        // Add the trees in the temporary buffer, possibly carrying any left
        // over. There are four cases.
        switch(tmp.size()) {
          case 0:
            // With 0 addends, push an empty tree at order i
            result.push_back(tree_type{});
            break;
          case 1:
            // With a single addend, the result is that tree at order i
            result.push_back(std::move(tmp[0]));
            break;
          case 2:
            // With two addends, the result is a null tree and a we carry the
            // linked result
            result.push_back(tree_type{});
            link(tmp[0], tmp[1]);
            carry = std::move(tmp[0]);
            break;
          case 3:
            // With three addends, the result is (any) one, and we carry the
            // merged remaining trees.
            result.push_back(std::move(tmp[0]));
            link(tmp[1], tmp[2]);
            carry = std::move(tmp[1]);
            break;
        }
      }

      // Finally, add the carry to the result.
      if(!carry.empty())
        result.push_back(std::move(carry));

      // Reset the previous r (we've moved everything out) and set the
      // the l to the new result.
      r.clear();
      l = std::move(result);
    }

  template<typename T, typename Comp, typename Alloc, typename Map>
    void mutable_binomial_heap<T, Comp, Alloc, Map>::push(value_type const& x) 
    {
      // Create a singleton forest and merge it into the heap.
      tree_list tmp = {tree_type{x}};
      map_[x] = tmp[0].root();
      merge(trees_, tmp);
      ++size_;
    }

  template<typename T, typename Comp, typename Alloc, typename Map>
    void mutable_binomial_heap<T, Comp, Alloc, Map>::push(value_type&& value) 
    {
      // Create a singleton forest and merge it into the heap.
      tree_list tmp = {tree_type{std::move(value)}};
      map_[value] = tmp[0].root();
      merge(trees_, tmp);
      ++size_;
    }

  template<typename T, typename Comp, typename Alloc, typename Map>
    template<typename... Args>
      void mutable_binomial_heap<T, Comp, Alloc, Map>::emplace(Args&&... args) 
      {
        // FIXME: This is currently broken, pending a fix in binomial_tree.
        assert(( false ));
      }

  template<typename T, typename Comp, typename Alloc, typename Map>
    void mutable_binomial_heap<T, Comp, Alloc, Map>::merge(mutable_binomial_heap& x) 
    {
      merge(trees_, x.trees_);
      size_ += x.size_;
      x.size_ = 0;
    }

  template<typename T, typename Comp, typename Alloc, typename Map>
    void mutable_binomial_heap<T, Comp, Alloc, Map>::pop() 
    {
      // Find the top of the heap and explode the tree into a list of children.
      // Note that we have to reverse the list in order to have it in the
      // right order. Then, reset the tree at this level.
      auto iter = search();
      auto kids = iter->explode();
      std::reverse(begin(kids), end(kids));
      map_.erase(iter->root()->value);
      iter->clear();

     // Merge the children back into the heap and decrement the size.
      merge(trees_, kids);
      --size_;
    }

  template<typename T, typename Comp, typename Alloc, typename Map>
    void mutable_binomial_heap<T, Comp, Alloc, Map>::update(value_type const& x)
    {
      assert(( get(x)->value == x ));
      node_type* p = get(x);
      if(!up_heap(p)) {
        down_heap(p);
      }
    }
    
  // Bubble the value up towards the top of the heap, returing true if the 
  // value moved. The weird if/do is there so that move is only assignd once.
  template<typename T, typename Comp, typename Alloc, typename Map>
    bool mutable_binomial_heap<T, Comp, Alloc, Map>::up_heap(node_type* p)
    {
      bool moved = false;
      node_type* q = p->parent;
      if(q && !compare(p, q)) {
        moved = true;
        do {
          exchange(p, q);
          p = q;
          q = p->parent;
        } while(q && !compare(p, q));
      }
      return moved;
    }

  // Bubble the vlaue down towards the bottom of the heap. Note that values
  // are only bubbled downwards to the left-most child (i.e., the largest
  // subtree).
  template<typename T, typename Comp, typename Alloc, typename Map>
    bool mutable_binomial_heap<T, Comp, Alloc, Map>::down_heap(node_type* p)
    {
      bool moved = false;
      node_type* q = p->child;
      if(q && compare(p, q)) {
        moved = true;
        do {
          exchange(p, q);
          p = q;
          q = p->child;
        } while(q && compare(p, q));
      }
      return moved;
    }

} // namespace origin

#endif
