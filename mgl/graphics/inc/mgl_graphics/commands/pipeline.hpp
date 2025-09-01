#pragma once

#include "mgl_graphics/command.hpp"
#include "mgl_graphics/pipeline.hpp"

#include "mgl_platform/commands.hpp"

namespace mgl::graphics
{
  class enable_pipeline : public render_command
  {
public:
    enable_pipeline(mgl::graphics::pipeline_ref p)
        : m_pipeline(p)
    { }

    void execute() override final
    {
      mgl::platform::gpu::commands::enable_pipeline(m_pipeline->api());
      m_pipeline->prepare();
    }

private:
    mgl::graphics::pipeline_ref m_pipeline;
  };

  class set_uniform : public render_command
  {
public:
    set_uniform(const std::string& name, bool value)
        : m_name(name)
        , m_value(value)
    { }

    set_uniform(const std::string& name, int value)
        : m_name(name)
        , m_value(value)
    { }

    set_uniform(const std::string& name, float value)
        : m_name(name)
        , m_value(value)
    { }

    set_uniform(const std::string& name, const glm::vec2& value)
        : m_name(name)
        , m_value(value)
    { }

    set_uniform(const std::string& name, const glm::vec3& value)
        : m_name(name)
        , m_value(value)
    { }

    set_uniform(const std::string& name, const glm::vec4& value)
        : m_name(name)
        , m_value(value)
    { }

    set_uniform(const std::string& name, const glm::mat2& value)
        : m_name(name)
        , m_value(value)
    { }

    set_uniform(const std::string& name, const glm::mat2x3& value)
        : m_name(name)
        , m_value(value)
    { }

    set_uniform(const std::string& name, const glm::mat2x4& value)
        : m_name(name)
        , m_value(value)
    { }

    set_uniform(const std::string& name, const glm::mat3& value)
        : m_name(name)
        , m_value(value)
    { }

    set_uniform(const std::string& name, const glm::mat3x2& value)
        : m_name(name)
        , m_value(value)
    { }

    set_uniform(const std::string& name, const glm::mat3x4& value)
        : m_name(name)
        , m_value(value)
    { }

    set_uniform(const std::string& name, const glm::mat4& value)
        : m_name(name)
        , m_value(value)
    { }

    set_uniform(const std::string& name, const glm::mat4x2& value)
        : m_name(name)
        , m_value(value)
    { }

    set_uniform(const std::string& name, const glm::mat4x3& value)
        : m_name(name)
        , m_value(value)
    { }

    void execute() override final
    {
      switch(m_value.type)
      {
        case pipeline::uniform_type::BOOL:
          mgl::platform::gpu::commands::set_program_uniform(m_name, m_value.data.b);
          break;
        case pipeline::uniform_type::INT:
          mgl::platform::gpu::commands::set_program_uniform(m_name, m_value.data.i);
          break;
        case pipeline::uniform_type::FLOAT:
          mgl::platform::gpu::commands::set_program_uniform(m_name, m_value.data.f);
          break;
        case pipeline::uniform_type::VEC2:
          mgl::platform::gpu::commands::set_program_uniform(m_name, m_value.data.vec2);
          break;
        case pipeline::uniform_type::VEC3:
          mgl::platform::gpu::commands::set_program_uniform(m_name, m_value.data.vec3);
          break;
        case pipeline::uniform_type::VEC4:
          mgl::platform::gpu::commands::set_program_uniform(m_name, m_value.data.vec4);
          break;
        case pipeline::uniform_type::MAT2:
          mgl::platform::gpu::commands::set_program_uniform(m_name, m_value.data.mat2);
          break;
        case pipeline::uniform_type::MAT2X3:
          mgl::platform::gpu::commands::set_program_uniform(m_name, m_value.data.mat2x3);
          break;
        case pipeline::uniform_type::MAT2X4:
          mgl::platform::gpu::commands::set_program_uniform(m_name, m_value.data.mat2x4);
          break;
        case pipeline::uniform_type::MAT3:
          mgl::platform::gpu::commands::set_program_uniform(m_name, m_value.data.mat3);
          break;
        case pipeline::uniform_type::MAT3X2:
          mgl::platform::gpu::commands::set_program_uniform(m_name, m_value.data.mat3x2);
          break;
        case pipeline::uniform_type::MAT3X4:
          mgl::platform::gpu::commands::set_program_uniform(m_name, m_value.data.mat3x4);
          break;
        case pipeline::uniform_type::MAT4:
          mgl::platform::gpu::commands::set_program_uniform(m_name, m_value.data.mat4);
          break;
        case pipeline::uniform_type::MAT4X2:
          mgl::platform::gpu::commands::set_program_uniform(m_name, m_value.data.mat4x2);
          break;
        case pipeline::uniform_type::MAT4X3:
          mgl::platform::gpu::commands::set_program_uniform(m_name, m_value.data.mat4x3);
          break;
        default: MGL_CORE_ASSERT(false, "Unknown uniform type");
      }
    }

private:
    mgl::graphics::pipeline_ref m_pipeline;
    std::string m_name;
    pipeline::uniform_value m_value;
  };

  class disable_pipeline : public render_command
  {
public:
    disable_pipeline() { }

    void execute() override final { mgl::platform::gpu::commands::disable_pipeline(); }
  };

} // namespace mgl::graphics
