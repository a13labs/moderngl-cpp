#include "mgl_engine/materials/color.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_window/window.hpp"

#include "shaders/color_material_glsl.h"
namespace mgl::engine
{
  static mgl::window::program_ref s_program;
  static mgl::window::uniform_ref s_color;

  color_material::color_material(const glm::vec4& color)
      : material()
      , m_color(color)
  {
    auto ctx = mgl::window::current_context();
    MGL_CORE_ASSERT(ctx, "No context!");

    if(!s_program)
    {
      mgl::opengl::shader shader_source(reinterpret_cast<const char*>(color_material_glsl));
      mgl::opengl::shaders glsl = { shader_source };

      s_program = ctx->program(glsl);
      s_color = s_program->uniform("color");
    }
  }

  void color_material::prepare()
  {
    s_color->set_value(m_color);
  }

  const mgl::window::program_ref& color_material::program()
  {
    return s_program;
  }

} // namespace mgl::engine