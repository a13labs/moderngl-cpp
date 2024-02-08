#pragma once

#include "buffer.hpp"
#include "framebuffer.hpp"
#include "sampler.hpp"
#include "texture.hpp"

#include "mgl_core/containers.hpp"
#include "mgl_core/memory.hpp"

namespace mgl::opengl
{
  class context;
  using context_ref = mgl::ref<context>;

  struct texture_binding
  {
    texture_ref texture;
    int32_t binding;
  };

  struct buffer_binding
  {
    buffer_ref buffer;
    int32_t binding;
  };

  struct sampler_binding
  {
    sampler_ref sampler;
    int32_t binding;
  };

  using texture_bindings = mgl::list<texture_binding>;
  using buffer_bindings = mgl::list<buffer_binding>;
  using sampler_bindings = mgl::list<sampler_binding>;

  class scope
  {
public:
    struct binding_data
    {
      int32_t binding;
      int32_t type;
      int32_t gl_object;
    };

    ~scope();

    void begin();
    void end();

private:
    friend class context;

    scope(const context_ref& ctx,
          framebuffer_ref framebuffer,
          int32_t enable_flags,
          const texture_bindings& textures,
          const buffer_bindings& uniform_buffers,
          const buffer_bindings& storage_buffers,
          const sampler_bindings& samplers);

    struct state
    {
      int32_t enable_flags;
      framebuffer_ref framebuffer;

      state()
          : enable_flags(0)
          , framebuffer(nullptr)
      { }

      state(int32_t flags, const framebuffer_ref& fb)
          : enable_flags(flags)
          , framebuffer(fb)
      { }

      bool operator==(const state& other) const
      {
        return enable_flags == other.enable_flags && framebuffer == other.framebuffer;
      }

      bool operator!=(const state& other) const { return !(*this == other); }

      bool operator==(const state* other) const
      {
        return enable_flags == other->enable_flags && framebuffer == other->framebuffer;
      }

      bool operator!=(const state* other) const { return !(*this == other); }
    };

    bool m_begin;
    context_ref m_ctx;
    sampler_bindings m_samplers;
    mgl::list<scope::binding_data> m_textures;
    mgl::list<scope::binding_data> m_buffers;

    state m_scope_state;
    state m_previous_state;
  };

  using scope_ref = mgl::ref<scope>;

} // namespace  mgl::opengl