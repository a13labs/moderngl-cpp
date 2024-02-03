#pragma once

#include "mgl_core/containers.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"
#include "mgl_platform/api/opengl.hpp"
#include "mgl_platform/window.hpp"

namespace mgl::graphics
{

  class buffer
  {
public:
    buffer(size_t size = 0, bool dynamic = false)
        : m_dynamic(dynamic)
        , m_size(size)
        , m_pos(0)
        , m_buffer(nullptr)
    { }

    ~buffer() = default;

    void allocate()
    {
      MGL_CORE_ASSERT(!m_buffer, "Buffer is already allocated");
      m_buffer = mgl::platform::current_context()->buffer(is_dynamic());
      m_buffer->orphan(m_size);
      m_pos = 0;
    }

    void free()
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not allocated");
      m_buffer->release();
      m_buffer = nullptr;
      m_pos = 0;
    }

    void seek(size_t offset)
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      MGL_CORE_ASSERT(offset < m_size, "Offset is out of bounds");
      m_pos = offset;
    }

    void write(const void* data, size_t size)
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      MGL_CORE_ASSERT(m_pos + size <= m_size, "Buffer overflow");
      m_buffer->write(data, size, m_pos);
      m_pos += size;
    }

    void write(float32_buffer& data)
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      MGL_CORE_ASSERT(m_pos + data.size() <= m_size, "Buffer overflow");
      m_buffer->write(data.data(), data.size() * sizeof(float), m_pos);
      m_pos += data.size() * sizeof(float);
    }

    void write(float64_buffer& data)
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      MGL_CORE_ASSERT(m_pos + data.size() <= m_size, "Buffer overflow");
      m_buffer->write(data.data(), data.size() * sizeof(double), m_pos);
      m_pos += data.size() * sizeof(double);
    }

    void write(int32_buffer& data)
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      MGL_CORE_ASSERT(m_pos + data.size() <= m_size, "Buffer overflow");
      m_buffer->write(data.data(), data.size() * sizeof(int32_t), m_pos);
      m_pos += data.size() * sizeof(int32_t);
    }

    void write(uint32_buffer& data)
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      MGL_CORE_ASSERT(m_pos + data.size() <= m_size, "Buffer overflow");
      m_buffer->write(data.data(), data.size() * sizeof(uint32_t), m_pos);
      m_pos += data.size() * sizeof(uint32_t);
    }

    void write(int16_buffer& data)
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      MGL_CORE_ASSERT(m_pos + data.size() <= m_size, "Buffer overflow");
      m_buffer->write(data.data(), data.size() * sizeof(int16_t), m_pos);
      m_pos += data.size() * sizeof(int16_t);
    }

    void write(uint16_buffer& data)
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      MGL_CORE_ASSERT(m_pos + data.size() <= m_size, "Buffer overflow");
      m_buffer->write(data.data(), data.size() * sizeof(uint16_t), m_pos);
      m_pos += data.size() * sizeof(uint16_t);
    }

    void write(int8_buffer& data)
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      MGL_CORE_ASSERT(m_pos + data.size() <= m_size, "Buffer overflow");
      m_buffer->write(data.data(), data.size() * sizeof(int8_t), m_pos);
      m_pos += data.size() * sizeof(int8_t);
    }

    void write(uint8_buffer& data)
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      MGL_CORE_ASSERT(m_pos + data.size() <= m_size, "Buffer overflow");
      m_buffer->write(data.data(), data.size() * sizeof(uint8_t), m_pos);
      m_pos += data.size() * sizeof(uint8_t);
    }

    void upload(const void* data, size_t size)
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->write(data, size);
      m_pos = size;
    }

    void upload(const mgl::float32_buffer& data)
    {
      upload(data.data(), data.size() * sizeof(float));
    }

    void upload(const mgl::float64_buffer& data)
    {
      upload(data.data(), data.size() * sizeof(double));
    }

    void upload(const mgl::int32_buffer& data)
    {
      upload(data.data(), data.size() * sizeof(int32_t));
    }

    void upload(const mgl::uint32_buffer& data)
    {
      upload(data.data(), data.size() * sizeof(uint32_t));
    }

    void upload(const mgl::int16_buffer& data)
    {
      upload(data.data(), data.size() * sizeof(int16_t));
    }

    void upload(const mgl::uint16_buffer& data)
    {
      upload(data.data(), data.size() * sizeof(uint16_t));
    }

    void upload(const mgl::int8_buffer& data) { upload(data.data(), data.size() * sizeof(int8_t)); }

    void upload(const mgl::uint8_buffer& data)
    {
      upload(data.data(), data.size() * sizeof(uint8_t));
    }

    void orphan(size_t size)
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->orphan(size);
      m_pos = 0;
    }

    size_t needle() const { return m_pos; }

    size_t size() const { return m_size; }

    bool is_dynamic() const { return m_dynamic; }

    mgl::platform::api::buffer_ref& api() { return m_buffer; }

private:
    bool m_dynamic;
    size_t m_pos;
    size_t m_size;
    mgl::platform::api::buffer_ref m_buffer;
  };

  using buffer_ref = mgl::ref<buffer>;

} // namespace mgl::graphics