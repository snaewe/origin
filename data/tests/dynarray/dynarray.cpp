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


template <typename T>
    struct dynarray_test_type
        :public dynarray<T, allocator_wrapper<T>>
    {
        using self = dynarray_test_type<T>;
        using alloc_type = allocator_wrapper<T>;
        using base = dynarray<T, alloc_type>;
        using typename base::pointer;
        using typename base::const_pointer;
        using typename base::value_type;
        using typename base::size_type;
        using typename base::allocator_type;
        
        // forwarding constructor for testing.
        /*template<typename... Args>
            explicit dynarray_test_type(Args...  args)
                :base(std::forward<Args>(args)...)
            { }
        */
        
        self& operator=(self const& rhs)
        {
            *static_cast<base*>(this) = static_cast<base const&>(rhs);
            return *this;
        }
        
                
        self& operator=(self&& rhs)
        {
            *static_cast<base*>(this) = static_cast<base&&>(rhs);
            return *this;
        }
        
        explicit dynarray_test_type(alloc_type const& alloc = allocator_type{})
            : base{alloc}
        { }

        explicit dynarray_test_type(self const& x)
            : base{x}
        { }

        explicit dynarray_test_type(self&& x)
            : base{std::move(x)}
        { }

        template<typename Iter>
            dynarray_test_type(Iter first_iter, Iter last_iter, alloc_type const& alloc = alloc_type{})
                : base(first_iter, last_iter, alloc)
            { }
            
        explicit dynarray_test_type(std::initializer_list<value_type> list, alloc_type const& alloc = alloc_type{})
            : base{list, alloc}
        { }
        
        explicit dynarray_test_type(size_type n, value_type const& x = value_type{},alloc_type const& alloc = alloc_type{})
            : base{n, x, alloc}
        { }
        
        static void run_tests()
        {
            
            // Constrcutor/Destructor tests
            default_constructor();
            reset_static_alloc_helper();
            
            allocator_constructor();
            reset_static_alloc_helper();
            
            n_item_constructor();
            reset_static_alloc_helper();
            
            copy_constructor();
            reset_static_alloc_helper();
            
            move_constructor();
            reset_static_alloc_helper();
            
            range_constructor();
            reset_static_alloc_helper();
            
            range_alloc_constructor();
            reset_static_alloc_helper();
            
            initializer_list_constructor();
            reset_static_alloc_helper();
            
            initializer_list_alloc_constructor();
            reset_static_alloc_helper();
            
            n_item_of_value_constructor();
            reset_static_alloc_helper();
            
            n_item_of_value_alloc_constructor();
            reset_static_alloc_helper();
            
            
            // Member Function Tests.
            copy_assignment();
            reset_static_alloc_helper();
            
            move_assignment();
            reset_static_alloc_helper();
            
            // More general member function tests.
            size_test();
            max_size_test();
            empty_test();
            indexing_operator();
            const_indexing_operator();
            at_index_function();
            const_at_index_function();
            front_function();
            const_front_function();
            back_function();
            const_back_function();
            data_function();
            const_data_function();
            
            // Iterator member function tests.
            run_iterator_functions();
            
            // Comparison operator tests.
            run_comparison_tests();
        }

        // =================================================================
        //                     Constructor/Destructor
        // =================================================================
        // -----------------------------------------------------------------
        // dynarray()
        static void default_constructor()
        {
            self subject;
            subject.default_constructor_test();
        }

        void default_constructor_test()
        {
            assert(static_alloc_helper::default_ctor_called);
            assert(static_alloc_helper::static_alloc_helper::allocated_memory_ptr == nullptr);
            assert(this->first == nullptr);
            assert(this->last == nullptr);
            assert(this->size() == 0);
            assert(this->empty());
        }

        // -----------------------------------------------------------------
        // dynarray(allocator_type const& alloc)
        static void allocator_constructor()
        {
            alloc_type testAlloc;
            reset_static_alloc_helper();
            self subject(testAlloc);
            subject.allocator_constructor_test();
        }
        
        void allocator_constructor_test()
        {
            assert(static_alloc_helper::copy_ctor_called);
            assert(!static_alloc_helper::copy_other_ctor_called);
            assert(this->first == nullptr);
            assert(this->last == nullptr);
            assert(this->size() == 0);
            assert(this->empty());
        }
        
        // -----------------------------------------------------------------
        // explicit dynarray(size_type n)
        static void n_item_constructor()
        {
            {
                self subject(5);
                subject.n_item_constructor_test();
            }
            assert(static_alloc_helper::destroy_called);
            assert(static_alloc_helper::destroy_call_count == 5);
            assert(static_alloc_helper::dtor_called);
        }
        
        void n_item_constructor_test()
        {
            assert(static_alloc_helper::default_ctor_called);
            assert(this->first != nullptr);
            assert(this->last != nullptr);
            for(pointer p = this->first; p != this->last;++p)
                assert(*p == value_type());
            assert(static_alloc_helper::construct_called);
            assert(static_alloc_helper::construct_call_count == 5);
            assert(this->size() == 5);
            assert(!this->empty());
        }
        
        // -----------------------------------------------------------------
        // copy constructor.
        // dynarray(dynarray const& x)
        static void copy_constructor()
        {
            self subject1(5);
            for(size_t i = 0; i < subject1.size(); ++i) {
                subject1.first[i] = i;
            }
            reset_static_alloc_helper();
            self subject2(subject1);
            subject2.copy_constructor_test(subject1);
        }
        
        void copy_constructor_test(self const& copyOf)
        {
            assert(this->first != nullptr);
            assert(this->last != nullptr);
            assert(static_alloc_helper::construct_called);
            assert(static_alloc_helper::construct_call_count == 5);
            assert(this->size() == 5);
            assert(!this->empty());
        }
        
        // -----------------------------------------------------------------
        // move constructor.
        // dynarray(dynarray&& x)
        static void move_constructor()
        {
            {
                self subject1(5);
                
                for(size_t i = 0; i < subject1.size(); ++i) {
                    subject1.first[i] = i;
                }
                assert(static_alloc_helper::allocate_called);
                reset_static_alloc_helper();
                self subject2(std::move(subject1));
                subject2.move_constructor_test();
            }
            assert(static_alloc_helper::deallocate_called);
        }
        
        void move_constructor_test()
        {
            assert(static_alloc_helper::move_ctor_called);
            assert(!static_alloc_helper::allocate_called);
            assert(this->first != nullptr);
            assert(this->last != nullptr);
            assert(!static_alloc_helper::construct_called);
            assert(static_alloc_helper::construct_call_count == 0);
            assert(this->size() == 5);
            assert(!this->empty());
            for(size_t i = 0; i < this->size(); ++i) {
                assert(this->first[i] == i);
            }
        }
        
        // -----------------------------------------------------------------
        // range constructor.
        // template<typename Iter> dynarray(Iter first, Iter last)
        static const size_t range_size = 6;
        
        static void range_constructor()
        {
            value_type val[range_size];
            pointer start_pos = val;
            pointer end_pos = val + range_size;
            for(pointer val_pos = start_pos; val_pos != end_pos; ++val_pos) {
                *val_pos = 100 - (end_pos - val_pos);
            }
            {
                self subject(start_pos, end_pos);
                subject.range_constructor_test();
            }
            assert(static_alloc_helper::destroy_called);
            assert(static_alloc_helper::destroy_call_count == range_size);
            assert(static_alloc_helper::deallocate_called);
        }
        
        void range_constructor_test()
        {
            assert(static_alloc_helper::allocate_called);
            assert(this->first != nullptr);
            assert(this->last != nullptr);
            assert(static_alloc_helper::construct_called);
            assert(static_alloc_helper::construct_call_count == range_size);
            assert(this->size() == range_size);
            assert(!this->empty());
            for(pointer val_pos = this->first; val_pos != this->last; ++val_pos) {
                assert((*val_pos == 100 - (this->last - val_pos)));
            }
        }

        // -----------------------------------------------------------------
        // range + allocator constructor.
        // template<typename Iter> dynarray(Iter first, Iter last, allocator_type const& alloc)
        static void range_alloc_constructor()
        {
            value_type val[range_size];
            pointer start_pos = val;
            pointer end_pos = val + range_size;
            for(pointer val_pos = start_pos; val_pos != end_pos; ++val_pos) {
                *val_pos = 100 - (end_pos - val_pos);
            }
            {
                self subject(start_pos, end_pos, alloc_type());
                subject.range_constructor_test();
                subject.range_alloc_constructor_test();
            }
        }
        
        void range_alloc_constructor_test()
        {
            assert(static_alloc_helper::copy_ctor_called);
        }
        
        // -----------------------------------------------------------------
        // initializer list constructor
        // dynarray(std::initializer_list<value_type> list)
        static void initializer_list_constructor()
        {
            std::initializer_list<T> temp_init_list({1, 6, 29,94});
            {
                self subject(temp_init_list);
                subject.initializer_list_constructor_test(temp_init_list);
            }
            assert(static_alloc_helper::destroy_called);
            assert(static_alloc_helper::destroy_call_count == temp_init_list.size());
            assert(static_alloc_helper::deallocate_called);
            
        }
        
        void initializer_list_constructor_test(std::initializer_list<T> const& init_list)
        {
            assert(static_alloc_helper::allocate_called);
            assert(static_alloc_helper::construct_called);
            assert(static_alloc_helper::construct_call_count = init_list.size());
            auto current_init_list_item = init_list.begin();
            for(pointer p = this->first; p != this->last; ++p) {
                assert(*p == *current_init_list_item);
                ++current_init_list_item;
            }
        }
        
        // -----------------------------------------------------------------
        // initializer list + alloc constructor
        // dynarray(std::initializer_list<value_type> list, allocator_type const& alloc)
        static void initializer_list_alloc_constructor()
        {
            std::initializer_list<T> temp_init_list({1, 6, 29,94});
            {
                self subject(temp_init_list, alloc_type());
                subject.initializer_list_constructor_test(temp_init_list);
                subject.initializer_list_alloc_constructor_test();
            }
        }
        
        void initializer_list_alloc_constructor_test()
        {
            assert(static_alloc_helper::copy_ctor_called);
        }
        
        // -----------------------------------------------------------------
        // n item of value constructor
        // explicit dynarray(size_type n, value_type const& x = value_type{})
        static const size_t n_item_count = 3;
        
        static void n_item_of_value_constructor()
        {
            {
                self subject(n_item_count, 28);
                subject.n_item_of_value_constructor_test();
            }
            assert(static_alloc_helper::destroy_called);
            assert(static_alloc_helper::destroy_call_count == n_item_count);
            assert(static_alloc_helper::deallocate_called);
        }
        
        void n_item_of_value_constructor_test()
        {
            assert(this->size() == n_item_count);
            for(pointer p = this->first; p != this->last; ++p) {
                assert(*p == 28);
            }
            assert(static_alloc_helper::allocate_called);
            assert(static_alloc_helper::allocated_memory_ptr != nullptr);
            assert(static_alloc_helper::construct_called);
            assert(static_alloc_helper::construct_call_count = n_item_count);
        }
        
        // -----------------------------------------------------------------
        // n item of value alloc constructor
        // explicit dynarray(size_type n, value_type const& x, allocator_type const& alloc)
        static void n_item_of_value_alloc_constructor()
        {
            {
                alloc_type a;
                self subject(n_item_count, 28, a);
                subject.n_item_of_value_constructor_test();
                subject.n_item_of_value_alloc_constructor_test();
            }
        }
        
        void n_item_of_value_alloc_constructor_test()
        {
            assert(static_alloc_helper::copy_ctor_called);
        }
        
        // =================================================================
        //                         Member Functions
        // =================================================================
        
        // -----------------------------------------------------------------
        // copy assignment
        // dynarray& operator=(dynarray const& x)    
        static void copy_assignment()
        {
            {
                self subject_to_copy({1, 2, 3, 4, 5});
                self copy_of_subject;
                copy_of_subject = subject_to_copy;
                copy_of_subject.copy_assignment_test(subject_to_copy);
            }
        }
        
        void copy_assignment_test(self const& copied_subject)
        {
            assert(this->size() == copied_subject.size());
            const_pointer current_item = this->first;
            const_pointer copied_item = copied_subject.first;
            for(;current_item != this->last; ++current_item, ++copied_item)
            {
                assert(*current_item == *copied_item);
            }
        }
        
        // -----------------------------------------------------------------
        // move assignment
        // dynarray& operator=(dynarray&& x)
        static void move_assignment()
        {
            {
                self subject_to_move({1, 2, 3, 4, 5});
                self test_subject;
                reset_static_alloc_helper();
                test_subject = std::move(subject_to_move);
                assert(subject_to_move.size() == 0);
                assert(subject_to_move.empty());
                test_subject.move_assignment_test();
            }
        }
        
        void move_assignment_test()
        {
            assert(this->size() == 5);
            float someNumebr = 0;
            const_pointer current_item = this->first;
            for(;current_item != this->last; ++current_item)
            {
                assert(*current_item == (someNumebr += 1));
            }
        }
        
        // -----------------------------------------------------------------
        // size
        // size_type size() const
        static void size_test()
        {
            self subject({1,2,3});
            assert(subject.size() == 3);
        }
           
        // -----------------------------------------------------------------
        // max_size
        // constexpr size_type max_size() const
        static void max_size_test()
        {
            allocator_type alloc;
            self subject;
            assert(alloc.max_size() == subject.max_size());
        }
        
        // -----------------------------------------------------------------
        // empty
        // bool empty() const
        static void empty_test()
        {
            {
                self subject({1,2,3});
                assert(!subject.empty());
            }
            {
                self subject;
                assert(subject.empty());
            }
        }
        
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
        
        // -----------------------------------------------------------------
        // begin function
        // iterator begin()
        static void begin_function()
        {
        }

        // -----------------------------------------------------------------
        // end function
        // iterator end()
        static void end_function()
        {
        }
        
        // -----------------------------------------------------------------
        // const begin function
        // const_iterator begin() const
        static void const_begin_function()
        {
        }
        
        // -----------------------------------------------------------------
        // const end function
        // const_iterator end() const
        static void const_end_function()
        {
        }
        
        // -----------------------------------------------------------------
        // cbegin function
        // const_iterator cbegin() const
        static void cbegin_function()
        {
        }

        // -----------------------------------------------------------------
        // cend function
        // const_iterator cend() const
        static void cend_function()
        {
        }
        
        // -----------------------------------------------------------------
        // reverse begin function
        // reverse_iterator rbegin()
        static void rbegin_function()
        {
        }
        
        // -----------------------------------------------------------------
        // reverse end function
        // reverse_iterator rend()
        static void rend_function()
        {
        }
        
        // -----------------------------------------------------------------
        // const reverse begin function
        // const_reverse_iterator rbegin() const
        static void const_rbegin_function()
        {
        }
        
        // -----------------------------------------------------------------
        // const reverse end function
        // const_reverse_iterator rend() const
        static void const_rend_function()
        {
        }
        
        // -----------------------------------------------------------------
        // crbegin function
        // const_reverse_iterator crbegin() const
        static void crbegin_function()
        {
        }
        
        // -----------------------------------------------------------------
        // crend function
        // const_reverse_iterator crend() const
        static void crend_function()
        {
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
        }
        
        // -----------------------------------------------------------------
        // inequality comparison
        // bool operator!=(dynarray const& x) const
        static void inequality_comparison()
        {
        }

        // -----------------------------------------------------------------
        // less than comparison
        // bool operator<(dynarray const& x) const
        static void less_than_comparison()
        {
        }

        // -----------------------------------------------------------------
        // greater than comparison
        // bool operator>(dynarray const& x) const
        static void greater_than_comparison()
        {
        }
        
        // -----------------------------------------------------------------
        // less than or equal to comparison
        // bool operator<=(dynarray const& x) const
        static void less_than_or_equal_to_comparison()
        {
        }
        
        // -----------------------------------------------------------------
        // greater than or equal to comparison
        // bool operator>=(dynarray const& x) const
        static void greater_than_or_equal_to_comparison()
        {
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
        }
        
        // -----------------------------------------------------------------
        // free function swap
        // template<typename T> void swap(dynarray<T>& a, dynarray<T>& b)
        static void free_function_swap()
        {
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
