#pragma once

#include "mgl_opengl/context.hpp"

namespace mgl::window::api
{
  // Map the opengl namespace to the api namespace, eventually we might have a vulkan api
  using context_ref = mgl::opengl::context_ref;
  using texture_ref = mgl::opengl::texture_ref;
  using program_ref = mgl::opengl::program_ref;
  using uniform_ref = mgl::opengl::uniform_ref;
  using buffer_ref = mgl::opengl::buffer_ref;

}; // namespace mgl::window::api