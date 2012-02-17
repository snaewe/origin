// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_OPTIONAL_HPP
#define ORIGIN_OPTIONAL_HPP

#include <vector>

#include <origin/range.hpp>

namespace origin
{
  // Vector (container)
  // A vector is a dynamically resizable sequence of contiguously allocated
  // elements.
  template<typename T, typename Alloc = std::allocator<T>>
    class o_vector
    {
      using base_type = std::vector<T, Alloc>;
    public:
      using allocator_type = typename base_type::allocator_type;

      using value_type      = typename base_type::value_type;
      using reference       = typename base_type::reference;
      using const_reference = typename base_type::const_reference;
      using pointer         = typename base_type::pointer;
      using const_pointer   = typename base_type::const_pointer;
      
      using size_type       = typename base_type::size_type;
      using difference_type = typename base_type::difference_type;
      
      using iterator       = typename base_type::iterator;
      using const_iterator = typename base_type::const_iterator;
      using reverse_iterator       = typename base_type::reverse_iterator;
      using const_reverse_iterator = typename base_type::const_reverse_iterator;
      
      
      // Default constructible
      explicit o_vector(const allocator_type& alloc = {}) 
        : vec(alloc) 
      { }
      
      
      
      // Movable
      o_vector(o_vector&& v, const allocator_type& alloc = {})
        : vec(std::move(v), alloc)
      { }
       
      o_vector& operator=(o_vector&& v) { return vec = std::move(v); }
      
      
      
      // Copyable
      o_vector(const o_vector& v, const allocator_type& alloc = {})
        : vec(v, alloc)
      { }
      
      o_vector& operator=(const o_vector& v) { return vec = v; }
      
      
      
      // Fill initialization
      explicit o_vector(size_type n, 
                        const value_type& value = {}, 
                        const allocator_type& alloc = {})
        : vec(n, value, alloc)
      { }
      
      void assign(size_type n, const value_type value)
      {
        vec.assign(n, value);
      }
      
      
      
      // Iterator range initialization
      // Initialize or assign the vector by copying the elements in the range 
      // [first, last).
      template<typename I>
        o_vector(I first, I last, const allocator_type& alloc = {})
          : vec(first, last, alloc)
        { }
        
      template<typename I>
        void assign(I first, I last) 
        { 
          vec.assign(first, last); 
        }
      
      
      
      // Range initialization
      // Initialize or assign the vector by copying the elements in range.
      template<typename R>
        explicit o_vector(const R& range, 
                          const allocator_type& alloc = {}, 
                          Requires<Input_range<R>()>* = {})
          : vec(o_begin(range), o_end(range), alloc)
        { }
      
      template<typename R>
        void assign(const R& range, Requires<Input_range<R>()>* = {}) 
        { 
          vec.assign(o_begin(range), o_end(range));
        }
      
      
      
      // Initializer list initialization
      // Initialize or assign the vector by copying the elements in list.
      o_vector(std::initializer_list<value_type> list, const allocator_type& alloc = {})
        : vec(list.begin(), list.end(), alloc)
      { }
      
      void assign(std::initializer_list<value_type> list)
      {
        vec.assign(list);
      }
      
      
      
      // Equality comparable
      bool operator==(const o_vector& v) const { return vec == v.vec; }
      bool operator!=(const o_vector& v) const { return vec != v.vec; }
      
      
      // Totally ordered
      bool operator<(const o_vector& v) const  { return vec < v.vec; }
      bool operator>(const o_vector& v) const  { return vec > v.vec; }
      bool operator<=(const o_vector& v) const { return vec <= v.vec; }
      bool operator>=(const o_vector& v) const { return vec >= v.vec; }
      
      
      
      // Size and capacity
      bool empty() const { return vec.empty(); }
      size_type size() const { return vec.size(); }
      size_type capacity() const { return vec.capacity(); }
      
      void resize(size_type n, const value_type& value = {}) 
      { 
        vec.resize(n, value); 
      }
      
      void reserve(size_type n)
      {
        vec.reserve(n);
      }
      
      
      
      // Element access
      reference       operator[](size_type n)       { return vec[n]; }
      const_reference operator[](size_type n) const { return vec[n]; }
      
      reference       at(size_type n)       { return vec.at(n); }
      const_reference at(size_type n) const { return vec.at(n); }
      
      reference       front()       { return vec.front(); }
      const_reference front() const { return vec.front(); }
      
      reference       back()       { return vec.back(); }
      const_reference back() const { return vec.back(); }
      
      
      
      // Data access
      pointer data()             { return vec.data(); }
      const_pointer data() const { return vec.data(); }
      
      
      
      // Push and pop back
      template<typename... Args>
        void emplace_back(Args&&... args) 
        { 
          vec.emplace(std::forward<Args>(args)...); 
        }

      void push_back(const value_type& value) { vec.push_back(value); }
      void push_back(value_type&& value) { vec.push_back(std::move(value)); }
      void pop_back() { vec.pop_back(); }
      
      
      // Insert
      template<typename... Args>
        iterator emplace(const_iterator pos, Args&&... args)
        {
          return vec.emplace(pos, std::forward<Args>(args)...);
        }
      
      iterator insert(const_iterator pos, const value_type& value) 
      { 
        return vec.insert(pos, value); 
      }
      
      iterator insert(const_iterator pos, value_type&& value)
      {
        return vec.insert(pos, value);
      }
      
      // Insert fill
      iterator insert(const_iterator pos, size_type n, const value_type& value)
      {
        return vec.insert(pos, n, value);
      }
      
      // Insert iterator range
      template<typename I>
        iterator insert(const_iterator pos, I first, I last)
        {
          return vec.insert(pos, first, last);
        }
        
      // Insert range
      template<typename R>
        iterator insert(const_iterator pos, const R& range, Requires<Input_range<R>()>* = {})
        {
          return vec.insert(pos, o_begin(range), o_end(range));
        }
      
      // Insert initializer list
      iterator insert(const_iterator pos, std::initializer_list<value_type> list)
      {
        return vec.insert(pos, list);
      }
      
      
      
      // Erase
      iterator erase(const_iterator pos) 
      { 
        return vec.erase(pos); 
      }
      
      iterator erase(const_iterator first, const_iterator last)
      {
        return vec.erase(first, last);
      }
      
      void clear() { vec.clear(); }
      
      
      
      // Swap
      void swap(o_vector& v) { vec.swap(v.vec); }
      
      
      // Iterable
      iterator begin() { return vec.begin(); }
      iterator end()   { return vec.end(); }
      
      const_iterator begin() const { return vec.begin(); }
      const_iterator end() const   { return vec.end(); }
      
      
    private:
      base_type vec;
    };
    
} // namespace origin


#endif

