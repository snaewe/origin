// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ITERATOR_ENUMERATING_ITERATOR_HPP
#define ORIGIN_ITERATOR_ENUMERATING_ITERATOR_HPP

#include <origin/iterator/facades.hpp>

namespace origin
{
  /**
   * An enumerating iterator is an iterator that enumerates (counts) the items
   * it iterates over. Enumerators, when dereferences, return a pair containing
   * the current count and the dereferenced reseult.
   *
   * Enumerators are maximally random acccess iterators.
   *
   * Note that the equality of these iterators is implemented only in terms of
   * the underlying iterator, not the actual count. One side effect of this is
   * that PTE iterators, have an unspecified count value.
   *
   * @note This is roughly equivalent to a zip iterator comprised of the
   * underlying iterator and a counting iterator.
   */
  template<typename Iter,
           typename Count = typename std::iterator_traits<Iter>::difference_type>
  class enumerating_iterator
    : public random_access_iterator_facade<
        enumerating_iterator<Iter, Count>,
        std::pair<Count, typename std::iterator_traits<Iter>::reference>,
        typename std::iterator_traits<Iter>::difference_type
      >
  {
    typedef Iter base_iterator;
  public:
    typedef Count size_type;
    typedef std::pair<
      size_type, typename std::iterator_traits<Iter>::reference
    > value_type;
    typedef value_type reference;
    typedef value_type pointer;
    typedef typename std::iterator_traits<Iter>::difference_type difference_type;

    // FIXME: GCC seems to have a problem accepting brace initializers on
    // the range_member. Update this when GCC is fixed.
    enumerating_iterator(base_iterator i = base_iterator{}, difference_type n = 0)
      : iter_{i}, count_{n}
    { }

    reference dereference() const
    { return {count_, *iter_}; }

    bool equal(enumerating_iterator const& x) const
    { return iter_ == x.iter_; }

    bool less(enumerating_iterator const& x) const
    { return iter_ < x.iter_; }

    void increment()
    { ++iter_; ++count_; }

    void decrement()
    { --iter_; --count_; }

    void advance(difference_type n)
    { --iter_ += n; count_ += n; }

    void distance(enumerating_iterator const& x)
    { return x.iter_ - iter_; }

  private:
    base_iterator iter_;
    difference_type count_;
  };

  /** Make an enumerating_iterator over the given iterator. */
  template<typename Iter,
           typename Count = typename std::iterator_traits<Iter>::difference_type>
  inline enumerating_iterator<Iter, Count>
  make_enumerating_iterator(Iter iter, Count n = 0)
  { return {iter, n}; }

} // namespace origin

#endif
