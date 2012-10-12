// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <random>
#include <stdexcept>

#include <origin/math/matrix/matrix.hpp>

using namespace std;
using namespace origin;

using Mat = matrix<double, 2>;
using Vec = matrix<double, 1>;

// Random numbers
default_random_engine eng(time(0));
// default_random_engine eng;
uniform_real_distribution<> dist;
auto rng = bind(ref(dist), ref(eng));



template <typename M1, typename T, typename M2>
  matrix<T, M1::order>
  scale_and_add(const M1& a, const T& c, const M2& b)
  {
    assert(same_extents(a.descriptor(), b.descriptor()));

    matrix<T, M1::order> r(a.descriptor());
    transform(a.begin(), a.end(), b.begin(), r.begin(), [&](T x, T y) { 
      return x * c + y; 
    });
    return r;
  }

double
dot_product(const Vec& a, const Vec& b)
{
  return inner_product(a.begin(), a.end(), b.begin(), 0.0);
}

void
classical_elimination(Mat& A, Vec& b)
{
  const size_t n = A.rows();
  for (size_t j = 0; j < n - 1; ++j) {
    // Pick the pivot. Make sure it's not 0.
    double pivot = A(j, j);
    if (pivot == 0)
      throw runtime_error("elimination error");
    
    for (size_t i = j + 1; i < n; ++i) {
      double m = A(i, j) / pivot;
      A[i](slice(j)) = scale_and_add(A[j](slice(j)), -m, A[i](slice(j)));
      b(i) -= m * b(j);
    }
  }
}

Vec 
back_substitution(const Mat& A, const Vec& b)
{
  const size_t n = A.rows();
  Vec x(n);
  for (size_t i = n - 1; i < n; --i) {
    // FIXME: I should write A(i, slice(i+1)) instead of what's below. That
    // would require me reduce dimensions for non-slice arguments to the
    // subscript operator.
    double s = b(i) - dot_product(A[i](slice(i + 1)), x(slice(i + 1)));

    if (double m = A(i, i))
      x(i) = s / m;
    else
      throw runtime_error("back substitution failure");
  }
  return x;
}

Vec 
classical_guassian_elimination(Mat A, Vec b)
{
  classical_elimination(A, b);
  return back_substitution(A, b);
}

Mat random_matrix(size_t n)
{
  Mat r(n, n);
  generate(r.begin(), r.end(), rng);
  return r;
}

Vec random_vector(size_t n)
{
  Vec r(n);
  generate(r.begin(), r.end(), rng);
  return r;
}

Vec multiply(const Mat& A, const Vec& x)
{
  const size_t n = A.rows();
  Vec r(n);
  for (size_t i = 0; i < n; ++i)
    r(i) = dot_product(A[i], x);
  return r;
}

void solve(size_t n)
{
  Mat A = random_matrix(3);
  Vec b = random_vector(3);

  cout << "A = " << A << '\n';
  cout << "b = " << b << '\n';
  try {
    Vec x = classical_guassian_elimination(A, b);
    cout << "==========\n";
    cout << "x = " << x << '\n';
    cout << "----------\n";
    auto r = multiply(A, x);
    cout << "Ax = b <=> " << r << '\n';

    // TODO: I could assert this, but I'm not going to.

  } catch(const exception& e) {
    cerr << e.what() << '\n';
  }
}

int main()
{
  solve(3);
}
