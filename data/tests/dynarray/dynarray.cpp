// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <cassert>
#include <string>
#include <iostream>
#include <origin/dynarray/dynarray.hpp>
#include "fake_allocator.hpp"

using namespace origin;
using namespace std;

template<typename T>
  struct dynarray_test_scope
  {
    using alloc_type = allocator_wrapper<T>;
    using dynarray_test_type = dynarray<T, alloc_type>;
    using pointer = typename dynarray_test_type::pointer;
    using const_pointer = typename dynarray_test_type::const_pointer;
    using value_type = typename dynarray_test_type::value_type;
    using size_type = typename dynarray_test_type::size_type;
    using allocator_type = typename dynarray_test_type::allocator_type;
    using iterator = typename dynarray_test_type::iterator;
    using const_iterator = typename dynarray_test_type::const_iterator;
    using reverse_iterator = typename dynarray_test_type::reverse_iterator;
    using const_reverse_iterator = typename dynarray_test_type::const_reverse_iterator;
  
  public:
    void run_tests()
    {
      // To be moved into something else so
      // this can be given arbitrarly.
      size_t default_size = 0;
      size_t n_items = 5;
      value_type n_items_value = 4;
      initializer_list<value_type> init_list({1, 6, 29, 94});
      size_t range_size = 6;
      value_type value_range[] = {0, 2, 56, 32, 55, 3};
      pointer range_start = value_range;
      pointer range_end = range_start + range_size;
      
      // Constructor Tests.
      // Default constructor.
      {
        dynarray_test_type subject;
        assert(static_alloc_helper::default_ctor_called);
        assert(static_alloc_helper::static_alloc_helper::allocated_memory_ptr == nullptr);
        assert(subject.size() == default_size);
        assert(subject.empty());
      }
      reset_static_alloc_helper();
      
      // Allocator Constructor.
      {
        alloc_type test_alloc;
        reset_static_alloc_helper();
        dynarray_test_type subject(test_alloc);
        
        assert(static_alloc_helper::copy_ctor_called);
        assert(!static_alloc_helper::copy_other_ctor_called);
        assert(subject.size() == default_size);
        assert(subject.empty());
      }
      reset_static_alloc_helper();
      
      // N Item Constructor
      {
        {
          dynarray_test_type subject(n_items);
          assert(subject.size() == n_items);
          assert(!subject.empty());
          for(iterator p = subject.begin(); p != subject.end(); ++p)
            assert(*p == value_type());
          
          // Allocator related assertions.
          assert(static_alloc_helper::default_ctor_called);
          // assert(static_alloc_helper::construct_called);
          // assert(static_alloc_helper::construct_call_count == 5);
          
        }
        // Destruction tests.
        assert(static_alloc_helper::destroy_called);
        assert(static_alloc_helper::destroy_call_count == n_items);
        assert(static_alloc_helper::dtor_called);
      }
      reset_static_alloc_helper();
      
      
      // n item of value constructor  
      {
        {
          dynarray_test_type subject(n_items, n_items_value);
          assert(subject.size() == n_items);
          assert(!subject.empty());
          for(iterator p = subject.begin(); p != subject.end(); ++p)
            assert(*p == n_items_value);
          
          // Allocator related assertions.
          assert(static_alloc_helper::default_ctor_called);
          // assert(static_alloc_helper::construct_called);
          // assert(static_alloc_helper::construct_call_count == 5);
          
        }
        // Destruction tests.
        assert(static_alloc_helper::destroy_called);
        assert(static_alloc_helper::destroy_call_count == n_items);
        assert(static_alloc_helper::dtor_called);
      }
      reset_static_alloc_helper();
      
      // n item of value alloc + constructor
      {
        {
          alloc_type test_alloc;
          reset_static_alloc_helper();
          dynarray_test_type subject(n_items, n_items_value, test_alloc);
          assert(subject.size() == n_items);
          assert(!subject.empty());
          for(iterator p = subject.begin(); p != subject.end(); ++p)
            assert(*p == n_items_value);
          
          // Allocator related assertions.
          assert(!static_alloc_helper::default_ctor_called);
          assert(static_alloc_helper::copy_ctor_called);  
          // assert(static_alloc_helper::construct_called);
          // assert(static_alloc_helper::construct_call_count == 5);
          
        }
        // Destruction tests.
        assert(static_alloc_helper::destroy_called);
        assert(static_alloc_helper::destroy_call_count == n_items);
        assert(static_alloc_helper::dtor_called);
      }
      reset_static_alloc_helper();
      
      // initializer list constructor
      {
        // init_list
        {
          dynarray_test_type subject(init_list);
          assert(subject.size() == init_list.size());
          assert(!subject.empty());
          assert(equal(init_list.begin(), init_list.end(), subject.begin()));
          assert(static_alloc_helper::default_ctor_called);
          assert(static_alloc_helper::allocate_called);
          // assert(static_alloc_helper::construct_called);
          // assert(static_alloc_helper::construct_call_count = init_list.size());
        }
        assert(static_alloc_helper::destroy_called);
        assert(static_alloc_helper::destroy_call_count == init_list.size());
        assert(static_alloc_helper::deallocate_called);
      }
      reset_static_alloc_helper();
      
      // initializer list + alloc constructor
      {
        // init_list
        {
          alloc_type test_alloc;
          reset_static_alloc_helper();
          dynarray_test_type subject(init_list, test_alloc);
          assert(subject.size() == init_list.size());
          assert(!subject.empty());
          assert(equal(init_list.begin(), init_list.end(), subject.begin()));
          
          assert(!static_alloc_helper::default_ctor_called);
          assert(static_alloc_helper::copy_ctor_called);
          assert(static_alloc_helper::allocate_called);
          
          // assert(static_alloc_helper::construct_called);
          // assert(static_alloc_helper::construct_call_count = init_list.size());
        }
        assert(static_alloc_helper::destroy_called);
        assert(static_alloc_helper::destroy_call_count == init_list.size());
        assert(static_alloc_helper::deallocate_called);
      }
      reset_static_alloc_helper();
      
      // range constructor.
      {
        {
          dynarray_test_type subject(range_start, range_end);
          assert(subject.size() == range_size);
          assert(!subject.empty());
          assert(static_alloc_helper::default_ctor_called);
          assert(static_alloc_helper::allocate_called);
          assert(equal(range_start, range_end, subject.begin()));
        }
        assert(static_alloc_helper::destroy_called);
        assert(static_alloc_helper::destroy_call_count == range_size);
        assert(static_alloc_helper::deallocate_called);
      }
      reset_static_alloc_helper();
      
      // range + allocator constructor.
      {
        {
          alloc_type test_alloc;
          reset_static_alloc_helper();
          dynarray_test_type subject(range_start, range_end, test_alloc);
          assert(subject.size() == range_size);
          assert(!subject.empty());
          assert(static_alloc_helper::copy_ctor_called);
          assert(!static_alloc_helper::default_ctor_called);
          assert(static_alloc_helper::allocate_called);
          assert(equal(range_start, range_end, subject.begin()));
        }
        assert(static_alloc_helper::destroy_called);
        assert(static_alloc_helper::destroy_call_count == range_size);
        assert(static_alloc_helper::deallocate_called);
      }
      reset_static_alloc_helper();
      
      // copy constructor.
      {
        {
          dynarray_test_type first_subject(range_start, range_end);
          reset_static_alloc_helper();
          dynarray_test_type second_subject(first_subject);
          assert(second_subject.size() == range_size);
          assert(!second_subject.empty());
          assert(first_subject == second_subject);
          
          assert(static_alloc_helper::copy_ctor_called);
          assert(static_alloc_helper::allocate_called);
          assert(equal(range_start, range_end, second_subject.begin()));
        }
        assert(static_alloc_helper::destroy_called);
        assert(static_alloc_helper::destroy_call_count == range_size*2);
        assert(static_alloc_helper::deallocate_called);
      }
      reset_static_alloc_helper();
      
      // Move Constructor.
      {
        {
          dynarray_test_type first_subject(range_start, range_end);
          reset_static_alloc_helper();
          dynarray_test_type second_subject(move(first_subject));
          assert(second_subject.size() == range_size);
          assert(!second_subject.empty());
          
          assert(static_alloc_helper::move_ctor_called);
          assert(!static_alloc_helper::allocate_called);
          assert(equal(range_start, range_end, second_subject.begin()));
        }
        assert(static_alloc_helper::destroy_called);
        assert(static_alloc_helper::destroy_call_count == range_size);
        assert(static_alloc_helper::deallocate_called);
      }
      reset_static_alloc_helper();
      

      // Testin Assignment operators
      // Copy Assignment
      {
        {
          dynarray_test_type first_subject(range_start, range_end);
          dynarray_test_type second_subject;
          reset_static_alloc_helper();
          second_subject = first_subject;
          
          assert(second_subject.size() == range_size);
          assert(!second_subject.empty());
          assert(first_subject == second_subject);
          
          assert(static_alloc_helper::copy_ctor_called);
          assert(static_alloc_helper::allocate_called);
          assert(equal(range_start, range_end, second_subject.begin()));
        }
        assert(static_alloc_helper::destroy_called);
        assert(static_alloc_helper::destroy_call_count == range_size*2);
        assert(static_alloc_helper::deallocate_called);
      }
      reset_static_alloc_helper();
      
      // Move Assignment
      {
        {
          dynarray_test_type first_subject(range_start, range_end);
          dynarray_test_type second_subject;
          reset_static_alloc_helper();
          second_subject = move(first_subject);
          assert(second_subject.size() == range_size);
          assert(!second_subject.empty());
          
          assert(static_alloc_helper::move_ctor_called);
          assert(!static_alloc_helper::allocate_called);
          assert(equal(range_start, range_end, second_subject.begin()));
        }
        assert(static_alloc_helper::destroy_called);
        assert(static_alloc_helper::destroy_call_count == range_size);
        assert(static_alloc_helper::deallocate_called);
      }
      reset_static_alloc_helper();
      
      // More general member function tests.
      // size function.
      {
        dynarray_test_type subject(4);
        assert(subject.size() == 4);
      }
      
      // max_size test
      {
          allocator_type alloc;
          dynarray_test_type subject;
          assert(alloc.max_size() == subject.max_size());
      }
      
      // empty function test.
      {
        {
            dynarray_test_type subject({1,2,3});
            assert(!subject.empty());
        }
        {
            dynarray_test_type subject;
            assert(subject.empty());
        }
      }
      
      // Indexing operator tests.
      {
        // Non-Const
        {
          dynarray_test_type subject(range_start, range_end);
          auto range_iter = range_start;
          for(size_type index = 0; index < subject.size(); ++index, ++range_iter)
          {
            assert(subject[index] == *range_iter);
          }
        }
        
        // Const version
        {
          const dynarray_test_type subject(range_start, range_end);
          auto range_iter = range_start;
          for(size_type index = 0; index < subject.size(); ++index, ++range_iter)
          {
            assert(subject[index] == *range_iter);
          }
        }
      }
      
      // At function
      {
        // Non-Const valid index
        {
          dynarray_test_type subject(init_list);
          assert(subject.at(0) == *init_list.begin());
        }

        // Non-Const invalid index
        {
          bool did_catch_exception = false;
          dynarray_test_type subject(init_list);
          try{
            subject.at(init_list.size());
          }catch(std::out_of_range const&) {
            did_catch_exception = true;
          }catch(...){
            assert(!"Caught unknown type of exception");
          }
          assert(did_catch_exception);
        }
        
        // Const valid index
        {
          const dynarray_test_type subject(init_list);
          assert(subject.at(0) == *init_list.begin());
        }

        // Const invalid index
        {
          bool did_catch_exception = false;
          const dynarray_test_type subject(init_list);
          try{
            subject.at(init_list.size());
          }catch(std::out_of_range const&) {
            did_catch_exception = true;
          }catch(...){
            assert(!"Caught unknown type of exception");
          }
          assert(did_catch_exception);
        }
      }
      
      /*
      front_function();
      const_front_function();
      back_function();
      const_back_function();
      data_function();
      const_data_function();
      */
      
      /*
      // Iterator member function tests.
      begin_function();
      end_function();
      const_begin_function();
      const_end_function();
      cbegin_function();
      cend_function();
      rbegin_function();
      rend_function();
      const_rbegin_function();
      const_rend_function();
      crbegin_function();
      crend_function();
      */
      
      /*
      // Comparison operator tests.
      equality_comparison();
      inequality_comparison();
      less_than_comparison();
      greater_than_comparison();
      less_than_or_equal_to_comparison();
      greater_than_or_equal_to_comparison();
      */
      
      /*
      // Utility functions and free functions.
      member_function_swap();
      free_function_swap(); 
      */
      
    }
  };
