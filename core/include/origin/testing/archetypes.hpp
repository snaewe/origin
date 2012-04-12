// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

namespace origin
{
  // NOTE: Within the _*archtype classes, the derived archetype class, Arch,
  // is an incomplete type. The parameter after Arch (usually T) is the
  // archetype's underlying value type. It must be a complete type.



  // Default constructible (facade)
  // Implements features related to default initialization.
  //
  // TODO: I could store additional metadata about default initialization
  // about the derived class.
  template <typename Arch, typename T>
    struct default_constructible_archetype
    {
      static_assert(Default_constructible<T>(), "");
    };



  // Move constructible (facade)
  template <typename Arch, typename T>
    struct move_constructible_archetype
    {
      static_assert(Move_constructible<T>(), "");
    };



  // Copy constructible (facade)
  // Implements functionality related to move construction. Note thta while all
  // copy constructible types are move constructible, this does not explicitly
  // refine the move constructible archetype. This helps prevent multiple
  // inheritances from sneaking into the archetype hierarchy.
  template <typename Arch, typename T>
    struct copy_constructible_archetype
    {
      static_assert(Copy_constructible<T>(), "");
    };



  // Movable (facade)
  // Implements features related to move semantics.
  //
  // TODO: Store additional information about data flow.
  template <typename Arch, typename T>
    struct movable_archetype : move_constructible_archetype<Arch, T>
    {
      static_assert(Movable<T>(), "");
    };
  


  // Copyable (facade)
  // Implements features related to copy semantics. A copyable type is also
  // movable.
  //
  // TODO: I could use this to store and query data flow information about
  // the derived objects.
  template <typename Arch, typename T>
    struct copyable_archetype : movable_archetype<Arch, T>
                              , copy_constructible_archetype<Arch, T>
    {
      static_assert(Copyable<T>(), "");
    };



  // Equality comparable (facade)
  // Implemnts the equality comparison facade for the derived archetype T.
  //
  // Note that explicitly providing a non-default argument for U will cause the
  // compiler to generate comparisons against type U. 
  template <typename Arch, typename T, typename U = default_t>
    struct equality_comparable_archetype
    {
      static_assert(Equality_comparable<T, U>(), "");
      friend bool operator==(const Arch& a, const U& b) { return a.value == b; }
      friend bool operator==(const U& a, const Arch& b) { return a == b.value; }
      friend bool operator!=(const Arch& a, const U& b) { return a.value != b; }
      friend bool operator!=(const U& a, const Arch& b) { return a != b.value; }
    };

  template <typename Arch, typename T>
    struct equality_comparable_archetype<Arch, T, default_t>
    {
      static_assert(Equality_comparable<T>(), "");
      friend bool operator==(const Arch& a, const Arch& b) { return a.value == b.value; }
      friend bool operator!=(const Arch& a, const Arch& b) { return a.value != b.value; }
    };



  // Weakly ordered (facade)
  // Implements the weak ordering facade for the derived archetype T.
  template <typename Arch, typename T, typename U = default_t>
    struct weakly_ordered_archetype
    {
      static_assert(Equality_comparable<T, U>(), "");
      friend bool operator<(const Arch& a, const U& b) { return a.value < b; }
      friend bool operator<(const U& a, const Arch& b) { return a < b.value; }
      friend bool operator>(const Arch& a, const U& b) { return a.value > b; }
      friend bool operator>(const U& a, const Arch& b) { return a > b.value; }
      friend bool operator<=(const Arch& a, const U& b) { return a.value <= b; }
      friend bool operator<=(const U& a, const Arch& b) { return a <= b.value; }
      friend bool operator>=(const Arch& a, const U& b) { return a.value >= b; }
      friend bool operator>=(const U& a, const Arch& b) { return a >= b.value; }
    };

  template <typename Arch, typename T>
    struct weakly_ordered_archetype<Arch, T, default_t>
    {
      static_assert(Equality_comparable<T>(), "");
      friend bool operator<(const Arch& a, const Arch& b) { return a.value < b.value; }
      friend bool operator>(const Arch& a, const Arch& b) { return a.value > b.value; }
      friend bool operator<=(const Arch& a, const Arch& b) { return a.value <= b.value; }
      friend bool operator>=(const Arch& a, const Arch& b) { return a.value >= b.value; }
    };



