// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_HEAP_BINARY_HEAP_HPP
#define ORIGIN_HEAP_BINARY_HEAP_HPP

#include <cassert>
#include <algorithm>

#include <origin/utility/meta.hpp>
#include <origin/utility/typestr.hpp>
#include <origin/heap/index.hpp>

namespace origin
{
  // NOTE: A note on heap ordering. The heap property, say max-heap, states
  // that the item on the top of the heap is greater than all other objects in
  // the heap. Note that when the heap is a tree, the heap property can be
  // described recursively; an element in the heap is greater than its children.
  //
  // In STL-style libraries, we tend to use less<T> as the default comparison,
  // even though max-heaps are typically the default. This leads to a subtle
  // rephrasing of the heap property. For a max heap, we say that an element in 
  // the heap is /not less/ than its children.

  // FIXME: Find a nice data structure reference for the documentation (CLR?).
  
  // FIXME: There's a lot of duplicated code and structures. It would be nice
  // unify the implementations with a base class that provided the basic tree
  // structure.
  
  
  /**
   * The binary heap data structure implements a classic, generic binary heap.
   * The order (min or max) of the heap is determined by a compare operation on
   * the value type. This defaults to std::less, resulting in max-heap.
   *
   * @tparam T      Type of data to be stored in the heap
   * @tparam Cont   The unerlying Random_Access_Container
   * @tparam Comp   A Strict_Weak_Ordering over T
   */
  template<typename T, 
           typename Comp = std::less<T>,
           typename Cont = std::vector<T>>
    class binary_heap
    {
      typedef Cont container_type;
    public:
      typedef T value_type;
      typedef Comp value_compare;
      typedef typename container_type::size_type size_type;

      /**
       * @brief Default constructor
       * Initialize an empty heap. A comparison function may optionally be given.
       *
       * @param comp  A value comparison Operation
       */
      binary_heap(value_compare const& comp = value_compare{})
        : data_{}, comp_{comp}
      { }

      // FIXME: This needs to be specialized for input and forwrad iterators.
      /**
       * @brief Range constructor
       * Initialize the heap over the sequence of elements in the range
       * [first, last). A comparison function may be optional given.
       *
       *
       * @param first   The first iterator in a range of objects
       * @param last    An iterator past the end of a range of objects
       * @param comp    A Comp function object.
       */
      template<typename ForwardIterator>
        binary_heap(ForwardIterator first,
                    ForwardIterator last,
                    value_compare const& comp = value_compare{})
          : data_{first, last}, comp_{comp}
        {
          std::make_heap(data_.begin(), data_.end(), comp_);
        }

      /**
       * @brief Initializer list constructor
       * Initialize the heap from a seuqence of objects. A comparison function
       * may be optionally given.
       *
       * @param list  An initializer list of objects of type T
       * @param comp  A Comp function object.
       */
      binary_heap(std::initializer_list<T> list, value_compare const& comp)
        : data_{list.begin(), list.end()}, comp_{comp}
        {
          std::make_heap(data_.begin(), data_.end(), comp_);
        }

      /** @name Properties */
      //@{
      /**
       * Return true if the heap has no elements.
       */
      bool empty() const
      {
        return data_.size() == 0;
      }

      /**
       * Return the number of elements in the heap.
       */
      size_type size() const
      {
        return data_.size();
      }

      /**
       * Return the value comparison operation.
       */
      value_compare value_comp() const
      {
        return comp_;
      }
      
      
      container_type const& data() const
      {
        return data_;
      }
      //@}

      /** @name Capacity */
      //@{
      /**
       * Return the current capacity of the heap. This is the number of objects
       * for which memory has been allocated, but not initialized.
       */
      size_type capacity() const
      {
        return data_.capacity();
      }

      // FIXME: What if n < size()? Do you truncate the heap? Is the heap still
      // valid after truncation? Should you never give up capacity once required?
      /**
       * Reserve additional memory for the heap.
       */
      void reserve(size_type n)
      {
        data_.reserve(n);
      }
      //@}


