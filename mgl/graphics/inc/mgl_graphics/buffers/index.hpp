#pragma once

#include "mgl_graphics/buffer.hpp"

namespace mgl::graphics
{
  class index_buffer : public buffer
  {
public:
    index_buffer(size_t size = 0, uint16_t element_size = 4, bool dynamic = false)
        : buffer(size, dynamic)
        , m_element_size(element_size)
    { }

    index_buffer(const index_buffer&) = delete;

    ~index_buffer() = default;

    uint16_t element_size() const { return m_element_size; }

private:
    uint16_t m_element_size;
  };

  using index_buffer_ref = mgl::ref<index_buffer>;
} // namespace mgl::graphics