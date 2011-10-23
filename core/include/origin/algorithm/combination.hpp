// Copyright(c) 2004-2007 Ben Bear
//
// Distributed under the Boost Software License, Version 1.0. See
// accompanying file LICENSE-Boost.txt or http://www.boost.org/LICENSE_1_0.txt)
// for terms and conditions.

// NOTE: These algorithms are adapted from the TR Proposal and library in the
// Boost Vault by Ben Bear. Note that this code is(currently) licensed under 
// a different set of terms.

#ifndef ORIGIN_ALGORITHM_COMBINATION_HPP
#define ORIGIN_ALGORITHM_COMBINATION_HPP

#include <algorithm>

namespace origin
{
  // Merge two combination subsequences. Note that this algorithm does not
  // rely on an ordering.
  template<typename Iter1, typename Iter2>
    void combination_merge_right__(Iter1 first1, Iter1 last1, Iter2 first2, Iter2 last2)
    {
      if(first1 == last1 || first2 == last2)
        return;

      Iter1 i = last1;
      Iter2 j = first2;
      while(i != first1 && j != last2)
        std::iter_swap(--i, j++);

      std::reverse(first1, i);
      std::reverse(first1, last1);

      std::reverse(j, last2);
      std::reverse(first2, last2);
    }

  // Compute the next combination.
  template<typename Iter>
    bool do_next_combination__(Iter first1, Iter last1, Iter first2, Iter last2)
    {
      if(first1 == last1 || first2 == last2)
        return false;

      Iter qmax = std::prev(last2);
      Iter pout1 = std::lower_bound(first1, last1, *qmax);
      bool fin = pout1 == first1;
      Iter left1, left2;
      if(!fin) {
        Iter pout = std::prev(pout1);
        Iter qin = std::upper_bound(first2, last2, *pout);
        std::iter_swap(pout, qin);
        left1 = std::next(pout);
        left2 = std::next(qin);
      } else {
        left1 = first1;
        left2 = first2;
      }
      combination_merge_right__(left1, last1, left2, last2);
      return !fin;
    }

  template<typename Iter, typename Comp>
    bool do_next_combination__(Iter first1, Iter last1, Iter first2, Iter last2, Comp comp)
    {
      if(first1 == last1 || first2 == last2)
        return false;

      Iter qmax = std::prev(last2);
      Iter pout1 = std::lower_bound(first1, last1, *qmax, comp);
      bool fin = pout1 == first1;
      Iter left1, left2;
      if(!fin) {
        Iter pout = std::prev(pout1);
        Iter qin = std::upper_bound(first2, last2, *pout, comp);
        std::iter_swap(pout, qin);
        left1 = std::next(pout);
        left2 = std::next(qin);
      } else {
        left1 = first1;
        left2 = first2;
      }
      combination_merge_right__(left1, last1, left2, last2);
      return !fin;
    }


  // Compute the next partial permutation. Returns true if the next partial
  // permutation exists.
  //
  // requires: BidirectionalIterator<Iter>
  // requires: TotallyOrdered<ValueType<Iter>
  template<typename Iter>
    inline bool next_partial_permutation(Iter first, Iter middle, Iter last)
    {
      if(first == middle)
        return false;

      std::reverse(middle, last);
      return std::next_permutation(first, last);
    }

  // Compute the next partial permutation. Returns true if the next partial
  // permutation exists.
  //
  // requires: BidirectionalIterator<Iter>
  // requires: Relation<Comp, ValueType<Iter>>
  template<typename Iter, typename Comp>
    inline bool next_partial_permutation(Iter first, Iter middle, Iter last, Comp comp)
    {
      // precondition: strict_weak_order(comp)
      if(first == middle)
        return false;

      std::reverse(middle, last);
      return std::next_permutation(first, last, comp);
    }

  template<typename Iter>
    inline bool prev_partial_permutation(Iter first, Iter middle, Iter last)
    {
      if(first == middle)
        return false;

      bool ret = std::prev_permutation(first, last);
      std::reverse(middle, last);
      return ret;
    }

  template<typename Iter, typename Comp>
    inline bool prev_partial_permutation(Iter first, Iter middle, Iter last, Comp comp)
    {
      if(first == middle)
        return false;

      bool ret = std::prev_permutation(first, last, comp);
      std::reverse(middle, last);
      return ret;
    }


  // Compute the next combination. 
  //
  // Return true if the next combination exists, false otherwise.
  //
  // requires: BidirectionalIterator<Iter>
  // requires: TotallyOrdered<ValueType<Iter>>
  template<typename Iter>
    inline bool next_combination(Iter first, Iter middle, Iter last)
    {
      return do_next_combination__(first, middle, middle, last);
    }

  // Compute the next combination. 
  //
  // Return true if the next combination exists, false otherwise.
  //
  // requires: BidirectionalIterator<Iter>
  // requires: Relation<Comp, ValueType<Iter>>
  template<typename Iter, typename Comp>
    inline bool next_combination(Iter first, Iter middle, Iter last, Comp comp)
    {
      return do_next_combination__(first, middle, middle, last, comp);
    }

  template<typename Iter>
    inline bool prev_combination(Iter first, Iter middle, Iter last)
    {
      return do_next_combination__(middle, last, first, middle);
    }

  template<typename Iter, typename Comp>
    inline bool prev_combination(Iter first, Iter middle, Iter last, Comp comp)
    {
      return do_next_combination__(middle, last, first, middle, comp);
    }

} // namespace origin

#endif
