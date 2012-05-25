// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MEMORY_HPP
#define ORIGIN_MEMORY_HPP

#include <cstdlib>
#include <utility>

#include <origin/concepts.hpp>
#include <origin/algorithm.hpp>
#include <origin/memory/allocation.hpp>
#include <origin/memory/temporary.hpp>

namespace origin
{
  // Initialization and destruction
  // The construct(alloc, x, args...) and destroy(alloc, x) algorithms in this
  // module take an allocator argument so that they may be overloaded for
  // specific allocator types. For basic allocators, no overloads should be
  // required.
  //
  // TODO: Should these allocators be in with allocation.hpp?



  // Trivial uninitialized copy
  // Returns true when the object pointed at by an output iterator of type O
  // is trivially copy initialized with the value T.
  //
  // FIXME: It seems probable that O needs to be a forward iterator or have
  // some other property that enables me to reason about its value type. Would
  // a zip iterator satisfy these requirements? I'm not sure.
  //
  // FIXME: The actual requirements should be Trivially_constructible, but
  // the corresponding type trait has not been implemented. For now, we ensure
  // that Value_type<O> is trivial and constructible over T.
  template <typename O, typename T>
    constexpr bool Trivial_uninitialized_copy()
    {
      return Trivial<Value_type<O>>() && Constructible<Value_type<O>, const T&>();
    }



  // Trivial uninitialized move
  // Returns true when the object pointed at by an output iterator of type O
  // trivially move initialized with the value T and does not propagate exceptions.
  //
  // FIXME: It seems probable that O needs to be a forward iterator or have
  // some other property that enables me to reason about its value type. Would
  // a zip iterator satisfy these requirements? I'm not sure.
  //
  // FIXME: The actual requirements should be Trivially_constructible, but
  // the corresponding type trait has not been implemented. For now, we ensure
  // that Value_type<O> is trivial and constructible over T.
  template <typename O, typename T>
    constexpr bool Trivial_uninitialized_move()
    {
      return Trivial<Value_type<O>>() && Constructible<Value_type<O>, T&&>();
    }



  // Construct
  // Initialize the allocated object pointed to by p using the given arguments,
  // which are forwarded to an appropriate constructor. This operation may be
  // overloaded for custom allocators or types.
  //
  // Template parameters:
  //    Alloc   - An Allocator type
  //    T       - The type of object being initialized
  //    Args... - The types of arguments forwarded to T's constructor
  //
  // Parameters:
  //    alloc   - An allocator object; unused in this definition
  //    x       - A reference to an uninitialized object being constructed
  //    args... - Arguments forwarded to T's constructor
  //
  // FIXME: If T is "allocator-friendly", then we should propagate the allocator
  // reference to the initialized class. I believe, but am not  certain that any
  // allocator-friendly class will have, for every constructor, an equivalent
  // constructor that takes an additional allocator.
  //
  // FIXME: Rewrite in terms of the allocator's pointer type.
  template <typename Alloc, typename T, typename... Args>
    inline void construct(Alloc& alloc, T* p, Args&&... args)
    {
      static_assert(Constructible<T, Args...>(), "");
      new (p) T(std::forward<Args>(args)...);
    }



  // Destroy
  // Destroy the object referred to by x, but do not deallocate its memory.
  // This operation may be overloaded for custom allocators or types.
  //
  // Template parameters:
  //    Alloc   - An Allocator type
  //    T       - The type of object being destroyed
  //
  // Parameters:
  //    alloc   - An allocator object; unused in this definition
  //    x       - A reference to an initialized object being destroyed
  //
  // FIXME: Rewrite using Alloc's pointer type.
  template <typename Alloc, typename T>
    inline void destroy(Alloc& alloc, T* p)
    {
      // static_assert(Destructible<T>(), "");
      p->~T();
    }



  // Destroy (range)
  // Destroy the objects in the range [first, last). Note that this does not
  // deallocate memory.
  //
  // FIXME: Use alloc's addressof function to get the address of *first.
  template <typename Alloc, typename I>
    inline auto destroy(Alloc& alloc, I first, I last)
      -> Requires<!Trivially_destructible<Value_type<I>>(), void>
    {
      // static_assert(Input_iterator<I>(), "");
      // assert(is_bounded_range(first, last));
      while (first != last) {
        destroy(alloc, &*first);
        ++first;
      }
    }



