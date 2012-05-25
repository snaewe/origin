// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MEMORY_ALLOCATION_HPP
#define ORIGIN_MEMORY_ALLOCATION_HPP

#include <cstddef>
#include <new>

#include <origin/memory/pointer.hpp>

namespace origin
{
  // Pointer type (alias)
  // The pointer type alias refers to the result of Alloc's allocate method,
  // which is a Void pointer of some pointer family.
  template <typename Alloc>
    using Pointer_type = decltype(std::declval<Alloc>().allocate(0));



  // Pointer type (alias)
  // The pointer type alias yields a pointer to an object of type T that 
  // would be allocated by Alloc.
  template <typename T, typename Alloc>
    using Pointer_to = Rebind_pointer<Pointer_type<Alloc>, T>;



  // Allocator (concept)
  //
  // An allocator is a facility that is responsible for allocating and
  // deallocating ojbects. The result of allocation is a void Pointer to the
  // allocated, but uninitialized object(s). Deallocation reverses that process,
  // releasing the allocated memory. Note that Pointer's are not required to
  // refer to in-process locations; they could refer to disk  locations or
  // objects on remote nodes. For example, a persistent memory allocator whose
  // Pointers are that can transfer values into and out of main memory. A
  // garbage collecting allocator returns Pointers that trace their 
  // reachability.
  //
  // A Basic allocator is one that that allocates naked C++ pointers. Most
  // allocators are (should be?) basic allocators.
  //
  // FIXME: Actually implement this concept. What are the actual requirements?
  // It must support a.allocate() and a.deallocate() and the result of 
  // allocate() must be a Void_pointer.
  template <typename Alloc>
    constexpr bool Allocator()
    {
      return true;
    }



  // Allocate
  // The allocate operation allocates n objects of the specified type using
  // the given allocator. Note that the allocated memory is uninitialized. Its 
  // use is as follows:
  //
  //    X *ptr = allocate<X>(alloc, 3);
  //
  // Which returns a pointer to 3 objects of type X, assuming that alloc is an
  // memory allocator. This operation is primarily provided as helper for untyped
  // allocators.
  //
  // Template parameters:
  //    T -- A non-void type; this argument must be given explicitly.
  //    Alloc -- An Allocator
  //
  // Parameters:
  //    alloc -- an object of Allocator type
  //    n -- the number of objects of type T to be allocated.
  template <typename T, typename Alloc>
    inline Pointer_to<T, Alloc> allocate(Alloc& alloc, std::size_t n = 1)
    {
      static_assert(!Void<T>(), "");
      using Ptr = Pointer_to<T, Alloc>;
      return n != 0
        ? static_ptr_cast<Ptr>(alloc.allocate(n * sizeof(T))) 
        : nullptr;
    }



  // Deallocate
  // The deallocate operation deallocate the memory pointed to by p. The value
  // p must have been previously allocated using a corresponding allocate
  // operation on the same alloc object.
  //
  // FIXME: Write the type requirements for this algorithm. Here, Alloc must
  // be an Allocator and Ptr must be in the family of allocated pointer types.
  template <typename Alloc, typename Ptr>
    void deallocate(Alloc& alloc, Ptr p, std::size_t n = 1)
    {
      static_assert(Same_pointer_family<Ptr, Pointer_type<Alloc>>(), "");
      alloc.deallocate(p, n);
    }



  // Default allocator
  // The default allocator describes allocates and deallocatese in-process 
  // memory using ::operator new and ::operator delete. Note that all allocators
  // for in-process memory should derive from this class.
  //
  // TODO: Should we also support placement allocation through this interface?
  //
  // TODO: Should there be a separate protocol for the allocation of arrays? It
  // is conceivable that an allocator may want to do separate book-keeping for
  // arrays than single objects. Other kinds of allocators may also return
  // different pointer types when allocating arrays vs. allocating objects.
  //
  // TODO: Should this be non-copyable? Clearly, I am not using the standard's
  // allocator design; does copying even make sense?
  class allocator
  {
  public:
    virtual ~allocator() { }

    // Allocate n bytes of memory. An exception may be thrown if the requested
    // number of bytes cannot be allocated.
    virtual void *allocate(std::size_t n)
    {
      return ::operator new(n);
    }

    // Deallocate the memory pointed to be p. The value p must have been
    // previously allocated using the corresponding allocate operation from the
    // same allocator object.
    virtual void deallocate(void *p, std::size_t n)
    {
      ::operator delete(p);
    }

    // Returns true if the the two allocators are equal. For all stateless
    // allocators, this is always true. Note that this operation *must* be
    // overloaded if the derived allocator is stateful.
    virtual bool equal(const allocator& x) const { return true; }
  };



  // Equality_comparable<allocator>
  // For all allocator-derived types, equality comparability is implemented
  // in terms of the allocator::equal method.
  inline bool operator==(const allocator& a, const allocator& b) 
  { 
    return a.equal(b);
  }

  inline bool operator!=(const allocator& a, const allocator& b)
  {
    return !a.equal(b);
  }



  // Default allocator
  // Returns a reference to the global default allocator.
  allocator& default_allocator();



  // Auto allocator
  // The auto allocator maintains a buffer of N bytes on the stack. If an
  // allocation request exceeds N bytes, the fall-back allocator is used
  // instead.
  template <std::size_t N, typename Alloc = allocator>
  class auto_allocator : public allocator
  {
  public:

    virtual void *allocate(std::size_t n)
    {
      return 0;
    }

    virtual void deallocate(void *p, std::size_t n)
    {
    }

    // Every instance of an auto allocator is distinct from every other. No
    // two are the same.
    virtual bool equal(const auto_allocator& x) const
    {
      return false;
    }

  private:
    char buf[N];
  };


} // namespace origin

#endif
