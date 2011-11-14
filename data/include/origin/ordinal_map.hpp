// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_ORDINAL_MAP_HPP
#define ORIGIN_ORDINAL_MAP_HPP

#include <vector>
#include <utility>

#include <origin/ordinal.hpp>
#include <origin/container.hpp>

namespace origin
{
  // FIXME: Define a variant that actually understands mapped value validity,
  // that is, which elements in the map are defined and which are not. This can 
  // be easily achieved using either an accompanying bitmap or by  making the 
  // elements optional (at greater cost of space and time). It should be 
  // called direct_map. The corresponding concept is Direct Map.
  //
  // The name "direct map" is intended to relate to the notion of direct-mapped
  // caches. The idea is that the key (or a part of it) contains the exact
  // location of the mapped value within the map. No search or transformation
  // is required for access.

  
  // If we consider maps to be functions (i.e., m[k] <=> f(x)), then 
  // Associative containers define partial functions. We can generalize that
  // notion to Associations and Partial Maps. More precisely an Association
  // is a Partial Map if, for any Map m and a key k, m.count(k) == 0. This is
  // our basic understanding of all associative containers.
  //
  // An Association is a Total Map if, for any Map m and all keys k in the
  // domain of m, m.count(k) > 0. Note that here, the domain is not simply 
  // all objects represented by the domain type, but rather the set of keys
  // over which a Map m is currently defined. Intuitively, a Total Map is one
  // where each key is implicitly mapped to some value. The value may or may
  // not have meaning, depending on the current state of the program, however.

  
  // FIXME: The ordinal_map is a Total, Direct, Unique Association.
  
  
  // NOTE: Total Maps don't have erase functions and their insert function is
  // more like a Multimap than a Map: it can't fail.

  // The ordinal map iterator provides an iterator type over the mappings in 
  // an ordinal map. Ordinal map iterators rae random access iterators.
  template<typename Key, typename Iter, typename Size>
    class ordinal_map_iterator
    {
    public:
      using value_type = std::pair<Key, Value_type<Iter>>;
      using reference = std::pair<Key, Iterator_reference<Iter>>;
      using pointer = std::pair<Key, Iterator_pointer<Iter>>;
      using difference_type = Distance_type<Iter>;
      using iterator_category = std::random_access_iterator_tag;
      
      ordinal_map_iterator() 
        : pos(), iter() 
      { }
      
      ordinal_map_iterator(Size n, Iter i) 
        : pos(n), iter(i) 
      { }
      
      // Enable iterator interoperability
      template<typename Other>
        ordinal_map_iterator(ordinal_map_iterator<Key, Other, Size> i, 
                             Requires<Convertible<Other, Iter>()>* = {}) 
          : pos(i.pos), iter(i.iter)
        { }
      
      // Equality_comparable
      bool operator==(ordinal_map_iterator const& x) const { return iter == x.iter; }
      bool operator!=(ordinal_map_iterator const& x) const { return iter != x.iter; }
      
      // Totally_ordered
      bool operator<(ordinal_map_iterator const& x) const  { return iter < x.iter; }
      bool operator>(ordinal_map_iterator const& x) const  { return iter > x.iter; }
      bool operator<=(ordinal_map_iterator const& x) const { return iter <= x.iter; }
      bool operator>=(ordinal_map_iterator const& x) const { return iter >= x.iter; }

      // Readable
      reference operator*() const  { return {ordinal_cast<Key>(pos), *iter}; }
      pointer   operator->() const { return {ordinal_cast<Key>(pos), &*iter}; }

      // Increment
      ordinal_map_iterator& operator++() { return advance(1); }
      
      ordinal_map_iterator operator++(int) 
      { 
        ordinal_map_iterator tmp(*this); 
        operator++(); 
        return tmp; 
      }
      
      // Decrement
      ordinal_map_iterator& operator--() { return advance(-1); }
      
      ordinal_map_iterator operator--(int) 
      { 
        ordinal_map_iterator tmp(*this); 
        operator--();
        return tmp; 
      }

      // Advance
      ordinal_map_iterator& operator+=(difference_type n) { return advance(n); }
      
      friend ordinal_map_iterator 
      operator+(ordinal_map_iterator i, difference_type n) 
      { 
        return i += n; 
      }
      
      friend ordinal_map_iterator 
      operator+(difference_type n, ordinal_map_iterator i) 
      { 
        return i += n;
      }

      ordinal_map_iterator& operator-=(difference_type n) { return advance(-n); }
      
      friend ordinal_map_iterator 
      operator-(ordinal_map_iterator i, difference_type n) 
      { 
        return i -= n; 
      }

      // Distance
      friend difference_type 
      operator-(ordinal_map_iterator const& i, ordinal_map_iterator const& j) 
      { 
        return i - j; 
      }
      
    private:
      ordinal_map_iterator& advance(difference_type n)
      {
        pos += n;
        iter += n;
        return *this;
      }
      
    private:
      Size pos;
      Iter iter;
    };

    
    
  // A vector map implements a Direct, Unique Map over a Random Access 
  // Container. Like all Direct Maps, the key type of the vector map is 
  // the size type of the underlying container. However, all keys are implicitly 
  // mapped to values. This is an optimization over true Direct Maps where the
  // mapped value may not be defined for a given key.
  //
  // The ordinal_map is a container adaptor, taking an underlying container as
  // a template parameter. The allocator for the map can be specified as an
  // argument to that parameter.
  template<typename K, typename V, typename Vec = std::vector<V>>
    class ordinal_map
    {
      using vector_type = Vec;
    public:
      using allocator_type  = typename vector_type::allocator_type;
      using size_type       = Size_type<Vec>;
      using difference_type = Distance_type<Vec>;

