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

  inline int swizzle_from_char(char c)
  {
    switch(c)
    {
      case 'R':
      case 'r': return GL_RED;
      case 'G':
      case 'g': return GL_GREEN;
      case 'B':
      case 'b': return GL_BLUE;
      case 'A':
      case 'a': return GL_ALPHA;
      case '0': return GL_ZERO;
      case '1': return GL_ONE;
    }
    MGL_CORE_ASSERT(false, "Invalid swizzle");
    return 0;
  }

  inline char char_from_swizzle(int c)
  {
    switch(c)
    {
      case GL_RED: return 'R';
      case GL_GREEN: return 'G';
      case GL_BLUE: return 'B';
      case GL_ALPHA: return 'A';
      case GL_ZERO: return '0';
      case GL_ONE: return '1';
    }
    MGL_CORE_ASSERT(false, "Invalid swizzle");
    return '?';
  }

} // namespace mgl::opengl::internal