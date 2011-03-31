// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ITERATOR_STRIDE_ITERATOR_HPP
#define ORIGIN_ITERATOR_STRIDE_ITERATOR_HPP

#include <iterator>

#include <origin/iterator/facades.hpp>

namespace origin
{
  // FIXME: Is int really the best type for the count? It's certainly the
  // easiest. I can actually write it so that the type of the ICE parameter
  // is derived from count (as make_signed<Count>::type, that actually works),
  // but I can't easily do the same for the accessor function, which is really
  // too bad. I'm not so sure that it's really worth the effort.

  /**
   * @ingroup iter
   *
   * A stride iterator is a an adaptor that advances an underlying iterator
   * several steps in a single increment.
   *
   * @tparam Iter   The underlying Iterator type
   */
  template<typename Iter>
  class stride_iterator
    : public random_access_iterator_facade<
        stride_iterator<Iter>,
        typename std::iterator_traits<Iter>::value_type,
        typename std::iterator_traits<Iter>::reference,
        typename std::iterator_traits<Iter>::pointer,
        typename std::iterator_traits<Iter>::difference_type
      >
  {
    typedef random_access_iterator_facade<
        stride_iterator<Iter>,
        typename std::iterator_traits<Iter>::value_type,
        typename std::iterator_traits<Iter>::reference,
        typename std::iterator_traits<Iter>::pointer,
        typename std::iterator_traits<Iter>::difference_type
      > base_type;
  public:
    typedef typename base_type::reference reference;
    typedef typename base_type::difference_type difference_type;

    stride_iterator()
      : iter_{}, stride_{}
    { }

    // FIXME: Is stride allowed to be negative -1?
    stride_iterator(Iter i, difference_type n = 1)
      : iter_{i}, stride_{n}
    { }

    reference dereference() const
    { return *iter_; }

    bool equal(stride_iterator const& x) const
    { return iter_ == x.iter_; }

    bool less(stride_iterator const& x) const
    { return iter_ < x.iter_; }

    void increment()
    { iter_ += stride_; }

    void decrement()
    { iter_ -=  stride_; }

    void advance(difference_type n)
    { stride_ += n * stride_; }

    // FIXME: What if this and x don't have the same stride?
    difference_type distance(stride_iterator const& x)
    { return iter_ - x.iter_ / stride_; }

  private:
    Iter iter_;
    difference_type stride_;
  };

  /**
   * @fn make_stride_iterator(i, n)
   * @fn make_stride_iterator(i)
   *
   * Create a stride iterator over the underlying iteator with the given
   * stride. The stride is generally omitted when constructing a PTE iterator.
   *
   * @tparam Iter     The underlying Iterator type
   *
   * @param i         The underlying iterator
   * @param n         The stride
   */
  //@{
  template<typename Iter, typename Distance>
  inline stride_iterator<Iter> make_stride_iterator(Iter i, Distance n)
  {
    // FIXME: requires Convertible<Distance, Iter::difference_type>
    return stride_iterator<Iter>{i, n};
  }

  template<typename Iter>
  inline stride_iterator<Iter> make_stride_iterator(Iter i)
  { return stride_iterator<Iter>{i}; }
  //@}


} // namespace origin

#endif