  // Totally ordered (facade)
  // Implements the total ordering facade for the derived archetype T.
  template <typename Arch, typename T, typename U = default_t>
    struct totally_ordered_archetype : equality_comparable_archetype<Arch, T, U> 
                                     , weakly_ordered_archetype<Arch, T, U>
    {  
      static_assert(Totally_ordered<T, U>(), "");
    };

  template <typename Arch, typename T>
    struct totally_ordered_archetype<Arch, T, default_t>
      : equality_comparable_archetype<Arch, T> 
      , weakly_ordered_archetype<Arch, T>
    {  
      static_assert(Totally_ordered<T>(), "");
    };



  // Regular (facade)
  // Exposes the regular type facade for the Derived class. A regular type is
  // default constructible, copy constructible, and equality comparable.
  template <typename Arch, typename T>
    struct regular_archetype : default_constructible_archetype<Arch, T>
                             , copyable_archetype<Arch, T>
                             , equality_comparable_archetype<Arch, T>
    { 
      static_assert(Regular<T>(), "");
    };



  // Function archetypes
  // The function archetypes expose the function call interfaces to their
  // underlying function objects.



  // Function (facade)
  template <typename Arch, typename F, typename... Args>
    class function_archetype : public copy_constructible_archetype<Arch, F>
    {
      static_assert(Function<F, Args...>(), "");

      Arch&       self()       { return *static_cast<Arch*>(this); }
      const Arch& self() const { return *static_cast<const Arch*>(this); }
    
    public:
      auto operator()(Args... args) -> Result_of<F(Args...)>
      {
        return self().value(args...);
      }

      auto operator()(Args... args) const -> Result_of<F(Args...)>
      {
        return self().value(args...);
      }
    };



  // FIXME: Do I need a regular function archetype? It would only provide
  // a constant overload.



  // Predicate (facade)
  //
  // Note that this does not refine function, only copy constructible. The
  // purpose for doing this is to not inherit hide function's operator() 
  // declarations.
  template <typename Arch, typename P, typename... Args>
    class predicate_archetype : public copy_constructible_archetype<Arch, P>
    { 
      static_assert(Predicate<P, Args...>(), "");

      Arch&       self()       { return *static_cast<Arch*>(this); }
      const Arch& self() const { return *static_cast<const Arch*>(this); }
    
    public:
      bool operator()(Args... args) const
      {
        return self().value(args...);
      }
    };



  // Relation (facade)
  //
  // Note that this does not refine any other function concepts in order to
  // avoid inhereting thir function call operators.
  template <typename Arch, typename R, typename T, typename U = default_t>
    class relation_archetype : public copy_constructible_archetype<Arch, R>
    {
      static_assert(Relation<R, T, U>(), "");

      Arch&       self()       { return *static_cast<Arch*>(this); }
      const Arch& self() const { return *static_cast<const Arch*>(this); }
    
    public:
      bool operator()(const T& t, const U& u) const
      {
        return self().value(t, u);
      }

      bool operator()(const U& u, const T& t) const
      {
        return self().value(u, t);
      }
    };

  template <typename Arch, typename R, typename T>
    class relation_archetype<Arch, R, T, default_t> 
      : public copy_constructible_archetype<Arch, R>
    { 
      static_assert(Relation<R, T>(), "");

      Arch&       self()       { return *static_cast<Arch*>(this); }
      const Arch& self() const { return *static_cast<const Arch*>(this); }
    
    public:
      bool operator()(const T& a, const T& b) const
      {
        return self().value(a, b);
      }
    };



  // FIXME: Write operation archetypes



  // Iterator archetypes
  // The iterator archetypes provide constrained views of an underlying
  // iterator type.



  // Make const reference
  // Make sure that input iterator references are only const. Note that the
  // default (when T is not a reference) is const T, ensuring that we can't
  // accidentally write to a temporary.
  //
  // FIXME: This isn't complete. I should have specializations for rvalue 
  // references and volatile types too. 
  template <typename T> struct make_const_reference { using type = const T; };
  template <typename T> struct make_const_reference<T&> { using type = const T&; };
  template <typename T> struct make_const_reference<const T&> { using type = const T&; };

  template <typename T>
    using Make_const_reference = typename make_const_reference<T>::type;



  // Const iterator reference (alias)
  // Returns a const-qualified version of the iterator reference type.
  template <typename I>
    using Const_iterator_reference = Make_const_reference<Iterator_reference<I>>;



