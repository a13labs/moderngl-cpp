#pragma once

#include "buffer.hpp"

#include "mgl_core/containers.hpp"
#include "mgl_core/string.hpp"

namespace mgl::opengl
{
  struct vertex_buffer
  {
    buffer_ref buffer;
    mgl::core::string buffer_layout;
    mgl::core::string_list attributes;
  };

  using vertex_buffer_list = mgl::core::list<vertex_buffer>;
} // namespace mgl::opengl