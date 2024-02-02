#pragma once

#include "mgl_core/containers.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"
#include "mgl_window/api/opengl.hpp"
#include "mgl_window/window.hpp"

namespace mgl::graphics
{

  class buffer
  {
public:
    buffer(size_t size = 0, bool dynamic = false)
        : m_dynamic(dynamic)
        , m_size(size)
        , m_buffer(nullptr)
    { }

    ~buffer() = default;

    void allocate()
    {
      MGL_CORE_ASSERT(!m_buffer, "Buffer is already allocated");
      m_buffer = mgl::window::current_context()->buffer(is_dynamic());
      m_buffer->orphan(m_size);
    }

    void free()
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not allocated");
      m_buffer->release();
      m_buffer = nullptr;
    }

    void upload(const void* data, size_t size)
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->write(data, size);
    }

    void upload(const mgl::float32_buffer& data)
    {
      upload(data.data(), data.size() * sizeof(float));
    }

    void upload(const mgl::uint32_buffer& data)
    {
      upload(data.data(), data.size() * sizeof(uint32_t));
    }

    void upload(const mgl::buffer<uint16_t>& data)
    {
      upload(data.data(), data.size() * sizeof(uint16_t));
    }

    void upload(const mgl::uint8_buffer& data)
    {
      upload(data.data(), data.size() * sizeof(uint8_t));
    }

    void orphan(size_t size)
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->orphan(size);
    }

    size_t size() const { return m_size; }

    bool is_dynamic() const { return m_dynamic; }

    mgl::window::api::buffer_ref& api() { return m_buffer; }

private:
    bool m_dynamic;
    size_t m_size;
    mgl::window::api::buffer_ref m_buffer;
  };

  using buffer_ref = mgl::ref<buffer>;

} // namespace mgl::graphics