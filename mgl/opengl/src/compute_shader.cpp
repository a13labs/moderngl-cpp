
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
#include "mgl_opengl/compute_shader.hpp"
#include "mgl_opengl/context.hpp"

#include "mgl_core/debug.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  void compute_shader::release()
  {
    if(m_released)
      return;

    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");

    m_released = true;

    glDeleteShader(m_shader_obj);
    glDeleteProgram(m_program_obj);
  }

  void compute_shader::run(int x, int y, int z)
  {
    MGL_CORE_ASSERT(!m_released, "Compute Shader already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");

    glUseProgram(m_program_obj);
    glDispatchCompute(x, y, z);
  }

  const mgl::string_list compute_shader::uniforms()
  {
    auto result = mgl::string_list();

    for(auto&& a : m_uniforms_map)
    {
      result.push_back(a.first);
    }

    return result;
  }

  const mgl::string_list compute_shader::uniform_blocks()
  {
    auto result = mgl::string_list();

    for(auto&& a : m_uniform_blocks_map)
    {
      result.push_back(a.first);
    }

    return result;
  }

} // namespace  mgl::opengl
