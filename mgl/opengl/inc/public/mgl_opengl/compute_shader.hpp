#pragma once

#include "gl_object.hpp"
#include "uniform.hpp"
#include "uniform_block.hpp"

#include "mgl_core/string.hpp"

namespace mgl::opengl
{
  class compute_shader : public gl_object
  {
public:
    ~compute_shader() = default;

    virtual void release() override final;

    const mgl::string_list uniforms() const;

    const mgl::string_list uniform_blocks() const;

    void run(int32_t x = 1, int32_t y = 1, int32_t z = 1);

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

    size_t num_uniforms() const { return m_uniforms_map.size(); }

    size_t num_uniform_blocks() const { return m_uniform_blocks_map.size(); }

    const uniform_ref operator[](const std::string& name) const { return uniform(name); }

private:
    friend class context;

    compute_shader(const context_ref& ctx, const std::string& source);

    int32_t m_shader_glo;
    uniforms_dict m_uniforms_map;
    uniform_blocks_dict m_uniform_blocks_map;
  };

  using compute_shader_ref = std::shared_ptr<compute_shader>;

} // namespace  mgl::opengl