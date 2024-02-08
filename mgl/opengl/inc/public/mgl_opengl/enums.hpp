#pragma once

#include "mgl_core/utils.hpp"

namespace mgl::opengl
{
  enum render_mode
  {
    POINTS = 0x0000,
    LINES = 0x0001,
    LINE_LOOP = 0x0002,
    LINE_STRIP = 0x0003,
    TRIANGLES = 0x0004,
    TRIANGLE_STRIP = 0x0005,
    TRIANGLE_FAN = 0x0006,
    LINES_ADJACENCY = 0x000A,
    LINE_STRIP_ADJACENCY = 0x000B,
    TRIANGLES_ADJACENCY = 0x000C,
    TRIANGLE_STRIP_ADJACENCY = 0x000D,
    PATCHES = 0x000E,
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
    NONE = 0x0000,
    NEVER = 0x0200,
    LESS = 0x0201,
    EQUAL = 0x0202,
    LESS_EQUAL = 0x0203,
    GREATER = 0x0204,
    NOT_EQUAL = 0x0205,
    GREATER_EQUAL = 0x0206,
    ALWAYS = 0x0207,
  };

  enum blend_equation_mode
  {
    ADD = 0x8006,
    SUBTRACT = 0x800A,
    REVERSE_SUBTRACT = 0x800B,
    MIN = 0x8007,
    MAX = 0x8008,
  };

  enum blend_factor
  {
    ZERO = 0,
    ONE = 1,
    SRC_COLOR = 0x0300,
    ONE_MINUS_SRC_COLOR = 0x0301,
    DST_COLOR = 0x0306,
    ONE_MINUS_DST_COLOR = 0x0307,
    SRC_ALPHA = 0x0302,
    ONE_MINUS_SRC_ALPHA = 0x0303,
    DST_ALPHA = 0x0304,
    ONE_MINUS_DST_ALPHA = 0x0305,
    CONSTANT_COLOR = 0x8001,
    ONE_MINUS_CONSTANT_COLOR = 0x8002,
    CONSTANT_ALPHA = 0x8003,
    ONE_MINUS_CONSTANT_ALPHA = 0x8004,
    SRC_ALPHA_SATURATE = 0x0308,
    SRC1_COLOR = 0x88F9,
    ONE_MINUS_SRC1_COLOR = 0x88FA,
    SRC1_ALPHA = 0x8589,
    ONE_MINUS_SRC1_ALPHA = 0x88FB,
  };

} // namespace  mgl::opengl
