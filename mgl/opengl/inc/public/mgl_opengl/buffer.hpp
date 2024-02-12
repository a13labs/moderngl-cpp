#pragma once

#include "mgl_core/debug.hpp"
#include "mgl_core/memory.hpp"

#include "gl_object.hpp"

namespace mgl::opengl
{
  class buffer;
  using buffer_ref = mgl::ref<buffer>;
  using buffers = mgl::ref_list<buffer>;

  /**
   * @class buffer
   * @brief Represents an OpenGL buffer object.
   */
  class buffer : public gl_object
  {
public:
    /**
     * @brief Destructor.
     */
    ~buffer() = default;

    /**
     * @brief Releases the buffer object.
     */
    void release();

    /**
     * @brief Clears the buffer data.
     */
    void clear();

    /**
     * @brief Orphans the buffer, allowing for faster data updates.
     * @param size The new size of the buffer.
     */
    void orphan(size_t size);

    /**
     * @brief Binds the buffer to a uniform block.
     * @param binding The binding point index.
     * @param size The size of the buffer to bind.
     * @param off The offset within the buffer to bind.
     */
    void bind_to_uniform_block(int binding = 0, size_t size = SIZE_MAX, size_t off = 0);

    /**
     * @brief Binds the buffer to a storage buffer.
     * @param binding The binding point index.
     * @param size The size of the buffer to bind.
     * @param off The offset within the buffer to bind.
     */
    void bind_to_storage_buffer(int binding = 0, size_t size = SIZE_MAX, size_t off = 0);

    /**
     * @brief Checks if the buffer is used for dynamic updates.
     * @return True if the buffer is dynamic, false otherwise.
     */
    bool dynamic() const { return m_dynamic; }

    /**
     * @brief Gets the size of the buffer in bytes.
     * @return The size of the buffer.
     */
    size_t size() const { return m_size; }

    /*
    * @brief returns the current position within the buffer.
    */
    size_t needle() const { return m_pos; }

    /**
     * @brief Seeks to a position within the buffer.
     * @param offset The offset to seek to.
     */
    void seek(size_t offset)
    {
      MGL_CORE_ASSERT(offset < m_size, "Offset is out of bounds");
      m_pos = offset;
    }

    void
    download(mgl::uint8_buffer& dst, size_t n_bytes = SIZE_MAX, size_t off = 0, size_t dst_off = 0)
    {
      download(dst.data(), dst.size() * sizeof(uint8_buffer), n_bytes, off, dst_off);
    }

    /**
     * @brief Reads data from the buffer into a uint8_buffer.
     * @param dst The destination buffer.
     * @param n_bytes The number of bytes to read.
     * @param off The offset within the buffer to read from.
     * @param dst_off The offset within the destination buffer to write to.
     */
    void
    download(mgl::uint16_buffer& dst, size_t n_bytes = SIZE_MAX, size_t off = 0, size_t dst_off = 0)
    {
      download(dst.data(), dst.size() * sizeof(uint16_t), n_bytes, off, dst_off);
    }

    /**
     * @brief Reads data from the buffer into a uint16_buffer.
     * @param dst The destination buffer.
     * @param n_bytes The number of bytes to read.
     * @param off The offset within the buffer to read from.
     * @param dst_off The offset within the destination buffer to write to.
     */
    void
    download(mgl::uint32_buffer& dst, size_t n_bytes = SIZE_MAX, size_t off = 0, size_t dst_off = 0)
    {
      download(dst.data(), dst.size() * sizeof(uint32_t), n_bytes, off, dst_off);
    }

    /**
     * @brief Reads data from the buffer into a uint32_buffer.
     * @param dst The destination buffer.
     * @param n_bytes The number of bytes to read.
     * @param off The offset within the buffer to read from.
     * @param dst_off The offset within the destination buffer to write to.
     */
    void
    download(mgl::uint64_buffer& dst, size_t n_bytes = SIZE_MAX, size_t off = 0, size_t dst_off = 0)
    {
      download(dst.data(), dst.size() * sizeof(uint64_t), n_bytes, off, dst_off);
    }

