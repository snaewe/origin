// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONTAINER_VECTOR_BASE_HPP
#define ORIGIN_CONTAINER_VECTOR_BASE_HPP

#include <cassert>

#include <origin/memory.hpp>
#include <origin/algorithm.hpp>

namespace origin
{
  // Vector base
  // The vector base implements the basic allocation logic for a dynamic array.
  // Note that the vector base is non-copyable. This is class represents a
  // resource: a block of allocated memory.
  //
  // Invariants:
  //    this->first <= this->last
  //    this->last <= this->limit
  //    is_uninitilized(this->last, this->limit)
  template <typename T>
    class vector_base
    {
    public:
      // Default constructible
      // Initialize the vector base with its default state; its first and
      // last pointers set to nullptr, and its size() is 0.
      vector_base() noexcept
        : alloc(default_allocator())
        , first {nullptr}, last {nullptr}, limit {nullptr}
      { }


      // Default with allocator
      // Default construct this object, using alloc as the allocator for
      // memory allocation and deallocation.
      explicit vector_base(allocator& a) noexcept
        : alloc(a), first {nullptr}, last {nullptr}, limit {nullptr}
      { }



      // Move semantics
      // Transfer the state of x into this vector base, and reset the state
      // of x to its default. Note that the vector base is not move-assignable.
      vector_base(vector_base&& x) noexcept;
      vector_base(vector_base&& x, allocator& a);

      vector_base& operator=(vector_base&&) = delete;



      // Copy semantics
      // A vector base is non-copyable. In order to copy a vector, allocate a
      // a new block of memory and then copy the values in the derived class.
      vector_base(const vector_base&) = delete;
      vector_base& operator=(const vector_base&) = delete;



      // Size construction
      // Initialize the vector base by allocating exactly n objects.
      vector_base(std::size_t n)
        : alloc(default_allocator())
        , first {allocate(n)}, last {first}, limit {first + n}
      { }

      // Size with allocator
      vector_base(std::size_t n, allocator& a)
        : alloc(a), first {allocate(n)}, last {first}, limit{first + n}
      { }



      // Destructible
      ~vector_base()
      {
        deallocate(first, capacity());
      }


      // Allocate and deallocate

      // Allocate n objects, returning a pointer to the first.
      T *allocate(std::size_t n)
      {
        return origin::allocate<T>(alloc, n);
      }

      // Deallocate n objects pointed to by p.
      void deallocate(T* p, std::size_t n)
      {
        origin::deallocate(alloc, p, n);
      }



      // Observers

      // Returns true if there are no initialized values in the allocated block
      // of memory.
      bool empty() const { return first == last; }

      // Returns true when every allocated object is initialized.
      bool full() const { return last == limit; }

      // Returns the number of initialized valies in the allocated block of
      // memory.
      std::size_t size() const { return last - first; }

      // Returns the total number of allocated objects in the block of memory.
      std::size_t capacity() const { return limit - first; }

      // Returns the residual capacity of the vector; the number of allocated
      // but not initialized values.
      std::size_t residual() const { return limit - last; }

      // Returns the next capacity of the vector base if the owning vector
      // is dynamically resized.
      //
      // TODO: Can I find a better name for this function?
      std::size_t next_capacity() const
      {
        return capacity() == 0 ? 4 : capacity() * 2;
      }


      // Mutators
      void swap(vector_base& x);

      T* move_to(T *first);

      template <typename I> 
        T* move_at_pos(T* pos, I first, I last);
      template <typename I> 
        void move_at_end(I first, I last);
      
      void move_at_end(vector_base<T>& x);


      template <typename I> 
        T* copy_at_pos(T* pos, I first, I last);
      template <typename I> 
        void copy_at_end(I first, I last);

      void copy_at_end(const vector_base<T>& x);


      template <typename U> 
        T* fill_at_pos(T* pos, std::size_t n, const U& value);
      template <typename U> 
        void fill_at_end(std::size_t n, const U& value);

      void erase_at_end(std::size_t n);
      void clear();


      template <typename... Args> 
        void insert(T* pos, Args&&... args);
      template <typename... Args> 
        void append(Args&&... args);

      template <typename I>
        void range_insert(T* pos, I first, I last);

      // TODO: These are good candidates for general purpose memory algorithms.
      void shift_right(T* pos, std::size_t n);
      void shift_left(T* pos);
      void shift_left(T* first, T* last);