  // Input iterator (facade)
  // An input iterator is copyable, weakly incrementable and equality 
  // comparable. It's result can be bound to a const reference, but not written.
  template <typename Arch, typename I>
    class input_iterator_archetype : public copyable_archetype<Arch, I>
                                   , public equality_comparable_archetype<Arch, I>
    {
      static_assert(Input_iterator<I>(), "");

      Arch&       self()       { return *static_cast<Arch*>(this); }
      const Arch& self() const { return *static_cast<const Arch*>(this); }

    public:
      Const_iterator_reference<I> operator*() const
      {
        return *(self().value);
      }


      Arch& operator++()
      {
        ++self().value;
        return self();
      }
    };



  // Output iteator (facade)
  // The output iterator allows writing through the dereferenced result. An
  // output iterator is copyable, but not equality comparable (although it
  // should be).
  template <typename Arch, typename I, typename T>
    class output_iterator_archetype : public copyable_archetype<Arch, I>
    {
      static_assert(Output_iterator<I, T>(), "");

      Arch&       self()       { return *static_cast<Arch*>(this); }
      const Arch& self() const { return *static_cast<const Arch*>(this); }

      // The proxy class allows the expression *i = x to be valid, but does
      // not allow x = *i (for any reasonable type of x) to be valid.
      struct proxy
      {
        Dereference_result<I> ref;

        explicit proxy(Dereference_result<I> ref) : ref(ref) { }

        proxy& operator=(const T& value) { ref = value; return *this; }
      };
    
    public:
      proxy operator*() const { return proxy { *(self().value) }; }

      Arch& operator++() { ++self().value; return self(); }
    };



  // Forward iterator (facade)
  template <typename Arch, typename I>
    class forward_iterator_archetype : public input_iterator_archetype<Arch, I>
    {
      static_assert(Forward_iterator<I>(), "");

      Arch&       self()       { return *static_cast<Arch*>(this); }
      const Arch& self() const { return *static_cast<const Arch*>(this); }

    public:
      // FIXME: I'm not sure why I have to explicitly pull this operation into 
      // scope. GCC isn't finding it.
      using input_iterator_archetype<Arch, I>::operator++;

      Arch operator++(int)
      {
        Arch tmp {self()};
        this->operator++();
        return tmp;
      }
    };



  // Bidirectional iterator (facade)
  template <typename Arch, typename I>
    class bidirectional_iterator_archetype 
      : public forward_iterator_archetype<Arch, I>
    {
      static_assert(Bidirectional_iterator<I>(), "");

      Arch&       self()       { return *static_cast<Arch*>(this); }
      const Arch& self() const { return *static_cast<const Arch*>(this); }

    public:
      Arch& operator--()
      {
        --self().value;
        return self();
      }

      Arch operator--(int)
      {
        Arch tmp {self()};
        this->operator--();
        return tmp;
      }
    };



  // Random access iterator (facade)
  template <typename Arch, typename I>
    class random_access_iterator_archetype 
      : public bidirectional_iterator_archetype<Arch, I>
    {
      static_assert(Random_access_iterator<I>(), "");

      Arch&       self()       { return *static_cast<Arch*>(this); }
      const Arch& self() const { return *static_cast<const Arch*>(this); }

    public:
      Arch& operator+=(Distance_type<I> n)
      {
        self().value += n;
        return self();
      }

      friend Arch operator+(Arch i, Distance_type<I> n)
      {
        i += n;
        return i;
      }

      friend Arch operator+(Distance_type<I> n, Arch i)
      {
        i += n;
        return i;
      }

      Arch& operator-=(Distance_type<I> n)
      {
        self().value -= n;
        return self();
      }

      friend Arch operator-(Arch i, Distance_type<I> n)
      {
        i -= n;
        return i;
      }

      friend Distance_type<I> operator-(const Arch& i, const Arch& j)
      {
        return i.value - j.value;
      }

      Const_iterator_reference<I> operator[](Distance_type<I> n) const
      {
        return self().value[n];
      }
    };



  // Selectors
  // A selector is a small class that helps the archetype class enumerate and
  // inherit the archetypes above.
  //
  // FIXME: I could do away with the refines tuple if I had access to a list
  // of base types. GCC has an extension that allows such queries, but it's
  // very much internal. I should wait until there's a more standard method.



  // Default constructible (archetype selector)
  // The default archetype is a placeholder for default initialization 
  // semantics.
  struct use_default_constructible
  {
    using refines = std::tuple<>;

    template <typename Arch, typename T>
      using impl = default_constructible_archetype<Arch, T>;
  };



