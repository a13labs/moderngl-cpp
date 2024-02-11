#pragma once

#include "mgl_core/memory.hpp"

#include "glm/glm.hpp"

namespace mgl::platform::api
{

  class program;
  using program_ref = mgl::ref<program>;

  class program
  {
public:
    virtual ~program() = default;

    virtual void release() = 0;

    virtual void bind() = 0;

    virtual void unbind() = 0;

    virtual void get_value(const std::string& name, bool& value) = 0;

    virtual void get_value(const std::string& name, uint8_t& value) = 0;

    virtual void get_value(const std::string& name, uint16_t& value) = 0;

    virtual void get_value(const std::string& name, uint32_t& value) = 0;

    virtual void get_value(const std::string& name, int& value) = 0;

    virtual void get_value(const std::string& name, float& value) = 0;

    virtual void get_value(const std::string& name, glm::vec2& value) = 0;

    virtual void get_value(const std::string& name, glm::vec3& value) = 0;

    virtual void get_value(const std::string& name, glm::vec4& value) = 0;

    virtual void get_value(const std::string& name, glm::mat2& value) = 0;

    virtual void get_value(const std::string& name, glm::mat2x3& value) = 0;

    virtual void get_value(const std::string& name, glm::mat2x4& value) = 0;

    virtual void get_value(const std::string& name, glm::mat3& value) = 0;

    virtual void get_value(const std::string& name, glm::mat3x2& value) = 0;

    virtual void get_value(const std::string& name, glm::mat3x4& value) = 0;

    virtual void get_value(const std::string& name, glm::mat4& value) = 0;

    virtual void get_value(const std::string& name, glm::mat4x2& value) = 0;

    virtual void get_value(const std::string& name, glm::mat4x3& value) = 0;

    virtual void get_value(const std::string& name, uint8_buffer& value) = 0;

    virtual void get_value(const std::string& name, uint16_buffer& value) = 0;

    virtual void get_value(const std::string& name, uint32_buffer& value) = 0;

    virtual void get_value(const std::string& name, int8_buffer& value) = 0;

    virtual void get_value(const std::string& name, int16_buffer& value) = 0;

    virtual void get_value(const std::string& name, int32_buffer& value) = 0;

    virtual void get_value(const std::string& name, float32_buffer& value) = 0;

    virtual void get_value(const std::string& name, float64_buffer& value) = 0;

    virtual void get_value(const std::string& name, uint8_t* value, size_t size) = 0;

    virtual void get_value(const std::string& name, uint16_t* value, size_t size) = 0;

    virtual void get_value(const std::string& name, uint32_t* value, size_t size) = 0;

    virtual void get_value(const std::string& name, int8_t* value, size_t size) = 0;

    virtual void get_value(const std::string& name, int16_t* value, size_t size) = 0;

    virtual void get_value(const std::string& name, int32_t* value, size_t size) = 0;

    virtual void get_value(const std::string& name, float* value, size_t size) = 0;

    virtual void get_value(const std::string& name, double* value, size_t size) = 0;

    virtual void set_value(const std::string& name, bool value) = 0;

    virtual void set_value(const std::string& name, uint8_t value) = 0;

    virtual void set_value(const std::string& name, uint16_t value) = 0;

    virtual void set_value(const std::string& name, uint32_t value) = 0;

    virtual void set_value(const std::string& name, int value) = 0;

    virtual void set_value(const std::string& name, float value) = 0;

    virtual void set_value(const std::string& name, const glm::vec2 value) = 0;

    virtual void set_value(const std::string& name, const glm::vec3 value) = 0;

    virtual void set_value(const std::string& name, const glm::vec4 value) = 0;

    virtual void set_value(const std::string& name, const glm::mat2 value) = 0;

    virtual void set_value(const std::string& name, const glm::mat2x3 value) = 0;

    virtual void set_value(const std::string& name, const glm::mat2x4 value) = 0;

    virtual void set_value(const std::string& name, const glm::mat3 value) = 0;

    virtual void set_value(const std::string& name, const glm::mat3x2 value) = 0;

    virtual void set_value(const std::string& name, const glm::mat3x4 value) = 0;

    virtual void set_value(const std::string& name, const glm::mat4 value) = 0;

    virtual void set_value(const std::string& name, const glm::mat4x2 value) = 0;

    virtual void set_value(const std::string& name, const glm::mat4x3 value) = 0;

    virtual void set_value(const std::string& name, const uint8_buffer& value) = 0;

    virtual void set_value(const std::string& name, const uint16_buffer& value) = 0;

    virtual void set_value(const std::string& name, const uint32_buffer& value) = 0;

    virtual void set_value(const std::string& name, const int8_buffer& value) = 0;

    virtual void set_value(const std::string& name, const int16_buffer& value) = 0;

    virtual void set_value(const std::string& name, const int32_buffer& value) = 0;

    virtual void set_value(const std::string& name, const float32_buffer& value) = 0;

    virtual void set_value(const std::string& name, const float64_buffer& value) = 0;

    virtual void set_value(const std::string& name, uint8_t* value, size_t size) = 0;

    virtual void set_value(const std::string& name, uint16_t* value, size_t size) = 0;

    virtual void set_value(const std::string& name, uint32_t* value, size_t size) = 0;

    virtual void set_value(const std::string& name, int8_t* value, size_t size) = 0;

    virtual void set_value(const std::string& name, int16_t* value, size_t size) = 0;

    virtual void set_value(const std::string& name, int32_t* value, size_t size) = 0;

    virtual void set_value(const std::string& name, float* value, size_t size) = 0;

    virtual void set_value(const std::string& name, double* value, size_t size) = 0;
  };
} // namespace mgl::platform::api