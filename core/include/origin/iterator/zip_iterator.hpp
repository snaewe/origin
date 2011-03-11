// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ITERATOR_ZIP_ITERATOR_HPP
#define ORIGIN_ITERATOR_ZIP_ITERATOR_HPP

#include <origin/iterator/facades.hpp>

namespace origin
{
  /**
   * @internal
   * Function to be applied to zipped iterators when incrementing.
   */
  struct zip_increment_
  {
    template<typename Iter>
    void operator()(Iter& x) const
    { ++x; }
  };

  /**
   * @internal
   * Function to be applied to zipped iterators when decrementing.
   */
  struct zip_decrement_
  {
    template<typename Iter>
    void operator()(Iter& x) const
    { --x; }
  };

  /**
   * @internal
   * Function to be applied when dereferencing an zip iterator.
   */
  template<typename Result>
  struct zip_dereference_
  {
    typedef Result result_type;

    template<typename... Args>
    Result operator()(Args&&... args) const
    { return Result{*args...}; }
  };

  // FIXME: Actually determine the minimum supportable iterator type. These
  // could, in theory, be random access iterators.

  // FIXME: What happens if the ranges are not of equal length? It might be
  // sufficient to say that dereferencing a zip iterator where one is PTE
  // will result in undefined behavior.

  /**
   * The zipped iterator wraps a tuple of individual iterators. Operations
   * (e.g,. increment, decrement, dereference, etc.) are applied to all
   * iterators at the same time, causing them to iterate in lock-step.
   *
   * The zipped iterator models the least model of all it's consitituent
   * iterator's models. For example, zipping over a list and a vector will
   * produce a Forward_Iterator.
   */
  template<typename... Iters>
  class zip_iterator
    : public bidirectional_iterator_facade<
        zip_iterator<Iters...>,
        std::tuple<typename std::iterator_traits<Iters>::value_type...>,
        std::tuple<typename std::iterator_traits<Iters>::reference...>,
        std::tuple<typename std::iterator_traits<Iters>::pointer...>,
        std::tuple<typename std::iterator_traits<Iters>::difference_type...>
      >
  {
  public:
    typedef std::tuple<Iters...> iterator_tuple;
    typedef std::tuple<
      typename std::iterator_traits<Iters>::value_type...
    > value_type;
    typedef std::tuple<
      typename std::iterator_traits<Iters>::reference...
    > reference;

    // FIXME: Not sure what goes here. Probably not a pointer.
    typedef value_type pointer;

    zip_iterator()
      : iters_{}
    { }

    zip_iterator(Iters... iters)
      : iters_{iters...}
    { }

    /**
     * Return the tuple of iterators, allowing access to individual components
     * of the zipped iterators.
     */
    iterator_tuple iterators() const
    { return iters_; }

    reference dereference() const
    { return tuple_invoke(zip_dereference_<reference>{}, iters_); }

    bool equal(zip_iterator const& x) const
    { return iters_ == x.iters_; }

    void increment()
    { tuple_for_each(iters_, zip_increment_{}); }

    void decrement()
    { tuple_for_each(iters_, zip_decrement_{}); }

    // FIXME: Implement advance and distance.

  private:
    iterator_tuple iters_;
  };

} // namespace origin

#endif
