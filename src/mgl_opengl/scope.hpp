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
#include "builtins.hpp"

namespace mgl::opengl
{
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
    mgl::core::list<scope::BindingData> m_textures;
    mgl::core::list<scope::BindingData> m_buffers;
    int m_enable_flags;
    int m_old_enable_flags;
    bool m_released;
  };

  inline bool scope::released()
  {
    return m_released;
  }
} // namespace  mgl::opengl