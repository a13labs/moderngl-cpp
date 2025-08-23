#pragma once

#include "object.hpp"
#include "uniform.hpp"

#include "mgl_core/containers.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"

namespace mgl::metal
{
  class context;

  using shaders_outputs = mgl::string_list;
  using fragment_outputs = mgl::dict<std::string, int>;

  class program : public object
  {
public:
    struct data_type
    {
      int32_t dimension;
      int32_t scalar_type;
      int32_t rows_length;
      int32_t row_length;
      bool normalizable;
    };

    struct attribute
    {
      std::string name;
      data_type* d_type;
      int32_t location;
      size_t array_length;
    };

    struct varying
    {
      std::string name;
      int32_t number;
      size_t array_length;
      int32_t dimension;
    };

    using varyings_dict = mgl::dict<std::string, varying>;
    using attributes_dict = mgl::dict<std::string, attribute>;

    ~program() = default;

    virtual void release() override final;

    void bind();
    void unbind();

    const mgl::string_list attributes(bool all = true) const
    {
      auto result = mgl::string_list();
      for(auto&& a : m_attributes_map)
      {
        if(!all && mgl::starts_with(a.first, "gl_"))
        {
          continue;
        }
        result.push_back(a.first);
      }
      return result;
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

    bool has_uniform(const std::string& name) const
    {
      return m_uniforms_map.find(name) != m_uniforms_map.end();
    }

    uniform_ref get_uniform(const std::string& name)
    {
      auto it = m_uniforms_map.find(name);
      if(it == m_uniforms_map.end())
      {
        return nullptr;
      }
      return it->second;
    }

    void get_value(const std::string& name, bool& value);
    void get_value(const std::string& name, uint8_t& value);
    void get_value(const std::string& name, uint16_t& value);
    void get_value(const std::string& name, uint32_t& value);
    void get_value(const std::string& name, uint64_t& value);
    void get_value(const std::string& name, int8_t& value);
    void get_value(const std::string& name, int16_t& value);
    void get_value(const std::string& name, int32_t& value);
    void get_value(const std::string& name, int64_t& value);
    void get_value(const std::string& name, float& value);
    void get_value(const std::string& name, double& value);
    void get_value(const std::string& name, glm::vec2& value);
    void get_value(const std::string& name, glm::vec3& value);
    void get_value(const std::string& name, glm::vec4& value);
    void get_value(const std::string& name, glm::mat2& value);
    void get_value(const std::string& name, glm::mat2x3& value);
    void get_value(const std::string& name, glm::mat2x4& value);
    void get_value(const std::string& name, glm::mat3& value);
    void get_value(const std::string& name, glm::mat3x2& value);
    void get_value(const std::string& name, glm::mat3x4& value);
    void get_value(const std::string& name, glm::mat4& value);
    void get_value(const std::string& name, glm::mat4x2& value);
    void get_value(const std::string& name, glm::mat4x3& value);

    void set_value(const std::string& name, bool value);
    void set_value(const std::string& name, uint8_t value);
    void set_value(const std::string& name, uint16_t value);
    void set_value(const std::string& name, uint32_t value);
    void set_value(const std::string& name, uint64_t value);
    void set_value(const std::string& name, int8_t value);
    void set_value(const std::string& name, int16_t value);
    void set_value(const std::string& name, int32_t value);
    void set_value(const std::string& name, int64_t value);
    void set_value(const std::string& name, float value);
    void set_value(const std::string& name, double value);
    void set_value(const std::string& name, const glm::vec2& value);
    void set_value(const std::string& name, const glm::vec3& value);
    void set_value(const std::string& name, const glm::vec4& value);
    void set_value(const std::string& name, const glm::mat2& value);
    void set_value(const std::string& name, const glm::mat2x3& value);
    void set_value(const std::string& name, const glm::mat2x4& value);
    void set_value(const std::string& name, const glm::mat3& value);
    void set_value(const std::string& name, const glm::mat3x2& value);
    void set_value(const std::string& name, const glm::mat3x4& value);
    void set_value(const std::string& name, const glm::mat4& value);
    void set_value(const std::string& name, const glm::mat4x2& value);
    void set_value(const std::string& name, const glm::mat4x3& value);

    void set_value(const std::string& name, const mgl::uint8_buffer& value);
    void set_value(const std::string& name, const mgl::uint16_buffer& value);
    void set_value(const std::string& name, const mgl::uint32_buffer& value);
    void set_value(const std::string& name, const mgl::uint64_buffer& value);
    void set_value(const std::string& name, const mgl::int8_buffer& value);
    void set_value(const std::string& name, const mgl::int16_buffer& value);
    void set_value(const std::string& name, const mgl::int32_buffer& value);
    void set_value(const std::string& name, const mgl::int64_buffer& value);
    void set_value(const std::string& name, const mgl::float32_buffer& value);
    void set_value(const std::string& name, const mgl::float64_buffer& value);

    const context_ref& ctx() const { return m_ctx; }

    // Metal-specific methods
    void* native_render_pipeline() const { return m_native_render_pipeline; }
    void* native_vertex_function() const { return m_native_vertex_function; }
    void* native_fragment_function() const { return m_native_fragment_function; }

private:
    friend class context;

    program(const context_ref& ctx,
            const std::string& vertex_shader,
            const std::string& fragment_shader,
            const std::string& geometry_shader,
            const std::string& tess_evaluation_shader,
            const std::string& tess_control_shader,
            const fragment_outputs& fragment_outputs,
            const shaders_outputs& varyings_names,
            bool interleaved);

    void init_uniforms();
    void init_attributes();

    context_ref m_ctx;
    void* m_native_render_pipeline;
    void* m_native_vertex_function;
    void* m_native_fragment_function;
    void* m_native_library;

    uniforms_dict m_uniforms_map;
    attributes_dict m_attributes_map;
    varyings_dict m_varyings_map;
  };

  using program_ref = mgl::ref<program>;

} // namespace mgl::metal