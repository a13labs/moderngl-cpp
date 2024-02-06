
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
#include "mgl_opengl/renderbuffer.hpp"
#include "mgl_opengl/context.hpp"
#include "mgl_opengl/framebuffer.hpp"

#include "mgl_opengl_internal/utils.hpp"

#include "mgl_core/debug.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  renderbuffer::renderbuffer(
      int32_t w, int32_t h, int32_t components, int32_t samples, const std::string& dtype)
  {
    MGL_CORE_ASSERT(components > 0 && components < 5, "Components must be 1, 2, 3 or 4");
    MGL_CORE_ASSERT((samples & (samples - 1)) == 0, "Samples must be a power of 2");
    MGL_CORE_ASSERT(samples <= mgl::opengl::internal::gl_max_samples(),
                    "Samples must be less than or equal to {0}",
                    mgl::opengl::internal::gl_max_samples());

    auto data_type = from_dtype(dtype);
    MGL_CORE_ASSERT(data_type, "Invalid data type got: '{0}'", dtype);

    int32_t format = data_type->internal_format[components];

    m_glo = 0;
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
      MGL_CORE_ASSERT(false, "Cannot create render buffer");
      return;
    }

    m_glo = glo;

    glBindRenderbuffer(GL_RENDERBUFFER, m_glo);

    if(samples == 0)
    {
      glRenderbufferStorage(GL_RENDERBUFFER, format, w, h);
    }
    else
    {
      glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, format, w, h);
    }
  }

  renderbuffer::renderbuffer(int32_t w, int32_t h, int32_t samples)
  {
    MGL_CORE_ASSERT((samples & (samples - 1)) == 0, "Samples must be a power of 2");
    MGL_CORE_ASSERT(samples <= mgl::opengl::internal::gl_max_samples(),
                    "Samples must be less than or equal to {0}",
                    mgl::opengl::internal::gl_max_samples());

    m_glo = 0;
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
      MGL_CORE_ERROR("Cannot create render buffer");
      return;
    }

    m_glo = glo;

    glBindRenderbuffer(GL_RENDERBUFFER, m_glo);

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
    MGL_CORE_ASSERT(m_glo, "render buffer already released");
    glDeleteRenderbuffers(1, (GLuint*)&m_glo);
    m_glo = 0;
  }

} // namespace  mgl::opengl
