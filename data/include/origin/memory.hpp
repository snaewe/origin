// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_MEMORY_HPP
#define ORIGIN_MEMORY_HPP

#include <memory>
#include <iterator>
#include <type_traits>

namespace origin
{
  // FIXME: For uninitialized copy and fill, is the is_trivial requirement
  // sufficiently general? What about trivially_copyable? Would that be more
  // general?

  /**
   * @fn uninitialized_fill(alloc, f, l, x)
   *
   * Initialize each element of the uninitialized range [f, l) with value x.
   * The element is initialized using the given allocator.
   *
   * @tparam Alloc  An Allocator type
   * @tparam Iter   A mutable Forward_Iterator type
   * @tparam T      A value type
   *
   * The value of type of Iter and T must be the same type.
   *
   * @param alloc   An allocator object
   * @param f       The first iterator in an iterator range
   * @param l       The last (PTE) iterator in an itetor range
   * @param x       An object
   */
  //@{
  template<typename Alloc, typename Iter, typename T>
    typename std::enable_if<
      !std::is_trivial<typename std::iterator_traits<Iter>::value_type>::value,
      void
    >::type
  uninitialized_fill(Alloc& alloc, Iter f, Iter l, T const& x)
  {
    for( ; f != l; ++f)
      alloc.construct(std::addressof(*f), x);
  }

  // Specialization for trivial types uses std::copy since trivial types have
  // no meaningful uninitialized state. Explicit construction is not required.
  template<typename Alloc, typename Iter, typename T>
    typename std::enable_if<
      std::is_trivial<typename std::iterator_traits<Iter>::value_type>::value,
      void
    >::type
  uninitialized_fill(Alloc&, Iter f, Iter l, T const& x)
  { std::fill(f, l, x); }
  //@}

  /**
   * @fn uninitialized_copy(alloc, f, l, result)
   *
   * Copy each element of the range [f, l) into the uninitialized range
   * beginning at result. Each copied element is initialized using the given
   * allocator.
   *
   * @tparam Alloc      An Allocator type
   * @tparam In_Iter    An Input_Iterator
   * @tparam Out_Iter   An Output_Iterator.
   *
   * @param alloc   An allocator object
   * @param f       The first iterator in an iterator range
   * @param l       The last (PTE) iterator in an itetor range
   * @param result  The first iterator in an unititialized range
   */
  //@{
  template<typename Alloc, typename In_Iter, typename Out_Iter>
    typename std::enable_if<
      !std::is_trivial<typename std::iterator_traits<Out_Iter>::value_type>::value,
      void
    >::type
  uninitialized_copy(Alloc& alloc, In_Iter f, In_Iter l, Out_Iter result)
  {
    for( ; f != l; ++f, ++result) {
      alloc.construct(std::addressof(*result), *f);
    }
  }

  // Specialization for trivial types uses std::copy since trivial types have
  // no meaningful uninitialized state. Explicit construction is not required.
  template<typename Alloc, typename In_Iter, typename Out_Iter>
    typename std::enable_if<
      std::is_trivial<typename std::iterator_traits<Out_Iter>::value_type>::value,
      void
    >::type
  uninitialized_copy(Alloc&, In_Iter f, In_Iter l, Out_Iter result)
  { std::copy(f, l, result); }
  //@}

  // FIXME: has_trivial_destructor is non-standard. The correct version trait
  // is_trivially_destructible, which GCC does not currently support.
  /**
   * @fn destroy(f, l, alloc)
   *
   * Destroy the objects in the range [f, l). Objects are destroyed using
   * the given allocator.
   *
   * @tparam Iter
   * @tparam Alloc
   *
   * @param f       The initial iterator
   * @param l       The final iterator
   * @param alloc   A reference to an allocator
   */
  //@{
  template<typename Alloc, typename Iter>
    typename std::enable_if<
      !std::has_trivial_destructor<
        typename std::iterator_traits<Iter>::value_type
      >::value,
      void>::type
  destroy(Alloc& alloc, Iter f, Iter l)
  {
    for( ; f != l; ++f)
      alloc.destroy(std::addressof(*f));
  }

  // Specialization for non-trivially destructible types,
  template<typename Alloc, typename Iter>
    typename std::enable_if<
      std::has_trivial_destructor<
        typename std::iterator_traits<Iter>::value_type
      >::value,
      void>::type
  destroy(Alloc& alloc, Iter f, Iter l)
  { }
  //@}

} // namespace origin

#endif
