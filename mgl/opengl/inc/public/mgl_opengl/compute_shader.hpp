#pragma once

#include "gl_object.hpp"
#include "uniform.hpp"
#include "uniform_block.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"

namespace mgl::opengl
{
  class compute_shader;
  using compute_shader_ref = mgl::ref<compute_shader>;

  /**
   * @brief The compute_shader class represents a compute shader in the OpenGL context.
   * 
   * It provides methods for running the compute shader, querying the number of uniforms and uniform blocks, and getting and setting uniform values.
   */
  class compute_shader : public gl_object
  {
public:
    /**
     * @brief The compute_shader constructor creates a new compute shader with the specified context, source, and filename.
     * 
     * @param ctx The context.
     * @param source The source.
     * @param filename The filename (optional).
     */
    ~compute_shader() = default;

    /**
     * @brief The release method releases the compute shader.
     */
    virtual void release() override final;

    /**
     * @brief The run method runs the compute shader with the specified number of work groups and barrier.
     * 
     * @param x The number of work groups in the x direction.
     * @param y The number of work groups in the y direction.
     * @param z The number of work groups in the z direction.
     * @param barrier Whether to insert a barrier after the compute shader.
     */
    void run(int32_t x = 1, int32_t y = 1, int32_t z = 1, bool barrier = true);

    /**
     * @brief The has_uniform method returns whether the compute shader has a uniform with the specified name.
     * 
     * @param name The name.
     * @return Whether the compute shader has a uniform with the specified name.
     */
    bool has_uniform(const std::string& name) const
    {
      return m_uniforms_map.find(name) != m_uniforms_map.end();
    }

    /**
     * @brief The uniform method returns the uniform with the specified name.
     * 
     * @param name The name.
     * @return The uniform with the specified name.
     */
    const uniform_ref uniform(const std::string& name) const
    {
      if(m_uniforms_map.find(name) == m_uniforms_map.end())
      {
        return nullptr;
      }
      return m_uniforms_map.at(name);
    }

    /**
     * @brief The num_uniforms method returns the names of uniforms of the compute shader.
     * 
     * @return The names of uniforms of the compute shader.
     */
    const mgl::string_list uniforms() const
    {
      auto result = mgl::string_list();
      for(auto&& a : m_uniforms_map)
      {
        result.push_back(a.first);
      }
      return result;
    }

    /**
     * @brief The has_uniform_block method returns whether the compute shader has a uniform block with the specified name.
     * 
     * @param name The name.
     * @return Whether the compute shader has a uniform block with the specified name.
     */
    bool has_uniform_block(const std::string& name) const
    {
      return m_uniform_blocks_map.find(name) != m_uniform_blocks_map.end();
    }

    /**
     * @brief The uniform_block method returns the uniform block with the specified name.
     * 
     * @param name The name.
     * @return The uniform block with the specified name.
     */
    const uniform_block_ref uniform_block(const std::string& name) const
    {
      if(m_uniform_blocks_map.find(name) == m_uniform_blocks_map.end())
      {
        return nullptr;
      }
      return m_uniform_blocks_map.at(name);
    }

    /**
     * @brief The num_uniform method returns the number of uniform blocks of the compute shader.
     * 
     * @return The number of uniform of the compute shader.
     */
    size_t num_uniforms() const { return m_uniforms_map.size(); }

    /**
     * @brief The num_uniform_blocks method returns the number of uniform blocks of the compute shader.
     * 
     * @return The number of uniform blocks of the compute shader.
     */
    size_t num_uniform_blocks() const { return m_uniform_blocks_map.size(); }

    /**
     * @brief The uniform_blocks method returns the names of uniform blocks of the compute shader.
     * 
     * @return The names of uniform blocks of the compute shader.
     */
    const mgl::string_list uniform_blocks() const
    {
      auto result = mgl::string_list();
      for(auto&& a : m_uniform_blocks_map)
      {
        result.push_back(a.first);
      }
      return result;
    }

    /**
     * @brief The operator[] method returns the uniform with the specified name.
     * 
     * @param name The name.
     * @return The uniform with the specified name.
     */
    const uniform_ref operator[](const std::string& name) const { return uniform(name); }

