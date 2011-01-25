
============
Origin.Range
============

Iterators are a fundamental abstraction of the C++ Standard and Origin
Libraries. However, programs are most frequently written using pairs of
iterators, denoting the *range* of iterable values. There are two cases where
this is especially true for iteratr-based loops. A typical implementation of an
iterator-based for loop might look something like this::

  Container c = { ... };
  for(typename Container::iterator i = c.begin(); i != c.end(); ++i) {
    typename std::iterator_traits<Container::iterator>::reference x = *i
    // ...
  }

Here, we have a pair of iterators, ``c.begin()`` and ``c.end()`` that denote
a range of objects being iterated over. The kinds of loops are fairly idiomatic,
which is to say that they occur with great frequency. The C++0x range-based
for-loop improves their writeability substantially. The loop above can now be
written as::

  Container c = { ... };
  for(auto& x : c) {
  }

The code shown here is (almost) exactly [#]_ the same loop shown above, but the
iterator component is nowhere to be seen. Instead, this syntax relies on the
type of ``c``, the ``Container`` to model the ``Range`` concept. A ``Range`` is
simply a type that encapsulates a pair of iterators: ``begin`` and ``end``. A
more formal definition of this type is given in `Range Concepts`_.

FIXME: Rearrange the following discussion.

We can build abstractions on top of ``Range``s types in much the same way that
we build abstractions on top of ``Iterators``. For example, if we want to
iterate over the list in reverse, we could write::

  Container c = { ... };
  for(auto& x : rev(c)) {
    // ...
  }

Here, the ``rev`` function constructs a new "reverse" range over the container
c, causing the iteration to proceed in reverse [#]_. Likewise, if we want to
iterator over a pair of containers in tandem (and in reverse), we can use
``zip`` to create a "zipped" range::

  vector<T> c1 = { ... };
  list<U> c2 = { ... };
  for(auto& x : zip(rev(c1, c2))) {
    auto& a = get<0>(x);
    auto& b = get<1>(y);
  }


.. [#] The name ``reverse`` refers the ``std::reverse`` algorithm, which
  performs an "eager" or immedaiate reversal of a range of objects. The ``rev``
  function simply denotes a pair of ``reverse_iterator`` objects.

Range Concepts
--------------
A range is defined by the following concept::

  concept Range<typename R>
    : Regular<R>
  {
    Range(R r)
    {
      IteratorType(R) i = begin(r);
      IteratorType(R) j = end(r);
    }
  };

Note that ``IteratorType(R)`` type accessor associates an iterator type with the
range. ``IteratorType(R)`` is currently implemented as ``range_traits<R>::iterator``.

The ``begin`` function takes a ``Range`` object and returns an iterator. The
iterator may be **past-the-end, but it is not **singular**.

The ``end`` function takes a ``Range`` object and returns a **past-the-end**
iterator into the underlying sequence.

Range Models
------------

Iterator Ranges
~~~~~~~~~~~~~~

Containers
~~~~~~~~~~
All ``Containers`` implicitly models the ``Range`` concept because---they
define ``begin`` and ``end`` member variables. Note, that if a ``Container``
``C`` is const-qualified, then the ``Iterator(C)`` logically yields the
container's constant iterator type.

Arrays
~~~~~~

Adaptors
~~~~~~~~

Range Constructors
-------------------
Range adaptors are created using **range constructors**. The library supports
the following range constructors.

* rev
* zip
* filter
* xform
* stride
* enumerate
* count