      /** @name Heap operations */
      //@{
      /**
       * Return the object on the top of the heap.
       */
      const value_type& top() const
      {
          return data_[0];
      }

      /**
       * Push an object onto the heap. This operation completes in O(lg n) time.
       */
      void push(value_type const& x)
      {
        data_.push_back(x);
        std::push_heap(data_.begin(), data_.end(), comp_);
      }
      
      /**
       * Move an object into the heap. This operation completes in O(lg n) time.
       */
      void push(value_type&& x)
      {
        data_.push_back(std::move(x));
        std::push_heap(data_.begin(), data_.end(), comp_);
      }
      
      /**
       * Emplace an object into the heap. This operation completes in O(lg n)
       * time.
       */
      template<typename... Args>
        void emplace(Args&&... args)
        {
          data_.emplace_back(std::forward<Args>(args)...);
          std::push_heap(data_.begin(), data_.end(), comp_);
        }

      /**
       * Pop the top object off the heap. This operation completes in O(lg n)
       * time.
       */
      void pop()
      {
        std::pop_heap(data_.begin(), data_.end(), comp_);
        data_.pop_back();
      }
      
      /**
       * Allow the top object to be moved out of the heap. After displacement,
       * the top object is in a valid but unspecified heap. The next operation
       * must be pop.
       */
      value_type displace()
      {
        return std::move(top());
      }
      //@}

    private:
      // FIXME: Use EBO to remove the overhead of the value compare function.
      container_type data_;
      value_compare comp_;
    };

  // FIXME: Consider prefixing the mutable_binary_heap with "basic_" and then
  // creating an alias that fixes the Container type as std::vector.
    
  // FIXME: Does the choice of std::vector depend on sizeof(T). If T is large,
  // we should probably be using a deque rather than a vector.
    
  /**
   * A mutable binary heap is a binary heap that allows the values in the to
   * be modified by an external program. The heap is updated to adjust the
   * location of the element within the data structure and preserve the heap
   * invariant.
   *
   * @tparam T      The value type
   * @tparam Comp   A Strict_Weak_Order on T
   * @tparam Cont   The underlying Random_Access_Container
   * @tparam Map    A Map associating T objects with Cont indexes
   */
  template<typename T, 
           typename Comp = std::less<T>, 
           typename Cont = std::vector<T>,
           typename Map = std::unordered_map<T, typename Cont::size_type>>
    class mutable_binary_heap
    {
      // FIXME: The underlying container type is not just a Random Access 
      // Container, but a Back Insertion (and erasure?) container that also
      // supports reserve. Note that the reserve can be abstracted for viable 
      // models that simply don't support it (e.g., deques). An array or
      // dynarray are not viable models because they don't support push_back.
    
      typedef Cont container_type;
      typedef Map map_type;
    public:
      typedef T value_type;
      typedef T& reference;
      typedef T const& const_reference;
      
      typedef Comp value_compare;
      
      typedef typename container_type::size_type size_type;
    public:

      /** @name Initialization */
      //@{
      /**
       * @brief Default constructor
       * Initialize an empty heap. The value compare function may be optionally 
       * given.
       *
       * @param comp  A value_compare function object
       */
      mutable_binary_heap(value_compare const& comp = value_compare{})
        : data_{}, comp_{comp}, map_{}
      { }

      // FIXME: Optimize this operation for forward iterators. Reserve first.
      // Also, is it possible to know, based on distance(first, last), the
      // eventual capacity of the map_ (for vector_maps only, of course). 
      /**
       * @brief Range constructor
       * Initialize the heap with the elements in the range [first, last).
       * 
       * @tparam Iter   A Forward_Iterator whose reference type is convertible
       * the the value type of the heap.
       * 
       * @param first   An iterator referencing the first object in the range
       * @param last    An iterator past the end of the the range
       * @param comp    A value_compare function object
       */
      template<typename Iter>
        mutable_binary_heap(Iter first,
                            Iter last,
                            value_compare const& comp = value_compare{}) 
          : data_{}, comp_{comp}, map_{}
        {
          while(first != last)
            push(*first++);
        }

