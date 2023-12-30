#pragma once

#include "mgl_opengl/context.hpp"

namespace mgl::window
{
  // Map the opengl namespace to the window namespace, eventually we might have a vulkan namespace
  using context = mgl::opengl::context_ref;
  using texture = mgl::opengl::texture_ref;
  using program = mgl::opengl::program_ref;
  using buffer = mgl::opengl::buffer_ref;
  using vertex_array = mgl::opengl::vertex_array_ref;

}; // namespace mgl::window