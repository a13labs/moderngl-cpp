#pragma once

#include "mgl_core/memory.hpp"

#include "gl_object.hpp"

namespace mgl::opengl
{
  class buffer;
  using buffer_ref = mgl::ref<buffer>;
  using buffers = mgl::ref_list<buffer>;

  class buffer : public gl_object
  {
public:
    ~buffer() = default;

    virtual void release() override final;

    void clear();
    void orphan(size_t size);

    void bind_to_uniform_block(int binding = 0, size_t size = SIZE_MAX, size_t off = 0);

    void bind_to_storage_buffer(int binding = 0, size_t size = SIZE_MAX, size_t off = 0);

    bool dynamic() const { return m_dynamic; }

    size_t size() const { return m_size; }

    void read(mgl::uint8_buffer& dst, size_t n_bytes = SIZE_MAX, size_t off = 0, size_t dst_off = 0)
    {
      read(dst.data(), dst.size() * sizeof(uint8_buffer), n_bytes, off, dst_off);
    }

    void
    read(mgl::uint16_buffer& dst, size_t n_bytes = SIZE_MAX, size_t off = 0, size_t dst_off = 0)
    {
      read(dst.data(), dst.size() * sizeof(uint16_t), n_bytes, off, dst_off);
    }

    void
    read(mgl::uint32_buffer& dst, size_t n_bytes = SIZE_MAX, size_t off = 0, size_t dst_off = 0)
    {
      read(dst.data(), dst.size() * sizeof(uint32_t), n_bytes, off, dst_off);
    }

    void
    read(mgl::uint64_buffer& dst, size_t n_bytes = SIZE_MAX, size_t off = 0, size_t dst_off = 0)
    {
      read(dst.data(), dst.size() * sizeof(uint64_t), n_bytes, off, dst_off);
    }

    void read(mgl::int8_buffer& dst, size_t n_bytes = SIZE_MAX, size_t off = 0, size_t dst_off = 0)
    {
      read(dst.data(), dst.size() * sizeof(int8_t), n_bytes, off, dst_off);
    }

    void read(mgl::int16_buffer& dst, size_t n_bytes = SIZE_MAX, size_t off = 0, size_t dst_off = 0)
    {
      read(dst.data(), dst.size() * sizeof(int16_t), n_bytes, off, dst_off);
    }

    void read(mgl::int32_buffer& dst, size_t n_bytes = SIZE_MAX, size_t off = 0, size_t dst_off = 0)
    {
      read(dst.data(), dst.size() * sizeof(int32_t), n_bytes, off, dst_off);
    }

    void read(mgl::int64_buffer& dst, size_t n_bytes = SIZE_MAX, size_t off = 0, size_t dst_off = 0)
    {
      read(dst.data(), dst.size() * sizeof(int64_t), n_bytes, off, dst_off);
    }

    void
    read(mgl::float32_buffer& dst, size_t n_bytes = SIZE_MAX, size_t off = 0, size_t dst_off = 0)
    {
      read(dst.data(), dst.size() * sizeof(float), n_bytes, off, dst_off);
    }

    void
    read(mgl::float64_buffer& dst, size_t n_bytes = SIZE_MAX, size_t off = 0, size_t dst_off = 0)
    {
      read(dst.data(), dst.size() * sizeof(double), n_bytes, off, dst_off);
    }

    void read(void* dst, size_t dst_sz, size_t n_bytes, size_t off, size_t dst_off);

    void write(const mgl::uint8_buffer& src, size_t off = 0)
    {
      write(src.data(), src.size() * sizeof(uint8_t), off);
    }

    void write(const mgl::uint16_buffer& src, size_t off = 0)
    {
      write(src.data(), src.size() * sizeof(uint16_t), off);
    }

    void write(const mgl::uint32_buffer& src, size_t off = 0)
    {
      write(src.data(), src.size() * sizeof(uint32_t), off);
    }

    void write(const mgl::uint64_buffer& src, size_t off = 0)
    {
      write(src.data(), src.size() * sizeof(uint64_t), off);
    }

    void write(const mgl::int8_buffer& src, size_t off = 0)
    {
      write(src.data(), src.size() * sizeof(int8_t), off);
    }

    void write(const mgl::int16_buffer& src, size_t off = 0)
    {
      write(src.data(), src.size() * sizeof(int16_t), off);
    }

    void write(const mgl::int32_buffer& src, size_t off = 0)
    {
      write(src.data(), src.size() * sizeof(int), off);
    }

    void write(const mgl::int64_buffer& src, size_t off = 0)
    {
      write(src.data(), src.size() * sizeof(int64_t), off);
    }

    void write(const mgl::float32_buffer& src, size_t off = 0)
    {
      write(src.data(), src.size() * sizeof(float), off);
    }

    void write(const mgl::float64_buffer& src, size_t off = 0)
    {
      write(src.data(), src.size() * sizeof(double), off);
    }

    void write(const void* src, size_t src_sz, size_t off = 0);

    void copy_to(const buffer_ref& dst, size_t size = SIZE_MAX, size_t off = 0, size_t dst_off = 0);

    static void copy(const buffer_ref& src,
                     const buffer_ref& dst,
                     size_t size = SIZE_MAX,
                     size_t off = 0,
                     size_t dst_off = 0);

private:
    friend class context;
    buffer(const context_ref& ctx, const void* data, size_t reserve, bool dynamic);

    size_t m_size;
    bool m_dynamic;
  };

} // namespace  mgl::opengl