      // FIXME: Implement me effeciently. See refactoring notes for the range
      // constructor above. Also note the option of reserving map capacity.
      /**
       * @brief Initializer list constructor
       * Initialize the heap with the elements from the given initializer list.
       * 
       * @param list  An initializer list of 
       * @param comp  A value_compare function object
       */
      mutable_binary_heap(std::initializer_list<T> list,
                          value_compare const& comp = value_compare{})
        : data_{}, comp_{comp}, map_{}
      {
        for(value_type const& x : list)
          push(x);
      }
      //@}

      /** @name Properties */
      //@{
      /**
       * Return true if the heap has no elements.
       */
      bool empty() const
      {
        return data_.empty();
      }

      /**
       * Return the number of elements in the heap.
       */
      size_type size() const
      {
        return data_.size();
      }

      /**
       * Return the value comparison function.
       */
      value_compare value_comp() const
      {
        return comp_;
      }

      // FIXME: I think that this should probably go away.
      /**
       * Return a reference to the underlying container.
       */
      container_type const& data() const
      {
        return data_;
      }
      //@}

      /** @name Capacity */
      //@{
      /**
       * Return the capacity allocated to the heap.
       */
      size_type capacity() const
      {
          return data_.capacity();
      }

      /**
       * Reserve additional capacity for the heap.
       */
      void reserve(size_type n)
      {
          data_.reserve(n);
      }
      //@}

      /** @name Heap operations */
      //@{
      /**
       * Return the element on the top of the heap.
       */
      value_type const& top() const
      {
          return data_[0];
      }

      /**
       * Push the given element onto the heap. The operation completes in
       * O(lg n) time.
       */
      void push(const value_type& x);

      /**
       * Move an object into the heap. This operation completes in O(lg n) time.
       */
      void push(value_type&& x);
      
      /**
       * Emplace an object into the heap. This operation completes in O(lg n)
       * time.
       */
      template<typename... Args>
        void emplace(Args&&... args);

      /**
       * Pop the top element from the heap. This operation completes in
       * O(lg n) time.
       */
      void pop();
      
      /**
       * Allow the top object to be moved out of the heap. After displacement,
       * the top object is in a valid but unspecified heap. The next operation
       * must be pop.
       */
      value_type displace()
      {
        return top();
      }
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
        assert(( get(index(x)) == x ));
        up_heap(index(x));
      }

      /**
       * Indicate that the given value should move away from the top of the
       * heap, that its value has decreased (w.r.t., the heap order).
       */
      void decrease(const value_type& x)
      {
        assert(( get(index(x)) == x ));
        down_heap(index(x));
      }
      //@}

    private:
      // Return the object at the given index.
      value_type& get(size_type n)
      {
        assert(( !empty() && n < size() ));
        return data_[n];
      }
      
      value_type const& get(size_type n) const
      {
        assert(( !empty() && n < size() ));
        return data_[n];
      }
      
      // Return the index of the given value. The value must be in the heap.
      size_type index(value_type const& x) const
      {
        assert(( map_.count(x) != 0 ));
        return (*map_.find(x)).second;
      }
      
      // Return true if n is the root index (0).
      static bool is_root(size_type n)
      {
        return !n;
      }
      
      // Return the parent index of n. The parent of 0 is 0.
      static size_type parent(size_type n)
      { 
        return is_root(n) ? 0 : (n - 1) / 2; 
      }
      
      // Return the left child index of n.
      static size_type left(size_type n)
      { 
        return n * 2 + 1; 
      }

      // Return the right child index of n.
      static size_type right(size_type n)
      { 
        return n * 2 + 2; 
      }
      
