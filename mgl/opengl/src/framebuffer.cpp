
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
  framebuffer::framebuffer(context* ctx)
      : gl_object(ctx)
  {
    MGL_CORE_ASSERT(ctx, "invalid context");
    MGL_CORE_ASSERT(ctx->m_default_framebuffer == nullptr, "default framebuffer already created");

    // According to glGet docs:
    // The initial value is GL_BACK if there are back buffers, otherwise it is GL_FRONT.

    // According to glDrawBuffer docs:
    // The symbolic constants GL_FRONT, GL_BACK, GL_LEFT, GL_RIGHT, and GL_FRONT_AND_BACK
    // are not allowed in the bufs array since they may refer to multiple buffers.

    // GL_COLOR_ATTACHMENT0 is causes error: 1282
    // This value is temporarily ignored

    // draw_buffers[0] = GL_COLOR_ATTACHMENT0;
    // draw_buffers[0] = GL_BACK_LEFT;

    int32_t bound_framebuffer = 0;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &bound_framebuffer);

#ifdef MGL_PLATFORM_MACOS
    if(mode == context_mode::STANDALONE)
    {
      int32_t renderbuffer = 0;
      glGenRenderbuffers(1, (GLuint*)&renderbuffer);
      glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
      glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, 4, 4);

      int32_t framebuffer = 0;
      glGenFramebuffers(1, (GLuint*)&framebuffer);
      glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
      glFramebufferRenderbuffer(
          GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, renderbuffer);
      bound_framebuffer = framebuffer;
    }
