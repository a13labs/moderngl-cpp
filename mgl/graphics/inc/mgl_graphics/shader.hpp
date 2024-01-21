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
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"
#include "mgl_window/api/opengl.hpp"

namespace mgl::graphics
{
  class shader
  {
public:
    virtual ~shader() = default;

    virtual void bind();
    virtual void unbind();

    virtual void prepare() = 0;
    virtual const mgl::string_list& attributes() = 0;
    virtual void load() = 0;
    virtual void unload() = 0;

    void get_uniform_value(const std::string& name, bool& value);
    void get_uniform_value(const std::string& name, uint8_t& value);
    void get_uniform_value(const std::string& name, uint16_t& value);
    void get_uniform_value(const std::string& name, uint32_t& value);
    void get_uniform_value(const std::string& name, int& value);
    void get_uniform_value(const std::string& name, float& value);
    void get_uniform_value(const std::string& name, const glm::vec2& value);
    void get_uniform_value(const std::string& name, const glm::vec3& value);
    void get_uniform_value(const std::string& name, const glm::vec4& value);
    void get_uniform_value(const std::string& name, const glm::mat2& value);
    void get_uniform_value(const std::string& name, const glm::mat2x3& value);
    void get_uniform_value(const std::string& name, const glm::mat2x4& value);
    void get_uniform_value(const std::string& name, const glm::mat3& value);
    void get_uniform_value(const std::string& name, const glm::mat3x2& value);
    void get_uniform_value(const std::string& name, const glm::mat3x4& value);
    void get_uniform_value(const std::string& name, const glm::mat4& value);
    void get_uniform_value(const std::string& name, const glm::mat4x2& value);
    void get_uniform_value(const std::string& name, const glm::mat4x3& value);
    void get_uniform_value(const std::string& name, uint8_t* value, size_t size);
    void get_uniform_value(const std::string& name, uint16_t* value, size_t size);
    void get_uniform_value(const std::string& name, uint32_t* value, size_t size);
    void get_uniform_value(const std::string& name, int* value, size_t size);
    void get_uniform_value(const std::string& name, float* value, size_t size);

    void set_uniform_value(const std::string& name, bool value);
    void set_uniform_value(const std::string& name, uint8_t value);
    void set_uniform_value(const std::string& name, uint16_t value);
    void set_uniform_value(const std::string& name, uint32_t value);
    void set_uniform_value(const std::string& name, int value);
    void set_uniform_value(const std::string& name, float value);
    void set_uniform_value(const std::string& name, const glm::vec2 value);
    void set_uniform_value(const std::string& name, const glm::vec3 value);
    void set_uniform_value(const std::string& name, const glm::vec4 value);
    void set_uniform_value(const std::string& name, const glm::mat2 value);
    void set_uniform_value(const std::string& name, const glm::mat2x3 value);
    void set_uniform_value(const std::string& name, const glm::mat2x4 value);
    void set_uniform_value(const std::string& name, const glm::mat3 value);
    void set_uniform_value(const std::string& name, const glm::mat3x2 value);
    void set_uniform_value(const std::string& name, const glm::mat3x4 value);
    void set_uniform_value(const std::string& name, const glm::mat4 value);
    void set_uniform_value(const std::string& name, const glm::mat4x2 value);
    void set_uniform_value(const std::string& name, const glm::mat4x3 value);
    void set_uniform_value(const std::string& name, uint8_t* value, size_t size);
    void set_uniform_value(const std::string& name, uint16_t* value, size_t size);
    void set_uniform_value(const std::string& name, uint32_t* value, size_t size);
    void set_uniform_value(const std::string& name, int* value, size_t size);
    void set_uniform_value(const std::string& name, float* value, size_t size);

    virtual const mgl::window::api::program_ref& native() = 0;

protected:
  };

  using shader_ref = mgl::ref<shader>;
} // namespace mgl::graphics
