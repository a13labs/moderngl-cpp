#include "mgl_opengl/renderbuffer.hpp"
#include "mgl_opengl/context.hpp"
#include "mgl_opengl/framebuffer.hpp"

#include "mgl_opengl_internal/utils.hpp"

#include "mgl_core/debug.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  renderbuffer::renderbuffer(const context_ref& ctx,
                             int32_t w,
                             int32_t h,
                             int32_t components,
                             int32_t samples,
                             const std::string& dtype)
      : gl_object(ctx)
  {
    MGL_CORE_ASSERT(components > 0 && components < 5,
                    "[Renderbuffer] Components must be 1, 2, 3 or 4.");
    MGL_CORE_ASSERT((samples & (samples - 1)) == 0, "[Renderbuffer] Samples must be a power of 2.");
    MGL_CORE_ASSERT(samples <= ctx->max_samples(),
                    "[Renderbuffer] Samples must be less than or equal to {0}.",
                    ctx->max_samples());

    auto data_type = from_dtype(dtype);
    MGL_CORE_ASSERT(data_type, "[Renderbuffer] Invalid data type got '{0}'.", dtype);

    int32_t format = data_type->internal_format[components];

    m_width = w;
    m_height = h;
    m_components = components;
    m_samples = samples;
    m_data_type = data_type;
    m_depth = false;

    GLuint glo = 0;
    glGenRenderbuffers(1, &glo);
    if(!glo)
    {
      MGL_CORE_ASSERT(false, "[Renderbuffer] Cannot create render buffer.");
      return;
    }

    gl_object::set_glo(glo);

    glBindRenderbuffer(GL_RENDERBUFFER, gl_object::glo());

    if(samples == 0)
    {
      glRenderbufferStorage(GL_RENDERBUFFER, format, w, h);
    }
    else
    {
      glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, format, w, h);
    }
  }

  renderbuffer::renderbuffer(const context_ref& ctx, int32_t w, int32_t h, int32_t samples)
      : gl_object(ctx)
  {
    MGL_CORE_ASSERT((samples & (samples - 1)) == 0, "[Renderbuffer] Samples must be a power of 2.");
    MGL_CORE_ASSERT(samples <= ctx->max_samples(),
                    "[Renderbuffer] Samples must be less than or equal to {0}.",
                    ctx->max_samples());

    m_width = w;
    m_height = h;
    m_components = 1;
    m_samples = samples;
    m_data_type = from_dtype("f4", 2);
    m_depth = true;

    GLuint glo = 0;
    glGenRenderbuffers(1, &glo);
    if(!glo)
    {
      MGL_CORE_ERROR("[Renderbuffer] Cannot create render buffer.");
      return;
    }

    gl_object::set_glo(glo);

    glBindRenderbuffer(GL_RENDERBUFFER, gl_object::glo());

    if(samples == 0)
    {
      glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, w, h);
    }
    else
    {
      glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH_COMPONENT24, w, h);
    }
  }

  void renderbuffer::release()
  {
    MGL_CORE_ASSERT(!gl_object::released(),
                    "[Renderbuffer] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Renderbuffer] Resource context not current.");
    GLuint glo = gl_object::glo();
    glDeleteRenderbuffers(1, &glo);
    gl_object::set_glo(GL_ZERO);
  }

} // namespace  mgl::opengl
