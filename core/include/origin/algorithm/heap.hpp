// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ALGORITHM_HEAP_HPP
#define ORIGIN_ALGORITHM_HEAP_HPP

namespace origin
{
  // D-ary heaps
  // The d-ary heap algorithms parallel the standard heap algorithms but
  // are explicitly parameterized over the arity of the heap.
  //
  // TODO: These algoriths don't strictly need to include the "d_heap"
  // part of the name to distinguish them from the standard algorithm names.
  // Because the first parameter isn't explicitly stated in say, 
  // push_heap(first, last), push_heap<3>(first, last) wouldn't be a canidate.
  // Consider renaming these algorithms.
  

  // Returns true if c is a child of p in a d-ary heap.
  template<std::size_t D, typename Size>
    inline bool is_d_heap_child(Size p, Size c)
    {
      static_assert(D > 1, "");
      return (c >= D * p) && (c < D * (p - 1));
    }
  
  // Returns true if c is the last child offset of in a heap.
  template<std::size_t D, typename Size>
    inline bool is_d_heap_last_child(Size c)
    {
      static_assert(D > 1, "");
      return (c % D) == (D - 1);
    }
  
  
  // Returns the offset m past first such that is_d_heap_n<D>(first, m) is 
  // false.
  template<std::size_t D, typename I, typename R>
    Distance_type<I> is_d_heap_until_distance(I first, Distance_type<I> n, R comp)
    {
      static_assert(D > 1, "");
      static_assert(Relational_query<I, R>(), "");

      Distance_type<I> p = 0;
      for(Distance_type<I> c = 1; c < n; ++c) {
        if(comp(first[p], first[c]))
          return c;
        if(is_d_heap_last_child<D>(c))
          ++p;
      }
      return n;
    }   
  
  
  // Is heap until n (relation)
  // Returns the last iterator iterator i for which is_d_heap<D>(first, i, comp)
  // is true.
  template<std::size_t D, typename I, typename R>
    inline I is_d_heap_until_n(I first, Distance_type<I> n, R comp)
    {
      static_assert(D > 1, "");
      static_assert(Relational_query<I, R>(), "");
      assert(is_readable_range(first, n));

      return first + is_d_heap_until_distance<D>(first, n, comp);
    }
    
  // Is heap until n (less)
  // Returns the last iterator i for which is_d_heap<D>(first, i) is true.
  template<std::size_t D, typename I>
    inline bool is_d_heap_until_n(I first, Distance_type<I> n)
    {
      return is_d_heap_until_n(first, n, lt{});
    }
  
  
  
  // Is heap until (relation)
  // Returns the last iterator i in [first, last) for which 
  // is_d_heap<D>(first, i, comp) is true.
  template<std::size_t D, typename I, typename R>
    inline I is_d_heap_until(I first, I last, R comp)
    {
      static_assert(D > 1, "");
      static_assert(Relational_query<I, R>(), "");
      return is_d_heap_until_n<D>(first, last - first, comp);
    }
    
  // Is heap until (less)
  // Returns the last iterator i in [first, last) for which 
  // is_d_heap<D>(first, last) is true.
  template<std::size_t D, typename I>
    inline I is_d_heap_until(I first, I last)
    {
      static_assert(Order_query<I>(), "");
      return is_d_heap_until<D>(first, last, lt{});
    }


    
  template<std::size_t D, typename I, typename R>
    bool is_d_heap_n(I first, Distance_type<I> n, R comp)
    {
      return is_d_heap_until_distance<D>(first, n, comp) == n;
    }
    
  
  // Is heap (relation)
  // Returns true if the elements of [first, last) are heap-ordered by comp.
  template<std::size_t D, typename I, typename R>
    bool is_d_heap(I first, I last, R comp)
    {
      static_assert(Relational_query<I, R>(), "");
      assert(is_readable_reange(first, last));
      return is_d_heap_until<D>(first, last, comp) == last;
    }
    
    
  // Is heap (less)
  // Returns true if the elements of [first, last) are max-heap ordered.
  template<std::size_t D, typename I>
    bool is_d_heap(I first, I last)
    {
      static_assert(Order_query<I>(), "");
      assert(is_readable_range(first, last));
      return is_d_heap<D>(first, last, lt{});
    }

    
  
  // D-heap parent
  // Returns the parent of offset of the child c in a d-ary heap.
  template<std::size_t D, typename Size>
    Size d_heap_parent(Size c)
    {
      return (c - 1) / D;
    }
  
  

    
  // Push d-heap back (counted range, relation)
  // Re-establish the heap ordering of [first, first + n) after an element
  // has been placed at the back of the range. This is essentially the up-heap
  // operation.
  template<std::size_t D, typename I, typename R>
    void push_d_heap_back_n(I first, Distance_type<I> n, R comp)
    {
      static_assert(Sort<I, R>(), "");
      assert(is_permutable_range(first, n));

      --n;
      Value_type<I> tmp = std::move(first[n]);

      // Walk up the heap, pushing values down.
      // TODO: This looks a lot like an early terminating move algorithm with
      // a highly customized step. It could be worth generalizing, but I'm
      // not quite sure how.
      Distance_type<I> p = d_heap_parent<D>(n);
      while(n > 0 && comp(first[p], first[n])) {
        first[n] = std::move(first[p]);
        n = p;
        p = d_heap_parent<D>(n);
      }

      // When done, put value back intot he heap.
      first[n] = std::move(tmp);
    }
  
    
    
