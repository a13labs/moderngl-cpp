#include "shaders/fragment/imgui.hpp"
#include "shaders/vertex/imgui.hpp"

#include "mgl_graphics/shaders/gui.hpp"

#include "mgl_platform/api/render_api.hpp"

namespace mgl::graphics::builtins
{

  void gui_shader::load()
  {
    MGL_CORE_ASSERT(mgl::shaders::imgui::vertex_shader_source().size() > 0,
                    "Vertex shader source is empty");
    MGL_CORE_ASSERT(mgl::shaders::imgui::fragment_shader_source().size() > 0,
                    "Fragment shader source is empty");

    m_program = mgl::platform::api::render_api::create_program(
        mgl::shaders::imgui::vertex_shader_source(), mgl::shaders::imgui::fragment_shader_source());
    set_uniform_value("tex", 0);
  }

  void gui_shader::prepare() { }

} // namespace mgl::graphics::builtins