      // Types
      using key_type    = K;
      using mapped_type = V;
      using value_type  = std::pair<K, V>;
      using reference       = Container_reference<Vec>;
      using const_reference = Container_reference<Vec const>;
      using pointer       = Container_pointer<Vec>;
      using const_pointer = Container_pointer<Vec const>;
      
      using iterator       = ordinal_map_iterator<K, Iterator_type<Vec>, size_type>;
      using const_iterator = ordinal_map_iterator<K, Iterator_type<Vec const>, size_type>;
      using reverse_iterator       = std::reverse_iterator<iterator>;
      using const_reverse_iterator = std::reverse_iterator<const_iterator>;


      // Default constructor
      ordinal_map(allocator_type const& alloc = {}) 
        : vec()
      { }
      
      // NOTE: Movable and Copyable are provided by default.

      // Range initialization
      // Initializes the map over a sequence of mappings (key/value pairs).
      template<typename Iter>
        ordinal_map(Iter first, Iter last)
          : vec()
        {
          static_assert(Input_iterator<Iter>(), "");
          static_assert(Convertible<Value_type<Iter>, value_type>(), "");

          while(first != last) {
            (*this)[first->first] = first->second;
          }
        }
        
      // List initialization
      // The initializer list constructor initializes a map over a sequence
      // of mappings (i.e., key/value pairs).
      //
      // FIXME: This could be generalized for convertible types.
      ordinal_map(std::initializer_list<value_type> list)
        : vec()
      {
        for(auto const& x : list)
          (*this)[x.first] = x.second;
      }
      
      // List initialization
      // The initializer list constructor initializess a map over a sequence
      // of mapped values.
      ordinal_map(std::initializer_list<mapped_type> list)
        : vec(list.begin(), list.end())
      { }
      
      // Fill initialization
      // The fill constructor provides a method of initializing a map over a
      // set of n mappings, optionally initialized to the value x.
      explicit ordinal_map(size_type n, mapped_type const& x = {})
        : vec(n, x)
      { }


      // Empty and size
      bool empty() const { return vec.empty(); }

      size_type size() const { return vec.size(); }
      
      // Resize the map to contain the specified number of mappings. If n
      // is greater than the current size, new mappings  will be added and
      // the mapped values initialized to x. If n is less than the current 
      // size, all mappings with keys greater than or equal to n will be 
      // destroyed.
      void resize(size_type n, mapped_type const& a = {}) { vec.resize(n, a); }


      // Max size
      constexpr size_type max_size() const { return vec.max_size(); }

      // Allocator
      allocator_type allocator() const { return vec.allocator(); }

      // Capacity
      size_type capacity() const { return vec.capacity(); }
      void reserve(size_type n) { vec.reserve(n); }


      // Return a reference to the mapped value indicated by the given key.
      // If no such key exists, the map is resized in order to accomodate
      // the new value. This may result in the creation of ord(k) - size() 
      // new mappings.
      mapped_type& operator[](key_type const& k)
      {
        maybe_resize(ord(k));
        return get(k);
      }

      // Return a reference to the mapped value indicated by the given key.
      // Note that the key must be in the map.
      mapped_type const& operator[](key_type const& k) const
      { 
        assert(( find(k) != end() ));
        return get(k); 
      }

      // Insert the given key/value pair into the map, returning an iterator to
      // the given position.
      iterator insert(value_type const& x)
      {
        maybe_resize(ord(x.first));
        get(x.first) = x.second;
        return begin() + ord(x.first);
      }


      // Clear and swap
      void clear() { vec.clear(); }
      void swap(ordinal_map& x) { vec.swap(x); }      

      
      // Return an iterator to the object indicated by the given key or an
      // iterator past the end if x is outside the bounds of the map.
      iterator find(key_type const& k)
      {
        return valid(k) ? begin() + ord(k) : end();
      }

      const_iterator find(key_type const& k) const
      {
        return valid(k) ? begin() + ord(k) : end();
      }
      
 
      // Return the number of objects associated with the given k. If the key
      // is in bounds, this is 1. If not, 0.
      size_type count(key_type const& k) const
      {
        return valid(k) ? 1 : 0;
      }

      // Iterators
      iterator begin() { return {0, vec.begin()}; }
      iterator end()   { return {size(), vec.end()}; }
      
      const_iterator begin() const { return {0, vec.begin()}; }
      const_iterator end() const   { return {size(), vec.end()}; }

      // Reversible iterators
      reverse_iterator rbegin() { return vec.rbegin(); }
      reverse_iterator rend()   { return vec.rend(); }
      
      const_reverse_iterator rbegin() const { return vec.rbegin(); }
      const_reverse_iterator rend() const   { return vec.rend(); }

    private:
      // Return true if the given key is valid (i.e., in bounds).
      bool valid(key_type const& k) const { return !empty() && ord(k) < size(); }

      // If the key is larger than the current size, allocate more mappings.
      // Note that the vector won't do this unless we push back. We're usually
      // just doing random-access insertion.
      void maybe_resize(size_type const& k)
      {
        if(k >= size()) {
          if(k >= capacity()) {
            // FIXME: The resize heuristic is a total guess. There's probably
            // a better one.
            reserve(capacity() == 0 ? 8 : 2 * k);
          }
          resize(k + 1);
        }
      }
      
      // Return the object at the mapped key.
      mapped_type&       get(key_type const& k)       { return vec[ord(k)]; }
      mapped_type const& get(key_type const& k) const { return vec[ord(k)]; }
      
    private:
      vector_type vec;
    };

} // namespace origin

#endif

