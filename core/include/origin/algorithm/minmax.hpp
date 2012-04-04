// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ALGORITHM_MINMAX_HPP
#define ORIGIN_ALGORITHM_MINMAX_HPP

namespace origin
{
  
  // Order selection
  // The following algorithms select ordered elements from a set.
  //
  //    select_0_2(i, j)
  //    select_0_2(i, j, comp)
  //    select_1_2(i, j)
  //    select_1_2(i, j, comp)
  //
  //    select_0_3(i, j)
  //    select_0_3(i, j, comp)
  //    select_1_3(i, j)
  //    select_1_3(i, j, comp)
  //    select_2_3(i, j)
  //    select_2_3(i, j, comp)
  //
  //    select_0_4(i, j)
  //    select_0_4(i, j, comp)
  //    select_1_4(i, j)
  //    select_1_4(i, j, comp)
  //    select_2_4(i, j)
  //    select_2_4(i, j, comp)
  //    select_3_4(i, j)
  //    select_3_4(i, j, comp)
  //
  // TODO: Write non-const overloads for all of these algorithms.
  //
  // FIXME: Write requirements and preconditions.


  
  // Select 0, 2 (relation)
  // Returns the minimum of (a, b).
  template<typename T, typename R>
    inline const T& select_0_2(const T& a, const T& b, R comp)
    {
      return comp(b, a) ? b : a;
    }
    
  // Select 0, 2 (less)
  // Returns the minimum of (a, b).
  template<typename T>
    inline const T& select_0_2(const T& a, const T& b)
    {
      return select_0_2(a, b, lt());
    }

    

  // Select 1, 2 (relation)
  // Returns the maximum of (a, b).
  template<typename T, typename R>
    inline const T& select_1_2(const T& a, const T& b, R comp)
    {
      return comp(b, a) ? a : b;
    }
    
  // Select 1, 2, (less)
  // Returns the maximum of (a, b).
  template<typename T>
    inline const T& select_1_2(const T& a, const T& b)
    {
      return select_1_2(a, b, lt());
    }
    
  
  
  // Select 0, 3 (relation)
  // Returns the minimum of (a, b, c).
  template<typename T, typename R>
    inline const T& select_0_3(const T& a, const T& b, const T& c, R comp)
    {
      return select_0_2(select_0_2(a, b, comp), c, comp);
    }
    
  // Select 0, 3 (less)
  // Returns the minimum of (a, b, c).
  template<typename T>
    inline const T& select_0_3(const T& a, const T& b, const T& c)
    {
      return select_0_3(a, b, c, lt());
    }
    

  
  // Returns the median of (a, b, c), assuming that (a, b) is ordered.
  template<typename T, typename R>
    inline const T& select_1_3_ab(const T& a, const T& b, const T& c, R comp)
    {
      return !comp(c, b) ? b : select_1_2(a, c, comp);
    }

    
    
  // Select 1, 3 (relation)
  // Returns the median of (a, b, c).
  template<typename T, typename R>
    inline const T& select_1_3(const T& a, const T& b, const T& c, R comp)
    {
      return comp(b, a) ? select_1_3_ab(b, a, c, comp)
                        : select_1_3_ab(a, b, c, comp);
    }

  // Select 1, 3 (less)
  // Returns the median of (a, b, c).
  template<typename T>
    inline const T& select_1_3(const T& a, const T& b, const T& c)
    {
      return select_1_3(a, b, c, lt());
    }
  
  
  
  // Select 2, 3 (relation)
  // Returns the maximum of (a, b, c).
  template<typename T, typename R>
    inline const T& select_2_3(const T& a, const T& b, const T& c, R comp)
    {
      return select_1_2(select_1_2(a, b, comp), c, comp);
    }
    
  // Select 2, 3 (less)
  // Returns the maximum of (a, b, c).
  template<typename T>
    inline const T& select_2_3(const T& a, const T& b, const T& c)
    {
      return select_2_3(a, b, c, lt());
    }
    
  
  
  // Select 0, 4 (relation)
  // Return the minimum of (a, b, c, d).
  template<typename T, typename R>
    inline const T& select_0_4(const T& a, const T& b, const T& c, const T& d, R comp)
    {
      return select_0_2(select_0_2(a, b, comp), select_0_2(c, d, comp), comp);
    }
    