  // Destroy (range)
  // If the value type being destroyed is trivially destructible, then don't
  // do anything.
  //
  // FIXME: What if if the destructor needs to do house-keeping for these
  // types too. It seems like the notion of triviality may depend on the 
  // allocator in addition to the type. That seems less than ideal.
  template <typename Alloc, typename I>
    inline auto destroy(Alloc& alloc, I first, I last)
      -> Requires<Trivially_destructible<Value_type<I>>(), void>
    { }



  // Uninitialized copy step
  // Initialize the object pointed to by result with the value of the object
  // pointed to by iter, incrementing both iter and result.
  template <typename Alloc, typename I, typename O>
    inline void uninitialized_copy_step(Alloc& alloc, I& iter, O& result)
    {
      construct(alloc, &*result, *iter);
      ++iter;
      ++result;
    }



  // Uninitialized copy
  // Copy each value in the input range [first, last) into an uninitialized 
  // object in the output range [result, ...).
  //
  // NOTE: When the object type pointed to by O can be nothrow-constructed
  // over the value type of I, then this operation is equivalent to
  // copy(first, last, result).
  template <typename Alloc, typename I, typename O>
    inline auto uninitialized_copy(Alloc& alloc, I first, I last, O result)
      -> Requires<!Trivial_uninitialized_copy<O, Value_type<I>>(), O>
    {
      I init = first;
      try {
        while (first != last)
          uninitialized_copy_step(alloc, first, result);
      } catch (...) {
        destroy(alloc, init, first);
        throw;
      }
      return result;
    }

  template <typename Alloc, typename I, typename O>
    inline auto uninitialized_copy(Alloc& alloc, I first, I last, O result)
      -> Requires<Trivial_uninitialized_copy<O, Value_type<I>>(), O>
    {
      return copy(first, last, result);
    }



  // Uninitialized copy n.
  //
  // Note: When the object type pointed to by O can be nothrow-constructed
  // over the value type of I, then this operation is equivalent to
  // copy_n(first, n, result).
  //
  // FIXME: This needs to return pair<I, O>, but I need to implement pair 
  // first.
  template <typename Alloc, typename I, typename O>
    inline auto uninitialized_copy_n(Alloc& alloc, I first, std::size_t n, O result)
      -> Requires<!Trivial_uninitialized_copy<O, Value_type<I>>(), O>
    {
      I init = first;
      try {
        while(n != 0) {
          uninitialized_copy_step(alloc, first, result);
          --n;
        }
      } catch (...) {
        destroy(alloc, init, first);
        throw;
      }
      return result;
    }

  template <typename Alloc, typename I, typename O>
    inline auto uninitialized_copy_n(Alloc& alloc, I first, std::size_t n, O result)
      -> Requires<Trivial_uninitialized_copy<O, Value_type<I>>(), O>
    {
      return copy_n(first, first + n, result);
    }



  // Uninitialized move
  //
  // For these algorithms, Value_type<I> is not nothrow-constructible, but is
  // constructible, then the algorithm will actually copy instead of moving.
  // This helps preserve the strong exception safety. This algorithm is still
  // allowed to throw, but it won't invalidate the source data if it does.
  //


  // Uninitialized mvoe step
  // Initialize the object pointed to by result by moving the representation of
  // iter's object into it. Increment both iter and result.
  template <typename Alloc, typename I, typename O>
    inline void uninitialized_move_step(Alloc& alloc, I& iter, O& result)
    {
      construct(alloc, &*result, std::move_if_noexcept(*iter));
      ++iter;
      ++result;
    }



