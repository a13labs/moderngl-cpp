#include "mgl_engine/materials/color.hpp"
#include "mgl_window/window.hpp"

namespace mgl::engine
{
  static const char* vertex_shader_source = R"(
#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 view;
uniform mat4 projection;

void main()
{
  gl_Position = projection * view * vec4(position, 1.0);
}
)";

  static const char* fragment_shader_source = R"(
#version 330 core

uniform vec4 color;
out vec4 frag_color;

void main()
{
  frag_color = color;
}
)";

  static mgl::window::program_ref s_program;
  static mgl::window::uniform_ref s_color;

  color_material::color_material(const glm::vec4& color)
      : material()
      , m_color(color)
  {
    if(!s_program)
    {
      s_program =
          mgl::window::current_context()->program({ vertex_shader_source, fragment_shader_source });

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