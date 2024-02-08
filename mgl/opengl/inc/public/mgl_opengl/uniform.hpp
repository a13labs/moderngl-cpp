#pragma once

#include "mgl_core/containers.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"

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

namespace mgl::opengl
{
  class uniform
  {
public:
    struct data_type
    {
      bool matrix;
      int dimension;
      int element_size;
    };

    uniform(const std::string& name, int gl_type, int program_obj, int location, size_t size);

    ~uniform();

    void get_value(bool& value) { get_value((void*)&value, sizeof(bool)); }

    void get_value(uint8_t& value) { get_value((void*)&value, sizeof(uint8_t)); }

    void get_value(uint16_t& value) { get_value((void*)&value, sizeof(uint16_t)); }

    void get_value(uint32_t& value) { get_value((void*)&value, sizeof(uint32_t)); }

    void get_value(int& value) { get_value((void*)&value, sizeof(int)); }

    void get_value(float& value) { get_value((void*)&value, sizeof(float)); }

    void get_value(const glm::vec2& value) { get_value((void*)&value, sizeof(glm::vec2)); }

    void get_value(const glm::vec3& value) { get_value((void*)&value, sizeof(glm::vec3)); }

    void get_value(const glm::vec4& value) { get_value((void*)&value, sizeof(glm::vec4)); }

    void get_value(const glm::mat2& value) { get_value((void*)&value, sizeof(glm::mat2)); }

    void get_value(const glm::mat2x3& value) { get_value((void*)&value, sizeof(glm::mat2x3)); }

    void get_value(const glm::mat2x4& value) { get_value((void*)&value, sizeof(glm::mat2x4)); }

    void get_value(const glm::mat3& value) { get_value((void*)&value, sizeof(glm::mat3)); }

    void get_value(const glm::mat3x2& value) { get_value((void*)&value, sizeof(glm::mat3x2)); }

    void get_value(const glm::mat3x4& value) { get_value((void*)&value, sizeof(glm::mat3x4)); }

    void get_value(const glm::mat4& value) { get_value((void*)&value, sizeof(glm::mat4)); }

    void get_value(const glm::mat4x2& value) { get_value((void*)&value, sizeof(glm::mat4x2)); }

    void get_value(const glm::mat4x3& value) { get_value((void*)&value, sizeof(glm::mat4x3)); }

    void get_value(uint8_t* value, size_t size) { get_value((void*)value, size * sizeof(uint8_t)); }

    void get_value(uint16_t* value, size_t size)
    {
      get_value((void*)value, size * sizeof(uint16_t));
    }

    void get_value(uint32_t* value, size_t size)
    {
      get_value((void*)value, size * sizeof(uint32_t));
    }

    void get_value(int* value, size_t size) { get_value((void*)value, size * sizeof(int)); }

    void get_value(float* value, size_t size) { get_value((void*)value, size * sizeof(float)); }

    void set_value(bool value)
    {
      auto tmp = value;
      set_value((void*)&tmp, sizeof(bool));
    }

    void set_value(uint8_t value)
    {
      auto tmp = value;
      set_value((void*)&tmp, sizeof(uint8_t));
    }

    void set_value(uint16_t value)
    {
      auto tmp = value;
      set_value((void*)&tmp, sizeof(uint16_t));
    }

    void set_value(uint32_t value)
    {
      auto tmp = value;
      set_value((void*)&tmp, sizeof(uint32_t));
    }

    void set_value(int value)
    {
      auto tmp = value;
      set_value((void*)&tmp, sizeof(int));
    }

    void set_value(float value)
    {
      auto tmp = value;
      set_value((void*)&tmp, sizeof(float));
    }

    void set_value(const glm::vec2 value)
    {
      auto tmp = value;
      set_value((void*)&tmp, sizeof(glm::vec2));
    }

    void set_value(const glm::vec3 value)
    {
      auto tmp = value;
      set_value((void*)&tmp, sizeof(glm::vec3));
    }

    void set_value(const glm::vec4 value)
    {
      auto tmp = value;
      set_value((void*)&tmp, sizeof(glm::vec4));
    }

    void set_value(const glm::mat2 value)
    {
      auto tmp = value;
      set_value((void*)&tmp, sizeof(glm::mat2));
    }

    void set_value(const glm::mat2x3 value)
    {
      auto tmp = value;
      set_value((void*)&tmp, sizeof(glm::mat2x3));
    }

    void set_value(const glm::mat2x4 value)
    {
      auto tmp = value;
      set_value((void*)&tmp, sizeof(glm::mat2x4));
    }

    void set_value(const glm::mat3 value)
    {
      auto tmp = value;
      set_value((void*)&tmp, sizeof(glm::mat3));
    }

    void set_value(const glm::mat3x2 value)
    {
      auto tmp = value;
      set_value((void*)&tmp, sizeof(glm::mat3x2));
    }

    void set_value(const glm::mat3x4 value)
    {
      auto tmp = value;
      set_value((void*)&tmp, sizeof(glm::mat3x4));
    }

    void set_value(const glm::mat4 value)
    {
      auto tmp = value;
      set_value((void*)&tmp, sizeof(glm::mat4));
    }

    void set_value(const glm::mat4x2 value)
    {
      auto tmp = value;
      set_value((void*)&tmp, sizeof(glm::mat4x2));
    }

    void set_value(const glm::mat4x3 value)
    {
      auto tmp = value;
      set_value((void*)&tmp, sizeof(glm::mat4x3));
    }

    void set_value(uint8_t* value, size_t size)
    {
      set_value((void*)&value, size * sizeof(uint8_t));
    }

    void set_value(uint16_t* value, size_t size)
    {
      set_value((void*)&value, size * sizeof(uint16_t));
    }

    void set_value(uint32_t* value, size_t size)
    {
      set_value((void*)&value, size * sizeof(uint32_t));
    }

    void set_value(int* value, size_t size) { set_value((void*)&value, size * sizeof(int)); }

    void set_value(float* value, size_t size) { set_value((void*)&value, size * sizeof(float)); }

private:
    void set_value(void* data, size_t size);
    void get_value(void* data, size_t size);

    std::string m_name;
    int m_program_obj;
    int m_gl_type;
    int m_location;
    int m_size;
    data_type* m_data_type;
    uint8_t* m_data;
  };

  using uniform_ref = mgl::ref<uniform>;
  using uniforms_dict = mgl::dict<std::string, uniform_ref>;

} // namespace  mgl::opengl