  // Move constructible (selector)
  // The default archetype is a placeholder for default initialization 
  // semantics.
  struct use_move_constructible
  {
    using refines = std::tuple<>;

    template <typename Arch, typename T>
      using impl = move_constructible_archetype<Arch, T>;
  };



  // Copy constructible (selector)
  // The default archetype is a placeholder for default initialization 
  // semantics.
  struct use_copy_constructible
  {
    using refines = std::tuple<>;

    template <typename Arch, typename T>
      using impl = copy_constructible_archetype<Arch, T>;
  };



  // Movable (selector)
  // The movable archetype is a placeholder for copy semantics.
  struct use_movable
  { 
    using refines = std::tuple<use_move_constructible>;

    template <typename Arch, typename T>
      using impl = movable_archetype<Arch, T>;
  };



  // Copyable (selector)
  // The copyable archetype is a placeholder for copy semantics
  struct use_copyable
  { 
    using refines = std::tuple<use_movable, use_copy_constructible>;

    template <typename Arch, typename T>
      using impl = copyable_archetype<Arch, T>;
  };

 

 // Equality comparable (selector)
  template <typename U = default_t>
    struct use_equality_comparable
    {
      using refines = std::tuple<>;

      template <typename Arch, typename T>
        using impl = equality_comparable_archetype<Arch, T, U>;
    };

  // FIXME: This should be merged with the primary template, but GCC was
  // giving an ICE when that happened.
  template <>
    struct use_equality_comparable<default_t>
    {
      using refines = std::tuple<>;

      template <typename Arch, typename T>
        using impl = equality_comparable_archetype<Arch, T>;
    };



  // Weakly ordered (selector)
  template <typename U = default_t>
    struct use_weakly_ordered
    {
      using refines = std::tuple<>;

      template <typename Arch, typename T>
        using impl = weakly_ordered_archetype<Arch, T, U>;
    };

  // FIXME: See the comments above about ICEs.
  template <>
    struct use_weakly_ordered<default_t>
    {
      using refines = std::tuple<>;

      template <typename Arch, typename T>
        using impl = weakly_ordered_archetype<Arch, T>;
    };



  // Totally ordered (selector)
  template <typename U = default_t>
    struct use_totally_ordered
    {
      using refines = std::tuple<use_equality_comparable<U>, use_weakly_ordered<U>>;
      
      template <typename Arch, typename T>
        using impl = totally_ordered_archetype<Arch, T, U>;
    };

  // FIXME: See the comment above about ICEs.
  template <>
    struct use_totally_ordered<default_t>
    {
      using refines = 
        std::tuple<use_equality_comparable<>, use_weakly_ordered<>>;
      
      template <typename Arch, typename T>
        using impl = totally_ordered_archetype<Arch, T>;
    };



  // Regular type (selector)
  struct use_regular
  {
    using refines = 
      std::tuple<use_default_constructible, use_copyable, use_equality_comparable<>>;

    template <typename Arch, typename T>
      using impl = regular_archetype<Arch, T>;
  };



  // Function (selector)
  template <typename... Args>
    struct use_function
    {
      using refines = std::tuple<use_copy_constructible>;

      template <typename Arch, typename F>
        using impl = function_archetype<Arch, F, Args...>;
    };



  // Predicate (selector)
  template <typename... Args>
    struct use_predicate
    {
      using refines = std::tuple<use_copy_constructible>;

      template <typename Arch, typename F>
        using impl = predicate_archetype<Arch, F, Args...>;
    };



   // Relation (selector)
  template <typename T, typename U = T>
    struct use_relation
    {
      using refines = std::tuple<use_copy_constructible>;

      template <typename Arch, typename R>
        using impl = relation_archetype<Arch, R, T, U>;
    }; 



  // Input iterator selector
  struct use_input_iterator
  {
    using refines = std::tuple<use_copyable, use_equality_comparable<>>;

    template <typename Arch, typename I>
      using impl = input_iterator_archetype<Arch, I>;
  };



  // Output iterator (selector)
  template <typename T>
    struct use_output_iterator
    {
      using refines = std::tuple<use_copyable>;

      template <typename Arch, typename I>
        using impl = output_iterator_archetype<Arch, I, T>;
    };



  // Forward iterator (selector)
  struct use_forward_iterator
  {
    using refines = std::tuple<use_input_iterator>;

    template <typename Arch, typename I>
      using impl = forward_iterator_archetype<Arch, I>;
  };



  // Bidirectional iterator (selector)
  struct use_bidirectional_iterator
  {
    using refines = std::tuple<use_forward_iterator>;

