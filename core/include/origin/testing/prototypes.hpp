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
  //
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
    using size_type = Size_type<value_list>;
    using position_list = std::vector<Size_type<value_list>>;

    value_list data;
    position_list trues;
    position_list falses;


    // The predicate function simply evaluates the integer value as either
    // true or false.
    struct predicate 
    {
      bool operator()(int v) {return v; } 
    };


    predicate_sequence(unsigned int size) : data(size, false) {};

    auto begin() -> decltype(o_begin(data)) { return o_begin(data); }
    auto end() -> decltype(o_end(data))     { return o_end(data); }

    auto begin() const -> decltype(o_begin(data)) { return o_begin(data); }
    auto end() const -> decltype(o_end(data))     { return o_end(data); }


    bool empty() const { return data.empty(); }
    size_type size() const { return data.size(); }


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
    

    // Return the list of true or false positions.
    const position_list& true_positions() const  { return trues; }    
    const position_list& false_positions() const { return falses; }
  };


  // Random bounded range with predicate distribution
  //
  // This distribution creates a prototype. It passes the knowledge about the
  // generated data to the prototype for verification purposes. This class is
  // similar to sequence distribution, but we need to inline the generate
  // algorithm to catch all the cases we are interested in.   This has to be
  // here because of inclusion dependencies 
  template <typename Size = std::uniform_int_distribution<unsigned int>>
  class predicate_sequence_distribution 
  {
    using Dist = std::bernoulli_distribution;
  public:
    using this_type = predicate_sequence_distribution<Size>;
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
      }
      return seq;
    }
    
    // Equality comparable
    bool operator==(const this_type& x) const { return equal(x); }
    bool operator!=(const this_type& x) const { return !equal(x); }
    
    // Returns the distribution describing the size.
    const Size& size_distribution() const { return size; }
    
    // Returns the distribution describing the distribution of true and
    // false values.
    const Dist& probability_distribution() const { return dist; }

  private:
    bool equal(const this_type& x) const
    {
      return size == x.size && dist == x.dist;
    }

  private:
    Size size;
    Dist dist;
  };

  // Default distribution (bounded_range_with_predicate)
  // This has to be here because of inclusion dependencies.
  template <>
  struct default_distribution_traits<predicate_sequence> {
    using Dist = predicate_sequence_distribution<>;

    static Dist get() { return Dist{}; }
  };

} // namespace origin
