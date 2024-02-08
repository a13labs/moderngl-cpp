#pragma once

#include "buffer.hpp"
#include "buffer_layout.hpp"

#include "mgl_core/containers.hpp"
#include "mgl_core/string.hpp"

namespace mgl::opengl
{
  struct vertex_buffer
  {
    buffer_ref buffer;
    buffer_layout layout;
    mgl::string_list attributes;
    int32_t vertex_count = 0;

    vertex_buffer(buffer_ref buffer, buffer_layout layout, mgl::string_list attributes)
        : buffer(buffer)
        , layout(layout)
        , attributes(attributes)
        , vertex_count(buffer->size() / layout.size())
    { }

    vertex_buffer(buffer_ref buffer, const std::string& layout, mgl::string_list attributes)
        : buffer(buffer)
        , layout(layout)
        , attributes(attributes)
        , vertex_count(buffer->size() / layout.size())
    { }
  };

  using vertex_buffer_list = mgl::list<vertex_buffer>;
} // namespace mgl::opengl