#include "mgl_graphics/shaders/gui.hpp"
#include "mgl_graphics/render.hpp"

#include "shaders/fragment/imgui.hpp"
#include "shaders/vertex/imgui.hpp"

namespace mgl::graphics::shaders
{
  static mgl::string_list s_attributes = { "Position", "UV", "Color" };

  void gui::load()
  {
    auto& render = mgl::graphics::current_render();
    auto ctx = render.context();
    // Load vertex and fragment shaders from generated source
    mgl::opengl::shaders glsl = { vertex_shader_source, fragment_shader_source };

    m_program = ctx->program(glsl);
    set_uniform_value("Texture", 0);
  }

  void gui::unload()
  {
    m_program->release();
  }

  void gui::prepare() { }

  mgl::window::api::program_ref& gui::native()
  {
    return m_program;
  }

  mgl::string_list& gui::attributes()
  {
    return s_attributes;
  }

} // namespace mgl::graphics::shaders