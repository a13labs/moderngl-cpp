#include "shaders/fragment/imgui.hpp"
#include "shaders/vertex/imgui.hpp"

#include "mgl_graphics/pipelines/gui.hpp"

#include "mgl_platform/gpu.hpp"

namespace mgl::graphics::builtins
{

  void gui_pipeline::load()
  {
    MGL_CORE_ASSERT(mgl::shaders::imgui::vertex_shader_source().size() > 0,
                    "Vertex shader source is empty");
    MGL_CORE_ASSERT(mgl::shaders::imgui::fragment_shader_source().size() > 0,
                    "Fragment shader source is empty");

    m_program = mgl::platform::gpu::create_pipeline(
        mgl::shaders::imgui::vertex_shader_source(), mgl::shaders::imgui::fragment_shader_source());
    set_uniform_value("tex", 0);
  }

  void gui_pipeline::prepare() { }

} // namespace mgl::graphics::builtins