/*
   Copyright 2022 Alexandre Pires (c.alexandre.pires@gmail.com)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
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
    RASTERIZER_DISCARD = BIT(4),
    PROGRAM_POINT_SIZE = BIT(5),
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

} // namespace  mgl::opengl
