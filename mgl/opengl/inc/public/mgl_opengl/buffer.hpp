#pragma once

#include "mgl_core/memory.hpp"

#include "gl_object.hpp"

namespace mgl::opengl
{
  class buffer : public gl_object
  {
public:
    ~buffer() = default;

    virtual void release() override final;

    void clear();
    void orphan(size_t size);

    void bind_to_uniform_block(int binding = 0, size_t size = 0, size_t offset = 0);

    void bind_to_storage_buffer(int binding = 0, size_t size = 0, size_t offset = 0);

    bool dynamic() const { return m_dynamic; }

    size_t size() const { return m_size; }

    void read(mgl::float32_buffer& dst, size_t size = 0, size_t offset = 0, size_t write_offset = 0)
    {
      read(dst.data(), dst.size(), size, offset, write_offset);
    }

    void read(mgl::uint32_buffer& dst, size_t size = 0, size_t offset = 0, size_t write_offset = 0)
    {
      read(dst.data(), dst.size(), size, offset, write_offset);
    }

    void read(mgl::uint8_buffer& dst, size_t size = 0, size_t offset = 0, size_t write_offset = 0)
    {
      read(dst.data(), dst.size(), size, offset, write_offset);
    }

    void read(mgl::int32_buffer& dst, size_t size = 0, size_t offset = 0, size_t write_offset = 0)
    {
      read(dst.data(), dst.size(), size, offset, write_offset);
    }

    void read(void* dst, size_t dst_sz, size_t read_sz, size_t read_off, size_t dst_off);

    void write(const mgl::float32_buffer& src, size_t offset = 0)
    {
      write(src.data(), src.size() * sizeof(float), offset);
    }

    void write(const mgl::uint32_buffer& src, size_t offset = 0)
    {
      write(src.data(), src.size() * sizeof(uint32_t), offset);
    }

    void write(const mgl::uint8_buffer& src, size_t offset = 0)
    {
      write(src.data(), src.size() * sizeof(uint8_t), offset);
    }

    void write(const mgl::int32_buffer& src, size_t offset = 0)
    {
      write(src.data(), src.size() * sizeof(int), offset);
    }

    void write(const void* src, size_t src_sz, size_t offset = 0);

private:
    friend class context;
    buffer(const context_ref& ctx, const void* data, size_t reserve, bool dynamic);

    size_t m_size;
    bool m_dynamic;
  };

  using buffer_ref = mgl::ref<buffer>;
  using buffers = mgl::ref_list<buffer>;

} // namespace  mgl::opengl