  // Uninitialized move
  // Move the elements in [first, last) into the uninitialized range of pointed
  // to by result.
  //
  // Template parameters:
  //    Alloc -- An Allocator type
  //    I -- An Input_iterator type
  //    O -- An Move_output_iterator type
  //
  // Parameters:
  //    alloc -- An Allocator
  //    first -- The first iterator in the input range
  //    last -- The last iterator in the input range
  //    result -- The first iterator in the output range
  //
  // Returns:
  //    The last iterator in the output range
  //
  // Note:  
  // When the object type pointed to by O can be nothrow-constructed over the
  // value type of I, then this operation is equivalent to move(first, last, 
  // result).
  //
  // FIXME: Write the type requirements of this algorithm.
  template <typename Alloc, typename I, typename O>
    inline auto uninitialized_move(Alloc& alloc, I first, I last, O result)
      -> Requires<!Trivial_uninitialized_move<O, Value_type<I>>(), O>
    {
      I init = first;
      try {
        while (first != last)
          uninitialized_move_step(alloc, first, result);
      } catch (...) {
        destroy(alloc, init, first);
        throw;
      }
      return result;
    }

  template <typename Alloc, typename I, typename O>
    inline auto uninitialized_move(Alloc& alloc, I first, I last, O result)
      -> Requires<Trivial_uninitialized_move<O, Value_type<I>>(), O>
    {
      return move(first, last, result);
    }



  // Uninitialzed move n
  // Move the elemnts in [first, first + n) into the region of uninitialized
  // range pointed to by result.
  //
  // Template parameters:
  //
  // Parameters:
  //
  // Return:
  //
  // TODO: Consider losening the restrictions on the optimizations. As far as
  // I can tell, we only need Nothrow_constructible<O, Value_type<I>&&>.
  template <typename Alloc, typename I, typename O>
    inline auto uninitialized_move_n(Alloc& alloc, I first, std::size_t n, O result)
      -> Requires<!Trivial_uninitialized_move<O, Value_type<I>>(), O>
    {
      I init = first;
      try {
        while (n != 0) {
          uninitialized_move_step(alloc, first, result);
          --n;
        }
      } catch (...) {
        destroy(alloc, init, first);
        throw;
      }
      return result;
    }

  // Optimization moving 
  template <typename Alloc, typename I, typename O>
    inline auto uninitialized_move_n(Alloc& alloc, I first, std::size_t n, O result)
      -> Requires<Trivial_uninitialized_move<O, Value_type<I>>(), O>
    {
      return move(first, first + n, result);
    }




  // Uninitialized fill step
  // Initialize the un-initialized object pointed to by iter with the given
  // value. Increment iter.
  template <typename Alloc, typename I, typename T>
    inline void uninitialized_fill_step(Alloc& alloc, I& iter, const T& value)
    {
      construct(alloc, &*iter, value);
      ++iter;
    }



  // Uninitialized fill
  // Initialize each object in [first, last) with the specified value.
  //
  // FIXME: Optimize where we can use memset.
  // 
  // FIXME: This algorithm does not work on iterators whose result types are
  // not references. This should be reflected in the type requirements.
  template <typename Alloc, typename I, typename T>
    inline auto uninitialized_fill(Alloc& alloc, I first, I last, const T& value)
      -> Requires<!Trivial_uninitialized_copy<I, T>(), void>
    {
      I init = first;
      try {
        while (first != last)
          uninitialized_fill_step(first, value);
      } catch(...) {
        destory(alloc, init, first);
        throw;
      }
    }

  template <typename Alloc, typename I, typename T>
    inline auto uninitialized_fill(Alloc& alloc, I first, I last, const T& value)
      -> Requires<Trivial_uninitialized_copy<I, T>(), void>
    {
      fill(first, last, value);
    }



  // Uninitialized fill n
  // Initialize each object in the range [first, first + n) with a copy of
  // value.
  template <typename Alloc, typename I, typename T>
    inline auto uninitialized_fill_n(Alloc& alloc, I first, std::size_t n, const T& value)
      -> Requires<!Trivial_uninitialized_copy<I, T>(), I>
    {
      I init = first;
      try {
        while (n != 0) {
          uninitialized_fill_step(first, value);
          --n;
        }
      } catch(...) {
        destroy(alloc, init, first);
        throw;
      }
      return first;
    }

  template <typename Alloc, typename I, typename T>
    inline auto uninitialized_fill_n(Alloc& alloc, I first, std::size_t n, const T& value)
      -> Requires<Trivial_uninitialized_copy<I, T>(), I>
    {
      // FIXME: Write this in terms of fill_n.
      fill(first, first + n, value);
      return first + n;
    }
}




#endif
