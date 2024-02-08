#pragma once

#include "gl_object.hpp"
#include "shader.hpp"
#include "uniform.hpp"
#include "uniform_block.hpp"

#include "mgl_core/containers.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"

namespace mgl::opengl
{
  class context;

  using shaders_outputs = mgl::string_list;
  using fragment_outputs = mgl::dict<std::string, int>;

  class program : public gl_object
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
      data_type* data_type;
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

    struct subroutine
    {
      enum type
      {
        VERTEX_SHADER = 0x8B31,
        FRAGMENT_SHADER = 0x8B30,
        GEOMETRY_SHADER = 0x8DD9,
        TESS_EVALUATION_SHADER = 0x8E87,
        TESS_CONTROL_SHADER = 0x8E88,
      };

      std::string name;
      int32_t index;
      subroutine::type shader;
    };

    using varyings_dict = mgl::dict<std::string, varying>;
    using attributes_dict = mgl::dict<std::string, attribute>;
    using subroutines_dict = mgl::dict<std::string, subroutine>;

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

    const mgl::string_list uniform_blocks() const
    {
      auto result = mgl::string_list();
      for(auto&& a : m_uniform_blocks_map)
      {
        result.push_back(a.first);
      }
      return result;
    }

    const mgl::string_list varyings() const
    {
      auto result = mgl::string_list();
      for(auto&& a : m_varyings_map)
      {
        result.push_back(a.first);
      }
      return result;
    }

    const mgl::string_list subroutines() const
    {
      auto result = mgl::string_list();
      for(auto&& a : m_subroutines_map)
      {
        result.push_back(a.first);
      }
      return result;
    }

    bool has_attribute(const std::string& name) const
    {
      return m_attributes_map.find(name) != m_attributes_map.end();
    }

    const attribute& get_attribute(const std::string& name) const
    {
      MGL_CORE_ASSERT(m_attributes_map.find(name) != m_attributes_map.end(), "Attribute not found");
      return m_attributes_map.at(name);
    }

    bool has_varying(const std::string& name) const
    {
      return m_varyings_map.find(name) != m_varyings_map.end();
    }

    const varying& get_varying(const std::string& name) const
    {
      MGL_CORE_ASSERT(m_varyings_map.find(name) != m_varyings_map.end(), "Varying not found");
      return m_varyings_map.at(name);
    }

    bool has_subroutine(const std::string& name) const
    {
      return m_subroutines_map.find(name) != m_subroutines_map.end();
    }

    const subroutine& get_subroutine(const std::string& name) const
    {
      MGL_CORE_ASSERT(m_subroutines_map.find(name) != m_subroutines_map.end(),
                      "Subroutine not found");
      return m_subroutines_map.at(name);
    }

    const uniform_ref uniform(const std::string& name) const
    {
      if(m_uniforms_map.find(name) == m_uniforms_map.end())
      {
        return nullptr;
      }
      return m_uniforms_map.at(name);
    }

    const uniform_block_ref uniform_block(const std::string& name) const
    {
      if(m_uniform_blocks_map.find(name) == m_uniform_blocks_map.end())
      {
        return nullptr;
      }
      return m_uniform_blocks_map.at(name);
    }

    int32_t geometry_input() const { return m_geometry_input; }

    int32_t geometry_output() const { return m_geometry_output; }

    int32_t geometry_vertices() const { return m_geometry_vertices; }

    bool is_transform() const { return m_transform; }

    size_t num_attributes() const { return m_attributes_map.size(); }

    size_t num_uniforms() const { return m_uniforms_map.size(); }

    size_t num_uniform_blocks() const { return m_uniform_blocks_map.size(); }

    size_t num_varyings() const { return m_varyings_map.size(); }

    size_t num_subroutines() const { return m_subroutines_map.size(); }

    const uniform_ref operator[](const std::string& name) const { return uniform(name); }

private:
    friend class context;

    program(const context_ref& ctx,
            const shaders& shaders,
            const shaders_outputs& outputs,
            const fragment_outputs& fragment_outputs,
            bool interleave);

    int32_t m_geometry_input;
    int32_t m_geometry_output;
    int32_t m_geometry_vertices;
    bool m_transform;

    uniforms_dict m_uniforms_map;
    uniform_blocks_dict m_uniform_blocks_map;
    attributes_dict m_attributes_map;
    varyings_dict m_varyings_map;
    subroutines_dict m_subroutines_map;
  };

  using program_ref = mgl::ref<program>;

} // namespace  mgl::opengl
