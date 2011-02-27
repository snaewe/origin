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
  // FIXME: It's a really bad idea to define colors so concretely. This will
  // colide with other libraries that define more concrete notions of color.
  // Also, it's possible to use boolean values to denote a black/white coloring.
  //
  // In short, there's a Color concept, and this type is only one very specific
  // model.
  //
  // For now, it's good enough.
  //
  // Consider renaming to graph_color_t to avoid collisions.

  /**
   * Color values used by graph algorithms.
   */
  enum color_t {
    white,
    black,
    gray,
    red,
    blue,
    green
  };

} // namespace origin

#endif
