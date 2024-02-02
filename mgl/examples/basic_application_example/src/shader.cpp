#include "shader.hpp"
#include "mgl_application/application.hpp"
#include "mgl_graphics/graphics.hpp"

#include "shaders/fragment/fragment.hpp"
#include "shaders/vertex/vertex.hpp"

static mgl::string_list s_attributes = { "in_vert", "in_color" };

void custom_shader::load()
{
  m_program = mgl::window::api::create_program(vertex_shader_source, fragment_shader_source);
}

void custom_shader::prepare() { }

mgl::string_list& custom_shader::attributes()
{
  return s_attributes;
}
