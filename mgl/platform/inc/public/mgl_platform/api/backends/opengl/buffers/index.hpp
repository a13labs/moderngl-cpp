#pragma once

#include "mgl_platform/api/buffers/index.hpp"

#include "mgl_opengl/buffer.hpp"

namespace mgl::platform::api::backends::opengl
{
  class index_buffer : public mgl::platform::api::index_buffer
  {
public:
    index_buffer(size_t size = 0, uint16_t element_size = 4, bool dynamic = false)
        : mgl::platform::api::index_buffer(size, element_size, dynamic)
        , m_pos(0)
        , m_buffer(nullptr)
    { }

    virtual ~index_buffer() = default;

    virtual void allocate() override final
    {
      MGL_CORE_ASSERT(!m_buffer, "Buffer is already allocated");
      auto& ctx = opengl_api::current_context();
      m_buffer = ctx->buffer(m_size, m_dynamic);
      m_buffer->orphan(m_size);
      m_pos = 0;
    }

    virtual void free() override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not allocated");
      m_buffer->release();
      m_buffer = nullptr;
      m_pos = 0;
    }

    virtual void seek(size_t offset) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      MGL_CORE_ASSERT(offset < m_size, "Offset is out of bounds");
      m_pos = offset;
    }

    virtual void write(const void* data, size_t size) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      MGL_CORE_ASSERT(m_pos + size <= m_size, "Buffer overflow");
      m_buffer->write(data, size, m_pos);
      m_pos += size;
    }

    virtual void write(float32_buffer& data) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      MGL_CORE_ASSERT(m_pos + data.size() <= m_size, "Buffer overflow");
      m_buffer->write(data.data(), data.size() * sizeof(float), m_pos);
      m_pos += data.size() * sizeof(float);
    }

    virtual void write(float64_buffer& data) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      MGL_CORE_ASSERT(m_pos + data.size() <= m_size, "Buffer overflow");
      m_buffer->write(data.data(), data.size() * sizeof(double), m_pos);
      m_pos += data.size() * sizeof(double);
    }

    virtual void write(int32_buffer& data) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      MGL_CORE_ASSERT(m_pos + data.size() <= m_size, "Buffer overflow");
      m_buffer->write(data.data(), data.size() * sizeof(int32_t), m_pos);
      m_pos += data.size() * sizeof(int32_t);
    }

    virtual void write(uint32_buffer& data) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      MGL_CORE_ASSERT(m_pos + data.size() <= m_size, "Buffer overflow");
      m_buffer->write(data.data(), data.size() * sizeof(uint32_t), m_pos);
      m_pos += data.size() * sizeof(uint32_t);
    }

    virtual void write(int16_buffer& data) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      MGL_CORE_ASSERT(m_pos + data.size() <= m_size, "Buffer overflow");
      m_buffer->write(data.data(), data.size() * sizeof(int16_t), m_pos);
      m_pos += data.size() * sizeof(int16_t);
    }

    virtual void write(uint16_buffer& data) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      MGL_CORE_ASSERT(m_pos + data.size() <= m_size, "Buffer overflow");
      m_buffer->write(data.data(), data.size() * sizeof(uint16_t), m_pos);
      m_pos += data.size() * sizeof(uint16_t);
    }

    virtual void write(int8_buffer& data) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      MGL_CORE_ASSERT(m_pos + data.size() <= m_size, "Buffer overflow");
      m_buffer->write(data.data(), data.size() * sizeof(int8_t), m_pos);
      m_pos += data.size() * sizeof(int8_t);
    }

    virtual void write(uint8_buffer& data) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      MGL_CORE_ASSERT(m_pos + data.size() <= m_size, "Buffer overflow");
      m_buffer->write(data.data(), data.size() * sizeof(uint8_t), m_pos);
      m_pos += data.size() * sizeof(uint8_t);
    }

    virtual void upload(const void* data, size_t size) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->write(data, size);
      m_pos = size;
    }

    virtual void upload(const mgl::float32_buffer& data) override final
    {
      upload(data.data(), data.size() * sizeof(float));
    }

    virtual void upload(const mgl::float64_buffer& data) override final
    {
      upload(data.data(), data.size() * sizeof(double));
    }

    virtual void upload(const mgl::int32_buffer& data) override final
    {
      upload(data.data(), data.size() * sizeof(int32_t));
    }

    virtual void upload(const mgl::uint32_buffer& data) override final
    {
      upload(data.data(), data.size() * sizeof(uint32_t));
    }

    virtual void upload(const mgl::int16_buffer& data) override final
    {
      upload(data.data(), data.size() * sizeof(int16_t));
    }

    virtual void upload(const mgl::uint16_buffer& data) override final
    {
      upload(data.data(), data.size() * sizeof(uint16_t));
    }

    virtual void upload(const mgl::int8_buffer& data) override final
    {
      upload(data.data(), data.size() * sizeof(int8_t));
    }

    virtual void upload(const mgl::uint8_buffer& data) override final
    {
      upload(data.data(), data.size() * sizeof(uint8_t));
    }

    virtual void orphan(size_t size) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->orphan(size);
      m_size = size;
      m_pos = 0;
    }

    virtual size_t needle() const override final { return m_pos; }

    mgl::opengl::buffer_ref& native() { return m_buffer; }

private:
    size_t m_pos;
    mgl::opengl::buffer_ref m_buffer;
  };
} // namespace mgl::platform::api::backends::opengl
