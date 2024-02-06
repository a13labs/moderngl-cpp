/*
   Copyright 2022 Alexandre Pires (c.alexandre.pires@gmail.com)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#pragma once

#include "gl_object.hpp"
#include "uniform.hpp"
#include "uniform_block.hpp"

#include "mgl_core/string.hpp"

namespace mgl::opengl
{
  class context;

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

    compute_shader(context* ctx, const std::string& source);

    int32_t m_shader_glo;
    uniforms_dict m_uniforms_map;
    uniform_blocks_dict m_uniform_blocks_map;
  };

  using compute_shader_ref = std::shared_ptr<compute_shader>;

} // namespace  mgl::opengl