    template <typename Arch, typename I>
      using impl = bidirectional_iterator_archetype<Arch, I>;
  };



  // Random access iterator (selector)
  struct use_random_access_iterator
  {
    using refines = std::tuple<use_bidirectional_iterator>;

    template <typename Arch, typename I>
      using impl = random_access_iterator_archetype<Arch, I>;
  };



  // Archetype framework
  // The archetype framework is a single class that constructs a single type
  // based on a number of selectors.



  template <typename Arch, typename T> struct inspect_selector;
  template <typename Arch, typename... Args> struct has_selector;

  // Inspect archetype
  // Returns true if true if Arch is contained in the list of T's refinements.
  //
  // FIXME: This is a terrible name...
  template <typename Arch, typename T>
    static constexpr bool Inspect_selector()
    {
      return inspect_selector<Arch, T>::value;
    }


  // Has archetype
  // Returns true if Arch is found in the list of archetypes.
  template <typename Arch, typename... Args>
    static constexpr bool Has_selector()
    {
      return has_selector<Arch, Args...>::value;
    }



  // Search for the selector Sel in the selector list [T, Args...]. Note that we
  // recursively search the set of refinements of each T in the list.
  template <typename Sel, typename T, typename... Args>
    struct has_selector<Sel, T, Args...>
      : bool_constant<
          Same<Sel, T>() 
          ? true 
          : (Inspect_selector<Sel, T>() 
             ? true
             : Has_selector<Sel, Args...>())
        >
    { };

  template <typename Sel>
    struct has_selector<Sel> : std::false_type { };


  // Inspect the list of refinements, rerpresented by T for the the given
  // Archetype. T is intended to be a tuple. This simply delegates the search
  // back to has_selector, in order to flatten the entire search.
  template <typename Sel, typename T>
    struct inspect_refinements : std::false_type { };

  template <typename Sel, typename... Args>
    struct inspect_refinements<Sel, std::tuple<Args...>>
      : has_selector<Sel, Args...>
    { };

  // Search the refinements of the T for the selector Sel.
  template <typename Sel, typename T>
    struct inspect_selector 
      : inspect_refinements<Sel, typename T::refines>
    { };



  // Dummy type
  // A dummy type used as a placholder or to distinguish overloads.
  struct dummy_t { };


  // Archetype
  // The archetype class wraps a given value, and exposes a specified set
  // of interface, indicated by a sequence of selector types.
  //
  // Note that T must be a complete type.
  template <typename T, typename... Sels>
    struct archetype : Sels::template impl<archetype<T, Sels...>, T>...
    {
      using This_type = archetype<T, Sels...>;
      using Value_type = T;

      // The archetype owns the interface-constrained value. Note that the
      // value can be moved into the archetype.
      T value;

      // FIXME: I could make this more effecient by aggregating flags instead
      // of recursing over sequences. Basically, this is deducing the set of
      // properties that will be enforced by the various constructors and
      // assignment operators.
      static constexpr bool Is_default_constructible = 
        Has_selector<use_default_constructible, Sels...>();
      
      static constexpr bool Is_move_constructible = 
        Has_selector<use_move_constructible, Sels...>();
      
      static constexpr bool Is_copy_constructible = 
        Has_selector<use_copy_constructible, Sels...>();

      static constexpr bool Is_movable = Has_selector<use_movable, Sels...>();
      static constexpr bool Is_copyable = Has_selector<use_copyable, Sels...>();

      // Initialization
      // This forwarding constructor allows the underlying value to be 
      // initialized over the given arguments. The dummy argument makes it
      // unlikely that this will be called by any algorithms.
      template <typename... Args>
        explicit archetype(dummy_t, Args&&... args)
          : value(std::forward<Args>(args)...)
        { }

      // Factory
      // This factory provides an interface for creating archetype values that
      // deprecates the need to use the dummy_t argument.
      template <typename... Args>
        static archetype create(Args&&... args)
        {
          return archetype(dummy_t {}, std::forward<Args>(args)...);
        }

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
    };



    // Archetype constructors
    // These aliases make it easier to create basic archetypes.



    // Unconstrained archetype (archetype)
    // The unconstrained archetype creates types that are immovable, 
    // non-copyable, and not default constructible.
    template <typename T>
      using Unconstrained_archetype = archetype<T>;



    // Default constructible (archetype)
    // The default constructible archetype creates types that can only be
    // default constructed. They cannot be moved or copied.
    template <typename T>
      using Default_constructible_archetype = archetype<T, use_default_constructible>;



