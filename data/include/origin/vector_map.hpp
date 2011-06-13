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
  // FIXME: This is not a sorted associative container, nor is it a hashed
  // associative container. It's more like a direct associative container.

  // FIXME: Define a variant that actually understands initialization. This
  // can be easily achieved using either an accompanying bitmap or by making
  // the elements optional (at greater cost).

  /**
   * The vector map adapts an underlying Vector into a unique associative
   * container. An allocator for the container can be specified as part of the
   * underlying vector.
   * 
   * @tparam T      The mapped type
   * @tparam Vec    The underlying Vector type
   */
  template<typename T, Vec = std::vector<T>>
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
        : data_{}, init_{}
      { }

      vector_map(allocator_type const& alloc = allocator_type{})
        : data_{}, init_{}
      { }
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

      /** @name Map Operations */
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
      
      size_type count(key_type const& x) count
      {
        return valid(k) ? 1 : 0;
      }

      /**
       * Insert the given key/value pair into the map. This operation
       * will always succeed.
       */
      pair<iterator, bool> insert(value_type const& x)
      {
        map_[x.first] = x.second;
        return {begin() + x.first, true};
      }
      
      /**
       * Erase the mapping indicated by the given iterator.
       * 
       * @note This operation does not eagerly erase the mapped value. It 
       * is erased when the mapping is overwritten or the map is destroyed.
       */
      void erase(iterator pos)
      { }
      
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
        if(k >= size()) {
          resize(2 * k);
        }
        return map_[k];
      }


      /**
       * Erase the association with the specified key. The mapped value at the
       * specified index is assigned to -1u.
       */
      void erase(key_type const& k)
      {
        assert(( has(k) ));
        map_[k] = -1u;
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
      //@}
      
    private:
      // Return true if the given key is valid (i.e., in bounds).
      bool valid(key_type const& k) const
      {
        return !empty() && k < size();
      }
      
    private:
      vector_type data_;
    };

} // namespace origin

#endif

