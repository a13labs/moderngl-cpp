#include "mgl_opengl/scope.hpp"
#include "mgl_opengl/context.hpp"
#include "mgl_opengl/framebuffer.hpp"
#include "mgl_opengl/sampler.hpp"

#include "mgl_core/debug.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  scope::scope(const context_ref& ctx,
               framebuffer_ref framebuffer,
               int32_t enable_flags,
               const texture_bindings& textures,
               const buffer_bindings& uniform_buffers,
               const buffer_bindings& storage_buffers,
               const sampler_bindings& samplers)
      : m_ctx(ctx)
  {
    MGL_CORE_ASSERT(framebuffer, "Framebuffer is null");
    MGL_CORE_ASSERT(framebuffer->ctx() == m_ctx, "Framebuffer context mismatch");

    m_scope_state = { enable_flags, framebuffer };

    m_textures = mgl::list<scope::binding_data>(textures.size());
    m_buffers = mgl::list<scope::binding_data>(uniform_buffers.size() + storage_buffers.size());
    m_samplers = samplers;

    m_previous_state = { enable_flag::INVALID, nullptr };

    int32_t i = 0;
    for(auto&& t : textures)
    {
      int32_t texture_type;
      int32_t texture_obj;

      MGL_CORE_ASSERT(t.texture, "Texture is null");
      MGL_CORE_ASSERT(t.texture->ctx() == m_ctx, "Texture context mismatch");

      texture_obj = t.texture->glo();

      switch(t.texture->texture_type())
      {
        case texture::type::TEXTURE_2D: {
          auto texture = std::dynamic_pointer_cast<texture_2d>(t.texture);
          MGL_CORE_ASSERT(texture != nullptr, "invalid texture");
          texture_type = texture->samples() ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        }
        break;
        case texture::type::TEXTURE_3D: {
          texture_type = GL_TEXTURE_3D;
        }
        break;
        case texture::type::TEXTURE_CUBE: {
          texture_type = GL_TEXTURE_CUBE_MAP;
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
    MGL_CORE_ASSERT(!m_begin, "Scope not ended");
    m_scope_state = { enable_flag::INVALID, nullptr };
    m_previous_state = { enable_flag::INVALID, nullptr };
    m_samplers.clear();
    m_textures.clear();
    m_buffers.clear();
  }

  void scope::begin()
  {
    MGL_CORE_ASSERT(!m_begin, "Scope already started");
    m_begin = true;

    const int32_t& flags = m_scope_state.enable_flags;

    m_previous_state = { m_ctx->enable_flags(), m_ctx->current_framebuffer() };
    m_ctx->set_enable_flags(flags);

    m_scope_state.framebuffer->use();

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
  }

  void scope::end()
  {
    MGL_CORE_ASSERT(m_begin, "Scope not started");
    const int32_t& flags = m_previous_state.enable_flags;

    m_ctx->set_enable_flags(flags);

    m_previous_state.framebuffer->use();

    m_previous_state = { enable_flag::INVALID, nullptr };

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