    // Move constructible (archetype)
    // The move constructible archetype creates types that are only move
    // move constructible.
    template <typename T>
      using Move_constructible_archetype = archetype<T, use_move_constructible>;



    // Copy constructible (archetype)
    // The copy constructible archetype creates types that are move
    // constructible and copy constructible.
    template <typename T>
      using Copy_constructible_archtype = archetype<T, use_copy_constructible>;



    // Movable (archetype)
    // The movable archetpye creates types that are move constructible and 
    // assignable type.
    template <typename T>
      using Movable_archetype = archetype<T, use_movable>;



    // Copyable (archetype)
    // The copyable archetype creates types that are movable, copy 
    //constructible, and copy assignable.
    template <typename T>
      using Copyable_archetype = archetype<T, use_copyable>;



    // FIXME: For the comparison archetypes, it might be useful to accept a
    // sequence of "other" types in order to construct a number of common
    // type relational operators in one shot. The ideal is:
    //
    //    Equality_comparable_archetype<Base, {int, T*}>
    //
    // where Base is the type being wrapped, and int and T* will eventually be
    // cross-type comparable with Base. Note that Base should be comparable
    // to itself since that is a requirement of any cross-type relation.

    template <typename T, typename U = default_t>
      using Equality_comparable_archetype = archetype<T, use_equality_comparable<U>>;

    template <typename T, typename U = default_t>
      using Weakly_ordered_archetype = archetype<T, use_weakly_ordered<U>>;

    template <typename T, typename U = default_t>
      using Totally_ordered_archetype = archetype<T, use_totally_ordered<U>>;

    template <typename T>
      using Regular_archetype = archetype<T, use_regular>;

    // FIXME: As with above, can we construct multiple call profiles with a
    // single using declaration? I would have to wrap different argument sets
    // in a tuple or type list.

    template <typename F, typename... Args>
      using Function_archetype = archetype<F, use_function<Args...>>;

    template <typename P, typename... Args>
      using Predicate_archetype = archetype<P, use_predicate<Args...>>;

    template <typename R, typename T, typename U = default_t>
      using Relation_archetype = archetype<R, use_relation<T, U>>;



    // Input iterator (archetype)
    // The input iterator archetype creates types that satsify the requirements
    // of the input iterator concept.
    template <typename I>
      using Input_iterator_archetype = archetype<I, use_input_iterator>;

    // Returns an archetype constrained input iterator.
    template <typename I>
      Input_iterator_archetype<I> as_input_iterator(I i)
      {
        return Input_iterator_archetype<I>(dummy_t {}, i);
      }



    // Output iterator (archetype)
    // The output iterator archetype creates types that only satisfy the 
    // requirements of the output iterator concept, writing values of type T.
    //
    // FIXME: Write an accessor for creating output iterators.
    template <typename I, typename T>
      using Output_iterator_archetype = archetype<I, use_output_iterator<T>>;

    

    // Forward iterator (archetype)
    // The forward iterator archetype creates types that only satisfy the
    // requirements of the forward iterator concept.
    template <typename I>
      using Forward_iterator_archetype = archetype<I, use_forward_iterator>;

    // Returns an archetype constrained forward iterator.
    template <typename I>
      Forward_iterator_archetype<I> as_forward_iterator(I i)
      {
        return Forward_iterator_archetype<I> {dummy_t {}, i};
      }



    // Bidirectional iterator (archetype)
    // The bidirectional iterator archetype creates types that only satisfy the
    // requirements of the bidirectional iterator concept.
    template <typename I>
      using Bidirectional_iterator_archetype = archetype<I, use_bidirectional_iterator>;

    // Returns an archetype constrained bidirectional iterator.
    template <typename I>
      Bidirectional_iterator_archetype<I> as_bidirectional_iterator(I i)
      {
        return Bidirectional_iterator_archetype<I> { dummy_t {}, i};
      }



    // Random access iterator (archetype)
    // The random access iterator archetype creates types that only satisfy the
    // requirements of the random access iterator concept.
    template <typename I>
      using Random_access_iterator_archetype = archetype<I, use_random_access_iterator>;

    // Returns an archetype constrained random access iterator.
    template <typename I>
      Random_access_iterator_archetype<I> as_random_access_iterator(I i)
      {
        return Random_access_iterator_archetype<I> {dummy_t {}, i};
      }
} // namespace origin
