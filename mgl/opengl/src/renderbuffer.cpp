
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

#include "mgl_core/debug.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  void renderbuffer::release()
  {
    if(m_released)
    {
      return;
    }

    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");

    m_released = true;
    glDeleteRenderbuffers(1, (GLuint*)&m_renderbuffer_obj);
  }

  attachment::type renderbuffer::attachment_type()
  {
    return attachment::RENDERBUFFER;
  }

} // namespace  mgl::opengl
