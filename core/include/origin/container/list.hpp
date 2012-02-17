// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_CONTAINER_LIST_HPP
#define ORIGIN_CONTAINER_LIST_HPP

#include <list>

#include <origin/range.hpp>

namespace origin
{
  // impltor (container)
  // A impltor is a dynamically resizable sequence of contiguously allocated
  // elements.
  template <typename T, typename Alloc = std::allocator<T>>
    class o_list
    {
      using base_type = std::list<T, Alloc>;
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
      explicit o_list(const allocator_type& alloc = {}) 
        : impl(alloc) 
      { }
      
      
      
      // Movable
      o_list(o_list&& x, const allocator_type& alloc = {})
        : impl(std::move(x.impl), alloc)
      { }
       
      o_list& operator=(o_list&& x) 
      {
        impl = std::move(x.impl); 
        return *this;
      }
      
      
      
      // Copyable
      o_list(const o_list& x, const allocator_type& alloc = {})
        : impl(x, alloc)
      { }
      
      o_list& operator=(const o_list& x) 
      { 
        return impl = x.impl;
        return *this;
      }
      
      
      
      // Fill initialization
      explicit o_list(size_type n, 
                      const value_type& value = {}, 
                      const allocator_type& alloc = {})
        : impl(n, value, alloc)
      { }
      
      void assign(size_type n, const value_type value)
      {
        impl.assign(n, value);
      }
      
      
      
      // Iterator range initialization
      // Initialize or assign the impltor by copying the elements in the range 
      // [first, last).
      template <typename I>
        o_list(I first, I last, const allocator_type& alloc = {})
          : impl(first, last, alloc)
        { }
        
      template <typename I>
        void assign(I first, I last) 
        { 
          impl.assign(first, last); 
        }
      
      
      
      // Range initialization
      // Initialize or assign the impltor by copying the elements in range.
      template <typename R>
        explicit o_list(const R& range, 
                        const allocator_type& alloc = {}, 
                        Requires<Input_range<R>()>* = {})
          : impl(o_begin(range), o_end(range), alloc)
        { }
      
      template <typename R>
        void assign(const R& range, Requires<Input_range<R>()>* = {}) 
        { 
          impl.assign(o_begin(range), o_end(range));
        }
      
      
      
      // Initializer list initialization
      // Initialize or assign the impltor by copying the elements in list.
      o_list(std::initializer_list<value_type> list, const allocator_type& alloc = {})
        : impl(list.begin(), list.end(), alloc)
      { }
      
      o_list& operator=(std::initializer_list<value_type> list)
      {
        impl = list;
        return *this;
      }
      
      void assign(std::initializer_list<value_type> list)
      {
        impl.assign(list);
      }
      
      
      
      // Equality comparable
      bool operator==(const o_list& v) const { return impl == v.impl; }
      bool operator!=(const o_list& v) const { return impl != v.impl; }
      
      
      // Totally ordered
      bool operator<(const o_list& v) const  { return impl < v.impl; }
      bool operator>(const o_list& v) const  { return impl > v.impl; }
      bool operator<=(const o_list& v) const { return impl <= v.impl; }
      bool operator>=(const o_list& v) const { return impl >= v.impl; }
      
      
      
      // Size and capacity
      bool empty() const { return impl.empty(); }
      size_type size() const { return impl.size(); }
      
      void resize(size_type n, const value_type& value = {}) 
      { 
        impl.resize(n, value); 
      }
      
      
      
      // Element access
      reference       front()       { return impl.front(); }
      const_reference front() const { return impl.front(); }
      
      reference       back()       { return impl.back(); }
      const_reference back() const { return impl.back(); }
      
      
      
      // Push and pop front
      template<typename... Args>
        void emplace_front(Args&&... args)
        {
          impl.emplace_front(std::forward<Args>(args)...);
        }
        
      void push_front(const value_type& value) { impl.push_front(value); }
      void push_front(value_type&& value) { impl.push_front(std::move(value)); }
      void pop_front() { impl.pop_front(); }
        

      
      // Push and pop back
      template<typename... Args>
        void emplace_back(Args&&... args) 
        { 
          impl.emplace_back(std::forward<Args>(args)...); 
        }

