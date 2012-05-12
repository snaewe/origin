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
      
      // initializer_list<value_type> subject_a_equal({1, 2, 3, 4});
      // initializer_list<value_type> subject_b_equal({1, 2, 3, 4});
      
      // initializer_list<value_type> subject_a_greater({1, 2, 3, 2});
      // initializer_list<value_type> subject_b_greater({1, 2, 3, 4});
      
      
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
      
      // front Function
      {
        // Non-Const
        {
          dynarray_test_type subject(init_list);
          assert(subject.front() == *init_list.begin());
        }
        
        // Const
        {
          const dynarray_test_type subject(init_list);
          assert(subject.front() == *init_list.begin());
        }
      }
      
      // back Function
      {
        // Non-Const
        {
          dynarray_test_type subject(range_start, range_end);
          pointer range_end_copy_temp = range_end;
          --range_end_copy_temp;
          assert(subject.back() == *range_end_copy_temp );
        }
        
        // Const
        {
          const dynarray_test_type subject(range_start, range_end);
          pointer range_end_copy_temp = range_end;
          --range_end_copy_temp;
          assert(subject.back() == *range_end_copy_temp);
        }
      }
      
      // data Function
      {
        // Non-Const
        {
          dynarray_test_type subject(range_start, range_end);
          assert(subject.data() != 0 );
          assert(subject.data()[0] == *subject.begin());
        }
        
        // Const
        {
          const dynarray_test_type subject(range_start, range_end);
          assert(subject.data() != 0);
          assert(subject.data()[0] == *subject.begin());
        }
      }
      
      // Iterator member function tests.
      {
        dynarray_test_type non_const_subject(range_start, range_end);
        const dynarray_test_type const_subject(range_start, range_end);
        pointer range_back = range_end;
        --range_back;
        
        // begin function
        {
          // Non-Const
          {
            iterator subject_iter = non_const_subject.begin();
            assert(*subject_iter == *range_start);
          }
          
          // Const
          {
            const_iterator subject_iter = const_subject.begin();
            assert(*subject_iter == *range_start);
          }
        }
        
        // end function
        {
          // Non-Const
          {
            iterator subject_iter = non_const_subject.end();
            --subject_iter;
            assert(*subject_iter == *range_back);
          }
          
          // Const
          {
            const_iterator subject_iter = const_subject.end();
            --subject_iter;
            assert(*subject_iter == *range_back);
          }
        }
        // explict const functions.
        // cbegin
        {

          const_iterator subject_iter = const_subject.cbegin();
          assert(*subject_iter == *range_start);
        }
        
        // cend
        {
          const_iterator subject_iter = const_subject.cend();
          --subject_iter;
          assert(*subject_iter == *range_back);
        }
        
        // Reverse iterator functions.
        // rbegin
        {
          // Non-Const
          {
            reverse_iterator subject_iter = non_const_subject.rbegin();
            assert(*subject_iter == *range_back);
          }
          
          // Const
          {
            const_reverse_iterator subject_iter = const_subject.rbegin();
            assert(*subject_iter == *range_back);
          }
        }
        
        // rend
        {
          // Non-Const
          {
            reverse_iterator subject_iter = non_const_subject.rend();
            --subject_iter;
            assert(*subject_iter == *range_start);
          }
          
          // Const
          {
            const_reverse_iterator subject_iter = const_subject.rend();
            --subject_iter;
            assert(*subject_iter == *range_start);
          }
        }
        
        // Explicit Const Functions.
        // crbegin Function.
        {
          const_reverse_iterator subject_iter = const_subject.crbegin();
          assert(*subject_iter == *range_back);
        }
        
        // crend Function
        {
          const_reverse_iterator subject_iter = const_subject.crend();
          --subject_iter;
          assert(*subject_iter == *range_start);
        }
      }

      
      // Comparison operator tests.
      {
        dynarray_test_type subject_a_equal({1, 2, 3, 4});
        dynarray_test_type subject_b_equal({1, 2, 3, 4});
        dynarray_test_type subject_a_less({1, 2, 3, 4});
        dynarray_test_type subject_b_less({1, 2, 3, 6});
        dynarray_test_type subject_a_greater({1, 2, 3, 5});
        dynarray_test_type subject_b_greater({1, 2, 3, 4});
        dynarray_test_type subject_a_larger_size_less({1, 2, 3});
        dynarray_test_type subject_b_larger_size_less({1, 2, 3, 4});
        
        // Equality Operator.
        {
          assert(subject_a_equal == subject_b_equal);
          assert(subject_b_equal == subject_a_equal);

          assert(!(subject_a_less == subject_b_less));
          assert(!(subject_b_less == subject_a_less));
          
          assert(!(subject_a_greater == subject_b_greater));
          assert(!(subject_b_greater == subject_a_greater));
          
          assert(!(subject_a_larger_size_less == subject_b_larger_size_less));
          assert(!(subject_b_larger_size_less == subject_a_larger_size_less));
        }
        
        // Inequality Operator.
        {
          assert(!(subject_a_equal != subject_b_equal));
          assert(!(subject_b_equal != subject_a_equal));

          assert(subject_a_less != subject_b_less);
          assert(subject_b_less != subject_a_less);
          
          assert(subject_a_greater != subject_b_greater);
          assert(subject_b_greater != subject_a_greater);
          
          assert(subject_a_larger_size_less != subject_b_larger_size_less);
          assert(subject_b_larger_size_less != subject_a_larger_size_less);
        }
        
        // less than Operator.
        {
          assert(!(subject_a_equal < subject_b_equal));
          assert(!(subject_b_equal < subject_a_equal));

          assert(subject_a_less < subject_b_less);
          assert(!(subject_b_less < subject_a_less));
          
          assert(!(subject_a_greater < subject_b_greater));
          assert(subject_b_greater < subject_a_greater);
          
          assert(subject_a_larger_size_less < subject_b_larger_size_less);
          assert(!(subject_b_larger_size_less < subject_a_larger_size_less));
        }
        
        // less than or equal Operator.
        {
          assert(subject_a_equal <= subject_b_equal);
          assert(subject_b_equal <= subject_a_equal);

          assert(subject_a_less <= subject_b_less);
          assert(!(subject_b_less <= subject_a_less));
          
          assert(!(subject_a_greater <= subject_b_greater));
          assert(subject_b_greater <= subject_a_greater);
          
          assert(subject_a_larger_size_less <= subject_b_larger_size_less);
          assert(!(subject_b_larger_size_less <= subject_a_larger_size_less));
        }
        
        // greater than Operator.
        {
          assert(!(subject_a_equal > subject_b_equal));
          assert(!(subject_b_equal > subject_a_equal));

          assert(!(subject_a_less > subject_b_less));
          assert(subject_b_less > subject_a_less);
          
          assert(subject_a_greater > subject_b_greater);
          assert(!(subject_b_greater > subject_a_greater));
          
          assert(!(subject_a_larger_size_less > subject_b_larger_size_less));
          assert(subject_b_larger_size_less > subject_a_larger_size_less);
        }

        // greater than Or Equal Operator.
        {
          assert(subject_a_equal >= subject_b_equal);
          assert(subject_b_equal >= subject_a_equal);

          assert(!(subject_a_less >= subject_b_less));
          assert(subject_b_less >= subject_a_less);
          
          assert(subject_a_greater >= subject_b_greater);
          assert(!(subject_b_greater >= subject_a_greater));
          
          assert(!(subject_a_larger_size_less >= subject_b_larger_size_less));
          assert(subject_b_larger_size_less >= subject_a_larger_size_less);
        }
      }
      
      // Utility Member Functions
      // swap Member Function.
      {
        dynarray_test_type subject_a(init_list);
        dynarray_test_type subject_b(range_start, range_end);
        subject_a.swap(subject_b);
        assert(equal(range_start, range_end, subject_a.begin()));
        assert(equal(init_list.begin(), init_list.end(), subject_b.begin()));
      }
      
      // swap Free Function
      {
        dynarray_test_type subject_a(init_list);
        dynarray_test_type subject_b(range_start, range_end);
        swap(subject_a, subject_b);
        assert(equal(range_start, range_end, subject_a.begin()));
        assert(equal(init_list.begin(), init_list.end(), subject_b.begin()));        
      }
    }
  };
int main()
{
  dynarray_test_scope<float> dynarray_tests;
  dynarray_tests.run_tests();
  return 0;
}