  // Select 0, 4 (less)
  // Return the minimum of (a, b, c, d).
  template<typename T>
    inline const T& select_0_4(const T& a, const T& b, const T& c, const T& d)
    {
      return select_0_4(a, b, c, d, lt());
    }
    

    
  // Select the 2nd ordered element of (a, b, c, d) assuming that (a, b) and 
  // (c, d) are both ordered.
  template<typename T, typename R>
    inline const T& select_1_4_ab_cd(const T& a, const T& b, const T& c, const T& d, R comp)
    {
      return comp(c, a) ? select_0_2(a, d, comp) 
                        : select_0_2(b, c, comp);
    }
    
  // Select the 2nd ordered element of (a, b, c, d) assuming that (a, b) is 
  // ordered and ordering (c, d).
  template<typename T, typename R>
    inline const T& select_1_4_ab(const T& a, const T& b, const T& c, const T& d, R comp)
    {
      return comp(d, c) ? select_1_4_ab_cd(a, b, d, c, comp)
                        : select_1_4_ab_cd(a, b, c, d, comp);
    }

    

  // Select 1, 4 (relation)
  // Returns the 2nd ordered element of (a, b, c, d).
  template<typename T, typename R>
    inline const T& select_1_4(const T& a, const T& b, const T& c, const T& d, R comp)
    {
      return comp(b, a) ? select_1_4_ab(b, a, c, d, comp)
                        : select_1_4_ab(a, b, c, d, comp);
    }

  // Select 1, 4 (less)
  // Return the minimum of (a, b, c, d).
  template<typename T>
    inline const T& select_1_4(const T& a, const T& b, const T& c, const T& d)
    {
      return select_1_4(a, b, c, d, lt());
    }
    
    
   
  // Select the 3rd ordered element of (a, b, c, d) assuming that (a, b) and 
  // (c, d) are both ordered.
  template<typename T, typename R>
    inline const T& select_2_4_ab_cd(const T& a, const T& b, const T& c, const T& d, R comp)
    {
      return comp(c, a) ? select_1_2(a, d, comp)
                        : select_1_2(b, c, comp);
    }
    
  // Select the 3rd ordered element of (a, b, c, d) assuming that (a, b) is 
  // ordered by ordering (c, d).
  template<typename T, typename R>
    inline const T& select_2_4_ab(const T& a, const T& b, const T& c, const T& d, R comp)
    {
      return comp(d, c) ? select_2_4_ab_cd(a, b, d, c, comp)
                        : select_2_4_ab_cd(a, b, c, d, comp);
    }
    
    

  // Select 2, 4 (relation)
  // Returns the 3rd ordered element of (a, b, c, d).  
  template<typename T, typename R>
    inline const T& select_2_4(const T& a, const T& b, const T& c, const T& d, R comp)
    {
      return comp(b, a) ? select_2_4_ab(b, a, c, d, comp)
                        : select_2_4_ab(a, b, c, d, comp);
    }
    
  // Select 2, 4 (less)
  // Returns the 3rd ordered element of (a, b, c, d).  
  template<typename T>
    inline const T& select_2_4(const T& a, const T& b, const T& c, const T& d)
    {
      return select_2_4(a, b, c, d, lt());
    }
  
  

  // Select 3, 4 (less)
  // Return the maximum of (a, b, c, d).
  template<typename T, typename R>
    inline const T& select_3_4(const T& a, const T& b, const T& c, const T& d, R comp)
    {
      return select_1_2(select_1_2(a, b, comp), select_1_2(c, d, comp), comp);
    }
    
  // Select 3, 4 (less)
  // Return the maximum of (a, b, c, d).
  template<typename T>
    inline const T& select_3_4(const T& a, const T& b, const T& c, const T& d)
    {
      return select_3_4(a, b, c, d, lt());
    }


    
  // Iterator selection
  // The following algorithms select ordered elements from a set of iterators.
  // The algorithms are:
  //
  //    iter_select_0_2(i, j)
  //    iter_select_0_2(i, j, comp)
  //    iter_select_1_2(i, j)
  //    iter_select_1_2(i, j, comp)
  //
  //    iter_select_0_3(i, j)
  //    iter_select_0_3(i, j, comp)
  //    iter_select_1_3(i, j)
  //    iter_select_1_3(i, j, comp)
  //    iter_select_2_3(i, j)
  //    iter_select_2_3(i, j, comp)
  //
  //    iter_select_0_4(i, j)
  //    iter_select_0_4(i, j, comp)
  //    iter_select_1_4(i, j)
  //    iter_select_1_4(i, j, comp)
  //    iter_select_2_4(i, j)
  //    iter_select_2_4(i, j, comp)
  //    iter_select_3_4(i, j)
  //    iter_select_3_4(i, j, comp)
  //
  // FIXME: Write requirements and preconditions.


    
  // Iterator select 0, 2 (relation)
  // Returns the iterator pointing to the minimum of (*i, *j)
  template<typename I, typename R>
    inline I iter_select_0_2(I i, I j, R comp)
    {
      return comp(*j, *i) ? j : i;
    }
    