    public:
      allocator& alloc;
      T *first;
      T *last;
      T *limit;
    };


  // Transfer the state of x into this vector base, and reset the state of x to
  // its default. Note that the vector base is not move-assignable.
  template <typename T>
    vector_base<T>::vector_base(vector_base&& x) noexcept
      : alloc(x.alloc), first {x.first}, last {x.last}, limit {x.limit}
    { 
      x.first = x.last = x.limit = nullptr;
    }

  // Initialize this object by moving the elements of x into it.
  //
  // If a == x.alloc, then data is moved by simply swapping pointers. If this is
  // not the case, then we must allocate new memory, and move the elements into
  // that, resetting x.
  //
  // This constructor may throw an exception if allocation of moving fails.
  //
  // NOTE: This is the only constructor of this class that does any value
  // initialization, which breaks the layering of the design. The alternative is
  // to have the derived vector implement this logic, which is reasonable but
  // somewhat intrusive.
  template <typename T>
    vector_base<T>::vector_base(vector_base&& x, allocator& a)
      : alloc(a), first {nullptr}, last {nullptr}, limit {nullptr}
    { 
      if (alloc == x.alloc) {
        swap(x);
      } else {
        first = allocate(x.size());
        move_at_end(x);
        x.last = x.first;
      }
    }


  // Exchange the reresentation of x with this object. The operation is only
  // valid when the allocators of this and x compare equal.
  //
  // Requires:
  //    this->alloc == x.alloc
  template <typename T>
    void vector_base<T>::swap(vector_base& x)
    {
      assert(alloc == x.alloc);
      std::swap(first, x.first);
      std::swap(last, x.last);
      std::swap(limit, x.limit);
    }



  // Move the elements of this vector base into the range pointed at by
  // result, and return a pointer past the end of the output range.
  template <typename T>
    T* vector_base<T>::move_to(T* result)
    {
      return uninitialized_move(alloc, first, last, result);
    }


  // FIXME: Document these operations.

  template <typename T>
    template <typename I>
      inline T* vector_base<T>::move_at_pos(T* pos, I first, I last)
      {
        assert(pos >= this->first && pos <= this->last);
        return uninitialized_move(this->alloc, first, last, pos);
      }

  template <typename T>
    template <typename I>
      inline void vector_base<T>::move_at_end(I first, I last)
      {
        this->last = move_at_pos(this->last, first, last);
      }

  template <typename T>
    inline void vector_base<T>::move_at_end(vector_base& x)
    {
      assert(this->last + x.size() <= this->limit);
      move_at_end(x.first, x.last);
    }


  // Copy the values in [first, last) into the uninitialized range of objects in
  // this buffer indicated by pos.
  //
  // Requires:
  //    pos >= this->first
  //    pos + (last - first) <= this->limit
  //    is_uninitilized(pos, pos + (last - first))
  template <typename T>
    template <typename I>
      inline T* vector_base<T>::copy_at_pos(T* pos, I first, I last)
      {
        assert(pos >= this->first && pos <= this->last);
        return uninitialized_copy(this->alloc, first, last, pos);
      }

  // Copy the values in [first, last) into the uninitialized range of objects at
  // the end of this buffer.
  //
  // Requires:
  //    this->last + (last - first) <= this->limit
  //    is_uninitialized(this->last, this->last + (last - first))
  template <typename T>
    template <typename I>
      inline void vector_base<T>::copy_at_end(I first, I last)
      {
        this->last = copy_at_pos(this->last, first, last);
      }

  // Copy the values in x into the uninitialized range of objects at the end of
  // this buffer.
  //
  // Requires:
  //    this->last + x.size() <= this->limit
  //    is_uninitialized(this->last, this->last + x.size())
  template <typename T>
    inline void vector_base<T>::copy_at_end(const vector_base& x)
    {
      assert(this->last + x.size() <= this->limit);
      copy_at_end(x.first, x.last);
    }

  // Fill the objects in the range [pos, pos + n) with the given value. Note
  // that this operation does not update the any of the pointers on the vector
  // base. This is assumed to have been done previously.
  //
  // Requires:
  //    Constructible<T, U>
  //    ???
  template <typename T>
    template <typename U>
      inline T* vector_base<T>::fill_at_pos(T* pos, std::size_t n, const U& value)
      {
        static_assert(Constructible<T, U>(), "");
        return uninitialized_fill_n(this->alloc, pos, n, value);
      }

