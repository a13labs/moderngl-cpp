#pragma once

#include "mgl_graphics/command.hpp"
#include "mgl_graphics/shader.hpp"

namespace mgl::graphics
{
  class enable_shader : public render_command
  {
public:
    enable_shader(mgl::graphics::shader_ref shader)
        : m_shader(shader)
    { }

    void execute() override final
    {
      mgl::platform::api::render_api::enable_program(m_shader->api());
      m_shader->prepare();
    }

private:
    mgl::graphics::shader_ref m_shader;
  };

  class set_shader_uniform : public render_command
  {
public:
    set_shader_uniform(const std::string& name, bool value)
        : m_name(name)
        , m_value(value)
    { }

    set_shader_uniform(const std::string& name, int value)
        : m_name(name)
        , m_value(value)
    { }

    set_shader_uniform(const std::string& name, float value)
        : m_name(name)
        , m_value(value)
    { }

    set_shader_uniform(const std::string& name, const glm::vec2& value)
        : m_name(name)
        , m_value(value)
    { }

    set_shader_uniform(const std::string& name, const glm::vec3& value)
        : m_name(name)
        , m_value(value)
    { }

    set_shader_uniform(const std::string& name, const glm::vec4& value)
        : m_name(name)
        , m_value(value)
    { }

    set_shader_uniform(const std::string& name, const glm::mat2& value)
        : m_name(name)
        , m_value(value)
    { }

    set_shader_uniform(const std::string& name, const glm::mat2x3& value)
        : m_name(name)
        , m_value(value)
    { }

    set_shader_uniform(const std::string& name, const glm::mat2x4& value)
        : m_name(name)
        , m_value(value)
    { }

    set_shader_uniform(const std::string& name, const glm::mat3& value)
        : m_name(name)
        , m_value(value)
    { }

    set_shader_uniform(const std::string& name, const glm::mat3x2& value)
        : m_name(name)
        , m_value(value)
    { }

    set_shader_uniform(const std::string& name, const glm::mat3x4& value)
        : m_name(name)
        , m_value(value)
    { }

    set_shader_uniform(const std::string& name, const glm::mat4& value)
        : m_name(name)
        , m_value(value)
    { }

    set_shader_uniform(const std::string& name, const glm::mat4x2& value)
        : m_name(name)
        , m_value(value)
    { }

    set_shader_uniform(const std::string& name, const glm::mat4x3& value)
        : m_name(name)
        , m_value(value)
    { }

    void execute() override final
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

private:
    mgl::graphics::shader_ref m_shader;
    std::string m_name;
    shader::uniform_value m_value;
  };

  class disable_shader : public render_command
  {
public:
    disable_shader() { }

    void execute() override final { mgl::platform::api::render_api::disable_program(); }
  };

} // namespace mgl::graphics