  // Iterator select 0, 2 (less)
  // Returns the iterator pointing to the minimum of (*i, *j)
  template<typename I>
    inline I iter_select_0_2(I i, I j)
    {
      return iter_select_0_2(i, j, lt());
    }

    

  // Iterator select 1, 2 (relation)
  // Returns the iterator pointing to the maximum of (*i, *j)
  template<typename I, typename R>
    inline I iter_select_1_2(I i, I j, R comp)
    {
      return comp(*j, *i) ? i : j;
    }
    
  // Select 1, 2, (less)
  // Returns the maximum of (a, b).
  template<typename I>
    inline I iter_select_1_2(I i, I j)
    {
      return iter_select_1_2(i, j, lt());
    }
    
  
  
  // Select 0, 3 (relation)
  // Returns the iterator pointing to the minimum of (*i, *j, *k).
  template<typename I, typename R>
    inline I iter_select_0_3(I i, I j, I k, R comp)
    {
      return ier_select_0_2(iter_select_0_2(i, j, comp), k, comp);
    }
    
  // Select 0, 3 (less)
  // Returns the iterator pointing to the minimum of (*i, *j, *k).
  template<typename I>
    inline I iter_select_0_3(I i, I j, I k)
    {
      return iter_select_0_3(i, j, k, lt());
    }
    

  
  // Returns the iterator pointing to the median of (*i, *j, *k), assuming that 
  // (*i, *j) is ordered.
  template<typename I, typename R>
    inline I iter_select_1_3_ij(I i, I j, I k, R comp)
    {
      return !comp(k, j) ? j : iter_select_1_2(i, k, comp);
    }

    
    
  // Select 1, 3 (relation)
  // Returns the iterator pointing to the median of (*i, *j, *k).
  template<typename I, typename R>
    inline I iter_select_1_3(I i, I j, I k, R comp)
    {
      return comp(j, i) ? iter_select_1_3_ab(j, i, k, comp)
                        : iter_select_1_3_ab(i, j, k, comp);
    }

  // Select 1, 3 (less)
  // Returns the iterator pointing to the median of (*i, *j, *k).
  template<typename I>
    inline I iter_select_1_3(I i, I j, I k)
    {
      return iter_select_1_3(i, j, k, lt());
    }
  
  
  
  // Select 2, 3 (relation)
  // Returns the iterator pointing to the maximum of (*i, *j, *k).
  template<typename I, typename R>
    inline I iter_select_2_3(I i, I j, I k, R comp)
    {
      return iter_select_1_2(iter_select_1_2(i, j, comp), k, comp);
    }
    
  // Select 2, 3 (less)
  // Returns the maximum of (i, j, k).
  template<typename I>
    inline I iter_select_2_3(I i, I j, I k)
    {
      return iter_select_2_3(i, j, k, lt());
    }
    
  
  
  // Select 0, 4 (relation)
  // Returns the iterator pointing to the minimum of (*i, *j, *k, *l).
  template<typename I, typename R>
    inline I iter_select_0_4(I i, I j, I k, I l, R comp)
    {
      return iter_select_0_2(iter_select_0_2(i, j, comp), 
                             iter_select_0_2(k, l, comp), comp);
    }
    
  // Select 0, 4 (less)
  // Returns the iterator pointing to the minimum of (*i, *j, *k, *l).
  template<typename I>
    inline I iter_select_0_4(I i, I j, I k, I l)
    {
      return iter_select_0_4(i, j, k, l, lt());
    }
    

    
  // Returns the iterator pointing to the 2nd ordered element of 
  // (*i, *j, *k, *l) assuming that (*i, j) and  (*k, *l) are both ordered.
  template<typename I, typename R>
    inline I iter_select_1_4_ab_cd(I i, I j, I k, I l, R comp)
    {
      return comp(k, i) ? iter_select_0_2(i, l, comp) 
                        : iter_select_0_2(j, k, comp);
    }
    
