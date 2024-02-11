#pragma once

#include "mgl_core/debug.hpp"
#include "mgl_core/memory.hpp"

namespace mgl::platform::api
{
  class buffer;
  using buffer_ref = mgl::ref<buffer>;

  class buffer
  {
public:
    buffer(size_t size, bool dynamic)
        : m_size(size)
        , m_dynamic(dynamic)
    { }

    virtual ~buffer() = default;

    virtual void allocate() = 0;

    virtual void free() = 0;

    virtual void seek(size_t offset) = 0;

    virtual void write(const void* data, size_t size) = 0;

    virtual void write(float32_buffer& data) = 0;

    virtual void write(float64_buffer& data) = 0;

    virtual void write(int32_buffer& data) = 0;

    virtual void write(uint32_buffer& data) = 0;

    virtual void write(int16_buffer& data) = 0;

    virtual void write(uint16_buffer& data) = 0;

    virtual void write(int8_buffer& data) = 0;

    virtual void write(uint8_buffer& data) = 0;

    virtual void upload(const void* data, size_t size) = 0;

    virtual void upload(const mgl::float32_buffer& data) = 0;

    virtual void upload(const mgl::float64_buffer& data) = 0;

    virtual void upload(const mgl::int32_buffer& data) = 0;

    virtual void upload(const mgl::uint32_buffer& data) = 0;

    virtual void upload(const mgl::int16_buffer& data) = 0;

    virtual void upload(const mgl::uint16_buffer& data) = 0;

    virtual void upload(const mgl::int8_buffer& data) = 0;

    virtual void upload(const mgl::uint8_buffer& data) = 0;

    virtual void orphan(size_t size) = 0;

    virtual size_t needle() const = 0;

    size_t size() const { return m_size; }

    bool is_dynamic() const { return m_dynamic; }

protected:
    size_t m_size;
    bool m_dynamic;
  };

} // namespace mgl::platform::api