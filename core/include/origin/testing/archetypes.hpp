// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

namespace origin
{
  // A dummy type
  struct dummy_t { };


  // Archetype declarations
  struct default_type;
  struct movable_type;
  struct copyable_type;


  template <typename Arch, typename T> struct inspect_archetype;
  template <typename Arch, typename... Args> struct has_archetype;

  // Inspect archetype
  // Returns true if true if Arch is contained in the list of T's refinements.
  template <typename Arch, typename T>
    static constexpr bool Inspect_archetype()
    {
      return inspect_archetype<Arch, T>::value;
    }


  // Has archetype
  // Returns true if Arch is found in the list of archetypes.
  template <typename Arch, typename... Args>
    static constexpr bool Has_archetype()
    {
      return has_archetype<Arch, Args...>::value;
    }



  // Has archetype implementation
  // This is gross.
  template <typename Arch, typename T, typename... Args>
    struct has_archetype<Arch, T, Args...>
      : bool_constant<
          Same<Arch, T>() 
          ? true 
          : (Inspect_archetype<Arch, T>() 
             ? true
             : Has_archetype<Arch, Args...>())
        >
    { };

  template <typename Arch>
    struct has_archetype<Arch> : std::false_type { };


  // Inspect the list of refinements, rerpresented by T for the the given
  // Archetype. T is intended to be a tuple. This simply delegates the search
  // back to has_archetype, in order to flatten the entire search.
  template <typename Arch, typename T>
    struct inspect_refinements : std::false_type { };

  template <typename Arch, typename... Args>
    struct inspect_refinements<Arch, std::tuple<Args...>>
      : has_archetype<Arch, Args...>
    { };

  // Inspect archetype implementation
  template <typename Arch, typename T>
    struct inspect_archetype 
      : inspect_refinements<Arch, typename T::refines>
    { };



  // Archetype
  // The archetype class wraps a given value, and exposes a specified set
  // of interface.
  template <typename T, typename... Archs>
    struct archetype : Archs::template impl<archetype<T, Archs...>>...
    {
      using This = archetype<T, Archs...>;
      using Value = T;

      // FIXME: I could make this more effecient by aggregating flags instead
      // of recursing over sequences.
      static constexpr bool Has_default = Has_archetype<default_type, Archs...>();
      static constexpr bool Has_movable = Has_archetype<movable_type, Archs...>();
      static constexpr bool Has_copyable = Has_archetype<copyable_type, Archs...>();

      static constexpr bool Is_default = Has_default;
      static constexpr bool Is_movable = Has_movable || Has_copyable;
      static constexpr bool Is_copyable = Has_copyable;

      // This construct is used to explicitly initialize archetype values
      // over the underlying value type. It's a pain, but it virtually
      // guarantees that this won't overlap with real constructor requirements.
      explicit archetype(dummy_t, const T& value = {}) : value(value) { }

      // Default initialization
      archetype()
        : value()
      {
        static_assert(Is_default, "");
      }

      // Movable
      archetype(archetype&& x)
        : value(std::move(x.value))
      { 
        static_assert(Is_movable, "");
      }

      archetype& operator=(archetype&& x)
      {
        static_assert(Is_movable, "");
        value = std::move(x.value);
        return *this;
      }

      // Copyable
      archetype(const archetype& x)
        : value(x.value)
      {
        static_assert(Is_copyable, "");
      }

      archetype& operator=(const archetype& x)
      {
        static_assert(Is_copyable, "");
        value = x.value;
        return *this;
      }


      T value;
    };



  // FIXME: I could do away with the refines tuple if I had access to a list
  // of base types. GCC has an extension that allows such queries, but it's
  // very much internal. I should wait until there's a more standard method.


  // Default type (archetype)
  // The default archetype is a placeholder for default initialization 
  // semantics.
  struct default_type
  {
    using refines = std::tuple<>;

    template <typename Derived>
      struct impl { };
  };

  
  // Movable type (archetype)
  // The movable archetype is a placeholder for copy semantics.
  struct movable_type
  { 
    using refines = std::tuple<>;

    template <typename Derived>
      struct impl { };
  };



