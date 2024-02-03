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

    void execute() override final;

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

    void execute() override final;

private:
    mgl::graphics::shader_ref m_shader;
    std::string m_name;
    shader::uniform_value m_value;
  };

  class disable_shader : public render_command
  {
public:
    disable_shader() { }

    void execute() override final;
  };

} // namespace mgl::graphics
