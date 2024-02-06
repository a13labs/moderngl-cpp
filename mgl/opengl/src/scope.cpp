
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

#include "mgl_core/debug.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  scope::scope(framebuffer_ref framebuffer,
               int32_t enable_flags,
               const texture_bindings& textures,
               const buffer_bindings& uniform_buffers,
               const buffer_bindings& storage_buffers,
               const sampler_bindings& samplers)
  {
    m_enable_flags = enable_flags;
    m_old_enable_flags = mgl::opengl::enable_flag::INVALID;
    m_framebuffer = framebuffer;
    m_old_framebuffer = m_bound_framebuffer;
    m_textures = mgl::list<scope::BindingData>(textures.size());
    m_buffers = mgl::list<scope::BindingData>(uniform_buffers.size() + storage_buffers.size());
    m_samplers = samplers;

    int32_t i = 0;
    for(auto&& t : textures)
    {
      int32_t texture_type;
      int32_t texture_obj;

      MGL_CORE_ASSERT(t.texture, "Texture is null");

      switch(t.texture->texture_type())
      {
        case texture::type::TEXTURE_2D: {
          auto texture = std::dynamic_pointer_cast<texture_2d>(t.texture);
          MGL_CORE_ASSERT(texture != nullptr, "invalid texture");
          texture_type = texture->samples() ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
          texture_obj = texture->glo();
        }
        /* code */
        break;
        case texture::type::TEXTURE_3D: {
          auto texture = std::dynamic_pointer_cast<texture_3d>(t.texture);
          MGL_CORE_ASSERT(texture != nullptr, "invalid texture");
          texture_type = GL_TEXTURE_3D;
          texture_obj = texture->glo();
        }
        break;
        case texture::type::TEXTURE_CUBE: {
          auto texture = std::dynamic_pointer_cast<texture_3d>(t.texture);
          MGL_CORE_ASSERT(texture != nullptr, "invalid texture");
          texture_type = GL_TEXTURE_CUBE_MAP;
          texture_obj = texture->glo();
        }
        break;
        default: MGL_CORE_ASSERT(false, "Invalid texture type"); return;
      }

      int32_t binding = t.binding;
      m_textures[i].binding = GL_TEXTURE0 + binding;
      m_textures[i].type = texture_type;
      m_textures[i].gl_object = texture_obj;
      i++;
    }

    i = 0;
    for(auto&& b : uniform_buffers)
    {
      MGL_CORE_ASSERT(b.buffer, "buffer is null");

      m_buffers[i].binding = b.binding;
      m_buffers[i].gl_object = b.buffer->glo();
      m_buffers[i].type = GL_UNIFORM_BUFFER;
      i++;
    }

    for(auto&& b : storage_buffers)
    {
      MGL_CORE_ASSERT(b.buffer, "buffer is null");

      m_buffers[i].binding = b.binding;
      m_buffers[i].gl_object = b.buffer->glo();
      m_buffers[i].type = GL_SHADER_STORAGE_BUFFER;
      i++;
    }

    m_begin = false;
  }

  scope::~scope()
  {
    if(m_begin)
    {
      end();
    }
    m_framebuffer = nullptr;
    m_old_framebuffer = nullptr;
    m_samplers.clear();
    m_textures.clear();
    m_buffers.clear();
  }

  void scope::begin()
  {
    MGL_CORE_ASSERT(!m_begin, "Scope already started");
    m_begin = true;

    const int32_t& flags = m_enable_flags;

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
    MGL_CORE_ASSERT(m_begin, "Scope not started");
    const int32_t& flags = m_old_enable_flags;

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

    for(auto&& sampler : m_samplers)
    {
      MGL_CORE_ASSERT(sampler.sampler, "invalid sampler");
      sampler.sampler->clear(sampler.binding);
    }

    for(auto&& buffer : m_buffers)
    {
      glBindBufferBase(buffer.type, buffer.binding, 0);
    }

    for(auto&& texture : m_textures)
    {
      glActiveTexture(texture.binding);
      glBindTexture(texture.type, 0);
    }

    m_begin = false;
  }

} // namespace  mgl::opengl