    /**
     * @brief get_value method gets a bool value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, bool& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    /**
     * @brief get_value method gets a uint8_t value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, uint8_t& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    /**
     * @brief get_value method gets a uint16_t value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, uint16_t& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    /**
     * @brief get_value method gets a uint32_t value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, uint32_t& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    /**
     * @brief get_value method gets a int32_t value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, int32_t& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    /**
     * @brief get_valuemethod gets a float value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, float& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    /**
     * @brief get_value method gets a glm::vec2 value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, glm::vec2& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    /**
     * @brief get_value method gets a glm::vec3 value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, glm::vec3& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    /**
     * @brief get_value method gets a glm::vec4 value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, glm::vec4& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    /**
     * @brief get_value method gets a glm::mat2 value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, glm::mat2& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    /**
     * @brief get_value method gets a glm::mat2x3 value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, glm::mat2x3& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    /**
     * @brief get_value method gets a glm::mat2x4 value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, glm::mat2x4& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    /**
     * @brief get_value method gets a glm::mat3 value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, glm::mat3& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    /**
     * @brief get_value method gets a glm::mat3x2 value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, glm::mat3x2& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    /**
     * @brief get_value method gets a glm::mat3x4 value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, glm::mat3x4& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    /**
     * @brief get_value method gets a glm::mat4 value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, glm::mat4& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    /**
     * @brief get_value method gets a glm::mat4x2 value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, glm::mat4x2& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    /**
     * @brief get_value method gets a glm::mat4x3 value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, glm::mat4x3& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    /**
     * @brief get_value method gets a uint8_buffer value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, uint8_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    /**
     * @brief get_value method gets a uint16_buffer value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, uint16_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    /**
     * @brief get_value method gets a uint32_buffer value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, uint32_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    /**
     * @brief get_value method gets a int8_buffer value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, int8_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    /**
     * @brief get_value method gets a int16_buffer value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, int16_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    /**
     * @brief get_value method gets a int32_buffer value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, int32_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    /**
     * @brief get_value method gets a float32_buffer value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, float32_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    /**
     * @brief get_value method gets a float64_buffer value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, float64_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value);
    }

    /**
     * @brief get_value method gets a uint8_t value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, uint8_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value, size);
    }

    /**
     * @brief get_value method gets a uint16_t value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, uint16_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value, size);
    }

    /**
     * @brief get_value method gets a uint32_t value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, uint32_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value, size);
    }

    /**
     * @brief get_value method gets a int8_t value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, int8_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value, size);
    }

    /**
     * @brief get_value method gets a int16_t value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, int16_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value, size);
    }

    /**
     * @brief get_value method gets a int32_t value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, int32_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value, size);
    }

    /**
     * @brief get_value method gets a float value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, float* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value, size);
    }

    /**
     * @brief get_value method gets a double value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void get_value(const std::string& name, double* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->get_value(value, size);
    }

    /**
     * @brief set_value method sets a bool value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, bool value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    /**
     * @brief set_value method sets a uint8_t value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, uint8_t value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    /**
     * @brief set_value method sets a uint16_t value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, uint16_t value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    /**
     * @brief set_value method sets a uint32_t value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, uint32_t value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    /**
     * @brief set_value method sets a int32_t value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, int32_t value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    /**
     * @brief set_value method sets a float value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, float value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    /**
     * @brief set_value method sets a glm::vec2 value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, const glm::vec2 value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    /**
     * @brief set_value method sets a glm::vec3 value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, const glm::vec3 value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    /**
     * @brief set_value method sets a glm::vec4 value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, const glm::vec4 value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    /**
     * @brief set_value method sets a glm::mat2 value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, const glm::mat2 value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    /**
     * @brief set_value method sets a glm::mat2x3 value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, const glm::mat2x3 value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    /**
     * @brief set_value method sets a glm::mat2x4 value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, const glm::mat2x4 value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    /**
     * @brief set_value method sets a glm::mat3 value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, const glm::mat3 value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    /**
     * @brief set_value method sets a glm::mat3x2 value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, const glm::mat3x2 value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    /**
     * @brief set_value method sets a glm::mat3x4 value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, const glm::mat3x4 value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    /**
     * @brief set_value method sets a glm::mat4 value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, const glm::mat4 value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    /**
     * @brief set_value method sets a glm::mat4x2 value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, const glm::mat4x2 value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    /**
     * @brief set_value method sets a glm::mat4x3 value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, const glm::mat4x3 value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    /**
     * @brief set_value method sets a uint8_buffer value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, const uint8_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    /**
     * @brief set_value method sets a uint16_buffer value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, const uint16_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    /**
     * @brief set_value method sets a uint32_buffer value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, const uint32_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    /**
     * @brief set_value method sets a int8_buffer value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, const int8_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    /**
     * @brief set_value method sets a int16_buffer value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, const int16_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    /**
     * @brief set_value method sets a int32_buffer value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, const int32_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    /**
     * @brief set_value method sets a float32_buffer value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, const float32_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    /**
     * @brief set_value method sets a float64_buffer value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, const float64_buffer& value)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value);
    }

    /**
     * @brief set_value method sets a uint8_t value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, uint8_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value, size);
    }

    /**
     * @brief set_value method sets a uint16_t value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, uint16_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value, size);
    }

    /**
     * @brief set_value method sets a uint32_t value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, uint32_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value, size);
    }

    /**
     * @brief set_value method sets a int8_t value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, int8_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value, size);
    }

    /**
     * @brief set_value method sets a int16_t value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, int16_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value, size);
    }

    /**
     * @brief set_value method sets a int32_t value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, int32_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value, size);
    }

    /**
     * @brief set_value method sets a float value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, float* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value, size);
    }

    /**
     * @brief set_value method sets a double value of the uniform with the specified name.
     * 
     * @param name The name.
     * @param value The value.
     */
    void set_value(const std::string& name, double* value, size_t size)
    {
      MGL_CORE_ASSERT(m_uniforms_map.find(name) != m_uniforms_map.end(),
                      "[Compute Shader] Uniform not found.");
      m_uniforms_map.at(name)->set_value(value, size);
    }

private:
    friend class context;

    /**
     * @brief The compute_shader class represents a compute shader.
     * 
     * @param ctx The context.
     * @param source The source code of the compute shader.
     * @param filename The filename of the compute shader (optional)
     */
    compute_shader(const context_ref& ctx,
                   const std::string& source,
                   const std::string& filename = "");

    std::string m_filename; ///< The filename of the compute shader.
    int32_t m_shader_glo; ///< The OpenGL handle of the compute shader.
    uniforms_dict m_uniforms_map; ///< The uniforms of the compute shader.
    uniform_blocks_dict m_uniform_blocks_map; ///< The uniform blocks of the compute shader.
  };

} // namespace  mgl::opengl