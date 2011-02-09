
= Modeling Concepts
This chapter is dedicated to discussion on modeling concepts: building types
that conform to the syntax and semantics of a concept.

Protocols and Facades are adaptive patterns used to facilitate the modeling of
concepts. A *protocol* specifies a set of member functions required by a type in
order to be automatically adapted as a model. A *facade* is a base class
template that provides a model's required semantics via a specified set of
member functions in the derived class. Note that a facade implies the use of a
protocol, but requires a class to implement the required syntax and semantics.

The use of protocols and facades to model concepts is a central theme of the
Origin C++0x libraries. Concepts often require sets of related methods that
derived from a distilled set of basic operations. By using these techniques to
define models, we hope reduce the amount of redundant code, which should make
the resulting source code easier to read and maintain. At least, that is the
hope.

In this section, we provide examples of the different protocols and facades
supported by the Origin C++0x Libraries.

== Basic Protocols
The following is a list of protocols supported by the Origin C++0x libraries.

=== Equality Protocol

The equality protocol supports the modeling of the `Equality_Comparable`
concept. There are two "facets" of equality comparability that are considered:
comparisons made on objects of the same type and comparisons made on objects
of different type. Implementing these protocols will implicitly cause the
resulting type to model the `Equality_Comparable` protocol.

Here, we describe the protocol as a concept over two types. The requirements
are specialized for the case where the types are the same.

  concept<typename T, typename U = T>
  Equality_Protocol
  {
    requires(T x, U y)
    { Boolean b = T::equal(x, y); }
  };

  template<typename T>
  concept Equality_Protocol<T, T>
  {
    requires(T x, T y)
    { Boolean b = x.equal(y); }
  };

The first (more general) concept expresses requirements when the types are
different. Here, the first type `T` must define a static member function
`equal` taking two parameters: an object of type `T` and an object of another
type. The result type of the expression is required to model the `Boolean`
concept. Any type `T` modeling `Equality_Protocol<T, U>` for some other
type `U` will have the following as valid expressions::

  x == y
  y == x
  x != y
  y != x

Where `x` and `y` are of type `T` and `U`, respectively. Note that equivalence
relations (here, equality) are required to be symmetric. Because of this, the
type `T` need only provide the single static `equal(x, y)` method in order to
support both `x == y` and `y == x`. This will not be the case for the ordered
protocol.

The specialization over one type, which is by far the more common case, require
the type to define a (non-static, constant) member function `equal` that takes
a single argument `x` of type `T`. Again, the result type is required to be
`Boolean`. Any type `T` that models `Equality_Protocol<T>` will implicitly
provide the following valid expressions::

  x == y
  x != y

Where `x` and `y` are of type `T`.

For example, consider the use of the ordered protocol to implement equality
comparisons for the `optional` class::

  template<typename T>
  class optional {
    ...
    bool equal(optional const& x) const
    {
      if(init_ == x.init_)
        return init_ ? *ptr() == x.*ptr() : true;
      else
        return false;
    }

    static bool equal(optional const& x, T const& y)
    { return x.init_ && *x.ptr() == y; }

    static bool equal(optional const& x, nullptr_t)
    { return !x.int_; }
  };

Here, the `optional` class defines three modes of equality comparison.
Specifically, this class models the following protocol concepts::

  Equivalence_Protocol<optional<T>>
  Equivalence_Protocol<optional<T>, T>
  Equivalence_Protocol<optional<T>, nullptr>

=== Order Protocol
Blah blah blah... Same with ordering.

::

  concept<typename T, typename U = T>
  Order_Protocol
  {
    requires(T x, U y)
    {
      Boolean b = T::less(x, y);
      Boolean b = T::less(y, x);
    }
  };

Note that because the the `less` function denotes an asymmetric relation, the
type must expose both overloads.

::

  template<typename T>
  concept Order_Protocol<T, T>
  {
    requires(T x, T y)
    { Boolean b = x.less(y); }
  };


