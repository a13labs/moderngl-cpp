#include "mgl_opengl/sampler.hpp"
#include "mgl_opengl/context.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/math.hpp"

#include "mgl_opengl_internal/utils.hpp"

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
      MGL_CORE_ASSERT(false, "Cannot create sampler");
      return;
    }

    m_glo = glo;
  }

  void sampler::release()
  {
    MGL_CORE_ASSERT(m_glo, "Sampler already released");
    glDeleteSamplers(1, (GLuint*)&m_glo);
    m_glo = 0;
  }

  void sampler::use(int index)
  {
    MGL_CORE_ASSERT(m_glo, "Sampler already released");
    glBindSampler(index, m_glo);
  }

  void sampler::clear(int index)
  {
    MGL_CORE_ASSERT(m_glo, "Sampler already released");
    glBindSampler(index, 0);
  }

  void sampler::set_repeat_x(bool value)
  {
    MGL_CORE_ASSERT(m_glo, "Sampler already released");
    m_repeat_x = value;

    if(value)
    {
      glSamplerParameteri(m_glo, GL_TEXTURE_WRAP_S, GL_REPEAT);
      return;
    }

    glSamplerParameteri(m_glo, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  }

  void sampler::set_repeat_y(bool value)
  {
    MGL_CORE_ASSERT(m_glo, "Sampler already released");
    m_repeat_y = value;

    if(value)
    {
      glSamplerParameteri(m_glo, GL_TEXTURE_WRAP_T, GL_REPEAT);
      return;
    }

    glSamplerParameteri(m_glo, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  void sampler::set_repeat_z(bool value)
  {
    MGL_CORE_ASSERT(m_glo, "Sampler already released");
    m_repeat_z = value;

    if(value)
    {
      glSamplerParameteri(m_glo, GL_TEXTURE_WRAP_R, GL_REPEAT);
      return;
    }

    glSamplerParameteri(m_glo, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  }

  void sampler::set_filter(const sampler::filter& value)
  {
    MGL_CORE_ASSERT(m_glo, "Sampler already released");
    m_filter = value;
    glSamplerParameteri(m_glo, GL_TEXTURE_MIN_FILTER, m_filter.min_filter);
    glSamplerParameteri(m_glo, GL_TEXTURE_MAG_FILTER, m_filter.mag_filter);
  }

  void sampler::set_compare_func(mgl::opengl::compare_func value)
  {
    MGL_CORE_ASSERT(m_glo, "Sampler already released");
    m_compare_func = value;
    if(m_compare_func == mgl::opengl::compare_func::NONE)
    {
      glSamplerParameteri(m_glo, GL_TEXTURE_COMPARE_MODE, GL_NONE);
      return;
    }

    glSamplerParameteri(m_glo, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glSamplerParameteri(m_glo, GL_TEXTURE_COMPARE_FUNC, m_compare_func);
  }

  void sampler::set_anisotropy(float value)
  {
    MGL_CORE_ASSERT(m_glo, "Sampler already released");

    if(m_ctx->max_anisotropy() == 0)
    {
      return;
    }

    m_anisotropy = (float)MGL_MIN(MGL_MAX(value, 1.0), m_ctx->max_anisotropy());

    glSamplerParameterf(m_glo, GL_TEXTURE_MAX_ANISOTROPY, m_anisotropy);
  }

  void sampler::set_border_color(const glm::vec4& value)
  {
    MGL_CORE_ASSERT(m_glo, "Sampler already released");
    m_border_color = value;
    glSamplerParameteri(m_glo, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glSamplerParameteri(m_glo, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glSamplerParameteri(m_glo, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    glSamplerParameterfv(m_glo, GL_TEXTURE_BORDER_COLOR, (GLfloat*)&m_border_color);
  }

  void sampler::set_min_lod(float value)
  {
    MGL_CORE_ASSERT(m_glo, "Sampler already released");
    m_min_lod = value;
    glSamplerParameterf(m_glo, GL_TEXTURE_MIN_LOD, m_min_lod);
  }

  void sampler::set_max_lod(float value)
  {
    MGL_CORE_ASSERT(m_glo, "Sampler already released");
    m_max_lod = value;
    glSamplerParameterf(m_glo, GL_TEXTURE_MAX_LOD, m_max_lod);
  }

} // namespace  mgl::opengl