    /**
     * @brief Reads data from the buffer into a uint64_buffer.
     * @param dst The destination buffer.
     * @param n_bytes The number of bytes to read.
     * @param off The offset within the buffer to read from.
     * @param dst_off The offset within the destination buffer to write to.
     */
    void
    download(mgl::int8_buffer& dst, size_t n_bytes = SIZE_MAX, size_t off = 0, size_t dst_off = 0)
    {
      download(dst.data(), dst.size() * sizeof(int8_t), n_bytes, off, dst_off);
    }

    /**
     * @brief Reads data from the buffer into a int8_buffer.
     * @param dst The destination buffer.
     * @param n_bytes The number of bytes to read.
     * @param off The offset within the buffer to read from.
     * @param dst_off The offset within the destination buffer to write to.
     */
    void
    download(mgl::int16_buffer& dst, size_t n_bytes = SIZE_MAX, size_t off = 0, size_t dst_off = 0)
    {
      download(dst.data(), dst.size() * sizeof(int16_t), n_bytes, off, dst_off);
    }

    /**
     * @brief Reads data from the buffer into a int16_buffer.
     * @param dst The destination buffer.
     * @param n_bytes The number of bytes to read.
     * @param off The offset within the buffer to read from.
     * @param dst_off The offset within the destination buffer to write to.
     */
    void
    download(mgl::int32_buffer& dst, size_t n_bytes = SIZE_MAX, size_t off = 0, size_t dst_off = 0)
    {
      download(dst.data(), dst.size() * sizeof(int32_t), n_bytes, off, dst_off);
    }

    /**
     * @brief Reads data from the buffer into a int32_buffer.
     * @param dst The destination buffer.
     * @param n_bytes The number of bytes to read.
     * @param off The offset within the buffer to read from.
     * @param dst_off The offset within the destination buffer to write to.
     */
    void
    download(mgl::int64_buffer& dst, size_t n_bytes = SIZE_MAX, size_t off = 0, size_t dst_off = 0)
    {
      download(dst.data(), dst.size() * sizeof(int64_t), n_bytes, off, dst_off);
    }

    /**
     * @brief Reads data from the buffer into a int64_buffer.
     * @param dst The destination buffer.
     * @param n_bytes The number of bytes to read.
     * @param off The offset within the buffer to read from.
     * @param dst_off The offset within the destination buffer to write to.
     */
    void download(mgl::float32_buffer& dst,
                  size_t n_bytes = SIZE_MAX,
                  size_t off = 0,
                  size_t dst_off = 0)
    {
      download(dst.data(), dst.size() * sizeof(float), n_bytes, off, dst_off);
    }

    /**
     * @brief Reads data from the buffer into a float32_buffer.
     * @param dst The destination buffer.
     * @param n_bytes The number of bytes to read.
     * @param off The offset within the buffer to read from.
     * @param dst_off The offset within the destination buffer to write to.
     */
    void download(mgl::float64_buffer& dst,
                  size_t n_bytes = SIZE_MAX,
                  size_t off = 0,
                  size_t dst_off = 0)
    {
      download(dst.data(), dst.size() * sizeof(double), n_bytes, off, dst_off);
    }

    /**
     * @brief Reads data from the buffer into a float64_buffer.
     * @param dst The destination buffer.
     * @param n_bytes The number of bytes to read.
     * @param off The offset within the buffer to read from.
     * @param dst_off The offset within the destination buffer to write to.
     */
    void download(void* dst, size_t dst_sz, size_t n_bytes, size_t off, size_t dst_off);

    /**
     * @brief Writes data to the buffer from a uint8_buffer.
     * @param src The source buffer.
     * @param off The offset within the buffer to write to.
     */
    void upload(const mgl::uint8_buffer& src, size_t off = 0)
    {
      upload(src.data(), src.size() * sizeof(uint8_t), off);
    }

    /**
     * @brief Writes data to the buffer from a uint16_buffer.
     * @param src The source buffer.
     * @param off The offset within the buffer to write to.
     */
    void upload(const mgl::uint16_buffer& src, size_t off = 0)
    {
      upload(src.data(), src.size() * sizeof(uint16_t), off);
    }

