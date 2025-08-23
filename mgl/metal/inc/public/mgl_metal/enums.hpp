#pragma once

#include "mgl_core/utils.hpp"

namespace mgl::metal
{
  enum render_mode
  {
    POINTS,
    LINES,
    LINE_STRIP,
    TRIANGLES,
    TRIANGLE_STRIP,
  };

  enum enable_flag
  {
    NOTHING = 0,
    BLEND = BIT(1),
    DEPTH_TEST = BIT(2),
    CULL_FACE = BIT(3),
    STENCIL_TEST = BIT(4),
  };

  enum compare_func
  {
    NEVER = 0,
    LESS = 1,
    EQUAL = 2,
    LEQUAL = 3,
    GREATER = 4,
    NOTEQUAL = 5,
    GEQUAL = 6,
    ALWAYS = 7,
  };

  enum blend_equation_mode
  {
    FUNC_ADD = 0,
    FUNC_SUBTRACT = 1,
    FUNC_REVERSE_SUBTRACT = 2,
    MIN = 3,
    MAX = 4,
  };

  enum blend_factor
  {
    ZERO = 0,
    ONE = 1,
    SRC_COLOR = 2,
    ONE_MINUS_SRC_COLOR = 3,
    SRC_ALPHA = 4,
    ONE_MINUS_SRC_ALPHA = 5,
    DST_ALPHA = 6,
    ONE_MINUS_DST_ALPHA = 7,
    DST_COLOR = 8,
    ONE_MINUS_DST_COLOR = 9,
    SRC_ALPHA_SATURATE = 10,
  };

} // namespace mgl::metal
