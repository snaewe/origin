// Copyright (c) 2008-2010 Kent State University
// Copyright (c) 2011 Texas A&M University
//
// This file is distributed under the MIT License. See the accompanying file
// LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
// and conditions.

#ifndef ORIGIN_GRAPH_COLOR_HPP
#define ORIGIN_GRAPH_COLOR_HPP

namespace origin
{
  // A Color (in the conceptual sense) is essentially an integral type and a
  // set of named values (as below). For example, bool can be associated with
  // a two-color.

  /**
   * A set of color values used by graph algorithms.
   */
  enum basic_color_t {
    white_color,
    black_color,
    gray_color,
    red_color,
    blue_color,
    green_color,
    cyan_color,
    magenta_color,
    yellow_color
  };
    
  /**
   * The color group template associates a set of names (actually static 
   * functions) with an integral value type. Note that the color template is 
   * not inented to be used as a value: just a binding between a value type
   * and some number of named values. Note that not all 
   */
  template<typename T = basic_color_t> struct color_traits;
  
  // The default coloring provides names for 9 different colors: black, white,
  // gray, and the two sets of primary colors (RBG and CMY).
  template<>
  struct color_traits<basic_color_t>
  {
    typedef basic_color_t value_type;
    static constexpr std::size_t num_colors = 9;

    static constexpr basic_color_t white() { return white_color; }
    static constexpr basic_color_t black() { return black_color; }
    static constexpr basic_color_t gray() { return gray_color; }
    static constexpr basic_color_t red() { return red_color; }
    static constexpr basic_color_t blue() { return blue_color; }
    static constexpr basic_color_t green() { return green_color; }
    static constexpr basic_color_t cyan() { return cyan_color; }
    static constexpr basic_color_t magenta() { return magenta_color; }
    static constexpr basic_color_t yellow() { return yellow_color; }
  };
  
  // Boolean types have two named colors: black and white.
  template<>
  struct color_traits<bool>
  {
    typedef bool value_type;
    static constexpr std::size_t num_colors = 2;

    static constexpr bool white() { return true; }
    static constexpr bool black() { return false; }
  };
  
  /**
   * Common operations for two-color systems, specifically the ability to
   * access an inverse color.
   */
  template<typename T>
  struct two_color_traits
  {
    typedef color_traits<T> colors;

    static constexpr T inv(T c)
    { return c == colors::white() ? colors::black() : colors::white(); }
  };

} // namespace origin

#endif
