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
      , m_pos(0)
  {
    GLuint glo = 0;
    glGenBuffers(1, &glo);
    if(glo == GL_ZERO)
    {
      MGL_CORE_ASSERT(false, "[Buffer] Error creating buffer.");
      return;
    }
    gl_object::set_glo(glo);
    glBindBuffer(GL_ARRAY_BUFFER, gl_object::glo());
    glBufferData(GL_ARRAY_BUFFER, reserve, data, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[Program] Error on creating buffer.");
  }

  void buffer::release()
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Buffer] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Buffer] Resource context not current.");
    GLuint glo = gl_object::glo();
    glDeleteBuffers(1, &glo);
    gl_object::set_glo(ZERO);
    m_size = 0;
    m_pos = 0;
  }

  void buffer::download(void* dst, size_t dst_sz, size_t n_bytes, size_t off, size_t dst_off)
  {
    if(n_bytes == SIZE_MAX)
    {
      n_bytes = m_size;
    }

    MGL_CORE_ASSERT(!gl_object::released(), "[Buffer] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Buffer] Resource context not current.");
    MGL_CORE_ASSERT(n_bytes >= 0, "[Buffer] Invalid size: {0}.", n_bytes)
    MGL_CORE_ASSERT(m_size >= off + n_bytes, "[Buffer] Source out of bounds.")
    MGL_CORE_ASSERT(dst_sz >= dst_off + n_bytes, "[Buffer] Destination out of bounds.")

    glBindBuffer(GL_ARRAY_BUFFER, gl_object::glo());
    auto map = glMapBufferRange(GL_ARRAY_BUFFER, off, n_bytes, GL_MAP_READ_BIT);
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[Buffer] Error mapping buffer.");
    std::copy((char*)map, (char*)map + n_bytes, (char*)dst + dst_off);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[Buffer] Error writing to buffer.");
  }

  void buffer::upload(const void* src, size_t src_sz, size_t off)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Buffer] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Buffer] Resource context not current.");
    MGL_CORE_ASSERT(src_sz + off <= m_size, "[Buffer] Source out of bounds.")

    glBindBuffer(GL_ARRAY_BUFFER, gl_object::glo());
    glBufferSubData(GL_ARRAY_BUFFER, (GLintptr)off, src_sz, src);
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[Buffer] Error writing to buffer.");
    m_pos = 0;
  }

  void buffer::clear()
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Buffer] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Buffer] Resource context not current.");

    glBindBuffer(GL_ARRAY_BUFFER, gl_object::glo());
    char* map = (char*)glMapBufferRange(GL_ARRAY_BUFFER, 0, m_size, GL_MAP_WRITE_BIT);
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[Buffer] Error mapping buffer.");
    std::fill(map, map + m_size, 0);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[Buffer] Error writing to buffer.");
    m_pos = 0;
  }

  void buffer::orphan(size_t size)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Buffer] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Buffer] Resource context not current.");

    if(size == SIZE_MAX)
    {
      size = m_size;
    }

    glBindBuffer(GL_ARRAY_BUFFER, gl_object::glo());
    glBufferData(GL_ARRAY_BUFFER, size, 0, m_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    m_size = size;
    m_pos = 0;
  }

  void buffer::bind_to_uniform_block(int binding, size_t size, size_t off)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Buffer] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Buffer] Resource context not current.");
    MGL_CORE_ASSERT(size >= 0, "[Buffer] Invalid data size: {0}.", size)
    MGL_CORE_ASSERT(off >= 0, "[Buffer] Invalid off: {0}.", off)

    if(size == SIZE_MAX)
    {
      size = m_size - off;
    }

    glBindBufferRange(GL_UNIFORM_BUFFER, binding, gl_object::glo(), off, size);
  }

  void buffer::bind_to_storage_buffer(int binding, size_t size, size_t off)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Buffer] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Buffer] Resource context not current.");
    MGL_CORE_ASSERT(size >= 0, "[Buffer] Invalid data size: {0}.", size)
    MGL_CORE_ASSERT(off >= 0, "[Buffer] Invalid off: {0}.", off)

    if(size == SIZE_MAX)
    {
      size = m_size - off;
    }

    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, binding, gl_object::glo(), off, size);
  }

  void buffer::copy_to(const buffer_ref& dst, size_t size, size_t off, size_t dst_off)
  {
    if(size == SIZE_MAX)
    {
      size = m_size - off;
    }

    MGL_CORE_ASSERT(dst, "[Buffer] Invalid buffer");
    MGL_CORE_ASSERT(!released() && !dst->released(),
                    "[Buffer] Resource already released or not valid.");
    MGL_CORE_ASSERT(ctx() == dst->ctx(), "[Buffer] Buffers must be in the same context");
    MGL_CORE_ASSERT(ctx()->is_current(), "[Buffer] Resource context not current.");
    MGL_CORE_ASSERT((off + size <= m_size && dst_off + size <= dst->m_size),
                    "[Buffer] Buffer overflow.");

    glBindBuffer(GL_COPY_READ_BUFFER, glo());
    glBindBuffer(GL_COPY_WRITE_BUFFER, dst->glo());
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, off, dst_off, size);
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[Buffer] Error copying buffer.");
  }

  void buffer::copy(
      const buffer_ref& src, const buffer_ref& dst, size_t size, size_t off, size_t dst_off)
  {
    if(size == SIZE_MAX)
    {
      size = src->m_size - off;
    }

    MGL_CORE_ASSERT(src && dst, "[Buffer] Invalid buffer");
    MGL_CORE_ASSERT(!src->released() && !dst->released(),
                    "[Buffer] Resource already released or not valid.");
    MGL_CORE_ASSERT(src->ctx() == dst->ctx(), "[Buffer] Buffers must be in the same context");
    MGL_CORE_ASSERT(src->ctx()->is_current(), "[Buffer] Resource context not current.");
    MGL_CORE_ASSERT((off + size <= src->m_size && dst_off + size <= dst->m_size),
                    "[Buffer] Buffer overflow.");

    glBindBuffer(GL_COPY_READ_BUFFER, src->glo());
    glBindBuffer(GL_COPY_WRITE_BUFFER, dst->glo());
    glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, off, dst_off, size);
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "[Buffer] Error copying buffer.");
  }

} // namespace  mgl::opengl
