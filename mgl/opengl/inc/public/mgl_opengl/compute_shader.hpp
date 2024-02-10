#pragma once

#include "gl_object.hpp"
#include "uniform.hpp"
#include "uniform_block.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"

namespace mgl::opengl
{
  class compute_shader : public gl_object
  {
public:
    ~compute_shader() = default;

    virtual void release() override final;

    void run(int32_t x = 1, int32_t y = 1, int32_t z = 1, bool barrier = true);

    bool has_uniform(const std::string& name) const
    {
      return m_uniforms_map.find(name) != m_uniforms_map.end();
    }

    const uniform_ref uniform(const std::string& name) const
    {
      if(m_uniforms_map.find(name) == m_uniforms_map.end())
      {
        return nullptr;
      }
      return m_uniforms_map.at(name);
    }

    const mgl::string_list uniforms() const
    {
      auto result = mgl::string_list();
      for(auto&& a : m_uniforms_map)
      {
        result.push_back(a.first);
      }
      return result;
    }

    bool has_uniform_block(const std::string& name) const
    {
      return m_uniform_blocks_map.find(name) != m_uniform_blocks_map.end();
    }

    const uniform_block_ref uniform_block(const std::string& name) const
    {
      if(m_uniform_blocks_map.find(name) == m_uniform_blocks_map.end())
      {
        return nullptr;
      }
      return m_uniform_blocks_map.at(name);
    }

    size_t num_uniforms() const { return m_uniforms_map.size(); }

    size_t num_uniform_blocks() const { return m_uniform_blocks_map.size(); }

    const mgl::string_list uniform_blocks() const
    {
      auto result = mgl::string_list();
      for(auto&& a : m_uniform_blocks_map)
      {
        result.push_back(a.first);
      }
      return result;
    }

    const uniform_ref operator[](const std::string& name) const { return uniform(name); }

    void get_value(const std::string& name, bool& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    void get_value(const std::string& name, uint8_t& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    void get_value(const std::string& name, uint16_t& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    void get_value(const std::string& name, uint32_t& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    void get_value(const std::string& name, int& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    void get_value(const std::string& name, float& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    void get_value(const std::string& name, glm::vec2& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    void get_value(const std::string& name, glm::vec3& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    void get_value(const std::string& name, glm::vec4& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    void get_value(const std::string& name, glm::mat2& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    void get_value(const std::string& name, glm::mat2x3& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    void get_value(const std::string& name, glm::mat2x4& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    void get_value(const std::string& name, glm::mat3& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    void get_value(const std::string& name, glm::mat3x2& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    void get_value(const std::string& name, glm::mat3x4& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    void get_value(const std::string& name, glm::mat4& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    void get_value(const std::string& name, glm::mat4x2& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    void get_value(const std::string& name, glm::mat4x3& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    void get_value(const std::string& name, uint8_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    void get_value(const std::string& name, uint16_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    void get_value(const std::string& name, uint32_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    void get_value(const std::string& name, int8_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    void get_value(const std::string& name, int16_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    void get_value(const std::string& name, int32_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    void get_value(const std::string& name, float32_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    void get_value(const std::string& name, float64_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    void get_value(const std::string& name, uint8_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value, size);
    }

    void get_value(const std::string& name, uint16_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value, size);
    }

    void get_value(const std::string& name, uint32_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value, size);
    }

    void get_value(const std::string& name, int8_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value, size);
    }

    void get_value(const std::string& name, int16_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value, size);
    }

    void get_value(const std::string& name, int32_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value, size);
    }

    void get_value(const std::string& name, float* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value, size);
    }

    void get_value(const std::string& name, double* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value, size);
    }

    void set_value(const std::string& name, bool value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    void set_value(const std::string& name, uint8_t value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    void set_value(const std::string& name, uint16_t value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    void set_value(const std::string& name, uint32_t value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    void set_value(const std::string& name, int value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    void set_value(const std::string& name, float value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    void set_value(const std::string& name, const glm::vec2 value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    void set_value(const std::string& name, const glm::vec3 value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    void set_value(const std::string& name, const glm::vec4 value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    void set_value(const std::string& name, const glm::mat2 value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    void set_value(const std::string& name, const glm::mat2x3 value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    void set_value(const std::string& name, const glm::mat2x4 value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    void set_value(const std::string& name, const glm::mat3 value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    void set_value(const std::string& name, const glm::mat3x2 value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    void set_value(const std::string& name, const glm::mat3x4 value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    void set_value(const std::string& name, const glm::mat4 value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    void set_value(const std::string& name, const glm::mat4x2 value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    void set_value(const std::string& name, const glm::mat4x3 value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    void set_value(const std::string& name, const uint8_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    void set_value(const std::string& name, const uint16_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    void set_value(const std::string& name, const uint32_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    void set_value(const std::string& name, const int8_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    void set_value(const std::string& name, const int16_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    void set_value(const std::string& name, const int32_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    void set_value(const std::string& name, const float32_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    void set_value(const std::string& name, const float64_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    void set_value(const std::string& name, uint8_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value, size);
    }

    void set_value(const std::string& name, uint16_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value, size);
    }

    void set_value(const std::string& name, uint32_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value, size);
    }

    void set_value(const std::string& name, int8_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value, size);
    }

    void set_value(const std::string& name, int16_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value, size);
    }

    void set_value(const std::string& name, int32_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value, size);
    }

    void set_value(const std::string& name, float* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value, size);
    }

    void set_value(const std::string& name, double* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value, size);
    }

private:
    friend class context;

    compute_shader(const context_ref& ctx,
                   const std::string& source,
                   const std::string& filename = "");

    std::string m_filename;
    int32_t m_shader_glo;
    uniforms_dict m_uniforms_map;
    uniform_blocks_dict m_uniform_blocks_map;
  };

  using compute_shader_ref = std::shared_ptr<compute_shader>;

} // namespace  mgl::opengl