#pragma once

#include "mgl_core/containers.hpp"
#include "mgl_core/math.hpp"
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

namespace mgl::metal
{
  class context;
  using context_ref = mgl::ref<context>;

  class program;

  class uniform
  {
public:
    enum data_type
    {
      BOOL = 0,
      INT = 1,
      UINT = 2,
      FLOAT = 3,
      DOUBLE = 4,
      SAMPLER = 5,
    };

    ~uniform() = default;

    inline int32_t location() const { return m_location; }
    inline const std::string& name() const { return m_name; }
    inline size_t array_length() const { return m_array_length; }
    inline int32_t dimension() const { return m_dimension; }
    inline data_type type() const { return m_data_type; }

    void get_value(bool& dst);
    void get_value(uint8_t& dst);
    void get_value(uint16_t& dst);
    void get_value(uint32_t& dst);
    void get_value(uint64_t& dst);
    void get_value(int8_t& dst);
    void get_value(int16_t& dst);
    void get_value(int32_t& dst);
    void get_value(int64_t& dst);
    void get_value(float& dst);
    void get_value(double& dst);
    void get_value(glm::vec2& dst);
    void get_value(glm::vec3& dst);
    void get_value(glm::vec4& dst);
    void get_value(glm::mat2& dst);
    void get_value(glm::mat2x3& dst);
    void get_value(glm::mat2x4& dst);
    void get_value(glm::mat3& dst);
    void get_value(glm::mat3x2& dst);
    void get_value(glm::mat3x4& dst);
    void get_value(glm::mat4& dst);
    void get_value(glm::mat4x2& dst);
    void get_value(glm::mat4x3& dst);

    void set_value(bool value);
    void set_value(uint8_t value);
    void set_value(uint16_t value);
    void set_value(uint32_t value);
    void set_value(uint64_t value);
    void set_value(int8_t value);
    void set_value(int16_t value);
    void set_value(int32_t value);
    void set_value(int64_t value);
    void set_value(float value);
    void set_value(double value);
    void set_value(const glm::vec2& value);
    void set_value(const glm::vec3& value);
    void set_value(const glm::vec4& value);
    void set_value(const glm::mat2& value);
    void set_value(const glm::mat2x3& value);
    void set_value(const glm::mat2x4& value);
    void set_value(const glm::mat3& value);
    void set_value(const glm::mat3x2& value);
    void set_value(const glm::mat3x4& value);
    void set_value(const glm::mat4& value);
    void set_value(const glm::mat4x2& value);
    void set_value(const glm::mat4x3& value);

    void set_value(const mgl::uint8_buffer& value);
    void set_value(const mgl::uint16_buffer& value);
    void set_value(const mgl::uint32_buffer& value);
    void set_value(const mgl::uint64_buffer& value);
    void set_value(const mgl::int8_buffer& value);
    void set_value(const mgl::int16_buffer& value);
    void set_value(const mgl::int32_buffer& value);
    void set_value(const mgl::int64_buffer& value);
    void set_value(const mgl::float32_buffer& value);
    void set_value(const mgl::float64_buffer& value);

private:
    friend class program;

    uniform(const context_ref& ctx,
            program* prg,
            int32_t location,
            const std::string& name,
            data_type type,
            int32_t dimension,
            size_t array_length,
            void* native_uniform);

    context_ref m_ctx;
    program* m_program;
    void* m_native_uniform;
    int32_t m_location;
    std::string m_name;
    data_type m_data_type;
    int32_t m_dimension;
    size_t m_array_length;
  };

  using uniform_ref = mgl::ref<uniform>;
  using uniforms_dict = mgl::dict<std::string, uniform_ref>;

} // namespace mgl::metal
