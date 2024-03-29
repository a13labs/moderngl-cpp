#include "shader.hpp"
#include "mgl_application/application.hpp"
#include "mgl_graphics/graphics.hpp"

#include "shaders/fragment/example.hpp"
#include "shaders/vertex/example.hpp"

void custom_shader::load()
{
  MGL_CORE_ASSERT(mgl::shaders::example::vertex_shader_source().size() > 0,
                  "Vertex shader source is empty");
  MGL_CORE_ASSERT(mgl::shaders::example::fragment_shader_source().size() > 0,
                  "Fragment shader source is empty");
  m_program = mgl::platform::api::render_api::create_program(
      mgl::shaders::example::vertex_shader_source(),
      mgl::shaders::example::fragment_shader_source());
}

void custom_shader::prepare() { }
