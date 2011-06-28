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

#include <origin/iterator/facades.hpp>

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

  // FIXME: The ordinal_map is a Total, Direct, Unique Association.
  
  // FIXME: Seriously, how many Total Maps could there possibly be? Still, there
  // semantically, this is what we have.
  
  // NOTE: Total Maps don't have erase functions and their insert function is
  // more like a Multimap than a Map: it can't fail.

  // FIXME: This needs to be moved into the standard library, but I have no
  // idea where it should go. Utility? Traits? Concepts? It's very fundamental.
  /**
   * Return the ordinal number associated the given object.
   * 
   * @note This is only enabled for unsigned integral numbers and for any other
   * type that specializes this operation.
   */
  template<typename T>
    inline typename std::enable_if<std::is_unsigned<T>::value, T>::type
    ord(T x)
    {
      return x;
    }


  /**
   * The vector map iterator provides an iterator type over the mappings in a 
   * vector map.
   */
  template<typename Key, typename Vec>
    class ordinal_map_iterator
      : public random_access_iterator_facade<
          ordinal_map_iterator<Key, Vec>,
          std::pair<Key, typename Vec::reference>,
          std::pair<Key, typename Vec::reference>,
          std::pair<Key, typename Vec::reference>,
          typename Vec::difference_type
        >
    {
      typedef Key key_type;
      typedef typename Vec::size_type size_type;
      typedef typename Vec::reference base_reference;
      typedef typename Vec::iterator base_iterator;
      typedef random_access_iterator_facade<
        ordinal_map_iterator<Key, Vec>,
        std::pair<key_type, base_reference>,
        std::pair<key_type, base_reference>,
        std::pair<key_type, base_reference>,
        typename Vec::difference_type
      > base_type;
    public:
      typedef typename base_type::reference reference;
      typedef typename base_type::difference_type difference_type;
      
      ordinal_map_iterator(size_type n, base_iterator i)
        : pos_{n}, iter_{i}
      { }
      
      bool equal(ordinal_map_iterator const& x) const
      {
        return iter_ == x.iter_;
      }
      
      bool less(ordinal_map_iterator const& x) const
      {
        return iter_ < x.iter_;
      }
      
      reference dereference() const
      {
        return {key_type{pos_}, *iter_};
      }
      
      void increment()
      {
        ++pos_;
        ++iter_;
      }
      
      void decrement()
      {
        --pos_;
        --iter_;
      }
      
      void advance(difference_type n)
      {
        pos_ += n;
        iter_ += n;
      }
      
      difference_type distance(ordinal_map_iterator const& x) const
      {
        return x.iter - iter_;
      }
      
    private:
      size_type pos_;
      base_iterator iter_;
    };
  
  /**
   * The constant vector map iterator provides a constant iterator type over 
   * the mappings in a vector map.
   */
  template<typename Key, typename Vec>
    class const_ordinal_map_iterator
      : public random_access_iterator_facade<
          const_ordinal_map_iterator<Key, Vec>,
          std::pair<Key, typename Vec::const_reference>,
          std::pair<Key, typename Vec::const_reference>,
          std::pair<Key, typename Vec::const_reference>,
          typename Vec::difference_type
        >
    {
      typedef Key key_type;
      typedef typename Vec::size_type size_type;
      typedef typename Vec::const_reference base_reference;
      typedef typename Vec::const_iterator base_iterator;
      typedef random_access_iterator_facade<
        const_ordinal_map_iterator<Key, Vec>,
        std::pair<key_type, base_reference>,
        std::pair<key_type, base_reference>,
        std::pair<key_type, base_reference>,
        typename Vec::difference_type
      > base_type;
    public:
      typedef typename base_type::reference reference;
      typedef typename base_type::difference_type difference_type;
      
      const_ordinal_map_iterator(size_type n, base_iterator i)
        : pos_{n}, iter_{i}
      { }
      
      const_ordinal_map_iterator(ordinal_map_iterator<Key, Vec> const& x)
        : pos_{x.pos_}, iter_{x.iter_}
      { }
      
      bool equal(const_ordinal_map_iterator const& x) const
      {
        return iter_ == x.iter_;
      }
      
      bool less(const_ordinal_map_iterator const& x) const
      {
        return iter_ < x.iter_;
      }
      
      reference dereference() const
      {
        return {key_type{pos_}, *iter_};
      }
      
      void increment()
      {
        ++pos_;
        ++iter_;
      }
      
      void decrement()
      {
        --pos_;
        --iter_;
      }
      
      void advance(difference_type n)
      {
        pos_ += n;
        iter_ += n;
      }

      difference_type distance(const_ordinal_map_iterator const& x) const
      {
        return x.iter_ - iter_;
      }
      
    private:
      size_type pos_;
      base_iterator iter_;
    };

  /**
   * A vector map implements a Direct, Unique Map over a Random Access 
   * Container. Like all Direct Maps, the key type of the vector map is 
   * the size type of the underlying container. However, all keys are implicitly 
   * mapped to values. This is an optimization over true Direct Maps where the
   * mapped value may not be defined for a given key.
   *
   * The ordinal_map is a container adaptor, taking an underlying container as
   * a template parameter. The allocator for the map can be specified as an
   * argument to that parameter.
   * 
   * @tparam K      An Ordinal Key type
   * @tparam V      The mapped value type, an Object type
   * @tparam Vec    The underlying Vector type
   */
  template<typename K, typename V, typename Vec = std::vector<V>>
    class ordinal_map
    {
      typedef Vec vector_type;
    public:
      typedef typename vector_type::allocator_type allocator_type;
      typedef typename vector_type::size_type size_type;

      typedef K key_type;
      typedef V mapped_type;
      typedef std::pair<key_type, mapped_type> value_type;
      typedef typename vector_type::reference reference;
      typedef typename vector_type::const_reference const_reference;
      typedef typename vector_type::pointer pointer;
      
      typedef ordinal_map_iterator<key_type, vector_type> iterator;
      typedef const_ordinal_map_iterator<key_type, vector_type> const_iterator;
      typedef std::reverse_iterator<iterator> reverse_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

      /** @name Initialization */
      //@{
      /**
       * @brief Default constructor
       */
      ordinal_map(allocator_type const& alloc = allocator_type{})
      { }

      // FIXME: Optimize for forward iterators.
      // FIXME: If we concepts, we could also make a range constructor that
      // accepted a sequence of mapped_types also. 
      /**
       * @name Range constructor
       * The range constructor initializes the map over a sequence of mappings
       * (i.e., key value pairs).
       * 
       * @tparam Iter   An Iterator satisfying Input_Iterator<Iter, value_type>

       * @param first   The first iterator a range
       * @param last    An iterator past the end of the range
       */
      template<typename Iter>
        ordinal_map(Iter first, Iter last)
          : data_{}
        {
          for( ; first != last; ++first)
            get(first->first) = first->second;
        }
        
      /**
       * @name Initializer list constructor (for value_types)
       * The initializer list constructor initializess a map over a sequence
       * of mappings (i.e., key/value pairs).
       * 
       * @param list  An initializer list containing mappings
       */
      ordinal_map(std::initializer_list<value_type> list)
        : data_{}
      {
        for(auto& x : list)
          get(x.first) = x.second;
      }
      
      /**
       * @name Initializer list constructor (for mapped_types)
       * The initializer list constructor initializess a map over a sequence
       * of mapped values.
       * 
       * @param list  An initializer list containing mapped values.
       */
      ordinal_map(std::initializer_list<mapped_type> list)
        : data_(list.begin(), list.end())
      { }
      
      /**
       * @name Fill constructor
       * The fill constructor provides a method of initializing a map over a
       * set of n mappings, optionally initialized to the value x.
       * 
       * @param n   The number of initial mappings
       * @param x   The default mapped value for each initial mapping.
       */
      explicit ordinal_map(size_type n, mapped_type const& x = mapped_type{})
        : data_(n, x)
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

      /** Member access */
      //@{
      /**
       * Return a reference to the mapped value indicated by the given key.
       * If no such key exists, the map is resized in order to accomodate
       * the new value. This may * result in the creation of ord(k) - size() 
       * new mappings.
       */
      mapped_type& operator[](key_type const& k)
      {
        maybe_resize(ord(k));
        return get(k);
      }

      /**
       * Return a reference to the mapped value indicated by the given key.
       * Note that the key must be in the map.
       * 
       * @pre find(k) != end()
       */
      mapped_type const& operator[](key_type const& k) const
      { 
        assert(( find(k) != end() ));
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
        maybe_resize(ord(x.first));
        get(x.first) = x.second;
        return begin() + ord(x.first);
      }

      /**
       * Erase the mapping at the specified position.
       * 
       * @note This operation is lazy. The erased object is not erased until
       * it is overwritten or the vector is cleared or destroyed.
       */
      void erase(iterator pos)
      { }
      
      /**
       * Erase the mapping specified by the given key.
       * 
       * @note This operation is lazy. The erased object is not erased until
       * it is re-assigned or the vector is cleared or destroyed.
       */
      void erase(key_type const& k)
      { }

      /**
       * Erase all keys in the map.
       */
      void clear()
      {
        data_.clear();
      }

      /**
       * Swap this map the other.
       */
      void swap(ordinal_map& x)
      {
        data_.swap(x);
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
        return valid(k) ? begin() + ord(k) : end();
      }

      const_iterator find(key_type const& k) const
      {
        return valid(k) ? begin() + ord(k) : end();
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
        return {0, data_.begin()};
      }
      
      iterator end()
      {
        return {size(), data_.end()};
      }
      
      const_iterator begin() const
      {
        return {0, data_.begin()};
      }
      
      const_iterator end() const
      {
        return {size(), data_.end()};
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

    private:
      // Return true if the given key is valid (i.e., in bounds).
      bool valid(key_type const& k) const
      {
        return !empty() && ord(k) < size();
      }

      // If the key is larger than the current size, allocate more mappings. 
      void maybe_resize(size_type const& k)
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
        return data_[ord(k)];
      }
      
      mapped_type const& get(key_type const& k) const
      {
        return data_[ord(k)];
      }
      
    private:
      vector_type data_;
    };

} // namespace origin

#endif

