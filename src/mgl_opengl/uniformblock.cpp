
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
#include "uniformblock.hpp"
#include "context.hpp"
#include "mgl_core/log.hpp"

namespace mgl::opengl
{
  uniform_block::uniform_block(const mgl::core::string& name, int program_obj, int index, size_t size, context* ctx)
  {
    m_context = ctx;
    m_name = name;
    m_program_obj = program_obj;
    m_index = index;
    m_size = size;
  }

  int uniform_block::binding()
  {
    const GLMethods& gl = m_context->gl();

    int binding = 0;
    gl.GetActiveUniformBlockiv(m_program_obj, m_index, GL_UNIFORM_BLOCK_BINDING, &binding);
    return binding;
  }

  void uniform_block::set_binding(int value)
  {
    const GLMethods& gl = m_context->gl();
    gl.UniformBlockBinding(m_program_obj, m_index, value);
  }

} // namespace  mgl::opengl
