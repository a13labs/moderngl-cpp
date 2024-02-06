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
#include "enums.hpp"

#include "mgl_core/memory.hpp"

#include "glm/vec4.hpp"

namespace mgl::opengl
{
  class context;

  class sampler
  {
public:
    struct filter
    {
      int32_t min_filter;
      int32_t mag_filter;
    };

    sampler();
    ~sampler() = default;

public:
    void use(int32_t index = 0);
    void clear(int32_t index = 0);

    void release();
    bool released() const { return m_glo == 0; };

    bool repeat_x() const { return m_repeat_x; }

    bool repeat_y() const { return m_repeat_y; }

    bool repeat_z() const { return m_repeat_z; }

    float anisotropy() const { return m_anisotropy; }

    float min_lod() const { return m_min_lod; }

    float max_lod() const { return m_max_lod; }

    const glm::vec4& border_color() const { return m_border_color; }

    const filter& sampler_filter() const { return m_filter; }

    mgl::opengl::compare_func compare_func() { return m_compare_func; }

    void set_border_color(float r, float g, float b, float a) { set_border_color({ r, g, b, a }); }

    int32_t glo() const { return m_glo; };

    void set_repeat_x(bool value);
    void set_repeat_y(bool value);
    void set_repeat_z(bool value);
    void set_filter(const filter& value);
    void set_compare_func(mgl::opengl::compare_func value);
    void set_anisotropy(float value);
    void set_border_color(const glm::vec4& value);
    float min_lod() const;
    void set_min_lod(float value);
    float max_lod() const;
    void set_max_lod(float value);

private:
    int32_t m_glo;
    float m_anisotropy;
    bool m_repeat_x;
    bool m_repeat_y;
    bool m_repeat_z;
    float m_min_lod;
    float m_max_lod;
    filter m_filter;
    glm::vec4 m_border_color;
    mgl::opengl::compare_func m_compare_func;
  };

  using sampler_ref = mgl::ref<sampler>;
  using samplers = mgl::ref_list<sampler>;

} // namespace  mgl::opengl
