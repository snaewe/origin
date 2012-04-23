// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <vector>

#include <origin/iterator.hpp>
#include <origin/iterator/transform_iterator.hpp>

namespace origin
{
  // Predicate sequence
  // A predicate sequence is a sequence of values and a predicate that knows
  // the positions of the elements that satisfy the predicate and those that
  // do not. The type is parameterized over the value type and the predicate
  // function, allowing greater flexibility.
  template <typename T = bool, typename Pred = identity_function>
    class predicate_sequence 
    {
    public:
      using value_list = std::vector<T>;
      using value_type = T;
      using size_type = typename value_list::size_type;

      using iterator = typename value_list::iterator;
      using const_iterator = typename value_list::const_iterator;

      using position_list = std::vector<size_type>;

      using predicate = Pred;

      // Data members
      value_list data;
      position_list trues;
      position_list falses;
      predicate pred;


      // Default constructible
      predicate_sequence(Pred pred = {}) : pred(pred) { }

      // Fill constructible
      predicate_sequence(size_type n, const T& value, Pred pred = {})
      {
        repeat(n, [this, &value]() -> void { push_back(value); });
      }

      auto begin() -> decltype(o_begin(data)) { return o_begin(data); }
      auto end() -> decltype(o_end(data))     { return o_end(data); }

      auto begin() const -> decltype(o_begin(data)) { return o_begin(data); }
      auto end() const -> decltype(o_end(data))     { return o_end(data); }

      bool empty() const { return data.empty(); }
      size_type size() const { return data.size(); }

      predicate predicate_func() const { return pred; }

      void push_back(const T& value)
      {
        if (pred(value))
          append_true(value);
        else
          append_false(value);
      }

    private:
      void append_true(const T& value) 
      {
        data.push_back(value);
        trues.push_back(data.size() - 1);
      }

      void append_false(const T& value)
      {
        data.push_back(value);
        falses.push_back(data.size() - 1);
      }
    public:
      
      // Return the positions of true elements.
      const position_list& true_positions() const  { return trues; }    
      
      // Return information about the number of true elements.
      bool none_true() const     { return trues.empty(); }
      bool all_true() const      { return trues.size() == data.size(); }
      size_type num_true() const { return trues.size(); }

      // Returns an iterator to the first true element.
      iterator       first_true()       { return data.begin() + trues.front(); }
      const_iterator first_true() const { return data.begin() + trues.front(); }
      
      // Returns the iterator the last true element.
      iterator       last_true()       { return data.begin() + trues.back(); }
      const_iterator last_true() const { return data.begin() + trues.back(); }

      iterator       nth_true(size_type n)       { return data.begin() + trues[n]; }
      const_iterator nth_true(size_type n) const { return data.begin() + trues[n]; }


      // Return the positions of false elements.
      const position_list& false_positions() const  { return falses; }    
      
      // Return information about the number of false elements.
      bool none_false() const     { return falses.empty(); }
      bool all_false() const      { return falses.size() == data.size(); }
      size_type num_false() const { return falses.size(); }

      // Returns an iterator to the first false element.
      iterator       first_false()       { return data.begin() + falses.front(); }
      const_iterator first_false() const { return data.begin() + falses.front(); }
      
      // Returns the iterator the last false element.
      iterator       last_false()       { return data.begin() + falses.back(); }
      const_iterator last_false() const { return data.begin() + falses.back(); }

      iterator       nth_false(size_type n)       { return data.begin() + falses[n]; }
      const_iterator nth_false(size_type n) const { return data.begin() + falses[n]; }
    };



  // Predicate sequence distribution (generator)
  // This distribution creates a prototype. It passes the knowledge about the
  // generated data to the prototype for verification purposes. This class is
  // similar to sequence distribution, but we need to inline the generate
  // algorithm to catch all the cases we are interested in. This has to be
  // here because of inclusion dependencies 
  template <typename T = bool,
            typename Pred = identity_function,
            typename Size = std::uniform_int_distribution<unsigned int>,
            typename Value = Default_distribution_type<T>>
    class predicate_sequence_distribution 
    {
      using This = predicate_sequence_distribution<T, Pred, Size, Value>;
    public:
      using result_type = predicate_sequence<T, Pred>;

