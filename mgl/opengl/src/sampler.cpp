#include "mgl_opengl/sampler.hpp"
#include "mgl_opengl/context.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/math.hpp"

#include "mgl_opengl_internal.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  sampler::sampler(const context_ref& ctx)
      : gl_object(ctx)
  {
    m_filter = { GL_LINEAR, GL_LINEAR };
    m_anisotropy = 0.0;
    m_repeat_x = true;
    m_repeat_y = true;
    m_repeat_z = true;
    m_compare_func = mgl::opengl::compare_func::NONE;
    m_border_color[0] = 0.0;
    m_border_color[1] = 0.0;
    m_border_color[2] = 0.0;
    m_border_color[3] = 0.0;
    m_min_lod = -1000.0;
    m_max_lod = 1000.0;

    GLuint glo = 0;
    glGenSamplers(1, &glo);

    if(!glo)
    {
      MGL_CORE_ASSERT(false, "[Sampler] Cannot create sampler.");
      return;
    }

    gl_object::set_glo(glo);
  }

  void sampler::release()
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Sampler] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Sampler] Resource context not current.");
    GLuint glo = gl_object::glo();
    glDeleteSamplers(1, &glo);
    gl_object::set_glo(GL_ZERO);
  }

  void sampler::use(int index)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Sampler] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Sampler] Resource context not current.");
    glBindSampler(index, gl_object::glo());
  }

  void sampler::clear(int index)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Sampler] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Sampler] Resource context not current.");
    glBindSampler(index, 0);
  }

  void sampler::set_repeat_x(bool value)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Sampler] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Sampler] Resource context not current.");
    m_repeat_x = value;

    if(value)
    {
      glSamplerParameteri(gl_object::glo(), GL_TEXTURE_WRAP_S, GL_REPEAT);
      return;
    }

    glSamplerParameteri(gl_object::glo(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  }

  void sampler::set_repeat_y(bool value)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Sampler] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Sampler] Resource context not current.");
    m_repeat_y = value;

    if(value)
    {
      glSamplerParameteri(gl_object::glo(), GL_TEXTURE_WRAP_T, GL_REPEAT);
      return;
    }

    glSamplerParameteri(gl_object::glo(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  void sampler::set_repeat_z(bool value)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Sampler] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Sampler] Resource context not current.");
    m_repeat_z = value;

    if(value)
    {
      glSamplerParameteri(gl_object::glo(), GL_TEXTURE_WRAP_R, GL_REPEAT);
      return;
    }

    glSamplerParameteri(gl_object::glo(), GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  }

  void sampler::set_filter(const sampler::filter& value)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Sampler] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Sampler] Resource context not current.");
    m_filter = value;
    glSamplerParameteri(gl_object::glo(), GL_TEXTURE_MIN_FILTER, m_filter.min_filter);
    glSamplerParameteri(gl_object::glo(), GL_TEXTURE_MAG_FILTER, m_filter.mag_filter);
  }

  void sampler::set_compare_func(mgl::opengl::compare_func value)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Sampler] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Sampler] Resource context not current.");
    m_compare_func = value;
    if(m_compare_func == mgl::opengl::compare_func::NONE)
    {
      glSamplerParameteri(gl_object::glo(), GL_TEXTURE_COMPARE_MODE, GL_NONE);
      return;
    }

    glSamplerParameteri(gl_object::glo(), GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glSamplerParameteri(gl_object::glo(), GL_TEXTURE_COMPARE_FUNC, m_compare_func);
  }

  void sampler::set_anisotropy(float value)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Sampler] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Sampler] Resource context not current.");

    if(gl_object::ctx()->max_anisotropy() == 0)
    {
      return;
    }

    m_anisotropy = (float)MGL_MIN(MGL_MAX(value, 1.0), gl_object::ctx()->max_anisotropy());

    glSamplerParameterf(gl_object::glo(), GL_TEXTURE_MAX_ANISOTROPY, m_anisotropy);
  }

  void sampler::set_border_color(const glm::vec4& value)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Sampler] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Sampler] Resource context not current.");

    m_border_color = value;
    glSamplerParameteri(gl_object::glo(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glSamplerParameteri(gl_object::glo(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glSamplerParameteri(gl_object::glo(), GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glSamplerParameterfv(gl_object::glo(), GL_TEXTURE_BORDER_COLOR, (GLfloat*)&m_border_color);
  }

  void sampler::set_min_lod(float value)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Sampler] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Sampler] Resource context not current.");
    m_min_lod = value;
    glSamplerParameterf(gl_object::glo(), GL_TEXTURE_MIN_LOD, m_min_lod);
  }

  void sampler::set_max_lod(float value)
  {
    MGL_CORE_ASSERT(!gl_object::released(), "[Sampler] Resource already released or not valid.");
    MGL_CORE_ASSERT(gl_object::ctx()->is_current(), "[Sampler] Resource context not current.");
    m_max_lod = value;
    glSamplerParameterf(gl_object::glo(), GL_TEXTURE_MAX_LOD, m_max_lod);
  }

} // namespace  mgl::opengl
