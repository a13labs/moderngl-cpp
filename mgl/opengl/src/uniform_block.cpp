#include "mgl_opengl/uniform_block.hpp"
#include "mgl_opengl/context.hpp"

#include "mgl_core/debug.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  uniform_block::uniform_block(const std::string& name, int program_obj, int index, size_t size)
  {
    m_name = name;
    m_program_obj = program_obj;
    m_index = index;
    m_size = size;
  }

  int uniform_block::binding()
  {
    int binding = 0;
    glGetActiveUniformBlockiv(m_program_obj, m_index, GL_UNIFORM_BLOCK_BINDING, &binding);
    return binding;
  }

  void uniform_block::set_binding(int value)
  {
    glUniformBlockBinding(m_program_obj, m_index, value);
  }

} // namespace  mgl::opengl
