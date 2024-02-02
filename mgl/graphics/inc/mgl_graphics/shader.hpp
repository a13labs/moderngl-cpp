#pragma once

#include "glm/mat2x2.hpp"
#include "glm/mat2x3.hpp"
#include "glm/mat2x4.hpp"
#include "glm/mat3x2.hpp"
#include "glm/mat3x3.hpp"
#include "glm/mat3x4.hpp"
#include "glm/mat4x2.hpp"
#include "glm/mat4x3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"
#include "mgl_window/api/opengl.hpp"

namespace mgl::graphics
{
  class shader
  {
public:
    virtual ~shader() = default;

    virtual void prepare() = 0;
    virtual const mgl::string_list& attributes() = 0;
    virtual void load() = 0;

    mgl::window::api::program_ref& api() { return m_program; }

    void bind()
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      mgl::window::api::enable_program(m_program);
      mgl::window::api::set_program_attributes(attributes());
    }

    void unbind() { mgl::window::api::disable_program(); }

    void unload()
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->release();
    }

    void get_uniform_value(const std::string& name, bool& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->get_value(value);
    }

    void get_uniform_value(const std::string& name, uint8_t& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->get_value(value);
    }

    void get_uniform_value(const std::string& name, uint16_t& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->get_value(value);
    }

    void get_uniform_value(const std::string& name, uint32_t& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->get_value(value);
    }

    void get_uniform_value(const std::string& name, int& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->get_value(value);
    }

    void get_uniform_value(const std::string& name, float& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->get_value(value);
    }

    void get_uniform_value(const std::string& name, const glm::vec2& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->get_value(value);
    }

    void get_uniform_value(const std::string& name, const glm::vec3& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->get_value(value);
    }

    void get_uniform_value(const std::string& name, const glm::vec4& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->get_value(value);
    }

    void get_uniform_value(const std::string& name, const glm::mat2& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->get_value(value);
    }

    void get_uniform_value(const std::string& name, const glm::mat2x3& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->get_value(value);
    }

    void get_uniform_value(const std::string& name, const glm::mat2x4& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->get_value(value);
    }

    void get_uniform_value(const std::string& name, const glm::mat3& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->get_value(value);
    }

    void get_uniform_value(const std::string& name, const glm::mat3x2& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->get_value(value);
    }

    void get_uniform_value(const std::string& name, const glm::mat3x4& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->get_value(value);
    }

    void get_uniform_value(const std::string& name, const glm::mat4& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->get_value(value);
    }

    void get_uniform_value(const std::string& name, const glm::mat4x2& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->get_value(value);
    }

    void get_uniform_value(const std::string& name, const glm::mat4x3& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->get_value(value);
    }

    void get_uniform_value(const std::string& name, uint8_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->get_value(value, size);
    }

    void get_uniform_value(const std::string& name, uint16_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->get_value(value, size);
    }

    void get_uniform_value(const std::string& name, uint32_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->get_value(value, size);
    }

    void get_uniform_value(const std::string& name, int* value, size_t size)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->get_value(value, size);
    }

    void get_uniform_value(const std::string& name, float* value, size_t size)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->get_value(value, size);
    }

    void set_uniform_value(const std::string& name, bool value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->set_value(value);
    }

    void set_uniform_value(const std::string& name, uint8_t value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->set_value(value);
    }

    void set_uniform_value(const std::string& name, uint16_t value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->set_value(value);
    }

    void set_uniform_value(const std::string& name, uint32_t value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->set_value(value);
    }

    void set_uniform_value(const std::string& name, int value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->set_value(value);
    }

    void set_uniform_value(const std::string& name, float value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->set_value(value);
    }

    void set_uniform_value(const std::string& name, const glm::vec2 value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->set_value(value);
    }

    void set_uniform_value(const std::string& name, const glm::vec3 value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->set_value(value);
    }

    void set_uniform_value(const std::string& name, const glm::vec4 value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->set_value(value);
    }

    void set_uniform_value(const std::string& name, const glm::mat2 value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->set_value(value);
    }

    void set_uniform_value(const std::string& name, const glm::mat2x3 value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->set_value(value);
    }

    void set_uniform_value(const std::string& name, const glm::mat2x4 value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->set_value(value);
    }

    void set_uniform_value(const std::string& name, const glm::mat3 value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->set_value(value);
    }

    void set_uniform_value(const std::string& name, const glm::mat3x2 value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->set_value(value);
    }

    void set_uniform_value(const std::string& name, const glm::mat3x4 value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->set_value(value);
    }

    void set_uniform_value(const std::string& name, const glm::mat4 value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->set_value(value);
    }

    void set_uniform_value(const std::string& name, const glm::mat4x2 value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->set_value(value);
    }

    void set_uniform_value(const std::string& name, const glm::mat4x3 value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->set_value(value);
    }

    void set_uniform_value(const std::string& name, uint8_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->set_value(value);
    }

    void set_uniform_value(const std::string& name, uint16_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->set_value(value);
    }

    void set_uniform_value(const std::string& name, uint32_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->set_value(value);
    }

    void set_uniform_value(const std::string& name, int* value, size_t size)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->set_value(value);
    }

    void set_uniform_value(const std::string& name, float* value, size_t size)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      (*m_program)[name]->set_value(value);
    }

protected:
    mgl::window::api::program_ref m_program = nullptr;
  };

  using shader_ref = mgl::ref<shader>;

} // namespace mgl::graphics