      // Return true if the elements at m and n violate the heap order. 
      bool less(size_type m, size_type n) const
      {
        return comp_(get(m), get(n));
      }
      
      // Return true if the elements at m and n observe the heap order.
      bool not_less(size_type m, size_type n) const
      {
        return !less(m, n);
      }

      // Swap two elements in the heap and exchange their indexes.
      void exchange(size_type m, size_type n)
      {
        std::swap(get(m), get(n));
        map_[get(m)] = m;
        map_[get(n)] = n;
      }

      size_type up_heap(size_type n);
      size_type down_heap(size_type n);

    private:
      container_type data_;
      value_compare comp_;
      map_type map_;
    };

  // Bubble the element at the index n up the heap. Return the new index after
  // bubbling. If the heap is already valid, then up_heap(n) == n.
  template<typename T, typename Comp, typename Map, typename Alloc>
    auto mutable_binary_heap<T, Comp, Map, Alloc>::up_heap(size_type n) -> size_type
    {
      while(!is_root(n)) {
        // If the heap order is violated (p < n), swap and repeat.
        size_type p = parent(n);
        if(less(p, n)) {
          exchange(n, p);
          n = p;
        } else
          break;
      }
      return n;
    }
  
  // Bubble the object at the given index down the heap. Return the new index 
  // of the bubbled element. If the heap is alread valid then down_heap(n) == n.
  template<typename T, typename Comp, typename Map, typename Alloc>
    auto mutable_binary_heap<T, Comp, Map, Alloc>::down_heap(size_type n) -> size_type
    {
      size_t c = left(n);
      while(c < size()) {
        // Find the greatest (not least) child to compare and swap.
        if(c < (size() - 1) && less(c, c + 1))
          ++c;
        
        // Swap if the heap order is violated (n < c).
        if(less(n, c)) {
          exchange(n, c);
          n = c;
          c = left(c);
        } else
          break;
      }
      return n;
    }
    
  template<typename T, typename Comp, typename Map, typename Alloc>
    void mutable_binary_heap<T, Comp, Map, Alloc>::push(value_type const& x)
    {
      // Push element into the heap structure
      size_type n = data_.size();
      data_.push_back(x);
      map_[x] = n;
      
      // Adjust the heap.
      up_heap(n);
    }

  template<typename T, typename Comp, typename Map, typename Alloc>
    void mutable_binary_heap<T, Comp, Map, Alloc>::push(value_type&& x)
    {
      // Push element into the heap structure
      size_type n = data_.size();
      data_.push_back(std::move(x));
      map_[x] = n;
      
      // Adjust the heap.
      up_heap(n);
    }

  template<typename T, typename Comp, typename Map, typename Alloc>
    template<typename... Args>
    void mutable_binary_heap<T, Comp, Map, Alloc>::emplace(Args&&... args)
    {
      // Push element into the heap structure
      size_type n = data_.size();
      data_.emplace_back(std::forward<Args>(args)...);
      map_[data_.back()] = n;
      
      // Adjust the heap.
      up_heap(n);
    }

  template<typename T, typename Comp, typename Map, typename Alloc>
    void mutable_binary_heap<T, Comp, Map, Alloc>::pop()
    {
      // Swap root with last element and erase the old root.
      exchange(0, data_.size() - 1);
      map_.erase(data_.back());
      data_.pop_back();

      // Preserve the heap invariant.
      if(!empty()) {
        down_heap(0);
      }
    }

  template<typename T, typename Comp, typename Map, typename Alloc>
    void mutable_binary_heap<T, Comp, Map, Alloc>::update(value_type const& x)
    {
      assert(( get(index(x)) == x ));

      // Try to adjust up the heap first. If the updated element is valid
      // w.r.t it's parent, try to bubble down.
      size_type n = index(x);
      if(up_heap(n) == n) {
        down_heap(n);
      }
    }

} // namespace origin


#endif // ORIGIN_HEAPS_BINARY_HEAP_HPP
