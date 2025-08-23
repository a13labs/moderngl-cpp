#import <Metal/Metal.h>
#import <Foundation/Foundation.h>

#include "mgl_metal/uniform.hpp"
#include "mgl_metal/context.hpp"
#include "mgl_metal/program.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_core/log.hpp"

namespace mgl::metal
{
  uniform::uniform(const context_ref& ctx,
                   program* prg,
                   int32_t location,
                   const std::string& name,
                   data_type type,
                   int32_t dimension,
                   size_t array_length,
                   void* native_uniform)
      : m_ctx(ctx)
      , m_program(prg)
      , m_native_uniform(native_uniform)
      , m_location(location)
      , m_name(name)
      , m_data_type(type)
      , m_dimension(dimension)
      , m_array_length(array_length)
  {
    MGL_CORE_ASSERT(ctx && ctx->is_valid(), "Invalid context");
    MGL_CORE_ASSERT(prg, "Invalid program");
  }

  // Get value implementations (dummy)
  void uniform::get_value(bool& dst)
  {
    // TODO: Implement Metal uniform value retrieval
    dst = false;
    MGL_CORE_WARN("[Metal Uniform] get_value(bool) not implemented");
  }

  void uniform::get_value(uint8_t& dst)
  {
    // TODO: Implement Metal uniform value retrieval
    dst = 0;
    MGL_CORE_WARN("[Metal Uniform] get_value(uint8_t) not implemented");
  }

  void uniform::get_value(uint16_t& dst)
  {
    // TODO: Implement Metal uniform value retrieval
    dst = 0;
    MGL_CORE_WARN("[Metal Uniform] get_value(uint16_t) not implemented");
  }

  void uniform::get_value(uint32_t& dst)
  {
    // TODO: Implement Metal uniform value retrieval
    dst = 0;
    MGL_CORE_WARN("[Metal Uniform] get_value(uint32_t) not implemented");
  }

  void uniform::get_value(uint64_t& dst)
  {
    // TODO: Implement Metal uniform value retrieval
    dst = 0;
    MGL_CORE_WARN("[Metal Uniform] get_value(uint64_t) not implemented");
  }

  void uniform::get_value(int8_t& dst)
  {
    // TODO: Implement Metal uniform value retrieval
    dst = 0;
    MGL_CORE_WARN("[Metal Uniform] get_value(int8_t) not implemented");
  }

  void uniform::get_value(int16_t& dst)
  {
    // TODO: Implement Metal uniform value retrieval
    dst = 0;
    MGL_CORE_WARN("[Metal Uniform] get_value(int16_t) not implemented");
  }

  void uniform::get_value(int32_t& dst)
  {
    // TODO: Implement Metal uniform value retrieval
    dst = 0;
    MGL_CORE_WARN("[Metal Uniform] get_value(int32_t) not implemented");
  }

  void uniform::get_value(int64_t& dst)
  {
    // TODO: Implement Metal uniform value retrieval
    dst = 0;
    MGL_CORE_WARN("[Metal Uniform] get_value(int64_t) not implemented");
  }

  void uniform::get_value(float& dst)
  {
    // TODO: Implement Metal uniform value retrieval
    dst = 0.0f;
    MGL_CORE_WARN("[Metal Uniform] get_value(float) not implemented");
  }

  void uniform::get_value(double& dst)
  {
    // TODO: Implement Metal uniform value retrieval
    dst = 0.0;
    MGL_CORE_WARN("[Metal Uniform] get_value(double) not implemented");
  }

  void uniform::get_value(glm::vec2& dst)
  {
    // TODO: Implement Metal uniform value retrieval
    dst = glm::vec2(0.0f);
    MGL_CORE_WARN("[Metal Uniform] get_value(glm::vec2) not implemented");
  }

  void uniform::get_value(glm::vec3& dst)
  {
    // TODO: Implement Metal uniform value retrieval
    dst = glm::vec3(0.0f);
    MGL_CORE_WARN("[Metal Uniform] get_value(glm::vec3) not implemented");
  }

