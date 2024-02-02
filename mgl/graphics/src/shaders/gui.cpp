#include "mgl_graphics/shaders/gui.hpp"

#include "shaders/fragment/imgui.hpp"
#include "shaders/vertex/imgui.hpp"

namespace mgl::graphics::builtins
{
  static mgl::string_list s_attributes = { "Position", "UV", "Color" };

  void gui_shader::load()
  {
    m_program = mgl::window::api::create_program(vertex_shader_source, fragment_shader_source);
    set_uniform_value("Texture", 0);
  }

  void gui_shader::prepare() { }

  mgl::string_list& gui_shader::attributes()
  {
    return s_attributes;
  }

} // namespace mgl::graphics::builtins