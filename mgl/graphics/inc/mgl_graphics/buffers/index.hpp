#pragma once

#include "mgl_graphics/buffer.hpp"

namespace mgl::graphics
{
  class index_buffer : public buffer
  {
public:
    index_buffer(size_t size = 0, bool dynamic = false)
        : buffer(size, dynamic)
    { }

    ~index_buffer() = default;
  };

  using index_buffer_ref = mgl::ref<index_buffer>;
} // namespace mgl::graphics