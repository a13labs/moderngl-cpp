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
    virtual ~buffer() = default;

    virtual void allocate() = 0;

    virtual void free() = 0;

    virtual void seek(size_t offset) = 0;

    virtual void write(const void* data, size_t size) = 0;

    virtual void write(uint8_buffer& data) = 0;

    virtual void write(uint16_buffer& data) = 0;

    virtual void write(uint32_buffer& data) = 0;

    virtual void write(uint64_buffer& data) = 0;

    virtual void write(int8_buffer& data) = 0;

    virtual void write(int16_buffer& data) = 0;

    virtual void write(int32_buffer& data) = 0;

    virtual void write(int64_buffer& data) = 0;

    virtual void write(float32_buffer& data) = 0;

    virtual void write(float64_buffer& data) = 0;

    virtual void upload(const void* data, size_t size) = 0;

    virtual void upload(const mgl::uint8_buffer& data) = 0;

    virtual void upload(const mgl::uint16_buffer& data) = 0;

    virtual void upload(const mgl::uint32_buffer& data) = 0;

    virtual void upload(const mgl::uint64_buffer& data) = 0;

    virtual void upload(const mgl::int8_buffer& data) = 0;

    virtual void upload(const mgl::int16_buffer& data) = 0;

    virtual void upload(const mgl::int32_buffer& data) = 0;

    virtual void upload(const mgl::int64_buffer& data) = 0;

    virtual void upload(const mgl::float32_buffer& data) = 0;

    virtual void upload(const mgl::float64_buffer& data) = 0;

    virtual void orphan(size_t size) = 0;

    virtual size_t needle() const = 0;

    virtual size_t size() const = 0;

    virtual bool is_dynamic() const = 0;
  };

  class vertex_buffer : public buffer
  {
public:
    virtual ~vertex_buffer() = default;

    virtual const std::string& layout() const = 0;

    virtual const mgl::string_list& attributes() const = 0;
  };

  class index_buffer : public buffer
  {
public:
    virtual ~index_buffer() = default;

    virtual uint16_t element_size() const = 0;
  };

  using index_buffer_ref = mgl::ref<index_buffer>;
  using vertex_buffer_ref = mgl::ref<vertex_buffer>;

} // namespace mgl::platform::api
