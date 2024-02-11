#pragma once

#include "mgl_platform/api/buffer.hpp"

namespace mgl::platform::api
{
  class vertex_buffer : public buffer
  {
public:
    vertex_buffer(const std::string& layout, size_t size = 0, bool dynamic = false)
        : buffer(size, dynamic)
        , m_layout(layout)
    { }

    virtual ~vertex_buffer() = default;

    const std::string& layout() const { return m_layout; }

protected:
    std::string m_layout;
  };

  using vertex_buffer_ref = mgl::ref<vertex_buffer>;
} // namespace mgl::platform::api