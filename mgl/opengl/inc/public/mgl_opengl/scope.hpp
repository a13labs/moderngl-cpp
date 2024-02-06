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
  struct texture_binding
  {
    texture_ref texture;
    int binding;
  };

  struct buffer_binding
  {
    buffer_ref buffer;
    int binding;
  };

  struct sampler_binding
  {
    sampler_ref sampler;
    int binding;
  };

  using texture_bindings = mgl::list<texture_binding>;
  using buffer_bindings = mgl::list<buffer_binding>;
  using sampler_bindings = mgl::list<sampler_binding>;

  class scope
  {
public:
    struct BindingData
    {
      int binding;
      int type;
      int gl_object;
    };

    ~scope() = default;

    void release();
    bool released();

    void begin();
    void end();

private:
    friend class context;
    scope() = default;

    context* m_context;
    framebuffer_ref m_framebuffer;
    framebuffer_ref m_old_framebuffer;
    sampler_bindings m_samplers;
    mgl::list<scope::BindingData> m_textures;
    mgl::list<scope::BindingData> m_buffers;
    int m_enable_flags;
    int m_old_enable_flags;
    bool m_released;
  };

  using scope_ref = mgl::ref<scope>;

  inline bool scope::released()
  {
    return m_released;
  }
} // namespace  mgl::opengl