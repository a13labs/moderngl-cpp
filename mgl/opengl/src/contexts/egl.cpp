
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
#ifdef MGL_OPENGL_EGL
#  include "mgl_opengl/context.hpp"

#  include "mgl_core/log.hpp"

#  include "EGL/egl.h"
#  include "EGL/eglext.h"
#  include <dlfcn.h>

static PFNEGLGETERRORPROC eglGetError = nullptr;
static PFNEGLGETDISPLAYPROC eglGetDisplay = nullptr;
static PFNEGLINITIALIZEPROC eglInitialize = nullptr;
static PFNEGLCHOOSECONFIGPROC eglChooseConfig = nullptr;
static PFNEGLBINDAPIPROC eglBindAPI = nullptr;
static PFNEGLCREATECONTEXTPROC eglCreateContext = nullptr;
static PFNEGLDESTROYCONTEXTPROC eglDestroyContext = nullptr;
static PFNEGLMAKECURRENTPROC eglMakeCurrent = nullptr;
static PFNEGLGETPROCADDRESSPROC eglGetProcAddress = nullptr;

static PFNEGLQUERYDEVICESEXTPROC eglQueryDevicesEXT = nullptr;
static PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT = nullptr;
static PFNEGLGETCURRENTDISPLAYPROC eglGetCurrentDisplay = nullptr;
static PFNEGLGETCURRENTCONTEXTPROC eglGetCurrentContext = nullptr;
static PFNEGLGETCURRENTSURFACEPROC eglGetCurrentSurface = nullptr;

static void* s_lib_egl = nullptr;
static void* s_lib_gl = nullptr;

static bool load_libraries()
{
  if(s_lib_egl || s_lib_gl)
  {
    return true;
  }

  s_lib_gl = dlopen("libGL.so", RTLD_LAZY);
  if(!s_lib_gl)
  {
    MGL_CORE_ERROR("'libGL.so' not loaded");
    return false;
  }

  s_lib_egl = dlopen("libEGL.so", RTLD_LAZY);
  if(!s_lib_egl)
  {
    MGL_CORE_ERROR("'libEGL.so' not loaded");
    return false;
  }

  eglGetError = (PFNEGLGETERRORPROC)dlsym(s_lib_egl, "eglGetError");
  if(!eglGetError)
  {
    MGL_CORE_ERROR("eglGetError not found");
    return false;
  }

  eglGetDisplay = (PFNEGLGETDISPLAYPROC)dlsym(s_lib_egl, "eglGetDisplay");
  if(!eglGetDisplay)
  {
    MGL_CORE_ERROR("eglGetDisplay not found");
    return false;
  }

  eglInitialize = (PFNEGLINITIALIZEPROC)dlsym(s_lib_egl, "eglInitialize");
  if(!eglInitialize)
  {
    MGL_CORE_ERROR("eglInitialize not found");
    return false;
  }

  eglChooseConfig = (PFNEGLCHOOSECONFIGPROC)dlsym(s_lib_egl, "eglChooseConfig");
  if(!eglChooseConfig)
  {
    MGL_CORE_ERROR("eglChooseConfig not found");
    return false;
  }

  eglBindAPI = (PFNEGLBINDAPIPROC)dlsym(s_lib_egl, "eglBindAPI");
  if(!eglBindAPI)
  {
    MGL_CORE_ERROR("eglBindAPI not found");
    return false;
  }

  eglCreateContext = (PFNEGLCREATECONTEXTPROC)dlsym(s_lib_egl, "eglCreateContext");
  if(!eglCreateContext)
  {
    MGL_CORE_ERROR("eglCreateContext not found");
    return false;
  }

  eglDestroyContext = (PFNEGLDESTROYCONTEXTPROC)dlsym(s_lib_egl, "eglDestroyContext");
  if(!eglDestroyContext)
  {
    MGL_CORE_ERROR("eglDestroyContext not found");
    return false;
  }

  eglMakeCurrent = (PFNEGLMAKECURRENTPROC)dlsym(s_lib_egl, "eglMakeCurrent");
  if(!eglMakeCurrent)
  {
    MGL_CORE_ERROR("eglMakeCurrent not found");
    return false;
  }

  eglGetProcAddress = (PFNEGLGETPROCADDRESSPROC)dlsym(s_lib_egl, "eglGetProcAddress");
  if(!eglGetProcAddress)
  {
    MGL_CORE_ERROR("eglGetProcAddress not found");
    return false;
  }

  eglQueryDevicesEXT = (PFNEGLQUERYDEVICESEXTPROC)eglGetProcAddress("eglQueryDevicesEXT");
  if(!eglQueryDevicesEXT)
  {
    MGL_CORE_ERROR("eglQueryDevicesEXT not found");
    return false;
  }

  eglGetPlatformDisplayEXT =
      (PFNEGLGETPLATFORMDISPLAYEXTPROC)eglGetProcAddress("eglGetPlatformDisplayEXT");
  if(!eglGetPlatformDisplayEXT)
  {
    MGL_CORE_ERROR("eglGetPlatformDisplayEXT not found");
    return false;
  }

  eglGetCurrentDisplay = (PFNEGLGETCURRENTDISPLAYPROC)eglGetProcAddress("eglGetCurrentDisplay");
  if(!eglGetCurrentDisplay)
  {
    MGL_CORE_ERROR("eglGetCurrentDisplay not found");
    return false;
  }

  eglGetCurrentContext = (PFNEGLGETCURRENTCONTEXTPROC)eglGetProcAddress("eglGetCurrentContext");
  if(!eglGetCurrentContext)
  {
    MGL_CORE_ERROR("eglGetCurrentContext not found");
    return false;
  }

  eglGetCurrentSurface = (PFNEGLGETCURRENTSURFACEPROC)eglGetProcAddress("eglGetCurrentSurface");
  if(!eglGetCurrentSurface)
  {
    MGL_CORE_ERROR("eglGetCurrentSurface not found");
    return false;
  }

  return true;
}

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
  ContextEGL::ContextEGL(context_mode::Enum mode, int required)
  {
    m_mode = mode;
    m_released = true;
    int device_index = 0;

    m_context = nullptr;

    if(!load_libraries())
    {
      MGL_CORE_ERROR("Error loading libegl.so or libgl.so");
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

        eglMakeCurrent(res->dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, res->ctx);
      }
      break;
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

        eglMakeCurrent(res->dpy, res->wnd, res->wnd, res->ctx);
      }
      break;
      default: {
        MGL_CORE_ERROR("Detect mode not supported");
        delete res;
        return;
      }
      break;
    }

    m_context = res;
    m_released = false;
  }

  ContextEGL::~ContextEGL()
  {
    if(m_context != nullptr)
      delete(EGLContextData*)m_context;
  }

  gl_function ContextEGL::load(const mgl::core::string& method)
  {
    if(!m_context)
      return nullptr;

    auto self = (EGLContextData*)m_context;

    void* proc = (void*)dlsym(s_lib_gl, method.c_str());
    if(!proc)
    {
      proc = (void*)eglGetProcAddress(method.c_str());
    }

    return proc;
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
} // namespace  mgl::opengl
#endif