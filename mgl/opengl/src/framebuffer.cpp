
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
#include "mgl_opengl/framebuffer.hpp"
#include "mgl_opengl/buffer.hpp"
#include "mgl_opengl/context.hpp"
#include "mgl_opengl/data_type.hpp"

#include "mgl_core/debug.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  void framebuffer::release()
  {
    if(m_released)
    {
      return;
    }

    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");

    m_released = true;

    if(m_framebuffer_obj)
    {
      glDeleteFramebuffers(1, (GLuint*)&m_framebuffer_obj);
      delete[] m_draw_buffers;
    }
  }

  void
  framebuffer::clear(float r, float g, float b, float a, float depth, const mgl::rect& viewport)
  {
    MGL_CORE_ASSERT(!m_released, "Framebuffer already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer_obj);

    if(m_framebuffer_obj)
    {
      glDrawBuffers(m_draw_buffers_len, m_draw_buffers);
    }

    glClearColor(r, g, b, a);
    glClearDepth(depth);

    for(int i = 0; i < m_draw_buffers_len; ++i)
    {
      glColorMaski(
          i, m_color_masks[i].r, m_color_masks[i].g, m_color_masks[i].b, m_color_masks[i].a);
    }

    glDepthMask(m_depth_mask);

    // Respect the passed in viewport even with scissor enabled
    if(viewport != mgl::null_viewport_2d)
    {
      glEnable(GL_SCISSOR_TEST);
      glScissor(viewport.x, viewport.y, viewport.width, viewport.height);
      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

      // restore scissor if enabled
      if(m_scissor_enabled)
      {
        glScissor(m_scissor.x, m_scissor.y, m_scissor.width, m_scissor.height);
      }
      else
      {
        glDisable(GL_SCISSOR_TEST);
      }
    }
    else
    {
      // clear with scissor if enabled
      if(m_scissor_enabled)
      {
        glEnable(GL_SCISSOR_TEST);
        glScissor(m_scissor.x, m_scissor.y, m_scissor.width, m_scissor.height);
      }

      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, m_context->m_bound_framebuffer->m_framebuffer_obj);
  }

  void framebuffer::use()
  {
    MGL_CORE_ASSERT(!m_released, "Framebuffer already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer_obj);

    if(m_framebuffer_obj)
    {
      glDrawBuffers(m_draw_buffers_len, m_draw_buffers);
    }

    glViewport(m_viewport.x, m_viewport.y, m_viewport.width, m_viewport.height);

    if(m_scissor_enabled)
    {
      glEnable(GL_SCISSOR_TEST);
      glScissor(m_scissor.x, m_scissor.y, m_scissor.width, m_scissor.height);
    }
    else
    {
      glDisable(GL_SCISSOR_TEST);
    }

    for(int i = 0; i < m_draw_buffers_len; ++i)
    {
      glColorMaski(
          i, m_color_masks[i].r, m_color_masks[i].g, m_color_masks[i].b, m_color_masks[i].a);
    }

    glDepthMask(m_depth_mask);

    m_context->m_bound_framebuffer = shared_from_this();
  }

  bool framebuffer::read_into(mgl::byte_buffer& dst,
                              const mgl::rect& viewport,
                              int components,
                              int attachment,
                              int alignment,
                              const char* dtype,
                              size_t write_offset)
  {
    MGL_CORE_ASSERT(!m_released, "Framebuffer already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8,
                    "alignment must be 1, 2, 4 or 8");
    data_type* data_type = from_dtype(dtype);
    MGL_CORE_ASSERT(data_type != nullptr, "invalid dtype");

    bool read_depth = false;

    if(attachment == -1)
    {
      components = 1;
      read_depth = true;
    }

    int x = 0;
    int y = 0;
    int width = m_width;
    int height = m_height;
    if(viewport != mgl::null_viewport_2d)
    {
      x = viewport.x;
      y = viewport.y;
      width = viewport.width;
      height = viewport.height;
    }

    size_t expected_size = width * components * data_type->size;
    expected_size = (expected_size + alignment - 1) / alignment * alignment;
    expected_size = expected_size * height;

    MGL_CORE_ASSERT(dst.size() >= write_offset + expected_size, "out of bounds");

    int pixel_type = data_type->gl_type;
    int base_format = read_depth ? GL_DEPTH_COMPONENT : data_type->base_format[components];

    char* ptr = (char*)dst.data() + write_offset;

    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer_obj);
    glReadBuffer(read_depth ? GL_NONE : (GL_COLOR_ATTACHMENT0 + attachment));
    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glReadPixels(x, y, width, height, base_format, pixel_type, ptr);
    glBindFramebuffer(GL_FRAMEBUFFER, m_context->m_bound_framebuffer->m_framebuffer_obj);

    return glGetError() == GL_NO_ERROR;
  }

  bool framebuffer::read_into(buffer_ref dst,
                              const mgl::rect& viewport,
                              int components,
                              int attachment,
                              int alignment,
                              const char* dtype,
                              size_t write_offset)
  {
    MGL_CORE_ASSERT(!m_released, "Framebuffer already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(alignment == 1 || alignment == 2 || alignment == 4 || alignment == 8,
                    "alignment must be 1, 2, 4 or 8");
    data_type* data_type = from_dtype(dtype);
    MGL_CORE_ASSERT(data_type != nullptr, "invalid dtype");

    bool read_depth = false;

    if(attachment == -1)
    {
      components = 1;
      read_depth = true;
    }

    int x = 0;
    int y = 0;
    int width = m_width;
    int height = m_height;
    if(viewport != mgl::null_viewport_2d)
    {
      x = viewport.x;
      y = viewport.y;
      width = viewport.width;
      height = viewport.height;
    }

    int pixel_type = data_type->gl_type;
    int base_format = read_depth ? GL_DEPTH_COMPONENT : data_type->base_format[components];

    glBindBuffer(GL_PIXEL_PACK_BUFFER, dst->m_buffer_obj);
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer_obj);
    glReadBuffer(read_depth ? GL_NONE : (GL_COLOR_ATTACHMENT0 + attachment));
    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glReadPixels(x, y, width, height, base_format, pixel_type, (void*)write_offset);
    glBindFramebuffer(GL_FRAMEBUFFER, m_context->m_bound_framebuffer->m_framebuffer_obj);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    return glGetError() == GL_NO_ERROR;
  }

  void framebuffer::set_color_mask(const color_masks& masks)
  {
    MGL_CORE_ASSERT(!m_released, "Framebuffer already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(masks.size() != (size_t)m_draw_buffers_len,
                    "color_mask must be a match buffers len");
    m_color_masks = masks;

    if(m_framebuffer_obj == m_context->m_bound_framebuffer->m_framebuffer_obj)
    {
      for(int i = 0; i < m_draw_buffers_len; ++i)
      {
        glColorMaski(
            i, m_color_masks[i].r, m_color_masks[i].g, m_color_masks[i].b, m_color_masks[i].a);
      }
    }
  }

  void framebuffer::set_depth_mask(bool value)
  {
    MGL_CORE_ASSERT(!m_released, "Framebuffer already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    m_depth_mask = value;

    if(m_framebuffer_obj == m_context->m_bound_framebuffer->m_framebuffer_obj)
    {
      glDepthMask(m_depth_mask);
    }
  }

  bool framebuffer::bits(int& red_bits,
                         int& green_bits,
                         int& blue_bits,
                         int& alpha_bits,
                         int& depth_bits,
                         int& stencil_bits)
  {
    MGL_CORE_ASSERT(!m_released, "Framebuffer already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    MGL_CORE_ASSERT(!m_framebuffer_obj, "Only the default_framebuffer have bits");
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer_obj);
    glGetFramebufferAttachmentParameteriv(
        GL_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE, &red_bits);
    glGetFramebufferAttachmentParameteriv(
        GL_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE, &green_bits);
    glGetFramebufferAttachmentParameteriv(
        GL_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE, &blue_bits);
    glGetFramebufferAttachmentParameteriv(
        GL_FRAMEBUFFER, GL_BACK_LEFT, GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE, &alpha_bits);
    glGetFramebufferAttachmentParameteriv(
        GL_FRAMEBUFFER, GL_DEPTH, GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE, &depth_bits);
    glGetFramebufferAttachmentParameteriv(
        GL_FRAMEBUFFER, GL_STENCIL, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &stencil_bits);
    glBindFramebuffer(GL_FRAMEBUFFER, m_context->m_bound_framebuffer->m_framebuffer_obj);

    return glGetError() == GL_NO_ERROR;
  }

  void framebuffer::set_viewport(const mgl::rect& r)
  {
    MGL_CORE_ASSERT(!m_released, "Framebuffer already released");
    MGL_CORE_ASSERT(m_context, "No context");
    if(m_context->released())
      return;

    m_viewport = r;

    if(m_framebuffer_obj == m_context->m_bound_framebuffer->m_framebuffer_obj)
    {
      glViewport(m_viewport.x, m_viewport.y, m_viewport.width, m_viewport.height);
    }
  }

  void framebuffer::set_scissor(const mgl::rect& r)
  {
    MGL_CORE_ASSERT(!m_released, "Framebuffer already released");
    MGL_CORE_ASSERT(m_context, "No context");
    if(m_context->released())
      return;

    m_scissor = r;

    if(m_framebuffer_obj == m_context->m_bound_framebuffer->m_framebuffer_obj)
    {
      glScissor(m_scissor.x, m_scissor.y, m_scissor.width, m_scissor.height);
    }
  }

} // namespace  mgl::opengl
