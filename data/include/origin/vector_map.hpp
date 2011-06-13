// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_VECTOR_MAP_HPP
#define ORIGIN_VECTOR_MAP_HPP

#include <vector>
#include <utility>

namespace origin
{
  // FIXME: Define a variant that actually understands mapped value validity. 
  // This can be easily achieved using either an accompanying bitmap or by 
  // making the elements optional (at greater cost of space and time). It should
  // be called direct_map. The corresponding concept is Direct Map.
  
  // If we consider maps to be functions (i.e., m[k] <=> f(x)), then 
  // Associative containers define partial functions. We can generalize that
  // notion to Associations and Partial Maps. More precisely an Association
  // is a Partial Map if, for any Map m and a key k, m.count(k) == 0. This is
  // our basic understanding of all associative containers.
  //
  // An Association is a Total Map if, for any Map m and all keys k in the
  // domain of m, m.count(k) > 0. Note that that, here, domain is not simply 
  // all objects represented by the domain type, but rather the set of keys
  // over which a Map m is currently defined. Intuitively, a Total Map is one
  // where each key is implicitly mapped to some value. The value may or may
  // not have meaning, depending on the current state of the program, however.

  // FIXME: The vector_map is a Total, Direct, Unique Association.
  
  // FIXME: Seriously, how many Total Maps could there possibly be? Still, there
  // semantically, this is what we have.
  
  // NOTE: Total Maps don't have erase functions and their insert function is
  // more like a Multimap than a Map: it can't fail.


  /**
   * A vector map implements a Direct, Unique Map over a Random Access 
   * Container. Like all Direct Maps, the key type of the vector map is 
   * the size type of the underlying container. However, all keys are implicitly 
   * mapped to values. This is an optimization over true Direct Maps where the
   * mapped value may not be defined for a given key.
   *
   * The vector_map is a container adaptor, taking an underlying container as
   * a template parameter. The allocator for the map can be specified as an
   * argument to that parameter.
   * 
   * @tparam T      The mapped type
   * @tparam Vec    The underlying Vector type
   */
  template<typename T, typename Vec = std::vector<T>>
    class vector_map
    {
      typedef Vec vector_type;
    public:
      typedef T mapped_type;
      typedef typename vector_type::size_type key_type;
      typedef std::pair<key_type, mapped_type> value_type;
      typedef T& reference;
      typedef T const& const_reference;
      typedef typename vector_type::pointer pointer;
      
      typedef typename vector_type::size_type size_type;
      
      typedef typename vector_type::allocator_type allocator_type;
      
      // FIXME: These are wrong. They need to return value types, not the 
      // mapped type.
      typedef typename vector_type::iterator iterator;
      typedef typename vector_type::const_iterator const_iterator;
      typedef typename vector_type::reverse_iterator reverse_iterator;
      typedef typename vector_type::const_reverse_iterator const_reverse_iterator;

      /** @name Initialization */
      //@{
      /**
       * @brief Default constructor
       */
      vector_map()
        : data_{}
      { }

      vector_map(allocator_type const& alloc)
        : data_{alloc}
      { }
      
      /**
       * @name Fill constructor
       */
      explicit vector_map(size_type n)
        : data_{n}
      { }
      
      vector_map(size_type n, mapped_type x)
        : data_{n, x}
      { }
      
      // FIXME: Write more constructors
      //@}
      
      /** @name Properties */
      //@{
      /**
       * Return true if the map has no elements.
       */
      bool empty() const
      {
        return data_.empty();
      }

      /**
       * Return the number of elements in the map.
       */
      size_type size() const
      {
        return data_.size();
      }
      
      /**
       * Resize the map to contain the specified number of mappings. If n
       * is greater than the current size, new default-initialized mappings 
       * will be added. If n is less than the current size, all mappings
       * with keys greater than or equal to n will be destroyed.
       */
      void resize(size_type n)
      {
        data_.resize(n);
      }
      
      /**
       * Resize the map to contain the specified number of mappings. If n
       * is greater than the current size, new mappings  will be added and
       * the mapped values initialized to x. If n is less than the current 
       * size, all mappings with keys greater than or equal to n will be 
       * destroyed.
       */
      void resize(size_type n, value_type const& x)
      {
        data_.resize(n, x);
      }
      
      /**
       * Return the maximum allowable size of the map.
       */
      constexpr size_type max_size() const
      {
        return data_.max_size();
      }
      
      /**
       * Return the allocator for the map.
       */
      allocator_type allocator() const
      {
        return data_.allocator();
      }
      //@}
      
      /** @name Capacity */
      //@{
      size_type capacity() const
      {
        return data_.capacity();
      }
      
      void reserve(size_type n)
      {
        data_.reserve(n);
      }
      //@}

      /** Member access */
      //@{
      /**
       * Return a reference to the mapped value indicated by the given key.
       * If no such value exists, the map is resized in order to accomodate
       * the new value.
       * 
       * @note This operation may result in the creation of many new, default
       * initialized values.
       */
      mapped_type& operator[](key_type const& k)
      {
        maybe_resize(k);
        return get(k);
      }
      //@}
      
      /** @name Modifiers */
      //@{
      /**
       * Insert the given key/value pair into the map, returning an iterator to
       * the given position.
       */
      iterator insert(value_type const& x)
      {
        maybe_resize(x.first);
        get(x.first) = x.second;
        return begin() + x.first;
      }
      //@}

      /** @name Map operations */
      //@{
      /**
       * Return an iterator to the object indicated by the given key or an
       * iterator past the end if x is outside the bounds of the map.
       */
      iterator find(key_type const& k)
      {
        return valid(k) ? begin() + k : end();
      }

      const_iterator find(key_type const& k) const
      {
        return valid(k) ? begin() + k : end();
      }
      
      /**
       * Return the number of objects associated with the given k. If the key
       * is in bounds, this is 1. If not, 0.
       */
      size_type count(key_type const& k) const
      {
        return valid(k) ? 1 : 0;
      }
      //@}
      
      /** @name Iterators */
      //@{
      iterator begin()
      {
        return data_.begin();
      }
      
      iterator end()
      {
        return data_.end();
      }
      
      const_iterator begin() const
      {
        return data_.begin();
      }
      
      const_iterator end() const
      {
        return data_.end();
      }
      
      reverse_iterator rbegin()
      {
        return data_.rbegin();
      }
      
      reverse_iterator rend()
      {
        return data_.rend();
      }
      
      const_reverse_iterator rbegin() const
      {
        return data_.rbegin();
      }
      
      const_reverse_iterator rend() const
      {
        return data_.rend();
      }
      //@}

      /** @name Operations */
      //@{
      void clear()
      {
        data_.clear();
      }

      void swap(vector_map& x)
      {
        data_.swap(x);
      }      
      //@}
      
    private:
      // Return true if the given key is valid (i.e., in bounds).
      bool valid(key_type const& k) const
      {
        return !empty() && k < size();
      }

      // If the key is larger than the current size, allocate more mappings. 
      void maybe_resize(key_type const& k)
      {
        if(k >= size()) {
          if(k >= capacity()) {
            // FIXME: The resize heursitic is a total guess. There's probably
            // a better one.
            reserve(capacity() == 0 ? 8 : 2 * k);
          }
          resize(k + 1);
        }
      }
      
      mapped_type& get(key_type const& k)
      {
        return data_[k];
      }
      
      mapped_type const& get(key_type const& k) const
      {
        return data_[k];
      }
      
    private:
      vector_type data_;
    };

} // namespace origin

#endif