  // Returns the iterator pointing to the 2nd ordered element of 
  // (*i, *j, *k, *l) assuming that (*i, j) is ordered, by ordering (*k, *l).
  template<typename I, typename R>
    inline I iter_select_1_4_ab(I i, I j, I k, I l, R comp)
    {
      return comp(l, k) ? iter_select_1_4_ab_cd(i, j, l, k, comp)
                        : iter_select_1_4_ab_cd(i, j, k, l, comp);
    }

    

  // Select 1, 4 (relation)
  // Returns the iterator pointing to the 2nd ordered element of 
  // (*i, *j, *k, *l).
  template<typename I, typename R>
    inline I iter_select_1_4(I i, I j, I k, I l, R comp)
    {
      return comp(j, i) ? iter_select_1_4_ab(j, i, k, l, comp)
                        : iter_select_1_4_ab(i, j, k, l, comp);
    }

  // Select 1, 4 (less)
  // Returns the iterator pointing to the 2nd ordered element of 
  // (*i, *j, *k, *l).
  template<typename I>
    inline I iter_select_1_4(I i, I j, I k, I l)
    {
      return iter_select_1_4(i, j, k, l, lt());
    }
    
    
   
  // Returns the iterator pointing to the 3rd ordered element of 
  // (*i, *j, *k, *l) assuming that (*i, j) and  (*k, *l) are both ordered.
  template<typename I, typename R>
    I iter_select_2_4_ab_cd(I i, I j, I k, I l, R comp)
    {
      return comp(k, i) ? iter_select_1_2(i, l, comp)
                        : iter_select_1_2(j, k, comp);
    }
    
  // Returns the iterator pointing to the 2nd ordered element of 
  // (*i, *j, *k, *l) assuming that (*i, j) is ordered, by ordering (*k, *l).
  template<typename I, typename R>
    I iter_select_2_4_ab(I i, I j, I k, I l, R comp)
    {
      return comp(l, k) ? iter_select_2_4_ab_cd(i, j, l, k, comp)
                        : iter_select_2_4_ab_cd(i, j, k, l, comp);
    }

    

  // Select 2, 4 (relation)
  // Returns the iterator pointing to the 3rd ordered element of 
  // (*i, *j, *k, *l).  
  template<typename I, typename R>
    I iter_select_2_4(I i, I j, I k, I l, R comp)
    {
      return comp(j, i) ? iter_select_2_4_ab(j, i, k, l, comp)
                        : iter_select_2_4_ab(i, j, k, l, comp);
    }
    
  // Select 2, 4 (less)
  // Returns the iterator pointing to the 3rd least element of (*i, *j, *k, *l).  
  template<typename I>
    inline I select_2_4(I i, I j, I k, I l)
    {
      return iter_select_2_4(i, j, k, l, lt());
    }
  
  

  // Select 3, 4 (relation)
  // Returns the iterator pointing to the maximum of (*i, *j, *k, *l).
  template<typename I, typename R>
    I iter_select_3_4(I i, I j, I k, I l, R comp)
    {
      return iter_select_1_2(iter_select_1_2(i, j, comp), 
                             iter_select_1_2(k, l, comp), comp);
    }
    
  // Select 3, 4 (less)
  // Returns the iterator pointing to the maximum of (*i, *j, *k, *l).
  template<typename I>
    inline I iter_select_3_4(I i, I j, I k, I l)
    {
      return iter_select_3_4(i, j, k, l, lt());
    }

    
    
    
  // Minimum
  // There are a number of minimum and maximum algorithms:
  //
  //    min(a, b)
  //    min(a, b, comp)
  //    min(list)
  //    min(list, comp)
  //
  // The following algorithms select the minimum among a sequence of elements.
  //
  //    min_element(first, last)
  //    min_element(first, last, comp)
  //    min_element(range)
  //    min_element(range, comp)

  
  
  // Min elemenet (relation)
  template<typename I, typename R>
    I o_min_element(I first, I last, R comp)
    {
      static_assert(Relational_query<I, R>(), "");
      assert(is_readable_range(first, last));
      assert(is_strict_weak_ordering(comp));

      if(first != last) {
        for(I i = o_next(first); i != last; ++i) {
          if(comp(*i, *first))
            first = i;
        }
      }
      return first;
    }
  
  
  // Min element (less)
  template<typename I>
    I o_min_element(I first, I last)
    {
      static_assert(Order_query<I>(), "");
      assert(is_readable_range(first, last));
      
      return o_min_element(first, last, lt());
    }
    
    
    
