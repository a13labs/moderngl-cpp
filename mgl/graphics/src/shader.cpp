#include "mgl_graphics/shader.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_graphics/render.hpp"

namespace mgl::graphics
{
  void shader::bind()
  {
    auto prg = native();
    MGL_CORE_ASSERT(prg != nullptr, "Program is null");
    mgl::window::api::enable_program(prg);
    mgl::window::api::set_program_attributes(attributes());
  }

  void shader::unbind()
  {
    mgl::window::api::disable_program();
  }

  void shader::get_uniform_value(const std::string& name, bool& value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->get_value(value);
  }

  void shader::get_uniform_value(const std::string& name, uint8_t& value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->get_value(value);
  }

  void shader::get_uniform_value(const std::string& name, uint16_t& value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->get_value(value);
  }

  void shader::get_uniform_value(const std::string& name, uint32_t& value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->get_value(value);
  }

  void shader::get_uniform_value(const std::string& name, int& value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->get_value(value);
  }

  void shader::get_uniform_value(const std::string& name, float& value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->get_value(value);
  }

  void shader::get_uniform_value(const std::string& name, const glm::vec2& value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->get_value(value);
  }

  void shader::get_uniform_value(const std::string& name, const glm::vec3& value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->get_value(value);
  }

  void shader::get_uniform_value(const std::string& name, const glm::vec4& value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->get_value(value);
  }

  void shader::get_uniform_value(const std::string& name, const glm::mat2& value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->get_value(value);
  }

  void shader::get_uniform_value(const std::string& name, const glm::mat2x3& value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->get_value(value);
  }

  void shader::get_uniform_value(const std::string& name, const glm::mat2x4& value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->get_value(value);
  }

  void shader::get_uniform_value(const std::string& name, const glm::mat3& value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->get_value(value);
  }

  void shader::get_uniform_value(const std::string& name, const glm::mat3x2& value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->get_value(value);
  }

  void shader::get_uniform_value(const std::string& name, const glm::mat3x4& value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->get_value(value);
  }

  void shader::get_uniform_value(const std::string& name, const glm::mat4& value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->get_value(value);
  }

  void shader::get_uniform_value(const std::string& name, const glm::mat4x2& value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->get_value(value);
  }

  void shader::get_uniform_value(const std::string& name, const glm::mat4x3& value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->get_value(value);
  }

  void shader::get_uniform_value(const std::string& name, uint8_t* value, size_t size)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->get_value(value, size);
  }

  void shader::get_uniform_value(const std::string& name, uint16_t* value, size_t size)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->get_value(value, size);
  }

  void shader::get_uniform_value(const std::string& name, uint32_t* value, size_t size)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->get_value(value, size);
  }

  void shader::get_uniform_value(const std::string& name, int* value, size_t size)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->get_value(value, size);
  }

  void shader::get_uniform_value(const std::string& name, float* value, size_t size)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->get_value(value, size);
  }

  void shader::set_uniform_value(const std::string& name, bool value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->set_value(value);
  }

  void shader::set_uniform_value(const std::string& name, uint8_t value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->set_value(value);
  }

  void shader::set_uniform_value(const std::string& name, uint16_t value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->set_value(value);
  }

  void shader::set_uniform_value(const std::string& name, uint32_t value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->set_value(value);
  }

  void shader::set_uniform_value(const std::string& name, int value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->set_value(value);
  }

  void shader::set_uniform_value(const std::string& name, float value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->set_value(value);
  }

  void shader::set_uniform_value(const std::string& name, const glm::vec2 value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->set_value(value);
  }

  void shader::set_uniform_value(const std::string& name, const glm::vec3 value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->set_value(value);
  }

  void shader::set_uniform_value(const std::string& name, const glm::vec4 value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->set_value(value);
  }

  void shader::set_uniform_value(const std::string& name, const glm::mat2 value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->set_value(value);
  }

  void shader::set_uniform_value(const std::string& name, const glm::mat2x3 value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->set_value(value);
  }

  void shader::set_uniform_value(const std::string& name, const glm::mat2x4 value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->set_value(value);
  }

  void shader::set_uniform_value(const std::string& name, const glm::mat3 value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->set_value(value);
  }

  void shader::set_uniform_value(const std::string& name, const glm::mat3x2 value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->set_value(value);
  }

  void shader::set_uniform_value(const std::string& name, const glm::mat3x4 value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->set_value(value);
  }

  void shader::set_uniform_value(const std::string& name, const glm::mat4 value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->set_value(value);
  }

  void shader::set_uniform_value(const std::string& name, const glm::mat4x2 value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->set_value(value);
  }

  void shader::set_uniform_value(const std::string& name, const glm::mat4x3 value)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->set_value(value);
  }

  void shader::set_uniform_value(const std::string& name, uint8_t* value, size_t size)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->set_value(value);
  }

  void shader::set_uniform_value(const std::string& name, uint16_t* value, size_t size)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->set_value(value);
  }

  void shader::set_uniform_value(const std::string& name, uint32_t* value, size_t size)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->set_value(value);
  }

  void shader::set_uniform_value(const std::string& name, int* value, size_t size)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->set_value(value);
  }

  void shader::set_uniform_value(const std::string& name, float* value, size_t size)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Program is null");
    (*this->native())[name]->set_value(value);
  }

} // namespace mgl::graphics