#pragma once

#include "mgl_opengl/context.hpp"

namespace mgl::window
{
  // Map the opengl namespace to the window namespace, eventually we might have a vulkan namespace
  using context_ref = mgl::opengl::context_ref;
  using texture_ref = mgl::opengl::texture_ref;
  using program_ref = mgl::opengl::program_ref;
  using uniform_ref = mgl::opengl::uniform_ref;
  using buffer_ref = mgl::opengl::buffer_ref;
  using vertex_array_ref = mgl::opengl::vertex_array_ref;

}; // namespace mgl::window