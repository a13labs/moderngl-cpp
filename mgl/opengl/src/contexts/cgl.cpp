#ifdef MGL_OPENGL_CGL

#  define GL_SILENCE_DEPRECATION

#  include "mgl_opengl/context.hpp"

#  include "mgl_core/log.hpp"

#  include <ApplicationServices/ApplicationServices.h>
#  include <OpenGL/OpenGL.h>

#  include "glad/gl.h"

struct CGLContextData
{
  CGLContextObj ctx;
  bool standalone;
  void* old_context;
};

namespace mgl::opengl
{
  ContextCGL::ContextCGL(context_mode::mode mode, int required)
  {
    m_mode = mode;
    m_released = true;
    int device_index = 0;

    m_context = nullptr;

    auto res = new CGLContextData;

    switch(mode)
    {
      case context_mode::STANDALONE: {
        res->standalone = true;

        GLint num_pixelformats = 0;
        CGLPixelFormatObj pixelformat = 0;

        CGLPixelFormatAttribute attribs[] = {
          kCGLPFAOpenGLProfile,
          (CGLPixelFormatAttribute)kCGLOGLPVersion_GL4_Core,
          (CGLPixelFormatAttribute)0,
        };

        CGLChoosePixelFormat(attribs, &pixelformat, &num_pixelformats);

        if(!pixelformat)
        {
          CGLPixelFormatAttribute attribs[] = {
            kCGLPFAOpenGLProfile,
            (CGLPixelFormatAttribute)kCGLOGLPVersion_GL3_Core,
            (CGLPixelFormatAttribute)0,
          };

          CGLChoosePixelFormat(attribs, &pixelformat, &num_pixelformats);

          if(!pixelformat)
          {
            CGLPixelFormatAttribute attribs[] = {
              (CGLPixelFormatAttribute)0,
            };

            CGLChoosePixelFormat(attribs, &pixelformat, &num_pixelformats);
          }
        }

        if(!pixelformat)
        {
          MGL_CORE_ERROR("[CGL Context] (standalone) Cannot find a suitable OpenGL pixel format.");
          delete res;
          return;
        }

        CGLContextObj cgl_context = nullptr;
        CGLCreateContext(pixelformat, nullptr, &cgl_context);
        CGLDestroyPixelFormat(pixelformat);

        if(!cgl_context)
        {
          delete res;
          return;
        }
        res->ctx = cgl_context;

        if(CGLSetCurrentContext(cgl_context) != kCGLNoError)
        {
          MGL_CORE_ERROR("[CGL Context] (standalone) CGLSetCurrentContext failed.");
          CGLDestroyContext(cgl_context);
          delete res;
          return;
        }
      }
      break;
      case context_mode::SHARE: {
        MGL_CORE_ASSERT(false, "[CGL Context] (share) Share mode not supported.")
        break;
      }
      case context_mode::ATTACHED: {
        res->standalone = false;

        CGLContextObj ctx_share = CGLGetCurrentContext();
        if(!ctx_share)
        {
          MGL_CORE_ERROR(
              "[CGL Context] (attached) CGLGetCurrentContext: cannot detect OpenGL context.");
          delete res;
          return;
        }

        res->ctx = ctx_share;

        if(CGLSetCurrentContext(res->ctx) != kCGLNoError)
        {
          MGL_CORE_ERROR("[CGL Context] (attached) (CGLSetCurrentContext failed.");
          CGLDestroyContext(res->ctx);
          delete res;
          return;
        }
        break;
      }
      default: {
        MGL_CORE_ERROR("[CGL Context] Detect mode not supported.");
        delete res;
        return;
        break;
      }
    }

    int gl_version = gladLoaderLoadGL();

    if(!gl_version)
    {
      MGL_CORE_ERROR("[CGL Context] Error loading OpenGL.");
      return;
    }
    else
    {
      MGL_CORE_INFO("[CGL Context] OpenGL version '{0}.{1}' found.",
                    GLAD_VERSION_MAJOR(gl_version),
                    GLAD_VERSION_MINOR(gl_version));
    }

    m_context = res;
    m_released = false;
  }

  ContextCGL::~ContextCGL()
  {
    if(m_context != nullptr)
      delete(CGLContextData*)m_context;
  }

  void ContextCGL::enter()
  {
    if(!m_context)
      return;

    auto self = (CGLContextData*)m_context;

    self->old_context = (void*)CGLGetCurrentContext();
    CGLSetCurrentContext(self->ctx);
  }

  void ContextCGL::exit()
  {
    if(!m_context)
      return;

    auto self = (CGLContextData*)m_context;
    CGLSetCurrentContext((CGLContextObj)self->old_context);
  }

  void ContextCGL::release()
  {
    if(!m_context)
      return;

    auto self = (CGLContextData*)m_context;
    CGLSetCurrentContext(nullptr);
    CGLDestroyContext(self->ctx);
    m_released = true;
  }

  bool ContextCGL::is_valid()
  {
    return m_context != nullptr && !m_released;
  }

  bool ContextCGL::is_current()
  {
    if(!m_context || m_released)
      return false;

    auto self = (CGLContextData*)m_context;
    return self->ctx == CGLGetCurrentContext();
  }
} // namespace  mgl::opengl

#endif