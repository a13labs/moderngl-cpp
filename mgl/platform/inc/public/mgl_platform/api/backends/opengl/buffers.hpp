#pragma once

#include "mgl_core/debug.hpp"
#include "mgl_core/memory.hpp"

#include "mgl_opengl/buffer.hpp"
#include "mgl_opengl/buffer_layout.hpp"

namespace mgl::platform::api::backends::opengl
{
  template <typename T>
  class buffer : public T
  {
public:
    buffer(size_t size, bool dynamic)
        : m_buffer(nullptr)
        , m_req_size(size)
        , m_req_dynamic(dynamic)
    { }

    virtual ~buffer() = default;

    virtual void allocate() override final
    {
      MGL_CORE_ASSERT(!m_buffer, "Buffer is already allocated");
      auto& ctx = opengl_api::current_context();
      m_buffer = ctx->buffer(m_req_size, m_req_dynamic);
      m_buffer->orphan(m_req_size);
    }

    virtual void free() override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not allocated");
      m_buffer->release();
      m_buffer = nullptr;
    }

    virtual void seek(size_t offset) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->seek(offset);
    }

    virtual void write(const void* data, size_t size) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->write(data, size);
    }

    virtual void write(uint8_buffer& data) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->write(data);
    }

    virtual void write(uint16_buffer& data) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->write(data);
    }

    virtual void write(uint32_buffer& data) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->write(data);
    }

    virtual void write(uint64_buffer& data) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->write(data);
    }

    virtual void write(int8_buffer& data) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->write(data);
    }

    virtual void write(int16_buffer& data) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->write(data);
    }

    virtual void write(int32_buffer& data) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->write(data);
    }

    virtual void write(int64_buffer& data) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->write(data);
    }

    virtual void write(float32_buffer& data) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->write(data);
    }

    virtual void write(float64_buffer& data) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->write(data);
    }

    virtual void upload(const void* data, size_t size) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->upload(data, size);
    }

    virtual void upload(const mgl::uint8_buffer& data) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->upload(data);
    }

    virtual void upload(const mgl::uint16_buffer& data) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->upload(data);
    }

    virtual void upload(const mgl::uint32_buffer& data) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->upload(data);
    }

    virtual void upload(const mgl::uint64_buffer& data) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->upload(data);
    }

    virtual void upload(const mgl::int8_buffer& data) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->upload(data);
    }

    virtual void upload(const mgl::int16_buffer& data) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->upload(data);
    }

    virtual void upload(const mgl::int32_buffer& data) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->upload(data);
    }

    virtual void upload(const mgl::int64_buffer& data) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->upload(data);
    }

    virtual void upload(const mgl::float32_buffer& data) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->upload(data);
    }

    virtual void upload(const mgl::float64_buffer& data) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->upload(data);
    }

    virtual void orphan(size_t size) override final
    {
      MGL_CORE_ASSERT(m_buffer, "Buffer is not initialized");
      m_buffer->orphan(size);
    }

    virtual size_t needle() const override final { return m_buffer->needle(); }

    virtual size_t size() const override final { return m_buffer->size(); }

    virtual bool is_dynamic() const override final { return m_buffer->dynamic(); }

    mgl::opengl::buffer_ref& native() { return m_buffer; }

private:
    size_t m_req_size;
    bool m_req_dynamic;
    mgl::opengl::buffer_ref m_buffer;
  };

  class vertex_buffer : public buffer<mgl::platform::api::vertex_buffer>
  {
public:
    vertex_buffer(const std::string& layout, size_t size = 0, bool dynamic = false)
        : buffer(size, dynamic)
        , m_layout(layout)
    { }

    virtual ~vertex_buffer() = default;

    virtual const std::string& layout() const override final { return m_layout.layout(); }

    const mgl::opengl::buffer_layout& native_layout() const { return m_layout; }

private:
    mgl::opengl::buffer_layout m_layout;
  };

  class index_buffer : public buffer<mgl::platform::api::index_buffer>
  {
public:
    index_buffer(size_t size = 0, uint16_t element_size = 4, bool dynamic = false)
        : buffer(size, dynamic)
        , m_element_size(element_size)
    { }

    virtual ~index_buffer() = default;

    virtual uint16_t element_size() const override final { return m_element_size; }

private:
    uint16_t m_element_size;
  };

  using vertex_buffer_ref = mgl::ref<vertex_buffer>;
  using index_buffer_ref = mgl::ref<index_buffer>;

} // namespace mgl::platform::api::backends::opengl