// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_HEAPS_INDEX_HPP
#define ORIGIN_HEAPS_INDEX_HPP

#include <cassert>
#include <vector>
#include <unordered_map>

namespace origin
{
  /**
   * The hash index class maintains an association between a key and an index.
   */
  template<typename Key, 
           typename Hash = std::hash<Key>, 
           typename Eq = std::equal_to<Key>, 
           typename Alloc = std::allocator<Key>>
    class hash_index
    {
    public:
      typedef Key key_type;
      typedef std::size_t mapped_type;
      typedef Hash hasher;
      typedef Eq key_equal;

      /**
       * Rebind the hash index over the specified allocator.
       */
      template<typename A>
        struct rebind
        {
          typedef hash_index<Key, Hash, Eq, A> type;
        };
      
      /** @name Index Operations */
      //@{
      /**
       * Return true if the map has the given key.
       */
      bool has(key_type const& x) const
      {
        return map_.find(x) != map_.end();
      }
      
      /**
       * Return the index associated with the given key.
       */
      mapped_type get(key_type const& k) const
      {
        assert(( has(k) ));
        return map_.find(k)->second;
      }
      
      /**
       * Associate the the given key with the specified value.
       */
      void put(key_type const& k, mapped_type v)
      {
        map_[k] = v;
      }
      
      /**
       * Erase the association for the given key.
       */
      void erase(key_type const& k)
      {
        map_.erase(k);
      }
      //@}
      
    private:
      std::unordered_map<Key, mapped_type, Hash, Eq, Alloc> map_;
    };

  /**
   * The vector index maintains an assocation between an unsigned integer (the 
   * key) and an  index. The numeric key values must be in the range [0, n) and 
   * preferrably dense (with no gaps). The mapped value -1u is used to gaps in 
   * the mapping.
   * 
   * @tparam Key    An Unsigned_Int type, size_t by default.
   * @tparam Alloc  An Allocator type
   */
  template<typename Key = std::size_t, 
           typename Alloc = std::allocator<Key>>
    class vector_index
    {
      static_assert(std::is_unsigned<Key>::value, "Key must be unsigned");
    public:
      typedef Key key_type;
      typedef std::size_t mapped_type;

      /**
       * Rebind the hash index over the specified allocator.
       */
      template<typename A>
        struct rebind
        {
          typedef vector_index<Key, A> type;
        };
        
      /**
       * @brief Default constructor
       */
      vector_index()
        : map_{4u, -1u}
      { }

      /** @name Index Operations */
      //@{
      /**
       * Return true if the map has the given key. This is true if the key
       * is a valid index of the vector and it's mapped value is not -1u.
       */
      bool has(key_type const& x) const
      {
        return !map_.empty() && x < map_.size() && map_[x] != -1u;
      }
      
      /**
       * Return the index associated with the given key.
       */
      mapped_type get(key_type const& k) const
      {
        assert(( has(k) ));
        return map_[k];
      }
      
      /**
       * Associate the the given key with the specified value. The vector is
       * resized if the key is out of bounds.
       */
      void put(key_type const& k, mapped_type v)
      {
        if(k >= map_.size()) {
          map_.resize(2 * k, -1u);
        }
        map_[k] = v;
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
      
    private:
      std::vector<mapped_type> map_;
    };

} // namespace origin

#endif