  // Fill the objects of [this->last, this->last + n) with the given value and
  // update this->last to this->last + n.
  //
  // Requires:
  //    Constructible<T, U>
  //    ???
  template <typename T>
    template <typename U>
      inline void vector_base<T>::fill_at_end(std::size_t n, const U& value) 
      {
        static_assert(Constructible<T, U>(), "");
        this->last = fill_at_pos(this->last, n, value);
      }

  // Erase n elements at the end of this buffer, updating this->last to point to
  // the new last initialized value.
  //
  // Requires:
  //    n < this->size()
  template <typename T>
    inline void vector_base<T>::erase_at_end(std::size_t n)
    {
      destroy(this->alloc, this->last - n, this->last);
      this->last -= n;
    }

  // Erase all the elements in this buffer, updating the buffer so that it is
  // empty.
  template <typename T>
    inline void vector_base<T>::clear()
    {
      destroy(this->alloc, this->first, this->last);
      this->last = this->first;
    }

  // Insert or emplace args into the position indicated by pos. Note that this
  // does not update out's last pointer; it is assumed that is done when data is
  // shifted to accommodate the insertion.
  //
  // Requires:
  //    Constructible<T, Args...>
  //    is_uninitialized(pos)
  template <typename T>
    template <typename... Args>
      inline void vector_base<T>::insert(T* pos, Args&&... args)
      {
        static_assert(Constructible<T, Args...>(), "");
        this->shift_right(pos, 1);
        construct(this->alloc, &*pos, std::forward<Args>(args)...);
      }

  // Insert or emplace the given arguments into the last object of the base
  // and update the last pointer to reflect the insertion.
  //
  // Requires:
  //    Constructible<T, Args...>
  //    this->last < this->limit
  template <typename T>
    template <typename... Args>
      inline void vector_base<T>::append(Args&&... args)
      {
        static_assert(Constructible<T, Args...>(), "");
        construct(this->alloc, &*this->last, std::forward<Args>(args)...);
        ++this->last;
      }

  // Copy the values in [first, last) into the buffer. Note that the buffer
  // must have sufficient capacity to accommodate the insertion.
  //
  // Requires:
  //    this->size() + distance(first, last) <= capacity
  template <typename T>
    template <typename I>
      void vector_base<T>::range_insert(T* pos, I first, I last)
      {
        T* end = this->last;                   // The original end of the buffer
        std::size_t k = end - pos;             // The size of [pos, end)
        std::size_t n = distance(first, last); // The size of [first, last)
        if (k > n) {
          // There are fewer elements being inserted than in the remainder of
          // the buffer, so we can simply shift and copy.
          shift_right(pos, k);
          copy(first, last, pos);
        } else {
          // There are more elements being inserted than in the remainder so
          // we break up the sequence of elements being inserted, copying some
          // elements into the uninitilized buffer, moving the remainder, and
          // copying the rest over the moved elements.
          I mid = next(first, k);
          copy_at_end(mid, last);
          move_at_end(pos, end);
          copy(first, mid, pos);
        }
      }

  // Shift the elements of the buffer to the "right", creating an n-object range
  // of uninitialized objects at pos. That is, [pos, pos + n) is uninitialized
  // after the shift operation and [pos + n, pos + 2 * n) hold the elements of
  // the original range.
  template <typename T>
    void vector_base<T>::shift_right(T* pos, std::size_t n)
    {
      // Move the last - n - 1 into the uninitalized region of the buffer and
      // update last so it points to the new end.
      T* last = this->last;
      T* mid = last - n;
      this->last = uninitialized_move(alloc, mid, last, last);
      move_backward(pos, mid, last);
    }

  // Shift the elements of the buffer to the "left", effectively erasing the n
  // elements pointed to by pos. That is, the elements [pos, pos + n) are
  // replaced by those in [pos + n, pos + 2 * n).
  template <typename T>
    void vector_base<T>::shift_left(T* pos)
    {
      T* mid = pos + 1;
      if (mid != this->last)
        move(mid, this->last, pos);
      --last;
      destroy(alloc, &*this->last);
    }

  // Shift the elements of the vector left over the range [first, last).
  template <typename T>
    void vector_base<T>::shift_left(T* first, T* last)
    {
      if (first != last) {
        if (last != this->last)
          move(last, this->last, first);
        erase_at_end(this->last - last);
      }
    }

} // namespace origin

#endif
