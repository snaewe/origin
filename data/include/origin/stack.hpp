// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_STACK_HPP
#define ORIGIN_STACK_HPP

#include <cassert>
#include <vector>

namespace origin
{
  /**
   * The stack class implements the traditional stack data structure.
   * 
   * @tparam T      The value type of the stack
   * @tparam Cont   The underlying container
   */
  template<typename T, typename Cont = std::vector<T>>
  class stack
  {
    typedef Cont container_type;
  public:
    typedef typename container_type::value_type value_type;
    typedef typename container_type::reference reference;
    typedef typename container_type::const_reference const_reference;
    typedef typename container_type::size_type size_type;
    
    stack()
      : data_()
    { }
    
    template<typename Iter>
      stack(Iter first, Iter last)
        : data_(first, last)
      { }
      
    bool empty() const
    {
      return data_.empty();
    }

    size_type size() const
    {
      return data_.size();
    }
    
    reference top()
    {
      return data_.back();
    }
    
    const_reference top() const
    {
      return data_.back();
    }
    
    void push(value_type const& x)
    {
      data_.push_back();
    }
    
    void push(value_type&& x)
    {
      data_.push_back(std::move(x));
    }
    
    template<typename... Args>
      void emplace(Args&&... args)
      {
        data_.emplace_back(std::forward<Args>(args)...);
      }
    
    void pop(value_type const& x)
    {
      assert(( !empty() ));
      data_.pop_back();
    }
    
    value_type displace()
    {
      assert(( !empty() ));
      return std::move(top());
    }
    
  private:
    container_type data_;
  };
  
  
} // namespace origin

#endif
