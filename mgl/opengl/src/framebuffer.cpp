
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

#include "mgl_opengl_internal/utils.hpp"

#include "mgl_core/debug.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  framebuffer::framebuffer(const attachments_ref& color_attachments,
                           attachment_ref depth_attachment)
  {
    MGL_CORE_ASSERT(color_attachments.size() ||
                        (color_attachments.size() == 0 && depth_attachment != nullptr),
                    "missing color attachments");
    int32_t width = 0;
    int32_t height = 0;
    int32_t samples = 0;

    int32_t i = 0;
    for(auto&& attachment : color_attachments)
    {
      switch(attachment->attachment_type())
      {
        case attachment::type::TEXTURE: {
          auto texture = std::dynamic_pointer_cast<texture_2d>(attachment);
          MGL_CORE_ASSERT(texture, "not a texture2D");

          if(texture->depth())
          {
            MGL_CORE_ASSERT(false, "color_attachments[{0}] is a depth attachment", i);
            return;
          }

          if(i == 0)
          {
            width = texture->width();
            height = texture->height();
            samples = texture->samples();
          }
          else
          {
            if(texture->width() != width || texture->height() != height ||
               texture->samples() != samples)
            {
              MGL_CORE_ASSERT(false, "the color_attachments have different sizes or samples");
              return;
            }
          }
        }
        break;
        case attachment::type::RENDERBUFFER: {
          auto renderbuffer = std::dynamic_pointer_cast<mgl::opengl::renderbuffer>(attachment);
          MGL_CORE_ASSERT(renderbuffer, "not a render buffer");

          if(renderbuffer->depth())
          {
            MGL_CORE_ASSERT(false, "color_attachments[{0}] is a depth attachment", i);
            return;
          }

          if(i == 0)
          {
            width = renderbuffer->width();
            height = renderbuffer->height();
            samples = renderbuffer->samples();
          }
          else
          {
            if(renderbuffer->width() != width || renderbuffer->height() != height ||
               renderbuffer->samples() != samples)
            {
              MGL_CORE_ASSERT(false, "the color_attachments have different sizes or samples");
              return;
            }
          }
        }
        break;
        default: MGL_CORE_ASSERT(false, "invalid attachment type"); return;
      }
      i++;
    }

    if(depth_attachment != nullptr)
    {
      switch(depth_attachment->attachment_type())
      {
        case attachment::type::TEXTURE: {
          auto texture = std::dynamic_pointer_cast<texture_2d>(depth_attachment);
          MGL_CORE_ASSERT(texture, "not a texture_2d");

          if(!texture->depth())
          {
            MGL_CORE_ASSERT(false, "the depth_attachment is a color attachment");
            return;
          }

          if(color_attachments.size())
          {
            if(texture->width() != width || texture->height() != height ||
               texture->samples() != samples)
            {
              MGL_CORE_ASSERT(false, "the depth_attachment have different sizes or samples");
              return;
            }
          }
          else
          {
            width = texture->width();
            height = texture->height();
            samples = texture->samples();
          }
        }
        break;
        case attachment::type::RENDERBUFFER: {
          auto renderbuffer =
              std::dynamic_pointer_cast<mgl::opengl::renderbuffer>(depth_attachment);
          MGL_CORE_ASSERT(renderbuffer, "not a render buffer");

          if(!renderbuffer->depth())
          {
            MGL_CORE_ASSERT(false, "the depth_attachment is a color attachment");
            return;
          }

          if(color_attachments.size())
          {
            if(renderbuffer->width() != width || renderbuffer->height() != height ||
               renderbuffer->samples() != samples)
            {
              MGL_CORE_ASSERT(false, "the depth_attachment have different sizes or samples");
              return;
            }
          }
          else
          {
            width = renderbuffer->width();
            height = renderbuffer->height();
            samples = renderbuffer->samples();
          }
        }
        break;
        default:
          MGL_CORE_ASSERT(false, "the depth_attachment must be a render buffer or texture");
          return;
      }
    }

    m_draw_buffers_len = color_attachments.size();
    m_draw_buffers = new unsigned[color_attachments.size()];
    m_color_masks = color_masks(color_attachments.size());

    for(size_t i = 0; i < color_attachments.size(); ++i)
    {
      auto attachment = color_attachments[i];

      m_draw_buffers[i] = GL_COLOR_ATTACHMENT0 + i;

      switch(attachment->attachment_type())
      {
        case attachment::type::TEXTURE: {
          auto texture = std::dynamic_pointer_cast<texture_2d>(attachment);
          MGL_CORE_ASSERT(texture, "not a texture_2d");
          m_color_masks[i] = { texture->components() >= 1,
                               texture->components() >= 2,
                               texture->components() >= 3,
                               texture->components() >= 4 };
        }
        break;
        case attachment::type::RENDERBUFFER: {
          auto renderbuffer = std::dynamic_pointer_cast<mgl::opengl::renderbuffer>(attachment);
          MGL_CORE_ASSERT(renderbuffer, "not a render buffer");
          m_color_masks[i] = { renderbuffer->components() >= 1,
                               renderbuffer->components() >= 2,
                               renderbuffer->components() >= 3,
                               renderbuffer->components() >= 4 };
        }
        break;
        default: MGL_CORE_ASSERT(false, "invalid attachment type"); return;
      }
    }

    m_depth_mask = (depth_attachment != nullptr);
    m_viewport = { 0, 0, width, height };
    m_dynamic = false;
    m_scissor_enabled = false;
    m_scissor = { 0, 0, width, height };
    m_width = width;
    m_height = height;
    m_samples = samples;

    int32_t glo = 0;
    glGenFramebuffers(1, (GLuint*)&glo);

    if(!glo)
    {
      MGL_CORE_ASSERT(false, "Failed to create framebuffer");
      return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, glo);

    if(!color_attachments.size())
    {
      glDrawBuffer(GL_NONE); // No color buffer is drawn to.
    }

    for(auto&& attachment : color_attachments)
    {
      switch(attachment->attachment_type())
      {
        case attachment::type::TEXTURE: {
          auto texture = std::dynamic_pointer_cast<texture_2d>(attachment);
          MGL_CORE_ASSERT(texture, "not a texture_2d");

          glFramebufferTexture2D(GL_FRAMEBUFFER,
                                 GL_COLOR_ATTACHMENT0 + i,
                                 texture->samples() ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D,
                                 texture->glo(),
                                 0);
        }
        break;
        case attachment::type::RENDERBUFFER: {
          auto renderbuffer = std::dynamic_pointer_cast<mgl::opengl::renderbuffer>(attachment);
          MGL_CORE_ASSERT(renderbuffer, "not a render buffer");

          glFramebufferRenderbuffer(
              GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, renderbuffer->glo());
        }
        break;
        default: MGL_CORE_ASSERT(false, "invalid attachment type"); return;
      }
    }

    if(depth_attachment)
    {
      switch(depth_attachment->attachment_type())
      {
        case attachment::type::TEXTURE: {
          auto texture = std::dynamic_pointer_cast<texture_2d>(depth_attachment);
          MGL_CORE_ASSERT(texture, "not a texture_2d");

          glFramebufferTexture2D(GL_FRAMEBUFFER,
                                 GL_DEPTH_ATTACHMENT,
                                 texture->samples() ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D,
                                 texture->glo(),
                                 0);
        }
        break;
        case attachment::type::RENDERBUFFER: {
          auto renderbuffer =
              std::dynamic_pointer_cast<mgl::opengl::renderbuffer>(depth_attachment);
          MGL_CORE_ASSERT(renderbuffer, "not a render buffer");

          glFramebufferRenderbuffer(
              GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer->glo());
        }
        break;
        default: MGL_CORE_ASSERT(false, "invalid attachment type"); return;
      }
    }

    int32_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    glBindFramebuffer(GL_FRAMEBUFFER, m_bound_framebuffer->m_glo);
    MGL_CORE_ASSERT(status == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete");

    m_glo = glo;
  }

  void framebuffer::release()
  {
    MGL_CORE_ASSERT(m_glo, "Framebuffer already released");
    glDeleteFramebuffers(1, (GLuint*)&m_glo);
    delete[] m_draw_buffers;
    m_glo = 0;
  }

  void
  framebuffer::clear(float r, float g, float b, float a, float depth, const mgl::rect& viewport)
  {
    MGL_CORE_ASSERT(m_glo != 0, "Framebuffer already released");
    glBindFramebuffer(GL_FRAMEBUFFER, m_glo);

    if(m_glo)
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

    glBindFramebuffer(GL_FRAMEBUFFER, m_context->m_bound_framebuffer->m_glo);
  }

  void framebuffer::use()
  {
    MGL_CORE_ASSERT(m_glo != 0, "Framebuffer already released");
    glBindFramebuffer(GL_FRAMEBUFFER, m_glo);

    if(m_glo)
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

  void framebuffer::read(mgl::uint8_buffer& dst,
                         const mgl::rect& viewport,
                         int components,
                         int attachment,
                         int alignment,
                         const char* dtype,
                         size_t write_offset)
  {
    MGL_CORE_ASSERT(m_glo != 0, "Framebuffer already released");
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

    glBindFramebuffer(GL_FRAMEBUFFER, m_glo);
    glReadBuffer(read_depth ? GL_NONE : (GL_COLOR_ATTACHMENT0 + attachment));
    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glReadPixels(x, y, width, height, base_format, pixel_type, ptr);
    glBindFramebuffer(GL_FRAMEBUFFER, m_context->m_bound_framebuffer->m_glo);

    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "OpenGL error");
  }

  void framebuffer::read(buffer_ref dst,
                         const mgl::rect& viewport,
                         int components,
                         int attachment,
                         int alignment,
                         const char* dtype,
                         size_t write_offset)
  {
    MGL_CORE_ASSERT(m_glo != 0, "Framebuffer already released");
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

    glBindBuffer(GL_PIXEL_PACK_BUFFER, dst->glo());
    glBindFramebuffer(GL_FRAMEBUFFER, m_glo);
    glReadBuffer(read_depth ? GL_NONE : (GL_COLOR_ATTACHMENT0 + attachment));
    glPixelStorei(GL_PACK_ALIGNMENT, alignment);
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    glReadPixels(x, y, width, height, base_format, pixel_type, (void*)write_offset);
    glBindFramebuffer(GL_FRAMEBUFFER, m_context->m_bound_framebuffer->m_glo);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "OpenGL error");
  }

  void framebuffer::set_color_mask(const color_masks& masks)
  {
    MGL_CORE_ASSERT(m_glo != 0, "Framebuffer already released");
    MGL_CORE_ASSERT(masks.size() != (size_t)m_draw_buffers_len,
                    "color_mask must be a match buffers len");
    m_color_masks = masks;

    if(m_glo == m_context->m_bound_framebuffer->m_glo)
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
    MGL_CORE_ASSERT(m_glo != 0, "Framebuffer already released");
    m_depth_mask = value;

    if(m_glo == m_context->m_bound_framebuffer->m_glo)
    {
      glDepthMask(m_depth_mask);
    }
  }

  void framebuffer::bits(int& red_bits,
                         int& green_bits,
                         int& blue_bits,
                         int& alpha_bits,
                         int& depth_bits,
                         int& stencil_bits)
  {
    MGL_CORE_ASSERT(m_glo != 0, "Framebuffer already released");
    MGL_CORE_ASSERT(!m_glo, "Only the default_framebuffer have bits");
    glBindFramebuffer(GL_FRAMEBUFFER, m_glo);
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
    glBindFramebuffer(GL_FRAMEBUFFER, m_context->m_bound_framebuffer->m_glo);

    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "OpenGL error");
  }

  void framebuffer::set_viewport(const mgl::rect& r)
  {
    MGL_CORE_ASSERT(m_glo != 0, "Framebuffer already released");

    m_viewport = r;

    if(m_glo == m_context->m_bound_framebuffer->m_glo)
    {
      glViewport(m_viewport.x, m_viewport.y, m_viewport.width, m_viewport.height);
    }
  }

  void framebuffer::set_scissor(const mgl::rect& r)
  {
    MGL_CORE_ASSERT(m_glo != 0, "Framebuffer already released");

    m_scissor = r;

    if(m_glo == m_context->m_bound_framebuffer->m_glo)
    {
      glScissor(m_scissor.x, m_scissor.y, m_scissor.width, m_scissor.height);
    }
  }

} // namespace  mgl::opengl
