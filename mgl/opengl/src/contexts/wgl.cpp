#ifdef MGL_OPENGL_WGL

#  include "mgl_opengl/context.hpp"

#  include "mgl_core/log.hpp"

#  include "glad/wgl.h"

static HINSTANCE hinst;

struct WGLContextData
{
  HGLRC ctx;
  HDC dc;
  HWND wnd;
  bool standalone;
};

namespace mgl::opengl
{
  ContextWGL::ContextWGL(context_mode::mode mode, int required)
  {
    m_mode = mode;
    m_released = true;
    int device_index = 0;

    m_context = nullptr;

    int wgl_version = gladLoaderLoadWGL(nullptr);

    if(!wgl_version)
    {
      MGL_CORE_ERROR("Error loading WGL");
      return;
    }

    auto res = new WGLContextData;

    switch(mode)
    {
      case context_mode::STANDALONE: {
        res->standalone = true;
        res->wnd = CreateWindow("glcontext", NULL, 0, 0, 0, 0, 0, NULL, NULL, hinst, NULL);

        if(!res->wnd)
        {
          MGL_CORE_ERROR("Error creating window");
          delete res;
          return;
        }

        res->dc = GetDC(res->wnd);

        if(!res->dc)
        {
          MGL_CORE_ERROR("Error getting device context");
          delete res;
          return;
        }

        PIXELFORMATDESCRIPTOR pfd = {
          sizeof(PIXELFORMATDESCRIPTOR),
          1,
          PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER,
          0,
          24,
        };

        int pixelformat = ChoosePixelFormat(res->dc, &pfd);

        if(!pixelformat)
        {
          MGL_CORE_ERROR("Error choosing pixel format");
          delete res;
          return;
        }

        if(!SetPixelFormat(res->dc, pixelformat, &pfd))
        {
          MGL_CORE_ERROR("Error setting pixel format");
          delete res;
          return;
        }

        int attribs[] = {
          WGL_CONTEXT_PROFILE_MASK_ARB,
          WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
          WGL_CONTEXT_MAJOR_VERSION_ARB,
          required / 100 % 10,
          WGL_CONTEXT_MINOR_VERSION_ARB,
          required / 10 % 10,
          0,
          0,
        };

        res->ctx = wglCreateContextAttribsARB(res->dc, 0, attribs);

        if(!res->ctx)
        {
          MGL_CORE_ERROR("Error creating context");
          delete res;
          return;
        }

        if(!wglMakeCurrent(res->dc, res->ctx))
        {
          MGL_CORE_ERROR("Error making context current");
          delete res;
          return;
        }
        break;
      }
      case context_mode::SHARE: {
        res->standalone = false;
        res->wnd = nullptr;

        HGLRC ctx_share = wglGetCurrentContext();

        if(!ctx_share)
        {
          MGL_CORE_ERROR("(share) wglGetCurrentContext: cannot detect OpenGL context");
          delete res;
          return;
        }

        res->dc = wglGetCurrentDC();

        if(!res->dc)
        {
          MGL_CORE_ERROR("(share) wglGetCurrentDC: cannot detect OpenGL context");
          delete res;
          return;
        }

        int attribs[] = {
          WGL_CONTEXT_PROFILE_MASK_ARB,
          WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
          WGL_CONTEXT_MAJOR_VERSION_ARB,
          required / 100 % 10,
          WGL_CONTEXT_MINOR_VERSION_ARB,
          required / 10 % 10,
          0,
          0,
        };

        res->ctx = wglCreateContextAttribsARB(res->dc, ctx_share, attribs);

        if(!res->ctx)
        {
          MGL_CORE_ERROR("(share) wglCreateContextAttribsARB failed (0x%x)", GetLastError());
          delete res;
          return;
        }

        if(!wglMakeCurrent(res->dc, res->ctx))
        {
          MGL_CORE_ERROR("(share) wglMakeCurrent failed (0x%x)", GetLastError());
          delete res;
          return;
        }
        break;
      }
      case context_mode::ATTACHED: {
        res->standalone = false;
        res->wnd = nullptr;

        HGLRC ctx_share = wglGetCurrentContext();

        if(!ctx_share)
        {
          MGL_CORE_ERROR("(share) wglGetCurrentContext: cannot detect OpenGL context");
          delete res;
          return;
        }

        res->dc = wglGetCurrentDC();

        if(!res->dc)
        {
          MGL_CORE_ERROR("(share) wglGetCurrentDC: cannot detect OpenGL context");
          delete res;
          return;
        }

        res->ctx = ctx_share;

        if(!wglMakeCurrent(res->dc, res->ctx))
        {
          MGL_CORE_ERROR("(share) wglMakeCurrent failed (0x%x)", GetLastError());
          delete res;
          return;
        }
        break;
      }
      default: {
        MGL_CORE_ERROR("Invalid context mode");
        delete res;
        return;
        break;
      }
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

  ContextWGL::~ContextWGL()
  {
    if(!m_released)
    {
      auto res = (WGLContextData*)m_context;

      if(res->ctx)
      {
        wglMakeCurrent(res->dc, nullptr);
        wglDeleteContext(res->ctx);
      }

      if(res->wnd)
      {
        ReleaseDC(res->wnd, res->dc);
      }

      delete res;
    }
  }

  void ContextWGL::enter()
  {
    if(!m_context)
      return;

    auto self = (WGLContextData*)m_context;
    wglMakeCurrent(self->dc, self->ctx);
  }

  void ContextWGL::exit()
  {
    if(!m_context)
      return;

    auto self = (WGLContextData*)m_context;
    wglMakeCurrent(self->dc, nullptr);
  }

  void ContextWGL::release()
  {
    if(!m_context)
      return;

    auto self = (WGLContextData*)m_context;

    if(self->ctx)
    {
      wglMakeCurrent(self->dc, nullptr);
      wglDeleteContext(self->ctx);
    }

    if(self->wnd)
    {
      ReleaseDC(self->wnd, self->dc);
    }

    delete self;
    m_context = nullptr;
    m_released = true;
  }

  bool ContextWGL::is_valid()
  {
    return m_context != nullptr && !m_released;
  }

} // namespace mgl::opengl

#endif