#pragma once

#include "mgl_opengl/enums.hpp"

namespace mgl::game::opengl
{
  enum render_mode
  {
    POINTS = mgl::opengl::render_mode::POINTS,
    LINES = mgl::opengl::render_mode::LINES,
    LINE_LOOP = mgl::opengl::render_mode::LINE_LOOP,
    LINE_STRIP = mgl::opengl::render_mode::LINE_STRIP,
    TRIANGLES = mgl::opengl::render_mode::TRIANGLES,
    TRIANGLE_STRIP = mgl::opengl::render_mode::TRIANGLE_STRIP,
    TRIANGLE_FAN = mgl::opengl::render_mode::TRIANGLE_FAN,
    LINES_ADJACENCY = mgl::opengl::render_mode::LINES_ADJACENCY,
    LINE_STRIP_ADJACENCY = mgl::opengl::render_mode::LINE_STRIP_ADJACENCY,
    TRIANGLES_ADJACENCY = mgl::opengl::render_mode::TRIANGLES_ADJACENCY,
    TRIANGLE_STRIP_ADJACENCY = mgl::opengl::render_mode::TRIANGLE_STRIP_ADJACENCY,
    PATCHES = mgl::opengl::render_mode::PATCHES,

    NONE
  };
}