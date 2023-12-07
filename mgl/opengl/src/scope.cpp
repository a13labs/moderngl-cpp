
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
#include "mgl_opengl/scope.hpp"
#include "mgl_opengl/context.hpp"
#include "mgl_opengl/framebuffer.hpp"
#include "mgl_opengl/sampler.hpp"

#include "mgl_core/log.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  void scope::release()
  {
    if(m_released)
    {
      return;
    }

    m_released = true;
    m_framebuffer = nullptr;
    m_old_framebuffer = nullptr;
    m_context = nullptr;
    m_samplers.clear();
    m_textures.clear();
    m_buffers.clear();
  }

  void scope::begin()
  {
    MGL_CORE_ASSERT(!m_released, "Scope released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");

    const int& flags = m_enable_flags;

    m_old_enable_flags = m_context->enable_flags();
    m_context->set_enable_flags(m_enable_flags);

    m_framebuffer->use();

    for(auto&& texture : m_textures)
    {
      glActiveTexture(texture.binding);
      glBindTexture(texture.type, texture.gl_object);
    }

    for(auto&& buffer : m_buffers)
    {
      glBindBufferBase(buffer.type, buffer.binding, buffer.gl_object);
    }

    for(auto&& sampler : m_samplers)
    {
      MGL_CORE_ASSERT(sampler.sampler, "invalid sampler");
      sampler.sampler->use(sampler.binding);
    }

    if(flags & mgl::opengl::enable_flag::BLEND)
    {
      glEnable(GL_BLEND);
    }
    else
    {
      glDisable(GL_BLEND);
    }

    if(flags & mgl::opengl::enable_flag::DEPTH_TEST)
    {
      glEnable(GL_DEPTH_TEST);
    }
    else
    {
      glDisable(GL_DEPTH_TEST);
    }

    if(flags & mgl::opengl::enable_flag::CULL_FACE)
    {
      glEnable(GL_CULL_FACE);
    }
    else
    {
      glDisable(GL_CULL_FACE);
    }

    if(flags & mgl::opengl::enable_flag::RASTERIZER_DISCARD)
    {
      glEnable(GL_RASTERIZER_DISCARD);
    }
    else
    {
      glDisable(GL_RASTERIZER_DISCARD);
    }

    if(flags & mgl::opengl::enable_flag::PROGRAM_POINT_SIZE)
    {
      glEnable(GL_PROGRAM_POINT_SIZE);
    }
    else
    {
      glDisable(GL_PROGRAM_POINT_SIZE);
    }
  }

  void scope::end()
  {
    MGL_CORE_ASSERT(!m_released, "Scope released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const int& flags = m_old_enable_flags;

    m_context->set_enable_flags(m_old_enable_flags);

    m_old_framebuffer->use();

    if(flags & mgl::opengl::enable_flag::BLEND)
    {
      glEnable(GL_BLEND);
    }
    else
    {
      glDisable(GL_BLEND);
    }

    if(flags & mgl::opengl::enable_flag::DEPTH_TEST)
    {
      glEnable(GL_DEPTH_TEST);
    }
    else
    {
      glDisable(GL_DEPTH_TEST);
    }

    if(flags & mgl::opengl::enable_flag::CULL_FACE)
    {
      glEnable(GL_CULL_FACE);
    }
    else
    {
      glDisable(GL_CULL_FACE);
    }

    if(flags & mgl::opengl::enable_flag::RASTERIZER_DISCARD)
    {
      glEnable(GL_RASTERIZER_DISCARD);
    }
    else
    {
      glDisable(GL_RASTERIZER_DISCARD);
    }

    if(flags & mgl::opengl::enable_flag::PROGRAM_POINT_SIZE)
    {
      glEnable(GL_PROGRAM_POINT_SIZE);
    }
    else
    {
      glDisable(GL_PROGRAM_POINT_SIZE);
    }
  }

} // namespace  mgl::opengl
