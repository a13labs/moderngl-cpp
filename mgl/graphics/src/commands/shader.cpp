#include "mgl_graphics/commands/shader.hpp"
#include "mgl_core/debug.hpp"

#include "mgl_platform/api/render_api.hpp"

namespace mgl::graphics
{
  void enable_shader::execute()
  {
    m_shader->bind();
    m_shader->prepare();
  }

  void set_shader_uniform::execute()
  {
    switch(m_value.type)
    {
      case shader::uniform_type::BOOL:
        mgl::platform::api::render_api::set_program_uniform(m_name, m_value.data.b);
        break;
      case shader::uniform_type::INT:
        mgl::platform::api::render_api::set_program_uniform(m_name, m_value.data.i);
        break;
      case shader::uniform_type::FLOAT:
        mgl::platform::api::render_api::set_program_uniform(m_name, m_value.data.f);
        break;
      case shader::uniform_type::VEC2:
        mgl::platform::api::render_api::set_program_uniform(m_name, m_value.data.vec2);
        break;
      case shader::uniform_type::VEC3:
        mgl::platform::api::render_api::set_program_uniform(m_name, m_value.data.vec3);
        break;
      case shader::uniform_type::VEC4:
        mgl::platform::api::render_api::set_program_uniform(m_name, m_value.data.vec4);
        break;
      case shader::uniform_type::MAT2:
        mgl::platform::api::render_api::set_program_uniform(m_name, m_value.data.mat2);
        break;
      case shader::uniform_type::MAT2X3:
        mgl::platform::api::render_api::set_program_uniform(m_name, m_value.data.mat2x3);
        break;
      case shader::uniform_type::MAT2X4:
        mgl::platform::api::render_api::set_program_uniform(m_name, m_value.data.mat2x4);
        break;
      case shader::uniform_type::MAT3:
        mgl::platform::api::render_api::set_program_uniform(m_name, m_value.data.mat3);
        break;
      case shader::uniform_type::MAT3X2:
        mgl::platform::api::render_api::set_program_uniform(m_name, m_value.data.mat3x2);
        break;
      case shader::uniform_type::MAT3X4:
        mgl::platform::api::render_api::set_program_uniform(m_name, m_value.data.mat3x4);
        break;
      case shader::uniform_type::MAT4:
        mgl::platform::api::render_api::set_program_uniform(m_name, m_value.data.mat4);
        break;
      case shader::uniform_type::MAT4X2:
        mgl::platform::api::render_api::set_program_uniform(m_name, m_value.data.mat4x2);
        break;
      case shader::uniform_type::MAT4X3:
        mgl::platform::api::render_api::set_program_uniform(m_name, m_value.data.mat4x3);
        break;
      default: MGL_CORE_ASSERT(false, "Unknown shader uniform type");
    }
  }

  void disable_shader::execute()
  {
    mgl::platform::api::render_api::disable_program();
  }
} // namespace mgl::graphics