  // Push_d_heap
  template<std::size_t D, typename I, typename R>
    void push_d_heap(I first, I last, R comp)
    {
      static_assert(Sort<I, R>(), "");
      assert(first != last);
      assert(is_permutable_range(first, last));

      push_d_heap_back_n<D>(first, last - first, comp);
    }
    
  template<std::size_t D, typename I>
    void push_d_heap(I first, I last)
    {
      static_assert(Sort<I>(), "");
      push_d_heap<D>(first, last, lt{});
    }
    
  template<std::size_t D, typename R, typename Rel>
    void push_d_heap(R&& range, Rel comp)
    {
      return push_d_heap<D>(o_begin(range), o_end(range), comp);
    }

  template<std::size_t D, typename R>
    void push_d_heap(R&& range)
    {
      return push_d_heap<D>(o_begin(range), o_end(range));
    }

    
    
  // Return the offset of the first child of p.
  template<std::size_t D, typename Size>
    Size d_heap_first_child(Size p)
    {
      return D * (p + 1) - (D - 1);
    }

  // Return the offset of the first child of p.
  template<std::size_t D, typename Size>
    Size d_heap_last_child(Size p)
    {
      return D * (p + 1);
    }
  

  // Select d-heap child.
  // This class is used to select an iterator pointing to the maximum child
  // in a d-ary heap. If sel is a function object of this type, then
  //
  //    sel(last, comp)
  //
  // Will select the maximum of the D - 1 elements preceeding last. Note that
  // last is a valid iterator in this context.
  template<std::size_t D, typename I, typename R> struct select_d_heap_child;
  
  // Specialization for binary heaps.
  template<typename I, typename R>
    struct select_d_heap_child<2, I, R>
    {
      I operator()(I last, R comp) const
      { 
        return iter_select_1_2(last - 1, last, comp);
      }
    };
    
  // Specialization for ternary heaps.
  template<typename I, typename R>
    struct select_d_heap_child<3, I, R>
    {
      I operator()(I last, R comp) const
      { 
        return iter_select_2_3(last - 2, last - 1, last, comp);
      }
    };

  // Specialization for quaternary heaps.
  template<typename I, typename R>
    struct select_d_heap_child<4, I, R>
    {
      I operator()(I last, R comp) const
      { 
        return iter_select_3_4(last - 3, last - 2, last - 1, last, comp);
      }
    };

    

  // Push d-heap front (counted range, relation)
  // Re-establish the heap ordering of [first, first + n) after an element
  // has been placed at the front of the range. This is essentially a down-heap 
  // operation.
  template<std::size_t D, typename I, typename R> 
    void push_d_heap_front_n(I first, Distance_type<I> n, R comp)
    {
      // assert(is_d_heap_n<D>(first, n, comp) == n);

      Distance_type<I> top = 0;
      
      Distance_type<I> p = 0; // The current parent (hole).
      Distance_type<I> c = p; // The current child
      I hole = first + p;
      Value_type<I> tmp = std::move(first[p]);
      
      while(c < (n - 1) / Distance_type<I>{D}) {  // FIXME: Probably not right...
        c = d_heap_last_child<D>(p);

        // Select the max (according to comp) of the current set of children.
        // Note that first + c referes to the last child of p.
        select_d_heap_child<D, I, R> sel;
        hole = sel(first + c, comp);
        first[p] = std::move(*hole);

        p = hole - first;
      }
      
      std::cout << c << "\n";
      *hole = std::move(tmp);
      
      /*
      // FIXME: Replace this with a switch
      if ((len & 1) == 0 && c == (len - 2) / 2) {
        c = 2 * (c + 1); 
        *(first + p) = _GLIBCXX_MOVE(*(first + (c - 1)));
        p = c - 1;
      }
      
      std::push_heap(first, p, top, _GLIBCXX_MOVE(value));
      */
    }
    
  
  // Pop d-heap
  template<std::size_t D, typename I, typename R>
    void pop_d_heap(I first, I last, R comp)
    {
      static_assert(Sort<I, R>(), "");
      assert(first != last);
      // assert(is_d_heap<D>(first, last, comp));
      
      iter_swap(first, --last);
      push_d_heap_front_n<D>(first, last - first, comp);
    }
    
  template<std::size_t D, typename I>
    void pop_d_heap(I first, I last)
    {
      static_assert(Sort<I>(), "");

      pop_d_heap<D>(first, last, lt{});
    }
    
  template<std::size_t D, typename R, typename Rel>
    void pop_d_heap(R&& range, R comp)
    {
      pop_d_heap<D>(o_begin(range), o_end(range), comp);
    }

  template<std::size_t D, typename R>
    void pop_d_heap(R&& range)
    {
      pop_d_heap<D>(o_begin(range), o_end(range));
    }
    

    
    
  template<typename I, typename R>
    void o_make_heap(I first, I last, R comp)
    {
      static_assert(Sort<I, R>(), "");
      std::make_heap(first, last, comp);
    }
    
  template<typename I>
    void o_make_heap(I first, I last)
    {
      static_assert(Sort<I>(), "");
      std::make_heap(first, last);
    }

    
    
  template<typename I, typename R>
    void o_sort_heap(I first, I last, R comp)
    {
      static_assert(Sort<I, R>(), "");
      std::sort_heap(first, last, comp);
    }

  template<typename I>
    void o_sort_heap(I first, I last)
    {
      static_assert(Sort<I>(), "");
      std::sort_heap(first, last);
    }

} // namespace origin

#endif