    /**
     * @brief Writes data to the buffer from a uint32_buffer.
     * @param src The source buffer.
     * @param off The offset within the buffer to write to.
     */
    void upload(const mgl::uint32_buffer& src, size_t off = 0)
    {
      upload(src.data(), src.size() * sizeof(uint32_t), off);
    }

    /**
     * @brief Writes data to the buffer from a uint64_buffer.
     * @param src The source buffer.
     * @param off The offset within the buffer to write to.
     */
    void upload(const mgl::uint64_buffer& src, size_t off = 0)
    {
      upload(src.data(), src.size() * sizeof(uint64_t), off);
    }

    /**
     * @brief Writes data to the buffer from a int8_buffer.
     * @param src The source buffer.
     * @param off The offset within the buffer to write to.
     */
    void upload(const mgl::int8_buffer& src, size_t off = 0)
    {
      upload(src.data(), src.size() * sizeof(int8_t), off);
    }

    /**
     * @brief Writes data to the buffer from a int16_buffer.
     * @param src The source buffer.
     * @param off The offset within the buffer to write to.
     */
    void upload(const mgl::int16_buffer& src, size_t off = 0)
    {
      upload(src.data(), src.size() * sizeof(int16_t), off);
    }

    /**
     * @brief Writes data to the buffer from a int32_buffer.
     * @param src The source buffer.
     * @param off The offset within the buffer to write to.
     */
    void upload(const mgl::int32_buffer& src, size_t off = 0)
    {
      upload(src.data(), src.size() * sizeof(int), off);
    }

    /**
     * @brief Writes data to the buffer from a int64_buffer.
     * @param src The source buffer.
     * @param off The offset within the buffer to write to.
     */
    void upload(const mgl::int64_buffer& src, size_t off = 0)
    {
      upload(src.data(), src.size() * sizeof(int64_t), off);
    }

    /**
     * @brief Writes data to the buffer from a float32_buffer.
     * @param src The source buffer.
     * @param off The offset within the buffer to write to.
     */
    void upload(const mgl::float32_buffer& src, size_t off = 0)
    {
      upload(src.data(), src.size() * sizeof(float), off);
    }

    /**
     * @brief Writes data to the buffer from a float64_buffer.
     * @param src The source buffer.
     * @param off The offset within the buffer to write to.
     */
    void upload(const mgl::float64_buffer& src, size_t off = 0)
    {
      upload(src.data(), src.size() * sizeof(double), off);
    }

    /**
     * @brief Writes data to the buffer from a void pointer.
     * @param src The source buffer.
     * @param src_sz The size of the source buffer.
     * @param off The offset within the buffer to write to.
     */
    void upload(const void* src, size_t src_sz, size_t off = 0);

    /**
     * @brief Writes data to the buffer.
     * @param data The data to write.
     * @param size The size of the data to write.
     */
    void write(const void* data, size_t size)
    {
      MGL_CORE_ASSERT(m_pos + size <= m_size, "Buffer overflow");
      upload(data, size, m_pos);
      m_pos += size;
    }

    /**
     * @brief Writes data to the buffer.
     * @param data The data to write.
     */
    void write(mgl::uint8_buffer& data)
    {
      MGL_CORE_ASSERT(m_pos + data.size() <= m_size, "Buffer overflow");
      upload(data.data(), data.size() * sizeof(uint8_t), m_pos);
      m_pos += data.size() * sizeof(uint8_t);
    }

    /**
     * @brief Writes data to the buffer.
     * @param data The data to write.
     */
    void write(mgl::uint16_buffer& data)
    {
      MGL_CORE_ASSERT(m_pos + data.size() <= m_size, "Buffer overflow");
      upload(data.data(), data.size() * sizeof(uint16_t), m_pos);
      m_pos += data.size() * sizeof(uint16_t);
    }

    /**
     * @brief Writes data to the buffer.
     * @param data The data to write.
     */
    void write(mgl::uint32_buffer& data)
    {
      MGL_CORE_ASSERT(m_pos + data.size() <= m_size, "Buffer overflow");
      upload(data.data(), data.size() * sizeof(uint32_t), m_pos);
      m_pos += data.size() * sizeof(uint32_t);
    }

