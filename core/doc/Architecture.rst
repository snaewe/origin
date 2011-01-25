
============
Architecture
============
The Origin C++0x Libraries are a federated collection of loosely coupled
software libraries called components. Each component of the Origin libraries is
independently versioned from the others. The Origin Distribution is a collection
of stable Origin components.

Any Origin component can be packaged and redistributed independently of the
Origin Distribution.

Versioning
==========
This allows, for example, the Origin.Core library to be released as version 1.2,
while the Origin.Graph library is released at version 1.0.

The Origin Distribution has a version number that corresponds to its release.
When distributed, the Origin Distribution version number supersedes that of
its components. This is to say that distributions only see Origin v1.0, not
that the Origin.Core is version 1.0.5.

Note---There can be some potential problems here... If a bugfix release of a
Distribution component is released, it should (could) trigger bugfix releases
of its dependents *and* the Distribution itself. This probably isn't too
serious of a problem.

Dependencies
============
Every Origin component declares a set of dependencies on other Origin components
or external libraries. These dependencies include the component name and the
version number upon which the library depends.

The Origin Distribution is implicitly depends on the transitive closure of all
of its components. In other words, a component in the Distribution cannot
depend on an Origin component that is not in the Distribution.

Dependencies on external libraries must also be declared using standard
mechanisms. The Origin Distribution depends on all of the external dependencies
of its components.

Component Structure
===================
Each Origin component has the following directory layout:

$COMPONENT/
  include/origin/$COMPONENT
  src/
  test/
  doc/

Note components may (but are not required to) have binary components (i.e.,
shared programs).

Headers
-------
The include directory contains the header files for the component. The
header files must reside in a subdirectory of the origin directory named
$COMPONENT. Small libraries may include a top-level header that includes all
of the component-specific headers.

Source
------
If the library has a binary component (i.e., a shared or static library), the
compiled source code is placed in this directory.

Testing
-------
The test suite for the component must be in the test directory.

FIXME: Stabilize the test suite documentation. Talk with Valentin about this
stuff. Maybe he has some good suggestions for building test suites.

Documentation
-------------
The documentation for the component must be found in the doc directory.

FIXME: What are the documentation components? Overview, Tutorials, References,
FAQ, etc.

FIXME: What is the preferred documentation format? I really like wiki-style
documentation, but we're going to need a pretty hefty toolchan to weave all of
the written documentation together with the reference documehtation.

Others
------
It's possible that we want to include additional files or programs (tools).
Samples, examples, tutorials, etc. These could be additional directories.

Source Tree
===========
The Source Tree contains the source code, documentation, etc. for all reviewed
Origin components. The root directory of the source tree is called the Root.
Each component is self-contained in its own subdirectory just under the Root.

There is (at least) one special subdirectory directly under root: cmake. The
cmake component implements the CMake-based build system for all Origin
components and can be used to support Origin-style projects outside the Origin
Source Tree. All that's required of external projects is to add this module
to the CMake path and include the Origin module.

Distribution
============
... What goes in the distribution?