  void uniform::get_value(glm::vec4& dst)
  {
    // TODO: Implement Metal uniform value retrieval
    dst = glm::vec4(0.0f);
    MGL_CORE_WARN("[Metal Uniform] get_value(glm::vec4) not implemented");
  }

  void uniform::get_value(glm::mat2& dst)
  {
    // TODO: Implement Metal uniform value retrieval
    dst = glm::mat2(1.0f);
    MGL_CORE_WARN("[Metal Uniform] get_value(glm::mat2) not implemented");
  }

  void uniform::get_value(glm::mat2x3& dst)
  {
    // TODO: Implement Metal uniform value retrieval
    dst = glm::mat2x3(1.0f);
    MGL_CORE_WARN("[Metal Uniform] get_value(glm::mat2x3) not implemented");
  }

  void uniform::get_value(glm::mat2x4& dst)
  {
    // TODO: Implement Metal uniform value retrieval
    dst = glm::mat2x4(1.0f);
    MGL_CORE_WARN("[Metal Uniform] get_value(glm::mat2x4) not implemented");
  }

  void uniform::get_value(glm::mat3& dst)
  {
    // TODO: Implement Metal uniform value retrieval
    dst = glm::mat3(1.0f);
    MGL_CORE_WARN("[Metal Uniform] get_value(glm::mat3) not implemented");
  }

  void uniform::get_value(glm::mat3x2& dst)
  {
    // TODO: Implement Metal uniform value retrieval
    dst = glm::mat3x2(1.0f);
    MGL_CORE_WARN("[Metal Uniform] get_value(glm::mat3x2) not implemented");
  }

  void uniform::get_value(glm::mat3x4& dst)
  {
    // TODO: Implement Metal uniform value retrieval
    dst = glm::mat3x4(1.0f);
    MGL_CORE_WARN("[Metal Uniform] get_value(glm::mat3x4) not implemented");
  }

  void uniform::get_value(glm::mat4& dst)
  {
    // TODO: Implement Metal uniform value retrieval
    dst = glm::mat4(1.0f);
    MGL_CORE_WARN("[Metal Uniform] get_value(glm::mat4) not implemented");
  }

  void uniform::get_value(glm::mat4x2& dst)
  {
    // TODO: Implement Metal uniform value retrieval
    dst = glm::mat4x2(1.0f);
    MGL_CORE_WARN("[Metal Uniform] get_value(glm::mat4x2) not implemented");
  }

  void uniform::get_value(glm::mat4x3& dst)
  {
    // TODO: Implement Metal uniform value retrieval
    dst = glm::mat4x3(1.0f);
    MGL_CORE_WARN("[Metal Uniform] get_value(glm::mat4x3) not implemented");
  }

  // Set value implementations (dummy)
  void uniform::set_value(bool value)
  {
    // TODO: Implement Metal uniform value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(bool) not implemented");
  }

  void uniform::set_value(uint8_t value)
  {
    // TODO: Implement Metal uniform value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(uint8_t) not implemented");
  }

  void uniform::set_value(uint16_t value)
  {
    // TODO: Implement Metal uniform value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(uint16_t) not implemented");
  }

  void uniform::set_value(uint32_t value)
  {
    // TODO: Implement Metal uniform value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(uint32_t) not implemented");
  }

  void uniform::set_value(uint64_t value)
  {
    // TODO: Implement Metal uniform value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(uint64_t) not implemented");
  }

  void uniform::set_value(int8_t value)
  {
    // TODO: Implement Metal uniform value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(int8_t) not implemented");
  }

  void uniform::set_value(int16_t value)
  {
    // TODO: Implement Metal uniform value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(int16_t) not implemented");
  }

  void uniform::set_value(int32_t value)
  {
    // TODO: Implement Metal uniform value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(int32_t) not implemented");
  }

