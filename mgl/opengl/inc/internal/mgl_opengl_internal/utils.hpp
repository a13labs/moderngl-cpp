#pragma once

#include "glad/gl.h"

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
} // namespace mgl::opengl::internal