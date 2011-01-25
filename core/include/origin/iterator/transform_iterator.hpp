// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ITERATOR_TRNSFORM_ITERATOR_HPP
#define ORIGIN_ITERATOR_TRNSFORM_ITERATOR_HPP

#include <origin/iterator/facades.hpp>

namespace origin
{
  /**
   * A trasnform iterator applies a transform function to the result type of
   * its underlying iterator when derefenced
   *
   * @tparam Iter The underlying Iterator
   * @tparam Transform A transform function.
   *
   * @todo Move this into the iterator library.
   */
  template<typename Iter, typename Transform>
  class transform_iterator
    : public random_access_iterator_facade<
        transform_iterator<Iter, Transform>,
        typename std::result_of<
          Transform(typename std::iterator_traits<Iter>::reference)
        >::type,
        typename std::iterator_traits<Iter>::difference_type
      >
  {
    typedef Iter base_iterator;
    typedef typename std::result_of<
          Transform(typename std::iterator_traits<Iter>::reference)
        >::type result_type;
  public:
    typedef typename std::remove_const<
      typename std::remove_reference<result_type>
    >::type value_type;
    typedef result_type reference;
    typedef typename std::add_pointer<value_type>::type pointer;
    typedef typename std::iterator_traits<Iter>::difference_type difference_type;

    // FIXME: GCC seems to have a problem accepting brace initializers on
    // the range_member. Update this when GCC is fixed.
    transform_iterator(base_iterator i, Transform f)
      : iter_{i}, func_{f}
    { }

    reference dereference() const
    { return func_(*iter_); }

    bool equal(transform_iterator const& x) const
    { return iter_ == x.iter_; }

    void increment()
    { ++iter_; }

    void decrement()
    { --iter_; }

    void advance(difference_type n)
    { iter_ += n; }

    difference_type distance(transform_iterator const& x)
    { return x.iter_ - iter_; }

  private:
    // FIXME: Use the empty base optimization to reduce the memory requirements
    // of the iterator type.
    base_iterator iter_;
    Transform func_;
  };

} // namespace origin

#endif
