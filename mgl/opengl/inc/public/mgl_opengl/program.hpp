#pragma once

#include "attribute.hpp"
#include "shader.hpp"
#include "subroutine.hpp"
#include "uniform.hpp"
#include "uniform_block.hpp"
#include "varying.hpp"

#include "mgl_core/containers.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"

namespace mgl::opengl
{
  using shaders_outputs = mgl::string_list;
  using fragment_outputs = mgl::dict<std::string, int>;

  class program
  {
public:
    program(const shaders& shaders,
            const shaders_outputs& outputs,
            const fragment_outputs& fragment_outputs,
            bool interleave);

    ~program() = default;

    void bind();
    void unbind();

    const mgl::string_list attributes(bool all = true);
    const mgl::string_list uniforms();
    const mgl::string_list uniform_blocks();
    const mgl::string_list varyings();
    const mgl::string_list subroutines();

    void release();

    bool released() const { return m_glo == 0; }

    const attribute_ref attribute(const std::string& name) const
    {
      if(m_attributes_map.find(name) == m_attributes_map.end())
      {
        return nullptr;
      }
      return m_attributes_map.at(name);
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

    const varying_ref varying(const std::string& name) const
    {
      if(m_varyings_map.find(name) == m_varyings_map.end())
      {
        return nullptr;
      }
      return m_varyings_map.at(name);
    }

    const subroutine_ref subroutine(const std::string& name) const
    {
      if(m_subroutines_map.find(name) == m_subroutines_map.end())
      {
        return nullptr;
      }
      return m_subroutines_map.at(name);
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

    int32_t glo() const { return m_glo; }

private:
    int32_t m_glo;
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