#endif

    m_draw_buffers_len = 1;
    m_draw_buffers = new unsigned[1];

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glGetIntegerv(GL_DRAW_BUFFER, (int32_t*)&m_draw_buffers[0]);
    glBindFramebuffer(GL_FRAMEBUFFER, bound_framebuffer);

    m_color_masks = { { true, true, true, true } };
    m_depth_mask = true;

    int32_t scissor_box[4] = {};
    glGetIntegerv(GL_SCISSOR_BOX, (int32_t*)&scissor_box);

    m_viewport = { scissor_box[0], scissor_box[1], scissor_box[2], scissor_box[3] };

    m_scissor_enabled = false;
    m_scissor = { scissor_box[0], scissor_box[1], scissor_box[2], scissor_box[3] };

    m_width = scissor_box[2];
    m_height = scissor_box[3];
    m_dynamic = true;
  }

  framebuffer::framebuffer(context* ctx,
                           const attachments_ref& color_attachments,
                           attachment_ref depth_attachment)
      : gl_object(ctx)
  {
    MGL_CORE_ASSERT(color_attachments.size() ||
                        (color_attachments.size() == 0 && depth_attachment != nullptr),
                    "missing color attachments");

    int32_t glo = 0;
    glGenFramebuffers(1, (GLuint*)&glo);

    if(!glo)
    {
      MGL_CORE_ASSERT(false, "Failed to create framebuffer");
      return;
    }

    m_glo = glo;

    glBindFramebuffer(GL_FRAMEBUFFER, m_glo);

    if(!color_attachments.size())
    {
      glDrawBuffer(GL_NONE); // No color buffer is drawn to.
    }

    int32_t width = 0;
    int32_t height = 0;
    int32_t samples = 0;

    m_draw_buffers_len = color_attachments.size();
    m_draw_buffers = new unsigned[color_attachments.size()];
    m_color_masks = color_masks(color_attachments.size());

    int32_t i = 0;
    for(auto&& attachment : color_attachments)
    {
      if(i == 0)
      {
        width = attachment->width();
        height = attachment->height();
        samples = attachment->samples();
      }
      else
      {
        MGL_CORE_ASSERT(attachment->width() == width, "color_attachments have different widths")
        MGL_CORE_ASSERT(attachment->height() == height, "color_attachments have different heights")
        MGL_CORE_ASSERT(attachment->samples() == samples,
                        "color_attachments have different samples")
      }

      MGL_CORE_ASSERT(!attachment->depth(), "color_attachments[{0}] is a depth attachment", i);

      m_draw_buffers[i] = GL_COLOR_ATTACHMENT0 + i;

      m_color_masks[i] = { attachment->components() >= 1,
                           attachment->components() >= 2,
                           attachment->components() >= 3,
                           attachment->components() >= 4 };

      switch(attachment->attachment_type())
      {
        case attachment::type::TEXTURE: {
          glFramebufferTexture2D(GL_FRAMEBUFFER,
                                 GL_COLOR_ATTACHMENT0 + i,
                                 attachment->samples() ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D,
                                 attachment->glo(),
                                 0);
        }
        break;
        case attachment::type::RENDERBUFFER: {
          glFramebufferRenderbuffer(
              GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_RENDERBUFFER, attachment->glo());
        }
        break;
        default: MGL_CORE_ASSERT(false, "invalid attachment type"); return;
      }

      i++;
    }

    if(depth_attachment)
    {
      MGL_CORE_ASSERT(!depth_attachment->depth(), "the depth_attachment is a depth attachment", i);

      if(width != 0 && height != 0 && samples != 0)
      {
        MGL_CORE_ASSERT(depth_attachment->width() == width,
                        "the depth_attachment have different width")
        MGL_CORE_ASSERT(depth_attachment->height() == height,
                        "the depth_attachment have different height")
        MGL_CORE_ASSERT(depth_attachment->samples() == samples,
                        "the depth_attachment have different samples")
      }
      else
      {
        width = depth_attachment->width();
        height = depth_attachment->height();
        samples = depth_attachment->samples();
      }

      switch(depth_attachment->attachment_type())
      {
        case attachment::type::TEXTURE: {
          glFramebufferTexture2D(GL_FRAMEBUFFER,
                                 GL_DEPTH_ATTACHMENT,
                                 depth_attachment->samples() ? GL_TEXTURE_2D_MULTISAMPLE
                                                             : GL_TEXTURE_2D,
                                 depth_attachment->glo(),
                                 0);
        }
        break;
        case attachment::type::RENDERBUFFER: {
          glFramebufferRenderbuffer(
              GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_attachment->glo());
        }
        break;
        default: MGL_CORE_ASSERT(false, "invalid attachment type"); return;
      }
    }

    MGL_CORE_ASSERT(width != 0, "invalid width");
    MGL_CORE_ASSERT(height != 0, "invalid height");
    MGL_CORE_ASSERT(samples != 0, "invalid samples");

    m_depth_mask = (depth_attachment != nullptr);
    m_viewport = { 0, 0, width, height };
    m_dynamic = false;
    m_scissor_enabled = false;
    m_scissor = { 0, 0, width, height };
    m_width = width;
    m_height = height;
    m_samples = samples;

    int32_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    glBindFramebuffer(GL_FRAMEBUFFER, m_ctx->m_bound_framebuffer->glo());
    MGL_CORE_ASSERT(status == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete");
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

    glBindFramebuffer(GL_FRAMEBUFFER, m_ctx->m_bound_framebuffer->glo());
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

    m_ctx->m_bound_framebuffer = framebuffer_ref(this);
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
    glBindFramebuffer(GL_FRAMEBUFFER, m_ctx->m_bound_framebuffer->glo());

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
    glBindFramebuffer(GL_FRAMEBUFFER, m_ctx->m_bound_framebuffer->glo());
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "OpenGL error");
  }

  void framebuffer::set_color_mask(const color_masks& masks)
  {
    MGL_CORE_ASSERT(m_glo != 0, "Framebuffer already released");
    MGL_CORE_ASSERT(masks.size() != (size_t)m_draw_buffers_len,
                    "color_mask must be a match buffers len");
    m_color_masks = masks;

    if(m_glo == m_ctx->m_bound_framebuffer->glo())
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

    if(m_glo == m_ctx->m_bound_framebuffer->glo())
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
    glBindFramebuffer(GL_FRAMEBUFFER, m_ctx->m_bound_framebuffer->glo());

    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "OpenGL error");
  }

  void framebuffer::set_viewport(const mgl::rect& r)
  {
    MGL_CORE_ASSERT(m_glo != 0, "Framebuffer already released");

    m_viewport = r;

    if(m_glo == m_ctx->m_bound_framebuffer->glo())
    {
      glViewport(m_viewport.x, m_viewport.y, m_viewport.width, m_viewport.height);
    }
  }

  void framebuffer::set_scissor(const mgl::rect& r)
  {
    MGL_CORE_ASSERT(m_glo != 0, "Framebuffer already released");

    m_scissor = r;

    if(m_glo == m_ctx->m_bound_framebuffer->glo())
    {
      glScissor(m_scissor.x, m_scissor.y, m_scissor.width, m_scissor.height);
    }
  }

} // namespace  mgl::opengl
