
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
  ContextCGL::ContextCGL(context_mode::Enum mode, int required)
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
          MGL_CORE_ERROR("cannot find a suitable OpenGL pixel format.");
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
          MGL_CORE_ERROR("CGLSetCurrentContext failed");
          CGLDestroyContext(cgl_context);
          delete res;
          return;
        }
      }
      break;
      case context_mode::SHARE: {
        res->standalone = false;

        CGLContextObj ctx_share = CGLGetCurrentContext();
        if(!ctx_share)
        {
          MGL_CORE_ERROR("(share) CGLGetCurrentContext: cannot detect OpenGL context");
          delete res;
          return;
        }

        CGLPixelFormatObj pixelformat = CGLGetPixelFormat(ctx_share);

        if(!pixelformat)
        {
          MGL_CORE_ERROR("cannot find a suitable OpenGL pixel format");
          delete res;
          return;
        }

        CGLContextObj cgl_context = nullptr;
        CGLError error = CGLCreateContext(pixelformat, ctx_share, &cgl_context);
        if(error != kCGLNoError)
        {
          MGL_CORE_ERROR("(share) CGLCreateContext.");
          delete res;
          return;
        }

        res->ctx = cgl_context;

        res->ctx = cgl_context;

        if(CGLSetCurrentContext(cgl_context) != kCGLNoError)
        {
          MGL_CORE_ERROR("CGLSetCurrentContext failed");
          CGLDestroyContext(cgl_context);
          delete res;
          return;
        }
      }
      break;
      default: {
        MGL_CORE_ERROR("Detect mode not supported");
        delete res;
        return;
      }
      break;
    }

    int gl_version = gladLoaderLoadGL();

    if(!gl_version)
    {
      MGL_CORE_ERROR("Error loading OpenGL");
      return;
    }
    else
    {
      MGL_CORE_INFO(
          "OpenGL {0}.{1}", GLAD_VERSION_MAJOR(gl_version), GLAD_VERSION_MINOR(gl_version));
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
} // namespace  mgl::opengl

#endif