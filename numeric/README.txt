
This library will implement BLAS levels 1, 2, and 3.

The library will have three major components: data structures, operations and
algorithms. The data structures component supports the basic types required by
a BLAS system, including static and dynamically bound vectors and matrices.
We also support a number of sparse matrices. The operations component defines
the set of basic operations required by applications using linear algebra
library. The algorithms component will include more advanced features such as
matrix decompositions, solvers, etc.


The design of this library more or less follows the Boost.uBlas library, except
that we will support both "regular" operations and expression templates. Regular
operations might be used to support debugging, whereas the expression template
system might be used for release.
