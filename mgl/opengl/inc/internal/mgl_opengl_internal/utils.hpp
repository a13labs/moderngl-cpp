#pragma once

#include "glad/gl.h"

#include "mgl_core/debug.hpp"

namespace mgl::opengl::internal
{
  inline void clean_glsl_name(char* name, int& name_len)
  {
    if(name_len && name[name_len - 1] == ']')
    {
      name_len -= 1;
      while(name_len && name[name_len] != '[')
      {
        name_len -= 1;
      }
    }
    name[name_len] = 0;
  }

  inline int32_t gl_version_code()
  {
    int32_t major = 0;
    int32_t minor = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);
    return major * 100 + minor * 10;
  }

  inline void framebuffer_error_message(int32_t status)
  {
    const char* message = "the framebuffer is not complete";

    switch(status)
    {
      case GL_FRAMEBUFFER_UNDEFINED: message = "the framebuffer is not complete (UNDEFINED)"; break;

      case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        message = "the framebuffer is not complete (INCOMPLETE_ATTACHMENT)";
        break;

      case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        message = "the framebuffer is not complete (INCOMPLETE_MISSING_ATTACHMENT)";
        break;

      case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
        message = "the framebuffer is not complete (INCOMPLETE_DRAW_BUFFER)";
        break;

      case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
        message = "the framebuffer is not complete (INCOMPLETE_READ_BUFFER)";
        break;

      case GL_FRAMEBUFFER_UNSUPPORTED:
        message = "the framebuffer is not complete (UNSUPPORTED)";
        break;

      case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
        message = "the framebuffer is not complete (INCOMPLETE_MULTISAMPLE)";
        break;

      case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
        message = "the framebuffer is not complete (INCOMPLETE_LAYER_TARGETS)";
        break;
    }

    MGL_CORE_ERROR("{0}", message);
  }

} // namespace mgl::opengl::internal