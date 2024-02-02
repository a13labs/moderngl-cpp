#include "mgl_graphics/shaders/text.hpp"

#include "shaders/fragment/text.hpp"
#include "shaders/vertex/text.hpp"

namespace mgl::graphics::builtins
{
  static mgl::string_list s_attributes = { "vertex" };

  void text_shader::load()
  {
    m_program = mgl::window::api::create_program(vertex_shader_source, fragment_shader_source);
    set_uniform_value("atlas", 0);
  }

  void text_shader::prepare() { }

  mgl::string_list& text_shader::attributes()
  {
    return s_attributes;
  }

} // namespace mgl::graphics::builtins