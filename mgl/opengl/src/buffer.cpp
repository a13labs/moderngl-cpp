#include "mgl_opengl/buffer.hpp"
#include "mgl_opengl/context.hpp"

#include "mgl_core/debug.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  buffer::buffer(const context_ref& ctx, const void* data, size_t reserve, bool dynamic)
      : gl_object(ctx)
      , m_size(reserve)
      , m_dynamic(dynamic)
  {
    GLuint glo = 0;
    glGenBuffers(1, &glo);
    if(glo == GL_ZERO)
    {
      MGL_CORE_ASSERT(false, "Error creating buffer");
      return;
    }
    gl_object::set_glo(glo);
    glBindBuffer(GL_ARRAY_BUFFER, gl_object::glo());
    glBufferData(GL_ARRAY_BUFFER, reserve, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
  }

  void buffer::release()
  {
    MGL_CORE_ASSERT(!gl_object::released(), "Buffer already released");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "Context not current");
    GLuint glo = gl_object::glo();
    glDeleteBuffers(1, &glo);
    gl_object::set_glo(ZERO);
  }

  void buffer::write(const void* src, size_t src_sz, size_t offset)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "Buffer already released");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "Context not current");
    MGL_CORE_ASSERT(offset >= 0, "invalid offset: {0}", offset)
    MGL_CORE_ASSERT(src_sz + offset <= m_size, "out of bounds")
    glBindBuffer(GL_ARRAY_BUFFER, gl_object::glo());
    glBufferSubData(GL_ARRAY_BUFFER, (GLintptr)offset, src_sz, src);
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "Error writing to buffer");
  }

  void buffer::read(void* dst, size_t dst_sz, size_t read_sz, size_t read_off, size_t dst_off)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "Buffer already released");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "Context not current");

    if(read_sz == 0)
    {
      read_sz = m_size;
    }

    MGL_CORE_ASSERT(read_sz >= 0, "invalid size: {0}", read_sz)
    MGL_CORE_ASSERT(read_off >= 0, "invalid offset: {0}", read_off)
    MGL_CORE_ASSERT(dst_off >= 0, "invalid write offset: {0}", dst_off)
    MGL_CORE_ASSERT(m_size >= read_sz + read_off, "out of bounds")
    MGL_CORE_ASSERT(dst_sz >= dst_off + read_sz, "out of bounds")

    glBindBuffer(GL_ARRAY_BUFFER, gl_object::glo());
    auto map = glMapBufferRange(GL_ARRAY_BUFFER, read_off, read_sz, GL_MAP_READ_BIT);
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "Error mapping buffer");
    std::copy((char*)map, (char*)map + read_sz, (char*)dst + dst_off);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "Error writing to buffer");
  }

  void buffer::clear()
  {
    MGL_CORE_ASSERT(!gl_object::released(), "Buffer already released");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "Context not current");

    glBindBuffer(GL_ARRAY_BUFFER, gl_object::glo());
    char* map = (char*)glMapBufferRange(GL_ARRAY_BUFFER, 0, m_size, GL_MAP_WRITE_BIT);
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "Error mapping buffer");
    std::fill(map, map + m_size, 0);
    glUnmapBuffer(GL_ARRAY_BUFFER);
  }

  void buffer::orphan(size_t size)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "Buffer already released");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "Context not current");

    if(size == SIZE_MAX)
    {
      size = m_size;
    }

    glBindBuffer(GL_ARRAY_BUFFER, gl_object::glo());
    glBufferData(GL_ARRAY_BUFFER, size, 0, m_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    m_size = size;
  }

  void buffer::bind_to_uniform_block(int binding, size_t size, size_t offset)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "Buffer already released");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "Context not current");
    MGL_CORE_ASSERT(size >= 0, "invalid data size: {0}", size)
    MGL_CORE_ASSERT(offset >= 0, "invalid offset: {0}", offset)

    if(size == SIZE_MAX)
    {
      size = m_size - offset;
    }

    glBindBufferRange(GL_UNIFORM_BUFFER, binding, gl_object::glo(), offset, size);
  }

  void buffer::bind_to_storage_buffer(int binding, size_t size, size_t offset)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "Buffer already released");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "Context not current");
    MGL_CORE_ASSERT(size >= 0, "invalid data size: {0}", size)
    MGL_CORE_ASSERT(offset >= 0, "invalid offset: {0}", offset)

    if(size == SIZE_MAX)
    {
      size = m_size - offset;
    }

    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, binding, gl_object::glo(), offset, size);
  }

} // namespace  mgl::opengl
