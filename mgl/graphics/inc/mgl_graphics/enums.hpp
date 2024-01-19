#pragma once

#include "mgl_core/utils.hpp"
#include "mgl_opengl/enums.hpp"

namespace mgl::graphics
{
  enum render_mode
  {
    POINTS,
    LINES,
    LINE_LOOP,
    LINE_STRIP,
    TRIANGLES,
    TRIANGLE_STRIP,
    TRIANGLE_FAN,
    LINES_ADJACENCY,
    LINE_STRIP_ADJACENCY,
    TRIANGLES_ADJACENCY,
    TRIANGLE_STRIP_ADJACENCY,
    PATCHES,
  };

  enum enable_flag
  {
    NOTHING = 0,
    BLEND = BIT(1),
    DEPTH_TEST = BIT(2),
    CULL_FACE = BIT(3),
    STENCIL_TEST = BIT(4),
    RASTERIZER_DISCARD = BIT(5),
    PROGRAM_POINT_SIZE = BIT(6),
    INVALID = 0x40000000
  };

  enum compare_func
  {
    NONE,
    NEVER,
    LESS,
    EQUAL,
    LESS_EQUAL,
    GREATER,
    NOT_EQUAL,
    GREATER_EQUAL,
    ALWAYS,
  };

  enum blend_equation_mode
  {
    ADD,
    SUBTRACT,
    REVERSE_SUBTRACT,
    MIN,
    MAX,
  };

  enum blend_factor
  {
    ZERO,
    ONE,
    SRC_COLOR,
    ONE_MINUS_SRC_COLOR,
    DST_COLOR,
    ONE_MINUS_DST_COLOR,
    SRC_ALPHA,
    ONE_MINUS_SRC_ALPHA,
    DST_ALPHA,
    ONE_MINUS_DST_ALPHA,
    CONSTANT_COLOR,
    ONE_MINUS_CONSTANT_COLOR,
    CONSTANT_ALPHA,
    ONE_MINUS_CONSTANT_ALPHA,
    SRC_ALPHA_SATURATE,
    SRC1_COLOR,
    ONE_MINUS_SRC1_COLOR,
    SRC1_ALPHA,
    ONE_MINUS_SRC1_ALPHA,
  };

  mgl::opengl::blend_factor to_api(blend_factor factor);

  mgl::opengl::blend_equation_mode to_api(blend_equation_mode mode);

  mgl::opengl::render_mode to_api(render_mode mode);

  mgl::opengl::compare_func to_api(compare_func func);

} // namespace mgl::graphics