int main()
{
  dynarray_test_scope<float> dynarray_tests;
  dynarray_tests.run_tests();
}


#if 0
        // -----------------------------------------------------------------
        // index operator
        // reference operator[](size_type n)
        static void indexing_operator()
        {
            std::initializer_list<T> init_list = {1,2,3,5};
            self subject(init_list);
            auto init_list_iter = init_list.begin();
            for(size_type index = 0; index < subject.size(); ++index)
            {
                assert(subject[index] == *init_list_iter);
                ++init_list_iter;
            }
        }
        
        // -----------------------------------------------------------------
        // const index operator
        // const_reference operator[](size_type n) const
        static void const_indexing_operator()
        {
            std::initializer_list<T> init_list = {1,2,3,5};
            self subject(init_list);
            const_indexing_operator_test(subject, init_list);
        }
        
        static void const_indexing_operator_test(self const& subject, initializer_list<T> const& init_list)
        {
            auto init_list_iter = init_list.begin();
            for(size_type index = 0; index < subject.size(); ++index)
            {
                assert(subject[index] == *init_list_iter);
                ++init_list_iter;
            }            
        }
        
        // -----------------------------------------------------------------
        // at index function
        // reference at(size_type n)
        static void at_index_function()
        {
            // two test cases for the at function
            // throws an exception and doesnt throw.
            self subject({1,2,3,4,5});
            
            // Regular access non-throwing.
            assert(subject.at(0) == 1);
            // invalid access.
            try{
                subject.at(7);
            }catch(std::out_of_range const& e){
            }catch(...) {
                assert(false && "Invalid exception throw");
            }
        }
        
        // -----------------------------------------------------------------
        // const at index function
        // const_reference at(size_type n) const
        static void const_at_index_function()
        {
            // two test cases for the at function
            // throws an exception and doesnt throw.
            const self subject({1,2,3,4,5});
            
            // Regular access non-throwing.
            assert(subject.at(0) == 1);
            // invalid access.
            try{
                subject.at(7);
            }catch(std::out_of_range const& e){
            }catch(...) {
                assert(false && "Invalid exception throw");
            }
        }
        
        // -----------------------------------------------------------------
        // front function
        // reference front()
        static void front_function()
        {
            self subject({1,2,3,4,5});
            assert(subject.front()==1);
            subject.front() =7;
            assert(subject.front() == 7);
        }
        
        // -----------------------------------------------------------------
        // const front function
        // const_reference front() const
        static void const_front_function()
        {
            const self subject({1,2,3,4,5});
            assert(subject.front()==1);
        }
        
        // -----------------------------------------------------------------
        // back function
        // reference back()
        static void back_function()
        {
            self subject({1,2,3,4,5});
            assert(subject.back()==5);
            subject.back() = 7;
            assert(subject.back() == 7);
        }
        
        // -----------------------------------------------------------------
        // const back function
        // const_reference back() const
        static void const_back_function()
        {
            const self subject({1,2,3,4,5});
            assert(subject.back() == 5);
        }
        
        // -----------------------------------------------------------------
        // data function
        // T* data()
        static void data_function()
        {
            {
                self subject;
                assert(subject.data() == nullptr);
            }
            {
                self subject({2});
                assert(subject.data() != 0);
                assert(*subject.data() == 2);
            }
        }
        
        // -----------------------------------------------------------------
        // const data function
        // const T* data() const
        static void const_data_function()
        {
            {
                const self subject;
                assert(subject.data() == nullptr);
            }
            {
                const self subject({2});
                assert(subject.data() != 0);
                assert(*subject.data() == 2);
            }
        }
        
        // =================================================================
        //                     Iterator test functions
        // =================================================================
        static void run_iterator_functions()
        {
            begin_function();
            end_function();
            const_begin_function();
            const_end_function();
            cbegin_function();
            cend_function();
            rbegin_function();
            rend_function();
            const_rbegin_function();
            const_rend_function();
            crbegin_function();
            crend_function();
        }
        
        // Helper functions to help test the iterator functions.
        pointer get_first()
        {
            return this->first;
        }
        
        const_pointer get_cfirst() const
        {
            return this->first;
        }
        
        pointer get_last()
        {
            return this->last;
        }
        
        const_pointer get_clast() const
        {
            return this->last;
        }
        
        // -----------------------------------------------------------------
        // begin function
        // iterator begin()
        static void begin_function()
        {
            self subject({1,2,3,4});
            iterator subject_iter = subject.begin();
            assert(subject_iter == subject.get_first());
            assert(*subject_iter == 1);
        }

        // -----------------------------------------------------------------
        // end function
        // iterator end()
        static void end_function()
        {
            self subject({1,2,3,4});
            iterator subject_iter = subject.end();
            assert(subject_iter == subject.get_last());
            assert(*(--subject_iter) == 4);
        }
        
        // -----------------------------------------------------------------
        // const begin function
        // const_iterator begin() const
        static void const_begin_function()
        {
            const self subject({1,2,3,4});
            const_iterator subject_iter = subject.begin();
            assert(subject_iter == subject.get_cfirst());
            assert(*subject_iter == 1);
        }
        
        // -----------------------------------------------------------------
        // const end function
        // const_iterator end() const
        static void const_end_function()
        {
            const self subject({1,2,3,4});
            const_iterator subject_iter = subject.end();
            assert(subject_iter == subject.get_clast());
            assert(*(--subject_iter) == 4);
        }
        
        // -----------------------------------------------------------------
        // cbegin function
        // const_iterator cbegin() const
        static void cbegin_function()
        {
            self subject({1,2,3,4});
            const_iterator subject_iter = subject.cbegin();
            assert(subject_iter == subject.get_cfirst());
            assert(*subject_iter == 1);
        }

        // -----------------------------------------------------------------
        // cend function
        // const_iterator cend() const
        static void cend_function()
        {
            self subject({1,2,3,4});
            const_iterator subject_iter = subject.cend();
            assert(subject_iter == subject.get_clast());
            assert(*(--subject_iter) == 4);
        }
        
        // -----------------------------------------------------------------
        // reverse begin function
        // reverse_iterator rbegin()
        static void rbegin_function()
        {
            self subject({1,2,3,4});
            reverse_iterator subject_iter = subject.rbegin();
            assert(subject_iter.base() == subject.get_last());
            assert(*subject_iter == 4);
        }
        
        // -----------------------------------------------------------------
        // reverse end function
        // reverse_iterator rend()
        static void rend_function()
        {
            self subject({1,2,3,4});
            reverse_iterator subject_iter = subject.rend();
            assert(subject_iter.base() == subject.get_first());
            assert(*(--subject_iter) == 1);
        }
        
        // -----------------------------------------------------------------
        // const reverse begin function
        // const_reverse_iterator rbegin() const
        static void const_rbegin_function()
        {
            const self subject({1,2,3,4});
            const_reverse_iterator subject_iter = subject.rbegin();
            assert(subject_iter.base() == subject.get_clast());
            assert(*subject_iter == 4);
        }
        
        // -----------------------------------------------------------------
        // const reverse end function
        // const_reverse_iterator rend() const
        static void const_rend_function()
        {
            const self subject({1,2,3,4});
            const_reverse_iterator subject_iter = subject.rend();
            assert(subject_iter.base() == subject.get_cfirst());
            assert(*(--subject_iter) == 1);
        }
        
        // -----------------------------------------------------------------
        // crbegin function
        // const_reverse_iterator crbegin() const
        static void crbegin_function()
        {
            self subject({1,2,3,4});
            const_reverse_iterator subject_iter = subject.crbegin();
            assert(subject_iter.base() == subject.get_clast());
            assert(*subject_iter == 4);
        }
        
        // -----------------------------------------------------------------
        // crend function
        // const_reverse_iterator crend() const
        static void crend_function()
        {
            self subject({1,2,3,4});
            const_reverse_iterator subject_iter = subject.crend();
            assert(subject_iter.base() == subject.get_cfirst());
            assert(*(--subject_iter) == 1);
        }

        // =================================================================
        //                     Comparison Operator tests
        // =================================================================
        static void run_comparison_tests()
        {
            equality_comparison();
            inequality_comparison();
            less_than_comparison();
            greater_than_comparison();
            less_than_or_equal_to_comparison();
            greater_than_or_equal_to_comparison();
        }
        
        // -----------------------------------------------------------------
        // equality comparison
        // bool operator==(dynarray const& x) const
        static void equality_comparison()
        {
            // a > b
            {
                self subject_a({1,2,3});
                self subject_b({1,1,3});
                assert(!(subject_a == subject_b));
            }
            
            // a < b
            {
                self subject_a({1,1,3});
                self subject_b({1,2,3});
                assert(!(subject_a == subject_b));
            }
            
            // a == b
            {
                self subject_a({1,2,3});
                self subject_b({1,2,3});
                assert(subject_a == subject_b);
            }
            
            // Different size tests
            {
                self subject_a({1,2,3,4});
                self subject_b({1,2,3});
                assert(!(subject_a == subject_b));
            }
            
            // Different size tests
            {
                self subject_a({1,2,3});
                self subject_b({1,2,3,4});
                assert(!(subject_a == subject_b));
            }
        }
        
        // -----------------------------------------------------------------
        // inequality comparison
        // bool operator!=(dynarray const& x) const
        static void inequality_comparison()
        {
            // a > b
            {
                self subject_a({1,2,3});
                self subject_b({1,1,3});
                assert(subject_a != subject_b);
            }
            
            // a < b
            {
                self subject_a({1,1,3});
                self subject_b({1,2,3});
                assert(subject_a != subject_b);
            }
            
            // a == b
            {
                self subject_a({1,2,3});
                self subject_b({1,2,3});
                assert(!(subject_a != subject_b));
            }
            
            // Different size tests
            {
                self subject_a({1,2,3,4});
                self subject_b({1,2,3});
                assert(subject_a != subject_b);
            }
            
            // Different size tests
            {
                self subject_a({1,2,3});
                self subject_b({1,2,3,4});
                assert(subject_a != subject_b);
            }
        }

        // -----------------------------------------------------------------
        // less than comparison
        // bool operator<(dynarray const& x) const
        static void less_than_comparison()
        {
            // a > b
            {
                self subject_a({1,2,3});
                self subject_b({1,1,3});
                assert(!(subject_a < subject_b));
            }
            
            // a < b
            {
                self subject_a({1,1,3});
                self subject_b({1,2,3});
                assert(subject_a < subject_b);
            }
            
            // a == b
            {
                self subject_a({1,2,3});
                self subject_b({1,2,3});
                assert(!(subject_a < subject_b));
            }
            
            // Different size tests
            {
                self subject_a({1,2,3,4});
                self subject_b({1,2,3});
                assert(!(subject_a < subject_b));
            }
            
            // Different size tests
            {
                self subject_a({1,2,3});
                self subject_b({1,2,3,4});
                assert(subject_a < subject_b);
            }
        }

        // -----------------------------------------------------------------
        // greater than comparison
        // bool operator>(dynarray const& x) const
        static void greater_than_comparison()
        {
            // a > b
            {
                self subject_a({1,2,3});
                self subject_b({1,1,3});
                assert(subject_a > subject_b);
            }
            
            // a < b
            {
                self subject_a({1,1,3});
                self subject_b({1,2,3});
                assert(!(subject_a > subject_b));
            }
            
            // a == b
            {
                self subject_a({1,2,3});
                self subject_b({1,2,3});
                assert(!(subject_a > subject_b));
            }
            
            // Different size tests
            {
                self subject_a({1,2,3,4});
                self subject_b({1,2,3});
                assert(subject_a > subject_b);
            }
            
            // Different size tests
            {
                self subject_a({1,2,3});
                self subject_b({1,2,3,4});
                assert(!(subject_a > subject_b));
            }
        }
        
        // -----------------------------------------------------------------
        // less than or equal to comparison
        // bool operator<=(dynarray const& x) const
        static void less_than_or_equal_to_comparison()
        {
            // a > b
            {
                self subject_a({1,2,3});
                self subject_b({1,1,3});
                assert(!(subject_a <= subject_b));
            }
            
            // a < b
            {
                self subject_a({1,1,3});
                self subject_b({1,2,3});
                assert(subject_a <= subject_b);
            }
            
            // a == b
            {
                self subject_a({1,2,3});
                self subject_b({1,2,3});
                assert(subject_a <= subject_b);
            }
            
            // Different size tests
            {
                self subject_a({1,2,3,4});
                self subject_b({1,2,3});
                assert(!(subject_a <= subject_b));
            }
            
            // Different size tests
            {
                self subject_a({1,2,3});
                self subject_b({1,2,3,4});
                assert(subject_a < subject_b);
            }
        }
        
        // -----------------------------------------------------------------
        // greater than or equal to comparison
        // bool operator>=(dynarray const& x) const
        static void greater_than_or_equal_to_comparison()
        {
            // a > b
            {
                self subject_a({1,2,3});
                self subject_b({1,1,3});
                assert(subject_a >= subject_b);
            }
            
            // a < b
            {
                self subject_a({1,1,3});
                self subject_b({1,2,3});
                assert(!(subject_a >= subject_b));
            }
            
            // a == b
            {
                self subject_a({1,2,3});
                self subject_b({1,2,3});
                assert(subject_a >= subject_b);
            }
            
            // Different size tests
            {
                self subject_a({1,2,3,4});
                self subject_b({1,2,3});
                assert(subject_a >= subject_b);
            }
            
            // Different size tests
            {
                self subject_a({1,2,3});
                self subject_b({1,2,3,4});
                assert(!(subject_a >= subject_b));
            }
        }
        
        
        // =================================================================
        //               Utility and free function tests
        // =================================================================
        static void run_utility_tests()
        {
            member_function_swap();
            free_function_swap();
        }

        
        // -----------------------------------------------------------------
        // member function swap
        // void swap(dynarray & x)
        static void member_function_swap()
        {
            self subject_x({1,2,3});
            self subject_y({3,2,1});
            
            subject_x.swap(subject_y);
            assert(subject_x == self({3,2,1}));
            assert(subject_y == self({1,2,3}));
        }
        
        // -----------------------------------------------------------------
        // free function swap
        // template<typename T> void swap(dynarray<T>& a, dynarray<T>& b)
        static void free_function_swap()
        {
            base subject_x({1,2,3});
            base subject_y({3,2,1});
            
            swap(subject_x, subject_y);
            assert(subject_x == base({3,2,1}));
            assert(subject_y == base({1,2,3}));
        }
    };
template<typename T>
    std::ostream& operator<<(std::ostream& out, dynarray_test_type<T> const& toprint)
    {
        out << "dynarray {";
        using const_pointer = typename dynarray_test_type<T>::const_pointer;
        for(const_pointer p = toprint.begin();p != toprint.end();)
        {
            out << *p;
            ++p;
            if(p == toprint.end())
            {
                break;
            }else{
                out << ", ";
            }
        }
        out << "}";
        return out;
    }



int main()
{
  dynarray_test_type<float>::run_tests();
  
}
#endif

