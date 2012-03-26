// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

namespace origin
{

  // Regular type (archetype)
  // The regular type archetype is the basis for all value archetypes used in
  // testing.
  template <typename T>
    class regular_type
    {
      static_assert(Regular<T>(), "");
    public:
      // Default initialization.
      regular_type() : value() { }

      // Movable
      regular_type(regular_type&& x) : value(std::move(x.value)) { }
      
      regular_type& operator=(regular_type&& x) 
      { 
        value = std::move(x.value);
        return *this;
      }

      // Copyable
      regular_type(const regular_type& x) : value(x.value) { }
      
      regular_type& operator=(regular_type x) 
      { 
        o_swap(value, x.value); 
        return *this;
      }

      // Copy conversion
      regular_type(const T& x) : value(x) { }


      // Equality comparison
      bool operator==(const regular_type& x) const { return value == x.value; }
      bool operator!=(const regular_type& x) const { return value != x.value; }

    private:
      T value;
    };


  
  // Random value generation support
  // All regular types should have a corresponding value generator.
  template <typename T>
    struct default_distribution_traits<regular_type<T>>
    {
      using Dist = 
        adapted_distribution<Default_distribution_type<T>, regular_type<T>>;

      static Dist get() { return Dist{}; }
    };



  // Input iterator type (archetype)
  // The input iterator archetype provides an input-only iterator for a given
  // container type, C. An input iterator is copyable, equality comparable,
  // and pre-incrementable. A limited form of post-increment is also supported.
  //
  // TODO: Implement a system for tracking 'live' input iterators. A live
  // input iterator is the most recent copy of an input iterator. When the
  // a live iterator is destroyed, control passes back to the original.
  //
  // Note that end iterators are not live in the same sense and are not
  // subject to this kind of control mechanism.
  template <typename I>
    class input_iterator_type
    {
      I iter;
    public:
      using value_type = Value_type<I>;
      using reference = Iterator_reference<I>;
      using pointer = Iterator_pointer<I>;
      using difference_type = Distance_type<I>;
      using iterator_category = std::input_iterator_tag;

      // When initialized over 
      input_iterator_type(I i)
        : iter(i)
      { }

      // Movable
      input_iterator_type(input_iterator_type&& x)
        : iter(std::move(x.iter))
      { }

      input_iterator_type& operator==(input_iterator_type&& x)
      {
        iter = std::move(x.iter);
        return *this;
      }

      // Copyable
      input_iterator_type(const input_iterator_type& x) 
        : iter(x.iter) 
      { }
      
      input_iterator_type& operator=(input_iterator_type x)
      {
        o_swap(iter, x);
        return *this;
      }

      // Equality_comparable
      // Two input iterators are equal only when they refer to the same
      // position in the same stream. Comparing iterators into different
      // streams is a conceptual error.
      bool operator==(const input_iterator_type& x) const { return iter == x.iter; }
      bool operator!=(const input_iterator_type& x) const { return !(*this == x); }

      // Readable
      reference operator*() const { return *iter; }
      pointer operator->() const { return &*iter; }

      // Incrementable
      input_iterator_type& operator++()
      {
        ++iter;
        return *this;
      }

      auto operator++(int) -> decltype(++iter) { return ++iter; }
    };


  template <typename I>
    input_iterator_type<I> as_input_iterator(I i) { return i; }


} // namespace origin