      void push_back(const value_type& value) { impl.push_back(value); }
      void push_back(value_type&& value) { impl.push_back(std::move(value)); }
      void pop_back() { impl.pop_back(); }
      
      
      
      // Insert
      template<typename... Args>
        iterator emplace(const_iterator pos, Args&&... args)
        {
          return impl.emplace(pos, std::forward<Args>(args)...);
        }
      
      iterator insert(const_iterator pos, const value_type& value) 
      { 
        return impl.insert(pos, value); 
      }
      
      iterator insert(const_iterator pos, value_type&& value)
      {
        return impl.insert(pos, value);
      }
      
      // Insert fill
      iterator insert(const_iterator pos, size_type n, const value_type& value)
      {
        return impl.insert(pos, n, value);
      }
      
      // Insert iterator range
      template<typename I>
        iterator insert(const_iterator pos, I first, I last)
        {
          return impl.insert(pos, first, last);
        }
        
      // Insert range
      template<typename R>
        iterator insert(const_iterator pos, const R& range, Requires<Input_range<R>()>* = {})
        {
          return impl.insert(pos, o_begin(range), o_end(range));
        }
      
      // Insert initializer list
      iterator insert(const_iterator pos, std::initializer_list<value_type> list)
      {
        return impl.insert(pos, list);
      }
      
      
      
      // Erase
      iterator erase(const_iterator pos) 
      { 
        return impl.erase(pos); 
      }
      
      iterator erase(const_iterator first, const_iterator last)
      {
        return impl.erase(first, last);
      }
      
      void clear() { impl.clear(); }
      
      
      
      // Splice
      void splice(const_iterator pos, o_list& x)  { impl.splice(pos, x); }
      void splice(const_iterator pos, o_list&& x) { impl.splice(pos, std::move(x)); }
      
      void splice(const_iterator pos, o_list& x, const_iterator i)
      {
        impl.splice(pos, x, i);
      }
      
      void splice(const_iterator pos, o_list&& x, const_iterator i)
      {
        impl.splice(pos, std::move(x), i);
      }
      
      void splice(const_iterator pos, o_list& x, const_iterator first, const_iterator last)
      {
        impl.splice(pos, x, first, last);
      }
      
      void splice(const_iterator pos, o_list&& x, const_iterator first, const_iterator last)
      {
        impl.splice(pos, std::move(x), first, last);
      }
      
      
      
      // Reverse
      void reverse() { impl.reverse(); }
      
      
      
      // Remove
      void remove(const value_type& value) { impl.remove(value); }
      
      template <typename P>
        void remove_if(P pred) { impl.remove(pred); }
      
      
      
      // Unique
      void unique() { impl.unique(); }
      
      template <typename R>
        void unique(R comp) { impl.unique(comp); }
      
      
      
      // Merge
      void merge(o_list& x)  { impl.merge(x); }
      void merge(o_list&& x) { impl.merge(std::move(x)); }
      
      template <typename R>
        void merge(o_list& x, R comp) { impl.merge(x, comp); }
      
      template <typename R>
        void merge(o_list&& x, R comp) { impl.merge(std::move(x), comp); }
      
      
      
      // Sort
      void sort() { impl.sort(); }
      
      template <typename R>
        void sort(R comp) { impl.sort(comp); }
      
      
      // Swap
      void swap(o_list& v) { impl.swap(v.impl); }
      
      
      
      // Iterable
      iterator begin() { return impl.begin(); }
      iterator end()   { return impl.end(); }
      
      const_iterator begin() const { return impl.begin(); }
      const_iterator end() const   { return impl.end(); }
      
      
    private:
      base_type impl;
    };
    
    
    
  // Specialization for std::swap.
  template <typename T, typename A>
    void swap(o_list<T, A>& a, o_list<T, A>& b) { a.swap(b); }
    
} // namespace origin


#endif

