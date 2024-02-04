/*
   Copyright 2022 Alexandre Pires (c.alexandre.pires@gmail.com)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#pragma once

#include "mgl_core/memory.hpp"

namespace mgl::opengl
{
  class context;
  class buffer
  {
public:
    ~buffer() = default;

public:
    void release();
    bool released();

    size_t size();
    bool dynamic();

    void read_into(mgl::float32_buffer& dst,
                   size_t size = 0,
                   size_t offset = 0,
                   size_t write_offset = 0);
    void
    read_into(mgl::uint32_buffer& dst, size_t size = 0, size_t offset = 0, size_t write_offset = 0);
    void
    read_into(mgl::uint8_buffer& dst, size_t size = 0, size_t offset = 0, size_t write_offset = 0);
    void
    read_into(mgl::int32_buffer& dst, size_t size = 0, size_t offset = 0, size_t write_offset = 0);

    void write(const void* src, size_t size, size_t offset = 0);
    void write(const mgl::float32_buffer& dst, size_t offset = 0);
    void write(const mgl::uint32_buffer& dst, size_t offset = 0);
    void write(const mgl::uint8_buffer& dst, size_t offset = 0);
    void write(const mgl::int32_buffer& dst, size_t offset = 0);

    void clear();
    void bind_to_uniform_block(int binding = 0, size_t size = 0, size_t offset = 0);
    void bind_to_storage_buffer(int binding = 0, size_t size = 0, size_t offset = 0);
    void orphan(size_t size);

    int glo();

private:
    friend class context;
    friend class framebuffer;
    friend class texture_2d;
    friend class texture_3d;
    friend class texture_array;
    friend class texture_cube;
    friend class vertex_array;

    buffer() = default;

    void read_into(
        void* dst, size_t dst_size, size_t read_size, size_t read_offset, size_t write_offset);

    context* m_context;
    int m_buffer_obj;
    size_t m_size;
    bool m_dynamic;
    bool m_released;
  };

  using buffer_ref = mgl::ref<buffer>;
  using buffers = mgl::ref_list<buffer>;

  inline int buffer::glo()
  {
    return m_buffer_obj;
  }

  inline void buffer::write(const mgl::float32_buffer& src, size_t offset)
  {
    write(src.data(), src.size() * sizeof(float), offset);
  }

  inline void buffer::write(const mgl::uint32_buffer& src, size_t offset)
  {
    write(src.data(), src.size() * sizeof(uint32_t), offset);
  }

  inline void buffer::write(const mgl::uint8_buffer& src, size_t offset)
  {
    write(src.data(), src.size() * sizeof(uint8_t), offset);
  }

  inline void buffer::write(const mgl::int32_buffer& src, size_t offset)
  {
    write(src.data(), src.size() * sizeof(int), offset);
  }

  inline void
  buffer::read_into(mgl::float32_buffer& dst, size_t size, size_t offset, size_t write_offset)
  {
    read_into(dst.data(), dst.size(), size, offset, write_offset);
  }

  inline void
  buffer::read_into(mgl::uint32_buffer& dst, size_t size, size_t offset, size_t write_offset)
  {
    read_into(dst.data(), dst.size(), size, offset, write_offset);
  }

  inline void
  buffer::read_into(mgl::uint8_buffer& dst, size_t size, size_t offset, size_t write_offset)
  {
    read_into(dst.data(), dst.size(), size, offset, write_offset);
  }

  inline void
  buffer::read_into(mgl::int32_buffer& dst, size_t size, size_t offset, size_t write_offset)
  {
    read_into(dst.data(), dst.size(), size, offset, write_offset);
  }

  inline size_t buffer::size()
  {
    return m_size;
  }

  inline bool buffer::dynamic()
  {
    return m_dynamic;
  }

  inline bool buffer::released()
  {
    return m_released;
  }

} // namespace  mgl::opengl
