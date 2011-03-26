// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ITERATOR_ENUMERATOR_HPP
#define ORIGIN_ITERATOR_ENUMERATOR_HPP

#include <origin/iterator/facades.hpp>

namespace origin
{
  // FIXME: How hard would this be to adapt to using enums as the underlying
  // count type. I'd basically have to implement a little casting layer in
  // the increment operation, but that shouldn't be too hard. Are there
  // legitimate uses for such a type?

  // FIXME: Would it be worthwhile to cache the current enumeration? This
  // would cause multiple dereferences to return the same object and we could
  // actually support the arrow operator.

  /**
   * An enumerator is a (maximally) random access iterator that enumerates
   * (counts) the items it iterates over. Enumerators, when dereferences,
   * return a pair containing the current count and the dereferenced reseult,
   * in that order.
   *
   * Note that the equality of these iterators is implemented only in terms of
   * the underlying iterator, not the actual count. One side effect of this is
   * that PTE iterators, have an unspecified count value.
   *
   * Enumerators do not support arrow notation. The pointer type of an
   * enumerator is the same as its reference type.
   *
   * @tparam Iter   The iterator being enumerated
   * @tparam Count  The type encoding the count. This is required to be an
   *                Incrementable type. By default, this is the same as the
   *                difference type of Iter.
   *
   * @note This is roughly equivalent to a zip iterator comprised of the
   * underlying iterator and a counting iterator.
   */
  template<typename Iter,
           typename Count = typename std::iterator_traits<Iter>::difference_type>
  class enumerator
    : public random_access_iterator_facade<
        enumerator<Iter, Count>,
        std::pair<Count, typename std::iterator_traits<Iter>::value_type>,
        std::pair<Count, typename std::iterator_traits<Iter>::reference>,
        std::pair<Count, typename std::iterator_traits<Iter>::reference>,
        typename std::iterator_traits<Iter>::difference_type
      >
  {
    typedef Iter base_iterator;
    typedef random_access_iterator_facade<
      enumerator<Iter, Count>,
      std::pair<Count, typename std::iterator_traits<Iter>::value_type>,
      std::pair<Count, typename std::iterator_traits<Iter>::reference>,
      std::pair<Count, typename std::iterator_traits<Iter>::reference>,
      typename std::iterator_traits<Iter>::difference_type
    > base_type;
  public:
    typedef Count size_type;
    typedef typename base_type::reference reference;
    typedef typename base_type::difference_type difference_type;

    enumerator(base_iterator i = base_iterator{}, difference_type n = 0)
      : iter_{i}, count_{n}
    { }

    reference dereference() const
    { return {count_, *iter_}; }

    bool equal(enumerator const& x) const
    { return iter_ == x.iter_; }

    bool less(enumerator const& x) const
    { return iter_ < x.iter_; }

    void increment()
    { ++iter_; ++count_; }

    void decrement()
    { --iter_; --count_; }

    void advance(difference_type n)
    { --iter_ += n; count_ += n; }

    difference_type distance(enumerator const& x)
    { return iter_ - x.iter_; }

  private:
    base_iterator iter_;
    difference_type count_;
  };

  // FIXME: Rename this function to enumerate. It's going to have to
  // conditionally enabled on is_iterator<Iter>.

  /**
   * @fn make_enumerator(iter)
   * @fn make_enumerator(iter, n)
   *
   * Construct an enumerator over the Iterator iter, starting at the optionally
   * specified value n.
   *
   * @tparam Iter   The underlying Iterator type
   * @tparam Count  The counted type, defaults to the iterator's difference type
   *
   * @param iter    The iterator being enumerated
   * @param n       The starting enumeration value
   */
  template<typename Iter,
           typename Count = typename std::iterator_traits<Iter>::difference_type>
  inline enumerator<Iter, Count>
  make_enumerator(Iter iter, Count n = 0)
  { return {iter, n}; }

} // namespace origin

#endif