  void uniform::set_value(int64_t value)
  {
    // TODO: Implement Metal uniform value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(int64_t) not implemented");
  }

  void uniform::set_value(float value)
  {
    // TODO: Implement Metal uniform value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(float) not implemented");
  }

  void uniform::set_value(double value)
  {
    // TODO: Implement Metal uniform value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(double) not implemented");
  }

  void uniform::set_value(const glm::vec2& value)
  {
    // TODO: Implement Metal uniform value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(glm::vec2) not implemented");
  }

  void uniform::set_value(const glm::vec3& value)
  {
    // TODO: Implement Metal uniform value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(glm::vec3) not implemented");
  }

  void uniform::set_value(const glm::vec4& value)
  {
    // TODO: Implement Metal uniform value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(glm::vec4) not implemented");
  }

  void uniform::set_value(const glm::mat2& value)
  {
    // TODO: Implement Metal uniform value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(glm::mat2) not implemented");
  }

  void uniform::set_value(const glm::mat2x3& value)
  {
    // TODO: Implement Metal uniform value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(glm::mat2x3) not implemented");
  }

  void uniform::set_value(const glm::mat2x4& value)
  {
    // TODO: Implement Metal uniform value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(glm::mat2x4) not implemented");
  }

  void uniform::set_value(const glm::mat3& value)
  {
    // TODO: Implement Metal uniform value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(glm::mat3) not implemented");
  }

  void uniform::set_value(const glm::mat3x2& value)
  {
    // TODO: Implement Metal uniform value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(glm::mat3x2) not implemented");
  }

  void uniform::set_value(const glm::mat3x4& value)
  {
    // TODO: Implement Metal uniform value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(glm::mat3x4) not implemented");
  }

  void uniform::set_value(const glm::mat4& value)
  {
    // TODO: Implement Metal uniform value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(glm::mat4) not implemented");
  }

  void uniform::set_value(const glm::mat4x2& value)
  {
    // TODO: Implement Metal uniform value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(glm::mat4x2) not implemented");
  }

  void uniform::set_value(const glm::mat4x3& value)
  {
    // TODO: Implement Metal uniform value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(glm::mat4x3) not implemented");
  }

  // Buffer set value implementations (dummy)
  void uniform::set_value(const mgl::uint8_buffer& value)
  {
    // TODO: Implement Metal uniform buffer value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(uint8_buffer) not implemented");
  }

  void uniform::set_value(const mgl::uint16_buffer& value)
  {
    // TODO: Implement Metal uniform buffer value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(uint16_buffer) not implemented");
  }

  void uniform::set_value(const mgl::uint32_buffer& value)
  {
    // TODO: Implement Metal uniform buffer value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(uint32_buffer) not implemented");
  }

  void uniform::set_value(const mgl::uint64_buffer& value)
  {
    // TODO: Implement Metal uniform buffer value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(uint64_buffer) not implemented");
  }

  void uniform::set_value(const mgl::int8_buffer& value)
  {
    // TODO: Implement Metal uniform buffer value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(int8_buffer) not implemented");
  }

  void uniform::set_value(const mgl::int16_buffer& value)
  {
    // TODO: Implement Metal uniform buffer value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(int16_buffer) not implemented");
  }

  void uniform::set_value(const mgl::int32_buffer& value)
  {
    // TODO: Implement Metal uniform buffer value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(int32_buffer) not implemented");
  }

  void uniform::set_value(const mgl::int64_buffer& value)
  {
    // TODO: Implement Metal uniform buffer value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(int64_buffer) not implemented");
  }

  void uniform::set_value(const mgl::float32_buffer& value)
  {
    // TODO: Implement Metal uniform buffer value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(float32_buffer) not implemented");
  }

  void uniform::set_value(const mgl::float64_buffer& value)
  {
    // TODO: Implement Metal uniform buffer value setting
    MGL_CORE_WARN("[Metal Uniform] set_value(float64_buffer) not implemented");
  }

} // namespace mgl::metal