  // Min element (range, relation)
  template<typename R, typename Rel>
    auto min_element(R&& range, Rel comp) -> decltype(o_begin(range))
    {
      static_assert(Range_order_query<R>(), "");

      return o_min_element(o_begin(range), o_end(range), comp);
    }

    
    
  // Min element (range, less)
  template<typename R>
    auto min_element(R&& range) -> decltype(o_begin(range))
    {
      static_assert(Range_order_query<R>(), "");

      return o_min_element(o_begin(range), o_end(range));
    }
    

  
  // Min (relation, const)
  template<typename T, typename R>
    const T& o_min(const T& a, const T& b, R comp)
    {
      static_assert(Relation<R, T>(), "");
      return comp(b, a) ? b : a;
    }
    

  
  // Min (relation, non-const)
  template<typename T, typename R>
    T& min(T& a, T& b, R comp)
    {
      static_assert(Relation<R, T>(), "");
      return comp(b, a) ? b : a;
    }
  
  
  
  // Min (less, const)
  template<typename T>
    const T& o_min(const T& a, const T& b)
    {
      static_assert(Totally_ordered<T>(), "");
      return b < a ? b : a;
    }
    
  
  // Min (less, non-const)
  template<typename T>
    T& min(T& a, T& b)
    {
      static_assert(Totally_ordered<T>(), "");
      return b < a ? b : a;
    }
      

      
  // Min (initializer list, relation)
  template<typename T, typename R>
    const T& o_min(std::initializer_list<T> list, R comp)
    {
      static_assert(Relation<R, T>(), "");
      assert(is_strict_weak_ordering(comp));
      assert(list.size() != 0);
      
      return *o_min_element(list.begin(), list.end(), comp);
    }
    
      
  // Min (initializer list, less)
  template<typename T>
    const T& o_min(std::initializer_list<T> list)
    {
      static_assert(Totally_ordered<T>(), "");
      assert(list.size() != 0);

      return *o_min_element(list.begin(), list.end());
    }

  
  
  // Minimum
  // There are a number of minimum and maximum algorithms:
  //
  //    minmax(a, b)
  //    minmax(a, b, comp)
  //    minmax(list)
  //    minmax(list, comp)
  //
  // The following algorithms select a minimum among a sequence of elements.
  //
  //    minmax_element(first, last)
  //    minmax_element(first, last, comp)
  //    minmax_element(range)
  //    minmax_element(range, comp)    


  
  // Max element (relation)
  template<typename I, typename R>
    I o_max_element(I first, I last, R comp)
    {
      static_assert(Relational_query<I, R>(), "");
      assert(is_readable_range(first, last));
      assert(is_strict_weak_ordering(comp));

      if(first != last) {
        for(I i = o_next(first); i != last; ++i) {
          if(comp(*first, *i))
            first = i;
        }
      }
      return first;
    }
  
  
  
  // Max element (less)
  template<typename I>
    I o_max_element(I first, I last)
    {
      static_assert(Order_query<I>(), "");
      assert(is_readable_range(first, last));
      
      return o_max_element(first, last, lt());
    }

  
  
  // Max element (range, relation)
  template<typename R, typename Rel>
    auto max_element(R&& range, Rel comp) -> decltype(o_begin(range))
    {
      static_assert(Range_order_query<R>(), "");

      return o_max_element(o_begin(range), o_end(range), comp);
    }

    
    
  // Max element (range, less)
  template<typename R>
    auto max_element(R&& range) -> decltype(o_begin(range))
    {
      static_assert(Range_order_query<R>(), "");

      return o_max_element(o_begin(range), o_end(range));
    }

    
    
  // Max (relation, const)
  template<typename T, typename R>
    const T& o_max(const T& a, const T& b, R comp)
    {
      static_assert(Relation<R, T>(), "");
      return comp(a, b) ? b : a;
    }
    

  
  // Max (relation, non-const)
  template<typename T, typename R>
    T& max(T& a, T& b, R comp)
    {
      static_assert(Relation<R, T>(), "");
      return comp(a, b) ? b : a;
    }
  
  
  
