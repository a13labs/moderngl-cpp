#include "mgl_opengl/framebuffer.hpp"
#include "mgl_opengl/buffer.hpp"
#include "mgl_opengl/context.hpp"
#include "mgl_opengl/data_type.hpp"

#include "mgl_opengl_internal.hpp"

#include "mgl_core/debug.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  framebuffer::framebuffer(const context_ref& ctx)
      : gl_object(ctx)
  {
    MGL_CORE_ASSERT(gl_object::ctx(), "[Framebuffer] Invalid context.");
    MGL_CORE_ASSERT(gl_object::ctx()->m_default_framebuffer == nullptr,
                    "[Framebuffer] Default framebuffer already created.");

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
    if(gl_object::ctx()->mode() == context_mode::STANDALONE)
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
    // m_draw_buffers = { GL_BACK_LEFT };
    m_draw_buffers.reserve(1);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GLint draw_buffer = 0;
    glGetIntegerv(GL_DRAW_BUFFER, &draw_buffer);
    m_draw_buffers.push_back(draw_buffer);

    glBindFramebuffer(GL_FRAMEBUFFER, bound_framebuffer);

    m_color_masks = { { true, true, true, true } };
    m_depth_mask = true;

    GLint scissor_box[4] = {};
    glGetIntegerv(GL_SCISSOR_BOX, (GLint*)&scissor_box);

    m_viewport = { scissor_box[0], scissor_box[1], scissor_box[2], scissor_box[3] };

    m_scissor_enabled = false;
    m_scissor = { scissor_box[0], scissor_box[1], scissor_box[2], scissor_box[3] };

    m_width = scissor_box[2];
    m_height = scissor_box[3];
    m_dynamic = true;

    // clear any errors
    glGetError();
  }

  framebuffer::framebuffer(const context_ref& ctx,
                           const attachments_ref& color_attachments,
                           attachment_ref depth_attachment)
      : gl_object(ctx)
  {
    MGL_CORE_ASSERT(color_attachments.size() ||
                        (color_attachments.size() == 0 && depth_attachment != nullptr),
                    "[Framebuffer] Missing color attachments.");

    GLuint glo = 0;
    int32_t width = 0;
    int32_t height = 0;
    int32_t samples = 0;

    glGenFramebuffers(1, &glo);

    if(!glo)
    {
      MGL_CORE_ASSERT(false, "[Framebuffer] Failed to create framebuffer.");
      return;
    }

    gl_object::set_glo(glo);
    glBindFramebuffer(GL_FRAMEBUFFER, gl_object::glo());

    if(!color_attachments.size())
    {
      glDrawBuffer(GL_NONE); // No color buffer is drawn to.
    }
    else
    {
      m_draw_buffers.reserve(color_attachments.size());
      m_color_masks.reserve(color_attachments.size());

      int32_t i = 0;
      for(auto&& attachment : color_attachments)
      {
        MGL_CORE_ASSERT(!attachment->depth(),
                        "[Framebuffer] 'color_attachments[{0}]' is a depth attachment.",
                        i);
        MGL_CORE_ASSERT(attachment->ctx() == gl_object::ctx(),
                        "[Framebuffer] 'color_attachments[{0}]' is from a different context.",
                        i);

        if(i == 0)
        {
          width = attachment->width();
          height = attachment->height();
          samples = attachment->samples();
        }
        else
        {
          MGL_CORE_ASSERT(attachment->width() == width,
                          "[Framebuffer] 'color_attachments[{0}]' have different widths.",
                          i)
          MGL_CORE_ASSERT(attachment->height() == height,
                          "[Framebuffer] 'color_attachments[{0}]' have different heights.",
                          i)
          MGL_CORE_ASSERT(attachment->samples() == samples,
                          "[Framebuffer] 'color_attachments[{0}]' have different samples.",
                          i)
        }

        GLenum draw_buffer = GL_COLOR_ATTACHMENT0 + i;
        m_draw_buffers.push_back(draw_buffer);
        m_color_masks.push_back({ attachment->components() >= 1,
                                  attachment->components() >= 2,
                                  attachment->components() >= 3,
                                  attachment->components() >= 4 });

        switch(attachment->attachment_type())
        {
          case attachment::type::TEXTURE: {
            glFramebufferTexture2D(GL_FRAMEBUFFER,
                                   draw_buffer,
                                   attachment->samples() ? GL_TEXTURE_2D_MULTISAMPLE
                                                         : GL_TEXTURE_2D,
                                   attachment->glo(),
                                   0);
          }
          break;
          case attachment::type::RENDERBUFFER: {
            glFramebufferRenderbuffer(
                GL_FRAMEBUFFER, draw_buffer, GL_RENDERBUFFER, attachment->glo());
          }
          break;
          default: MGL_CORE_ASSERT(false, "[Framebuffer] Invalid attachment type."); return;
        }

        i++;
      }

      // clear any errors
      glGetError();
    }

    if(depth_attachment)
    {
      MGL_CORE_ASSERT(depth_attachment->depth(),
                      "[Framebuffer] 'depth_attachment' is not a depth attachment.");
      MGL_CORE_ASSERT(depth_attachment->ctx() == gl_object::ctx(),
                      "[Framebuffer] 'depth_attachment' is from a different context.");

      if(width != 0 && height != 0 && samples != 0)
      {
        MGL_CORE_ASSERT(depth_attachment->width() == width,
                        "[Framebuffer] 'depth_attachment' have different width.")
        MGL_CORE_ASSERT(depth_attachment->height() == height,
                        "[Framebuffer] 'depth_attachment' have different height.")
        MGL_CORE_ASSERT(depth_attachment->samples() == samples,
                        "[Framebuffer] 'depth_attachment' have different samples.")
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
        default: MGL_CORE_ASSERT(false, "[Framebuffer] Invalid attachment. type"); return;
      }
    }

    MGL_CORE_ASSERT(width != 0,
                    "[Framebuffer] Invalid width, neither color nor depth attachments.");
    MGL_CORE_ASSERT(height != 0,
                    "[Framebuffer] Invalid height, neither color nor depth attachments.");

    m_depth_mask = (depth_attachment != nullptr);
    m_viewport = { 0, 0, width, height };
    m_dynamic = false;
    m_scissor_enabled = false;
    m_scissor = { 0, 0, width, height };
    m_width = width;
    m_height = height;
    m_samples = samples;

#ifdef MGL_CORE_ENABLE_ASSERTS
    int32_t status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    MGL_CORE_ASSERT(status == GL_FRAMEBUFFER_COMPLETE,
                    "[Framebuffer] Framebuffer is not complete.");
#endif

    glBindFramebuffer(GL_FRAMEBUFFER, gl_object::ctx()->m_bound_framebuffer->glo());
  }

  void framebuffer::release()
  {
    MGL_CORE_ASSERT(!m_dynamic && !gl_object::released() || m_dynamic,
                    "[Framebuffer] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Framebuffer] Resource context not current.");
    GLuint glo = gl_object::glo();
    glDeleteFramebuffers(1, &glo);
    m_draw_buffers.clear();
    gl_object::set_glo(GL_ZERO);
  }

  void
  framebuffer::clear(float r, float g, float b, float a, float depth, const mgl::rect& viewport)
  {
    MGL_CORE_ASSERT(!m_dynamic && !gl_object::released() || m_dynamic,
                    "[Framebuffer] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Framebuffer] Resource context not current.");
    glBindFramebuffer(GL_FRAMEBUFFER, gl_object::glo());

    if(m_dynamic)
    {
      glDrawBuffers(m_draw_buffers.size(), m_draw_buffers.data());
    }

    glClearColor(r, g, b, a);
    glClearDepth(depth);

    for(int32_t i = 0; i < m_color_masks.size(); ++i)
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

    glBindFramebuffer(GL_FRAMEBUFFER, gl_object::ctx()->m_bound_framebuffer->glo());

    // clear any errors
    glGetError();
  }

  void framebuffer::use()
  {
    MGL_CORE_ASSERT(!m_dynamic && !gl_object::released() || m_dynamic,
                    "[Framebuffer] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Framebuffer] Resource context not current.");
    glBindFramebuffer(GL_FRAMEBUFFER, gl_object::glo());

    if(m_dynamic)
    {
      glDrawBuffers(m_draw_buffers.size(), m_draw_buffers.data());
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

    for(int32_t i = 0; i < m_color_masks.size(); ++i)
    {
      glColorMaski(
          i, m_color_masks[i].r, m_color_masks[i].g, m_color_masks[i].b, m_color_masks[i].a);
    }

    glDepthMask(m_depth_mask);

    gl_object::ctx()->m_bound_framebuffer = shared_from_this();

    // clear any error
    glGetError();
  }

  void framebuffer::read(mgl::uint8_buffer& dst,
                         const mgl::rect& v,
                         int32_t components,
                         int32_t attachment,
                         int32_t align,
                         const char* dtype,
                         size_t dst_off)
  {
    MGL_CORE_ASSERT(!m_dynamic && !gl_object::released() || m_dynamic,
                    "[Framebuffer] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Framebuffer] Resource context not current.");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "[Framebuffer] Alignment must be 1, 2, 4 or 8.");

    data_type* data_type = from_dtype(dtype);
    MGL_CORE_ASSERT(data_type != nullptr, "[Framebuffer] Invalid data type.");

    bool read_depth = false;

    if(attachment == -1)
    {
      MGL_CORE_ASSERT(m_depth_mask, "[Framebuffer] No depth attachment.");
      components = 1;
      read_depth = true;
    }
    else
    {
      MGL_CORE_ASSERT(attachment < m_draw_buffers.size(), "[Framebuffer] Invalid attachment.");
    }

    mgl::rect view = v;
    if(view == mgl::null_viewport_2d)
    {
      view = { 0, 0, m_width, m_height };
    }

    size_t expected_size = view.width * components * data_type->size;
    expected_size = (expected_size + align - 1) / align * align;
    expected_size = expected_size * view.height;

    MGL_CORE_ASSERT(dst.size() >= dst_off + expected_size,
                    "[Framebuffer] Destination out of bounds.");

    int32_t pixel_type = data_type->gl_type;
    int32_t base_format = read_depth ? GL_DEPTH_COMPONENT : data_type->base_format[components];

    char* ptr = (char*)dst.data() + dst_off;

    glBindFramebuffer(GL_FRAMEBUFFER, gl_object::glo());
    glReadBuffer(read_depth ? GL_NONE : (GL_COLOR_ATTACHMENT0 + attachment));
    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glReadPixels(view.x, view.y, view.width, view.height, base_format, pixel_type, ptr);
    glBindFramebuffer(GL_FRAMEBUFFER, gl_object::ctx()->m_bound_framebuffer->glo());
  }

  void framebuffer::read(buffer_ref dst,
                         const mgl::rect& v,
                         int32_t components,
                         int32_t attachment,
                         int32_t align,
                         const char* dtype,
                         size_t dst_off)
  {
    MGL_CORE_ASSERT(!m_dynamic && !gl_object::released() || m_dynamic,
                    "[Framebuffer] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Framebuffer] Resource context not current.");
    MGL_CORE_ASSERT(align == 1 || align == 2 || align == 4 || align == 8,
                    "[Framebuffer] Alignment must be 1, 2, 4 or 8.");
    MGL_CORE_ASSERT(dst->ctx() == gl_object::ctx(), "buffer is from a different context");

    data_type* data_type = from_dtype(dtype);
    MGL_CORE_ASSERT(data_type != nullptr, "[Framebuffer] Invalid data type.");

    bool read_depth = false;

    if(attachment == -1)
    {
      MGL_CORE_ASSERT(m_depth_mask, "[Framebuffer] No depth attachment.");
      components = 1;
      read_depth = true;
    }
    else
    {
      MGL_CORE_ASSERT(attachment < m_draw_buffers.size(), "[Framebuffer] Invalid attachment.");
    }

    mgl::rect view = v;
    if(view == mgl::null_viewport_2d)
    {
      view = { 0, 0, m_width, m_height };
    }

    int32_t pixel_type = data_type->gl_type;
    int32_t base_format = read_depth ? GL_DEPTH_COMPONENT : data_type->base_format[components];

    glBindBuffer(GL_PIXEL_PACK_BUFFER, dst->glo());
    glBindFramebuffer(GL_FRAMEBUFFER, gl_object::glo());
    glReadBuffer(read_depth ? GL_NONE : (GL_COLOR_ATTACHMENT0 + attachment));
    glPixelStorei(GL_PACK_ALIGNMENT, align);
    glPixelStorei(GL_UNPACK_ALIGNMENT, align);
    glReadPixels(view.x, view.y, view.width, view.height, base_format, pixel_type, (void*)dst_off);
    glBindFramebuffer(GL_FRAMEBUFFER, gl_object::ctx()->m_bound_framebuffer->glo());
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
  }

  void framebuffer::set_color_mask(const opengl::color_mask& mask)
  {
    MGL_CORE_ASSERT(m_color_masks.size() == 1,
                    "[Framebuffer] color_mask must be a match buffers len.");
    m_color_masks[0] = mask;

    if(gl_object::ctx()->m_bound_framebuffer.get() != this)
    {
      return;
    }

    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Framebuffer] Resource context not current.");
    glColorMask(mask.r, mask.g, mask.b, mask.a);
  }

  void framebuffer::set_color_mask(const color_masks& masks)
  {
    MGL_CORE_ASSERT(masks.size() == m_color_masks.size(),
                    "[Framebuffer] color_mask must be a match buffers len.");
    m_color_masks = masks;

    if(gl_object::ctx()->m_bound_framebuffer.get() != this)
    {
      return;
    }

    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Framebuffer] Resource context not current.");
    for(int32_t i = 0; i < m_color_masks.size(); ++i)
    {
      glColorMaski(
          i, m_color_masks[i].r, m_color_masks[i].g, m_color_masks[i].b, m_color_masks[i].a);
    }
  }

  void framebuffer::set_depth_mask(bool value)
  {
    m_depth_mask = value;

    if(gl_object::ctx()->m_bound_framebuffer.get() != this)
    {
      return;
    }

    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Framebuffer] Resource context not current.");
    glDepthMask(m_depth_mask);
  }

  void framebuffer::bits(int32_t& red_bits,
                         int32_t& green_bits,
                         int32_t& blue_bits,
                         int32_t& alpha_bits,
                         int32_t& depth_bits,
                         int32_t& stencil_bits)
  {
    MGL_CORE_ASSERT(!m_dynamic && !gl_object::released() || m_dynamic,
                    "[Framebuffer] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Framebuffer] Resource context not current.");
    glBindFramebuffer(GL_FRAMEBUFFER, gl_object::glo());
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
    glBindFramebuffer(GL_FRAMEBUFFER, gl_object::ctx()->m_bound_framebuffer->glo());
    MGL_CORE_ASSERT(glGetError() == GL_NO_ERROR, "OpenGL error");
  }

  void framebuffer::set_viewport(const mgl::rect& r)
  {
    m_viewport = r;

    if(gl_object::ctx()->m_bound_framebuffer.get() != this)
    {
      return;
    }

    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Framebuffer] Resource context not current.");
    glViewport(m_viewport.x, m_viewport.y, m_viewport.width, m_viewport.height);
  }

  void framebuffer::set_scissor(const mgl::rect& r)
  {
    m_scissor = r;

    if(gl_object::ctx()->m_bound_framebuffer.get() != this)
    {
      return;
    }

    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Framebuffer] Resource context not current.");
    glScissor(m_scissor.x, m_scissor.y, m_scissor.width, m_scissor.height);
  }

} // namespace  mgl::opengl
