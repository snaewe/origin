// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011-2012 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#include <vector>

#include <origin/iterator.hpp>

namespace origin
{
  // Predicate sequence
  // A predicate sequence is a sequence of bivalent (two-valued) elements whose
  // values are conveniently called true and false. Note that the underlying
  // storage mechanism is a vector of ints and not a vector of bools (because of
  // the vector<bool> optimization). The data structure also encodes the
  // position of each element.
  class predicate_sequence 
  {
  public:
    using value_list = std::vector<int>;
    using value_type = int;
    using size_type = typename value_list::size_type;

    using iterator = typename value_list::iterator;
    using const_iterator = typename value_list::const_iterator;

    using position_list = std::vector<size_type>;

    value_list data;
    position_list trues;
    position_list falses;

    predicate_sequence() = default;

    predicate_sequence(size_type n, bool value)
    {
      if (value)
        repeat(n, [this]() -> void { append_true(); });
      else
        repeat(n, [this]() -> void { append_false(); });
    }

    auto begin() -> decltype(o_begin(data)) { return o_begin(data); }
    auto end() -> decltype(o_end(data))     { return o_end(data); }

    auto begin() const -> decltype(o_begin(data)) { return o_begin(data); }
    auto end() const -> decltype(o_end(data))     { return o_end(data); }


    bool empty() const { return data.empty(); }
    size_type size() const { return data.size(); }

    // Returns the default predicate, which simply converts an element of
    // this sequence to a boolean value.
    to_bool_function predicate() const { return {}; }


    void push_back(value_type x)
    {
      if (x)
        append_true();
      else
        append_false();
    }

    void append_true() 
    {
      data.push_back(1);
      trues.push_back(data.size() - 1);
    }

    void append_false()
    {
      data.push_back(0);
      falses.push_back(data.size() - 1);
    }
    

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
  // algorithm to catch all the cases we are interested in.   This has to be
  // here because of inclusion dependencies 
  template <typename Size = std::uniform_int_distribution<unsigned int>>
    class predicate_sequence_distribution 
    {
      using Dist = std::bernoulli_distribution;
      using This = predicate_sequence_distribution<Size>;
    public:
      using result_type = predicate_sequence;


      predicate_sequence_distribution(const Size& s = Size {0, 32}, double p = 0.5)
        : size {s}, dist {p}
      { }

      template <typename Eng>
      result_type operator()(Eng& eng)
      {
        result_type seq;
        auto n = size(eng);
        while (n != 0) {
          seq.push_back(dist(eng));
          --n;
        }
        return seq;
      }
      
      // Equality comparable
      bool operator==(const This& x) const { return equal(x); }
      bool operator!=(const This& x) const { return !equal(x); }
      
      // Returns the distribution describing the size.
      const Size& size_distribution() const { return size; }
      
      // Returns the distribution describing the distribution of true and
      // false values.
      const Dist& probability_distribution() const { return dist; }

    private:
      bool equal(const This& x) const
      {
        return size == x.size && dist == x.dist;
      }

    private:
      Size size;
      Dist dist;
    };



  // Default distribution
  // This has to be here because of inclusion dependencies.
  template <>
    struct default_distribution_traits<predicate_sequence>
    {
      using Dist = predicate_sequence_distribution<>;

      static Dist get() { return Dist{}; }
    };

} // namespace origin
