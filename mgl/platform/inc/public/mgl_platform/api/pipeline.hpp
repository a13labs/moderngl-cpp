#pragma once

#include "mgl_core/memory.hpp"

#include "glm/glm.hpp"

namespace mgl::platform::api
{

  class pipeline;
  using pipeline_ref = mgl::ref<pipeline>;

  class pipeline
  {
public:
    virtual ~pipeline() = default;

    virtual void release() = 0;

    virtual void bind() = 0;

    virtual void unbind() = 0;

    virtual void get_uniform_value(const std::string& name, bool& value) = 0;

    virtual void get_uniform_value(const std::string& name, uint8_t& value) = 0;

    virtual void get_uniform_value(const std::string& name, uint16_t& value) = 0;

    virtual void get_uniform_value(const std::string& name, uint32_t& value) = 0;

    virtual void get_uniform_value(const std::string& name, int& value) = 0;

    virtual void get_uniform_value(const std::string& name, float& value) = 0;

    virtual void get_uniform_value(const std::string& name, double& value) = 0;

    virtual void get_uniform_value(const std::string& name, glm::vec2& value) = 0;

    virtual void get_uniform_value(const std::string& name, glm::vec3& value) = 0;

    virtual void get_uniform_value(const std::string& name, glm::vec4& value) = 0;

    virtual void get_uniform_value(const std::string& name, glm::mat2& value) = 0;

    virtual void get_uniform_value(const std::string& name, glm::mat2x3& value) = 0;

    virtual void get_uniform_value(const std::string& name, glm::mat2x4& value) = 0;

    virtual void get_uniform_value(const std::string& name, glm::mat3& value) = 0;

    virtual void get_uniform_value(const std::string& name, glm::mat3x2& value) = 0;

    virtual void get_uniform_value(const std::string& name, glm::mat3x4& value) = 0;

    virtual void get_uniform_value(const std::string& name, glm::mat4& value) = 0;

    virtual void get_uniform_value(const std::string& name, glm::mat4x2& value) = 0;

    virtual void get_uniform_value(const std::string& name, glm::mat4x3& value) = 0;

    virtual void get_uniform_value(const std::string& name, uint8_buffer& value) = 0;

    virtual void get_uniform_value(const std::string& name, uint16_buffer& value) = 0;

    virtual void get_uniform_value(const std::string& name, uint32_buffer& value) = 0;

    virtual void get_uniform_value(const std::string& name, int8_buffer& value) = 0;

    virtual void get_uniform_value(const std::string& name, int16_buffer& value) = 0;

    virtual void get_uniform_value(const std::string& name, int32_buffer& value) = 0;

    virtual void get_uniform_value(const std::string& name, float32_buffer& value) = 0;

    virtual void get_uniform_value(const std::string& name, float64_buffer& value) = 0;

    virtual void get_uniform_value(const std::string& name, uint8_t* value, size_t size) = 0;

    virtual void get_uniform_value(const std::string& name, uint16_t* value, size_t size) = 0;

    virtual void get_uniform_value(const std::string& name, uint32_t* value, size_t size) = 0;

    virtual void get_uniform_value(const std::string& name, uint64_t& value) = 0;

    virtual void get_uniform_value(const std::string& name, int8_t& value) = 0;

    virtual void get_uniform_value(const std::string& name, int16_t& value) = 0;

    virtual void get_uniform_value(const std::string& name, int8_t* value, size_t size) = 0;

    virtual void get_uniform_value(const std::string& name, int16_t* value, size_t size) = 0;

    virtual void get_uniform_value(const std::string& name, int32_t* value, size_t size) = 0;

    virtual void get_uniform_value(const std::string& name, int64_t& value) = 0;

    virtual void get_uniform_value(const std::string& name, float* value, size_t size) = 0;

    virtual void get_uniform_value(const std::string& name, double* value, size_t size) = 0;

    virtual void set_uniform_value(const std::string& name, bool value) = 0;

    virtual void set_uniform_value(const std::string& name, uint8_t value) = 0;

    virtual void set_uniform_value(const std::string& name, uint16_t value) = 0;

    virtual void set_uniform_value(const std::string& name, uint32_t value) = 0;

    virtual void set_uniform_value(const std::string& name, uint64_t value) = 0;

    virtual void set_uniform_value(const std::string& name, int8_t value) = 0;

    virtual void set_uniform_value(const std::string& name, int16_t value) = 0;

    virtual void set_uniform_value(const std::string& name, int value) = 0;

    virtual void set_uniform_value(const std::string& name, int64_t value) = 0;

    virtual void set_uniform_value(const std::string& name, float value) = 0;

    virtual void set_uniform_value(const std::string& name, double value) = 0;

  virtual void set_uniform_value(const std::string& name, const glm::vec2& value) = 0;

  virtual void set_uniform_value(const std::string& name, const glm::vec3& value) = 0;

  virtual void set_uniform_value(const std::string& name, const glm::vec4& value) = 0;

  virtual void set_uniform_value(const std::string& name, const glm::mat2& value) = 0;

  virtual void set_uniform_value(const std::string& name, const glm::mat2x3& value) = 0;

  virtual void set_uniform_value(const std::string& name, const glm::mat2x4& value) = 0;

  virtual void set_uniform_value(const std::string& name, const glm::mat3& value) = 0;

  virtual void set_uniform_value(const std::string& name, const glm::mat3x2& value) = 0;

  virtual void set_uniform_value(const std::string& name, const glm::mat3x4& value) = 0;

  virtual void set_uniform_value(const std::string& name, const glm::mat4& value) = 0;

  virtual void set_uniform_value(const std::string& name, const glm::mat4x2& value) = 0;

  virtual void set_uniform_value(const std::string& name, const glm::mat4x3& value) = 0;

  // Note: by-value overloads for glm types are intentionally omitted to avoid
  // overload ambiguity with the const-reference versions above. Callers may
  // pass temporaries; they will bind to the const& overloads.

  virtual void set_uniform_value(const std::string& name, const uint8_buffer& value) = 0;

    virtual void set_uniform_value(const std::string& name, const uint16_buffer& value) = 0;

    virtual void set_uniform_value(const std::string& name, const uint32_buffer& value) = 0;

    virtual void set_uniform_value(const std::string& name, const mgl::uint64_buffer& value) = 0;

    virtual void set_uniform_value(const std::string& name, const int8_buffer& value) = 0;

    virtual void set_uniform_value(const std::string& name, const int16_buffer& value) = 0;

    virtual void set_uniform_value(const std::string& name, const int32_buffer& value) = 0;

    virtual void set_uniform_value(const std::string& name, const mgl::int64_buffer& value) = 0;

    virtual void set_uniform_value(const std::string& name, const float32_buffer& value) = 0;

    virtual void set_uniform_value(const std::string& name, const float64_buffer& value) = 0;

    virtual void set_uniform_value(const std::string& name, uint8_t* value, size_t size) = 0;

    virtual void set_uniform_value(const std::string& name, uint16_t* value, size_t size) = 0;

    virtual void set_uniform_value(const std::string& name, uint32_t* value, size_t size) = 0;

    virtual void set_uniform_value(const std::string& name, int8_t* value, size_t size) = 0;

    virtual void set_uniform_value(const std::string& name, int16_t* value, size_t size) = 0;

    virtual void set_uniform_value(const std::string& name, int32_t* value, size_t size) = 0;

    virtual void set_uniform_value(const std::string& name, float* value, size_t size) = 0;

    virtual void set_uniform_value(const std::string& name, double* value, size_t size) = 0;

    virtual bool has_uniform(const std::string& name) = 0;
  };
} // namespace mgl::platform::api