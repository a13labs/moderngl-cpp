#include "mgl_graphics/commands/shader.hpp"
#include "mgl_core/debug.hpp"

namespace mgl::graphics
{
  void enable_shader::execute()
  {
    m_shader->bind();
    m_shader->prepare();
  }

  void disable_shader::execute()
  {
    mgl::window::api::disable_program();
  }
} // namespace mgl::graphics