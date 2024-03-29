#include "shaders/fragment/text.hpp"
#include "shaders/vertex/text.hpp"

#include "mgl_graphics/shaders/text.hpp"

#include "mgl_platform/api/render_api.hpp"

namespace mgl::graphics::builtins
{
  void text_shader::load()
  {
    MGL_CORE_ASSERT(mgl::shaders::text::vertex_shader_source().size() > 0,
                    "Vertex shader source is empty");
    MGL_CORE_ASSERT(mgl::shaders::text::fragment_shader_source().size() > 0,
                    "Fragment shader source is empty");
    m_program = mgl::platform::api::render_api::create_program(
        mgl::shaders::text::vertex_shader_source(), mgl::shaders::text::fragment_shader_source());
    set_uniform_value("atlas", 0);
    set_uniform_value("color", glm::vec4(1.0, 1.0, 1.0, 1.0));
    set_uniform_value("px_range", 64.f);
  }

  void text_shader::prepare() { }

} // namespace mgl::graphics::builtins