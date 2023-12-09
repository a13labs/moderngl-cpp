
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
#include "mgl_opengl/sampler.hpp"
#include "mgl_opengl/context.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/math.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  void sampler::release()
  {
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");

    if(m_released)
      return;

    m_released = true;
    glDeleteSamplers(1, (GLuint*)&m_sampler_obj);
  }

  void sampler::use(int index)
  {
    MGL_CORE_ASSERT(!m_released, "Sampler already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    glBindSampler(index, m_sampler_obj);
  }

  void sampler::clear(int index)
  {
    MGL_CORE_ASSERT(!m_released, "Sampler already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    glBindSampler(index, 0);
  }

  void sampler::set_repeat_x(bool value)
  {
    MGL_CORE_ASSERT(!m_released, "Sampler already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    m_repeat_x = value;

    if(value)
    {
      glSamplerParameteri(m_sampler_obj, GL_TEXTURE_WRAP_S, GL_REPEAT);
      return;
    }

    glSamplerParameteri(m_sampler_obj, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  }

  void sampler::set_repeat_y(bool value)
  {
    MGL_CORE_ASSERT(!m_released, "Sampler already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    m_repeat_y = value;

    if(value)
    {
      glSamplerParameteri(m_sampler_obj, GL_TEXTURE_WRAP_T, GL_REPEAT);
      return;
    }

    glSamplerParameteri(m_sampler_obj, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  void sampler::set_repeat_z(bool value)
  {
    MGL_CORE_ASSERT(!m_released, "Sampler already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    m_repeat_z = value;

    if(value)
    {
      glSamplerParameteri(m_sampler_obj, GL_TEXTURE_WRAP_R, GL_REPEAT);
      return;
    }

    glSamplerParameteri(m_sampler_obj, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  }

  void sampler::set_filter(const sampler::Filter& value)
  {
    MGL_CORE_ASSERT(!m_released, "Sampler already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    m_filter = value;
    glSamplerParameteri(m_sampler_obj, GL_TEXTURE_MIN_FILTER, m_filter.min_filter);
    glSamplerParameteri(m_sampler_obj, GL_TEXTURE_MAG_FILTER, m_filter.mag_filter);
  }

  void sampler::set_compare_func(mgl::opengl::compare_func value)
  {
    MGL_CORE_ASSERT(!m_released, "Sampler already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    m_compare_func = value;
    if(m_compare_func == mgl::opengl::compare_func::NONE)
    {
      glSamplerParameteri(m_sampler_obj, GL_TEXTURE_COMPARE_MODE, GL_NONE);
      return;
    }

    glSamplerParameteri(m_sampler_obj, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glSamplerParameteri(m_sampler_obj, GL_TEXTURE_COMPARE_FUNC, m_compare_func);
  }

  void sampler::set_anisotropy(float value)
  {
    MGL_CORE_ASSERT(!m_released, "Sampler already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");

    if(m_context->max_anisotropy() == 0)
      return;

    m_anisotropy = (float)MGL_MIN(MGL_MAX(value, 1.0), m_context->max_anisotropy());
    glSamplerParameterf(m_sampler_obj, GL_TEXTURE_MAX_ANISOTROPY, m_anisotropy);
  }

  void sampler::set_border_color(const glm::vec4& value)
  {
    MGL_CORE_ASSERT(!m_released, "Sampler already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    m_border_color = value;
    glSamplerParameteri(m_sampler_obj, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glSamplerParameteri(m_sampler_obj, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glSamplerParameteri(m_sampler_obj, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glSamplerParameterfv(m_sampler_obj, GL_TEXTURE_BORDER_COLOR, (GLfloat*)&m_border_color);
  }

  void sampler::set_min_lod(float value)
  {
    MGL_CORE_ASSERT(!m_released, "Sampler already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    m_min_lod = value;
    glSamplerParameterf(m_sampler_obj, GL_TEXTURE_MIN_LOD, m_min_lod);
  }

  void sampler::set_max_lod(float value)
  {
    MGL_CORE_ASSERT(!m_released, "Sampler already released");
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    m_max_lod = value;
    glSamplerParameterf(m_sampler_obj, GL_TEXTURE_MAX_LOD, m_max_lod);
  }

} // namespace  mgl::opengl