    /**
     * @brief Writes data to the buffer.
     * @param data The data to write.
     */
    void write(mgl::uint64_buffer& data)
    {
      MGL_CORE_ASSERT(m_pos + data.size() <= m_size, "Buffer overflow");
      upload(data.data(), data.size() * sizeof(uint64_t), m_pos);
      m_pos += data.size() * sizeof(uint64_t);
    }

    /**
     * @brief Writes data to the buffer.
     * @param data The data to write.
     */
    void write(mgl::int8_buffer& data)
    {
      MGL_CORE_ASSERT(m_pos + data.size() <= m_size, "Buffer overflow");
      upload(data.data(), data.size() * sizeof(int8_t), m_pos);
      m_pos += data.size() * sizeof(int8_t);
    }

    /**
     * @brief Writes data to the buffer.
     * @param data The data to write.
     */
    void write(mgl::int16_buffer& data)
    {
      MGL_CORE_ASSERT(m_pos + data.size() <= m_size, "Buffer overflow");
      upload(data.data(), data.size() * sizeof(int16_t), m_pos);
      m_pos += data.size() * sizeof(int16_t);
    }

    /**
     * @brief Writes data to the buffer.
     * @param data The data to write.
     */
    void write(mgl::int32_buffer& data)
    {
      MGL_CORE_ASSERT(m_pos + data.size() <= m_size, "Buffer overflow");
      upload(data.data(), data.size() * sizeof(int32_t), m_pos);
      m_pos += data.size() * sizeof(int32_t);
    }

    /**
     * @brief Writes data to the buffer.
     * @param data The data to write.
     */
    void write(mgl::int64_buffer& data)
    {
      MGL_CORE_ASSERT(m_pos + data.size() <= m_size, "Buffer overflow");
      upload(data.data(), data.size() * sizeof(int64_t), m_pos);
      m_pos += data.size() * sizeof(int64_t);
    }

    /**
     * @brief Writes data to the buffer.
     * @param data The data to write.
     */
    void write(float32_buffer& data)
    {
      MGL_CORE_ASSERT(m_pos + data.size() <= m_size, "Buffer overflow");
      upload(data.data(), data.size() * sizeof(float), m_pos);
      m_pos += data.size() * sizeof(float);
    }

    /**
     * @brief Writes data to the buffer.
     * @param data The data to write.
     */
    void write(float64_buffer& data)
    {
      MGL_CORE_ASSERT(m_pos + data.size() <= m_size, "Buffer overflow");
      upload(data.data(), data.size() * sizeof(double), m_pos);
      m_pos += data.size() * sizeof(double);
    }

    /**
     * @brief Copies data from the buffer to another buffer.
     * @param dst The destination buffer.
     * @param size The number of bytes to copy.
     * @param off The offset within the buffer to copy from.
     * @param dst_off The offset within the destination buffer to copy to.
     */
    void copy_to(const buffer_ref& dst, size_t size = SIZE_MAX, size_t off = 0, size_t dst_off = 0);

    /**
     * @brief Copies data from the buffer to another buffer.
     * @param src The source buffer.
     * @param size The number of bytes to copy.
     * @param off The offset within the buffer to copy from.
     * @param dst_off The offset within the destination buffer to copy to.
     */
    static void copy(const buffer_ref& src,
                     const buffer_ref& dst,
                     size_t size = SIZE_MAX,
                     size_t off = 0,
                     size_t dst_off = 0);

private:
    friend class context;
    /**
     * @brief Constructor.
     * @param ctx The associated context.
     * @param data The initial data to store in the buffer.
     * @param reserve The initial size of the buffer.
     * @param dynamic True if the buffer is used for dynamic updates, false otherwise.
     */
    buffer(const context_ref& ctx, const void* data, size_t reserve, bool dynamic);

    size_t m_size; ///< The size of the buffer.
    bool m_dynamic; ///< True if the buffer is dynamic, false otherwise.
    size_t m_pos; ///< The current position within the buffer.
  };

} // namespace  mgl::opengl
