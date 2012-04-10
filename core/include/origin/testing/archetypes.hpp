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
  struct default_constructible_type;
  struct move_constructible_type;
  struct copy_constructible_type;
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
  //
  // Note that T must be a complete type.
  template <typename T, typename... Archs>
    struct archetype : Archs::template impl<T, archetype<T, Archs...>>...
    {
      using This_type = archetype<T, Archs...>;
      using Value_type = T;

      // FIXME: I could make this more effecient by aggregating flags instead
      // of recursing over sequences. Basically, this is deducing the set of
      // properties that will be enforced by the various constructors and
      // assignment operators.
      static constexpr bool Is_default_constructible = 
        Has_archetype<default_constructible_type, Archs...>();
      
      static constexpr bool Is_move_constructible = 
        Has_archetype<move_constructible_type, Archs...>();
      
      static constexpr bool Is_copy_constructible = 
        Has_archetype<copy_constructible_type, Archs...>();

      static constexpr bool Is_movable = Has_archetype<movable_type, Archs...>();
      static constexpr bool Is_copyable = Has_archetype<copyable_type, Archs...>();

      // This construct is used to explicitly initialize archetype values
      // over the underlying value type. It's a pain, but it virtually
      // guarantees that this won't overlap with real constructor requirements.
      explicit archetype(dummy_t, const T& value = {}) : value(value) { }

      // Default initialization
      archetype()
        : value()
      {
        static_assert(Is_default_constructible, "");
      }

      // Movable
      archetype(archetype&& x)
        : value(std::move(x.value))
      { 
        // We allow this to be used for move and copy constructible types just
        // in case we found copy but not move.
        static_assert(Is_move_constructible || Is_copy_constructible, "");
      }

      archetype& operator=(archetype&& x)
      {
        // Copyable refines movable so we don't also need to check for Copyable.
        static_assert(Is_movable, "");
        value = std::move(x.value);
        return *this;
      }


      // Copyable
      archetype(const archetype& x)
        : value(x.value)
      {
        static_assert(Is_copy_constructible, "");
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



  // NOTE: Within the _*archtype classes, the derived archetype class T
  // is an incomplete type. It would be great to statically assert properties
  // of the T (or rather T::Value_type) at class level, but it's not possible
  // using this design.



  // Default constructible (archetype)
  // Implements features related to default initialization.
  //
  // TODO: I could store additional metadata about default initialization
  // about the derived class.
  template <typename T, typename Arch>
    struct default_constructible_archetype
    {
      static_assert(Default_constructible<T>(), "");
    };


  // Default constructible (archetype selector)
  // The default archetype is a placeholder for default initialization 
  // semantics.
  struct default_constructible_type
  {
    using refines = std::tuple<>;

    template <typename T, typename Arch>
      using impl = default_constructible_archetype<T, Arch>;
  };



  // Move constructible (archetype)
  template <typename T, typename Arch>
    struct move_constructible_archetype
    {
      static_assert(Move_constructible<T>(), "");
    };



  // Move constructible (archetype selector)
  // The default archetype is a placeholder for default initialization 
  // semantics.
  struct move_constructible_type
  {
    using refines = std::tuple<>;

    template <typename T, typename Arch>
      using impl = move_constructible_archetype<T, Arch>;
  };



  // Copy constructible (archetype)
  // Implements functionality related to move construction. Note thta while all
  // copy constructible types are move constructible, this does not explicitly
  // refine the move constructible archetype. This helps prevent multiple
  // inheritances from sneaking into the archetype hierarchy.
  template <typename T, typename Arch>
    struct copy_constructible_archetype
    {
      static_assert(Copy_constructible<T>(), "");
    };



  // Copy constructible (archetype selector)
  // The default archetype is a placeholder for default initialization 
  // semantics.
  struct copy_constructible_type
  {
    using refines = std::tuple<>;

    template <typename T, typename Arch>
      using impl = copy_constructible_archetype<T, Arch>;
  };



  // Movable (archetype)
  // Implements features related to move semantics.
  //
  // TODO: Store additional information about data flow.
  template <typename T, typename Arch>
    struct movable_archetype : move_constructible_archetype<T, Arch>
    {
      static_assert(Movable<T>(), "");
    };
  


  // Movable (archetype selector)
  // The movable archetype is a placeholder for copy semantics.
  struct movable_type
  { 
    using refines = std::tuple<move_constructible_type>;

    template <typename T, typename Arch>
      using impl = movable_archetype<T, Arch>;
  };



  // Copyable (archetype)
  // Implements features related to copy semantics. A copyable type is also
  // movable.
  //
  // TODO: I could use this to store and query data flow information about
  // the derived objects.
  template <typename T, typename Arch>
    struct copyable_archetype : movable_archetype<T, Arch>
                              , copy_constructible_archetype<T, Arch>
    {
      static_assert(Copyable<T>(), "");
    };



  // Copyable (archetype selector)
  // The copyable archetype is a placeholder for copy semantics
  struct copyable_type
  { 
    using refines = std::tuple<movable_type, copy_constructible_type>;

    template <typename T, typename Arch>
      using impl = copyable_archetype<T, Arch>;
  };



  // Equality comparable (archetype)
  // Implemnts the equality comparison facade for the derived archetype T.
  template <typename T, typename Arch, typename Other = default_t>
    struct equality_comparable_archetype;

  template <typename T, typename Arch>
    struct equality_comparable_archetype<T, Arch, default_t>
    {
      static_assert(Equality_comparable<T>(), "");
      friend bool operator==(const Arch& a, const Arch& b) { return a.value == b.value; }
      friend bool operator!=(const Arch& a, const Arch& b) { return a.value != b.value; }
    };

  template <typename T, typename Arch, typename Other>
    struct equality_comparable_archetype
    {
      static_assert(Equality_comparable<T, Other>(), "");
      friend bool operator==(const Arch& a, const Other& b) { return a.value == b; }
      friend bool operator==(const Other& a, const Arch& b) { return a == b.value; }
      friend bool operator!=(const Arch& a, const Other& b) { return a.value != b; }
      friend bool operator!=(const Other& a, const Arch& b) { return a != b.value; }
    };



  // Equality comparable (archetype selector)
  // An explicit argument may be given to define cross-type equality 
  // comparaison.
  template <typename Other = default_t>
    struct equality_comparable_type
    {
      using refines = std::tuple<>;

      template <typename T, typename Arch>
        using impl = equality_comparable_archetype<T, Arch, Other>;
    };

  // FIXME: This should be merged with the primary template, but GCC was
  // giving an ICE when that happened.
  template <>
    struct equality_comparable_type<default_t>
    {
      using refines = std::tuple<>;

      template <typename T, typename Arch>
        using impl = equality_comparable_archetype<T, Arch>;
    };



  // Equality comparable (archetype)
  // Implements the weak ordering facade for the derived archetype T.
  template <typename T, typename Arch, typename Other = default_t>
    struct weakly_ordered_archetype;

  template <typename T, typename Arch>
    struct weakly_ordered_archetype<T, Arch, default_t>
    {
      static_assert(Equality_comparable<T>(), "");
      friend bool operator<(const Arch& a, const Arch& b) { return a.value < b.value; }
      friend bool operator>(const Arch& a, const Arch& b) { return a.value > b.value; }
      friend bool operator<=(const Arch& a, const Arch& b) { return a.value <= b.value; }
      friend bool operator>=(const Arch& a, const Arch& b) { return a.value >= b.value; }
    };

  template <typename T, typename Arch, typename Other>
    struct weakly_ordered_archetype
    {
      static_assert(Equality_comparable<T, Other>(), "");
      friend bool operator<(const Arch& a, const Other& b) { return a.value < b; }
      friend bool operator<(const Other& a, const Arch& b) { return a < b.value; }
      friend bool operator>(const Arch& a, const Other& b) { return a.value > b; }
      friend bool operator>(const Other& a, const Arch& b) { return a > b.value; }
      friend bool operator<=(const Arch& a, const Other& b) { return a.value <= b; }
      friend bool operator<=(const Other& a, const Arch& b) { return a <= b.value; }
      friend bool operator>=(const Arch& a, const Other& b) { return a.value >= b; }
      friend bool operator>=(const Other& a, const Arch& b) { return a >= b.value; }
    };



  // Weakly ordered (archetype selector)
  template <typename Other = default_t>
    struct weakly_ordered_type
    {
      using refines = std::tuple<>;

      template <typename T, typename Arch>
        using impl = weakly_ordered_archetype<T, Arch, Other>;
    };

  // FIXME: See the comments above about ICEs.
  template <>
    struct weakly_ordered_type<default_t>
    {
      using refines = std::tuple<>;

      template <typename T, typename Arch>
        using impl = weakly_ordered_archetype<T, Arch>;
    };



  // Totally ordered (archetype)
  // Implements the total ordering facade for the derived archetype T.
  //
  // FIXME": Secialize the archetype so I can constrain it correctly.
  template <typename T, typename Arch, typename Other = default_t>
    struct totally_ordered_archetype
      : equality_comparable_archetype<T, Arch, Other>
      , weakly_ordered_archetype<T, Arch, Other>
    { 
    };



  // Totally ordered (archetype selector)
  template <typename Other = default_t>
    struct totally_ordered_type
    {
      using refines = 
        std::tuple<equality_comparable_type<Other>, weakly_ordered_type<Other>>;
      
      template <typename T, typename Arch>
        using impl = totally_ordered_archetype<T, Arch, Other>;
    };

  // FIXME: See the comment above about ICEs.
  template <>
    struct totally_ordered_type<default_t>
    {
      using refines = 
        std::tuple<equality_comparable_type<>, weakly_ordered_type<>>;
      
      template <typename T, typename Arch>
        using impl = totally_ordered_archetype<T, Arch>;
    };



  // Regular (archetype)
  // Exposes the regular type facade for the Derived class.
  template <typename T, typename Arch>
    struct regular_archetype : default_constructible_archetype<T, Arch>
                             , copyable_archetype<T, Arch>
                             , equality_comparable_archetype<T, Arch>
    { 
      static_assert(Regular<T>(), "");
    };



  // Regular type (archetype selector)
  // The regular type archetype default constructible, copy constructible, and
  // equality comparable.
  struct regular_type
  {
    using refines = std::tuple<default_constructible_type, 
                               copyable_type, 
                               equality_comparable_type<>>;

    template <typename T, typename Arch>
      using impl = regular_archetype<T, Arch>;
  };




  template <typename F, typename Arch, typename... Args>
    struct function_archetype : copy_constructible_archetype<F, Arch>
    {
    private:
      Arch*       self_()       { return static_cast<Arch*>(this); }
      const Arch* self_() const { return static_cast<const Arch*>(this); }
    public:

      auto operator()(Args... args) -> Result_of<F(Args...)>
      {
        return self_()->value(args...);
      }

      auto operator()(Args... args) const -> Result_of<F(Args...)>
      {
        return self_()->value(args...);
      }
    };

  template <typename... Args>
    struct function_type
    {
      using refines = std::tuple<copy_constructible_type>;

      template <typename F, typename Arch>
        using impl = function_archetype<F, Arch, Args...>;
    };

} // namespace origin
