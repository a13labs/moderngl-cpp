#ifdef MGL_OPENGL_EGL
#  include "mgl_opengl/context.hpp"

#  include "mgl_core/log.hpp"

#  include "glad/egl.h"
#  include "glad/gl.h"

struct EGLContextData
{
  EGLContext ctx;
  EGLDisplay dpy;
  EGLConfig cfg;
  EGLSurface wnd;

  int standalone;
};
namespace mgl::opengl
{
  ContextEGL::ContextEGL(context_mode::mode mode, int required)
  {
    m_mode = mode;
    m_released = true;
    int device_index = 0;

    m_context = nullptr;

    int egl_version = gladLoaderLoadEGL(nullptr);

    if(!egl_version)
    {
      MGL_CORE_ERROR("Error loading EGL");
      return;
    }

    auto res = new EGLContextData;

    switch(mode)
    {
      case context_mode::STANDALONE: {
        res->standalone = true;
        res->wnd = EGL_NO_SURFACE;

        EGLint num_devices;
        if(!eglQueryDevicesEXT(0, NULL, &num_devices))
        {
          MGL_CORE_ERROR("eglQueryDevicesEXT failed (0x%x)", eglGetError());
          delete res;
          return;
        }

        if(device_index >= num_devices)
        {
          MGL_CORE_ERROR(
              "requested device index %d, but found %d devices", device_index, num_devices);
          delete res;
          return;
        }

        EGLDeviceEXT* devices = (EGLDeviceEXT*)malloc(sizeof(EGLDeviceEXT) * num_devices);
        if(!eglQueryDevicesEXT(num_devices, devices, &num_devices))
        {
          MGL_CORE_ERROR("eglQueryDevicesEXT failed (0x%x)", eglGetError());
          free(devices);
          delete res;
          return;
        }
        EGLDeviceEXT device = devices[device_index];
        free(devices);

        res->dpy = eglGetPlatformDisplayEXT(EGL_PLATFORM_DEVICE_EXT, device, 0);
        if(res->dpy == EGL_NO_DISPLAY)
        {
          MGL_CORE_ERROR("eglGetPlatformDisplayEXT failed (0x%x)", eglGetError());
          delete res;
          return;
        }

        EGLint major, minor;
        if(!eglInitialize(res->dpy, &major, &minor))
        {
          MGL_CORE_ERROR("eglInitialize failed (0x%x)", eglGetError());
          delete res;
          return;
        }

        EGLint config_attribs[] = { EGL_SURFACE_TYPE,
                                    EGL_PBUFFER_BIT,
                                    EGL_BLUE_SIZE,
                                    8,
                                    EGL_GREEN_SIZE,
                                    8,
                                    EGL_RED_SIZE,
                                    8,
                                    EGL_DEPTH_SIZE,
                                    8,
                                    EGL_RENDERABLE_TYPE,
                                    EGL_OPENGL_BIT,
                                    EGL_NONE };

        EGLint num_configs = 0;
        if(!eglChooseConfig(res->dpy, config_attribs, &res->cfg, 1, &num_configs))
        {
          MGL_CORE_ERROR("eglChooseConfig failed (0x%x)", eglGetError());
          delete res;
          return;
        }

        if(!eglBindAPI(EGL_OPENGL_API))
        {
          MGL_CORE_ERROR("eglBindAPI failed (0x%x)", eglGetError());
          delete res;
          return;
        }

        int ctxattribs[] = {
          EGL_CONTEXT_MAJOR_VERSION,
          required / 100 % 10,
          EGL_CONTEXT_MINOR_VERSION,
          required / 10 % 10,
          EGL_CONTEXT_OPENGL_PROFILE_MASK,
          EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
          // EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE, 1,
          EGL_NONE,
        };

        res->ctx = eglCreateContext(res->dpy, res->cfg, EGL_NO_CONTEXT, ctxattribs);
        if(!res->ctx)
        {
          MGL_CORE_ERROR("eglCreateContext failed (0x%x)", eglGetError());
          delete res;
          return;
        }

        if(!eglMakeCurrent(res->dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, res->ctx))
        {
          MGL_CORE_ERROR("eglMakeCurrent failed (0x%x)", eglGetError());
          delete res;
          return;
        }
        break;
      }
      case context_mode::SHARE: {
        res->standalone = false;

        EGLContext ctx_share = eglGetCurrentContext();
        if(!ctx_share)
        {
          MGL_CORE_ERROR("(share) eglGetCurrentContext: cannot detect OpenGL context");
          delete res;
          return;
        }

        res->wnd = eglGetCurrentSurface(EGL_DRAW);
        if(!res->wnd)
        {
          MGL_CORE_ERROR("(share) m_eglGetCurrentSurface failed (0x%x)", eglGetError());
          delete res;
          return;
        }

        res->dpy = eglGetCurrentDisplay();
        if(res->dpy == EGL_NO_DISPLAY)
        {
          MGL_CORE_ERROR("eglGetCurrentDisplay failed (0x%x)", eglGetError());
          delete res;
          return;
        }

        EGLint config_attribs[] = { EGL_SURFACE_TYPE,
                                    EGL_WINDOW_BIT,
                                    EGL_BLUE_SIZE,
                                    8,
                                    EGL_GREEN_SIZE,
                                    8,
                                    EGL_RED_SIZE,
                                    8,
                                    EGL_DEPTH_SIZE,
                                    24,
                                    EGL_RENDERABLE_TYPE,
                                    EGL_OPENGL_BIT,
                                    EGL_NONE };

        EGLint num_configs = 0;
        if(!eglChooseConfig(res->dpy, config_attribs, &res->cfg, 1, &num_configs))
        {
          MGL_CORE_ERROR("eglChooseConfig failed (0x%x)", eglGetError());
          delete res;
          return;
        }

        if(!eglBindAPI(EGL_OPENGL_API))
        {
          MGL_CORE_ERROR("eglBindAPI failed (0x%x)", eglGetError());
          delete res;
          return;
        }

        int ctxattribs[] = {
          EGL_CONTEXT_MAJOR_VERSION,
          required / 100 % 10,
          EGL_CONTEXT_MINOR_VERSION,
          required / 10 % 10,
          EGL_CONTEXT_OPENGL_PROFILE_MASK,
          EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
          // EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE, 1,
          EGL_NONE,
        };

        res->ctx = eglCreateContext(res->dpy, res->cfg, ctx_share, ctxattribs);
        if(!res->ctx)
        {
          MGL_CORE_ERROR("eglCreateContext failed (0x%x)", eglGetError());
          delete res;
          return;
        }

        if(!eglMakeCurrent(res->dpy, res->wnd, res->wnd, res->ctx))
        {
          MGL_CORE_ERROR("eglMakeCurrent failed (0x%x)", eglGetError());
          delete res;
          return;
        }
        break;
      }
      case context_mode::ATTACHED: {
        res->standalone = false;

        EGLContext ctx_share = eglGetCurrentContext();
        if(!ctx_share)
        {
          MGL_CORE_ERROR("(share) eglGetCurrentContext: cannot detect OpenGL context");
          delete res;
          return;
        }

        res->wnd = eglGetCurrentSurface(EGL_DRAW);
        if(!res->wnd)
        {
          MGL_CORE_ERROR("(share) m_eglGetCurrentSurface failed (0x%x)", eglGetError());
          delete res;
          return;
        }

        res->dpy = eglGetCurrentDisplay();
        if(res->dpy == EGL_NO_DISPLAY)
        {
          MGL_CORE_ERROR("eglGetCurrentDisplay failed (0x%x)", eglGetError());
          delete res;
          return;
        }

        res->ctx = ctx_share;
        if(!eglMakeCurrent(res->dpy, res->wnd, res->wnd, res->ctx))
        {
          MGL_CORE_ERROR("eglMakeCurrent failed (0x%x)", eglGetError());
          delete res;
          return;
        }
        break;
      }
      default: {
        MGL_CORE_ERROR("Detect mode not supported");
        delete res;
        return;
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

  ContextEGL::~ContextEGL()
  {
    if(m_context != nullptr)
      delete(EGLContextData*)m_context;
  }

  void ContextEGL::enter()
  {
    if(!m_context)
      return;

    auto self = (EGLContextData*)m_context;
    eglMakeCurrent(self->dpy, self->wnd, self->wnd, self->ctx);
  }

  void ContextEGL::exit()
  {
    if(!m_context)
      return;

    auto self = (EGLContextData*)m_context;
    eglMakeCurrent(self->dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
  }

  void ContextEGL::release()
  {
    if(!m_context)
      return;

    auto self = (EGLContextData*)m_context;
    eglDestroyContext(self->dpy, self->ctx);
    m_released = true;
  }

  bool ContextEGL::is_valid()
  {
    return m_context != nullptr && !m_released;
  }

  bool ContextEGL::is_current()
  {
    if(!m_context || m_released)
      return false;

    auto self = (EGLContextData*)m_context;
    return eglGetCurrentContext() == self->ctx;
  }
} // namespace  mgl::opengl
#endif