  // Copyable type (archetype)
  // The copyable archetype is a placeholder for copy semantics
  struct copyable_type
  { 
    using refines = std::tuple<>;

    template <typename Derived>
      struct impl : movable_type::template impl<Derived> { };
  };


  // Equality comparable type.
  // An explicit argument may be given to define cross-type equality 
  // comparaison.
  template <typename T = default_t>
    struct equality_comparable_type;

  // The default version of equality comparble is defined in terms of the
  // derived type.
  template <>
    struct equality_comparable_type<default_t>
    {
      using refines = std::tuple<>;

      template <typename Derived>
        struct impl
        {
          friend bool operator==(const Derived& a, const Derived& b) { return a.value == b.value; }
          friend bool operator!=(const Derived& a, const Derived& b) { return a.value != b.value; }
        };
    };

  // Cross-type equality generates all of the relational interfaces that would
  // normally be expected.
  template <typename Other>
    struct equality_comparable_type
    {
      using refines = std::tuple<>;

      template <typename Derived>
        struct impl
        {
          friend bool operator==(const Derived& a, const Other& b) { return a.value == b; }
          friend bool operator==(const Other& a, const Derived& b) { return a == b.value; }
          friend bool operator!=(const Derived& a, const Other& b) { return a.value != b; }
          friend bool operator!=(const Other& a, const Derived& b) { return a != b.value; }
        };
    };



  // Weakly ordered (archetype)
  template <typename T = default_t>
    struct weakly_ordered_type;

  template <>
    struct weakly_ordered_type<default_t>
    {
      using refines = std::tuple<>;

      template <typename Derived>
        struct impl
        {
          friend bool operator<(const Derived& a, const Derived& b) { return a.value < b.value; }
          friend bool operator>(const Derived& a, const Derived& b) { return a.value > b.value; }
          friend bool operator<=(const Derived& a, const Derived& b) { return a.value <= b.value; }
          friend bool operator>=(const Derived& a, const Derived& b) { return a.value >= b.value; }
        };
    };

  // Cross-type equality generates all of the relational interfaces that would
  // normally be expected.
  template <typename Other>
    struct weakly_ordered_type
    {
      using refines = std::tuple<>;

      template <typename Derived>
        struct impl
        {
          friend bool operator<(const Derived& a, const Other& b) { return a.value < b; }
          friend bool operator<(const Other& a, const Derived& b) { return a < b.value; }

          friend bool operator>(const Derived& a, const Other& b) { return a.value > b; }
          friend bool operator>(const Other& a, const Derived& b) { return a > b.value; }

          friend bool operator<=(const Derived& a, const Other& b) { return a.value <= b; }
          friend bool operator<=(const Other& a, const Derived& b) { return a <= b.value; }

          friend bool operator>=(const Derived& a, const Other& b) { return a.value >= b; }
          friend bool operator>=(const Other& a, const Derived& b) { return a >= b.value; }
        };
    };



  // Totally ordered (archetype)
  template <typename T = default_t>
    struct totally_ordered_type;

  template <>
    struct totally_ordered_type<default_t>
    {
      using refines = 
        std::tuple<equality_comparable_type<>, weakly_ordered_type<>>;

      template <typename Derived>
        struct impl : equality_comparable_type<>::template impl<Derived>
                    , weakly_ordered_type<>::template impl<Derived>
        { };
    };

  // Cross-type equality generates all of the relational interfaces that would
  // normally be expected.
  template <typename Other>
    struct totally_ordered_type
    {
      using refines = 
        std::tuple<equality_comparable_type<Other>, weakly_ordered_type<Other>>;

      template <typename Derived>
        struct impl : equality_comparable_type<Other>::template impl<Derived>
                    , weakly_ordered_type<Other>::template impl<Derived>
        { };
    };



  // Regular type (archetype)
  // The regular type archetype default constructible, copy constructible, and
  // equality comparable.
  struct regular_type
  {
    using refines = 
      std::tuple<default_type, copyable_type, equality_comparable_type<>>;

    template <typename Derived>
      struct impl : default_type::template impl<Derived>
                  , copyable_type::template impl<Derived>
                  , equality_comparable_type<>::template impl<Derived>
      { };
  };



/*
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
*/
} // namespace origin