  // Max (less, const)
  template<typename T>
    const T& o_max(const T& a, const T& b)
    {
      static_assert(Totally_ordered<T>(), "");
      return a < b ? b : a;
    }
    
  
  // Max (less, non-const)
  template<typename T>
    T& max(T& a, T& b)
    {
      static_assert(Totally_ordered<T>(), "");
      return a < b ? b : a;
    }
      

      
  // Max (initializer list, relation)
  template<typename T, typename R>
    const T& o_max(std::initializer_list<T> list, R comp)
    {
      static_assert(Relation<R, T>(), "");
      assert(is_strict_weak_ordering(comp));
      assert(list.size() != 0);
      
      return *o_max_element(list.begin(), list.end(), comp);
    }
    
      
  // Max (initializer list, less)
  template<typename T>
    const T& o_max(std::initializer_list<T> list)
    {
      static_assert(Totally_ordered<T>(), "");
      assert(list.size() != 0);

      return *o_max_element(list.begin(), list.end());
    }
  


  // Minimum and maximum
  // There are a number of minimum and maximum algorithms:
  //
  //    minmax(a, b)
  //    minmax(a, b, comp)
  //    minmax(list)
  //    minmax(list, comp)
  //
  // The following algorithms select the minimum and maximum from a range
  // of elements.
  //
  //    minmax_element(first, last)
  //    minmax_element(first, last, comp)
  //    minmax_element(range)
  //    minmax_element(range, comp)    

  
  
  // Minmax element (relation)
  //
  // FIXME: Implement this algorithm.
  template<typename I, typename R>
    std::pair<I, I> o_minmax_element(I first, I last, R comp)
    {
      static_assert(Relational_query<I, R>(), "");
      assert(is_readable_range(first, last));
      assert(is_strict_weak_ordering(comp));

      return std::minmax_element(first, last, comp);
    }
  
  
  
  // Minmax element (less)
  template<typename I>
    I o_minmax_element(I first, I last)
    {
      static_assert(Order_query<I>(), "");
      assert(is_readable_range(first, last));
      
      return o_minmax_element(first, last, lt());
    }

  
  
  // Minmax (relation, const)
  template<typename T, typename R>
    std::pair<const T&, const T&> o_minmax(const T& a, const T& b, R comp)
    {
      static_assert(Relation<R, T>(), "");
      
      using P = std::pair<const T&, const T&>;
      return comp(b, a) ? P{b, a} : P{a, b};
    }
    

  
  // Minmax element (range, relation)
  template<typename R, typename Rel>
    auto minmax_element(R&& range, Rel comp) -> decltype(o_begin(range))
    {
      static_assert(Range_order_query<R>(), "");

      return o_minmax_element(o_begin(range), o_end(range), comp);
    }

    
    
  // Minmax element (range, less)
  template<typename R>
    auto minmax_element(R&& range) -> decltype(o_begin(range))
    {
      static_assert(Range_order_query<R>(), "");

      return o_minmax_element(o_begin(range), o_end(range));
    }

    
    
  // Minmax (relation, non-const)
  template<typename T, typename R>
    std::pair<T&, T&> minmax(T& a, T& b, R comp)
    {
      static_assert(Relation<R, T>(), "");
      
      using P = std::pair<T&, T&>;
      return comp(b, a) ? P{b, a} : P{a, b};
    }
  
  
  
  // Minmax (less, const)
  template<typename T>
    std::pair<const T&, const T&> o_minmax(const T& a, const T& b)
    {
      static_assert(Totally_ordered<T>(), "");
      
      using P = std::pair<const T&, const T&>;
      return b < a ? P{b, a} : P{a, b};
    }
    
  
  // Minmax (less, non-const)
  template<typename T>
    std::pair<T&, T&> minmax(T& a, T& b)
    {
      static_assert(Totally_ordered<T>(), "");
      
      using P = std::pair<T&, T&>;
      return b > a ? P{b, a} : P{a, b};
    }
      

      
  // Minmax (initializer list, relation)
  template<typename T, typename R>
    std::pair<const T&, const T&> o_minmax(std::initializer_list<T> list, R comp)
    {
      static_assert(Relation<R, T>(), "");
      assert(is_strict_weak_ordering(comp));
      assert(list.size() != 0);
      
      return o_minmax_element(list.begin(), list.end(), comp);
    }
    
      
  // Minmax (initializer list, less)
  template<typename T>
    std::pair<const T&, const T&> o_minmax(std::initializer_list<T> list)
    {
      static_assert(Totally_ordered<T>(), "");
      assert(list.size() != 0);

      return *o_minmax_element(list.begin(), list.end());
    }
  
  
  
} // namespace origin

#endif