      predicate_sequence_distribution(const Size& s = Size {0, 32})
        : size {s}
      { }

      predicate_sequence_distribution(const Size& s, const Value& value)
        : size {s}, value {value}
      { }

      template <typename Eng>
        result_type operator()(Eng& eng)
        {
          result_type seq;
          auto n = size(eng);
          while (n != 0) {
            seq.push_back(value(eng));
            --n;
          }
          return seq;
        }
      
      // Equality comparable
      bool operator==(const This& x) const { return equal(x); }
      bool operator!=(const This& x) const { return !equal(x); }
      
      // Returns the distribution describing the size.
      const Size& size_dist() const { return size; }
      
      // Returns the distribution describing the distribution of true and
      // false values.
      const Value& value_dist() const { return value; }

    private:
      bool equal(const This& x) const
      {
        return size == x.size && value == x.value;
      }

    private:
      Size size;
      Value value;
    };



  // Default distribution
  // This has to be here because of inclusion dependencies.
  template <typename T, typename Pred>
    struct default_distribution_traits<predicate_sequence<T, Pred>>
    {
      using Dist = predicate_sequence_distribution<T, Pred>;

      static Dist get() { return Dist{}; }
    };



  // Indexed value
  // An indexed value is pair comprising an object and its position in a
  // sequence.
  template <typename T>
    class indexed_value
    {
    private:
      using value_type = T;
      using size_type = std::size_t;
    public:

      // The projector value function returns a reference to the indexed
      // value of this pair.
      struct project_value
      {
        T&       operator()(indexed_value& x) const       { return x.value; }
        const T& operator()(const indexed_value& x) const { return x.value; }
      };


      template <typename U>
        explicit indexed_value(size_type n, U&& u)
          : value(std::forward<U>(u)), index(n)
        { }

      // Equality comparable
      bool operator==(const indexed_value& x) const { return value == x.value; }
      bool operator!=(const indexed_value& x) const { return value != x.value; }

      // Totally ordered
      bool operator<(const indexed_value& x) const { return value < x.value; }
      bool operator>(const indexed_value& x) const { return value > x.value; }
      bool operator<=(const indexed_value& x) const { return value <= x.value; }
      bool operator>=(const indexed_value& x) const { return value >= x.value; }

      value_type value;
      size_type index;
    };

  // Output streamable
  template <typename Char, typename Traits, typename T>
    std::basic_ostream<Char, Traits>& 
    operator<<(std::basic_ostream<Char, Traits>& os, const indexed_value<T>& x)
    {
      return os << '(' << x.index << ", " << x.value << ')';
    }



  // Stable sequence
  // The stable sequence prototype is used to observe the property of algorithms
  // that are required to have a stability property.
  //
  // TODO: In order to determine if stability holds -- probably under some
  // ordering relation -- we have to find all equivalence classes in the 
  // sequence and check that, for each subsequence, the indexese are non-
  // decreasing.
  template <typename T>
    class stable_sequence
    {
    public:
      using value_type = indexed_value<T>;
      using sequence_type = std::vector<value_type>;

      using iterator = typename sequence_type::iterator;
      using const_iterator = typename sequence_type::const_iterator;


      // Insert the value into the sequence, appending its index.
      template <typename U>
        void push_back(U&& value)
        {
          value_type x(seq.size(), std::forward<U>(value));
          seq.push_back(std::move(x));;
        }


      // Iterators
      iterator begin() { return seq.begin(); }
      iterator end() { return seq.end(); }

      const_iterator begin() const { return seq.begin(); }
      const_iterator end() const { return seq.end(); }

    private:
      sequence_type seq;
    };

} // namespace origin
