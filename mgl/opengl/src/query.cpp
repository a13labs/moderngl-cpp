#include "mgl_opengl/query.hpp"
#include "mgl_opengl/context.hpp"

#include "mgl_core/debug.hpp"

#include "glad/gl.h"

namespace mgl::opengl
{
  query::query(const context_ref& ctx,
               bool samples,
               bool any_samples,
               bool time_elapsed,
               bool primitives_generated)
      : m_ctx(ctx)
  {

    if(!(samples || any_samples || time_elapsed || primitives_generated))
    {
      samples = true;
      any_samples = true;
      time_elapsed = true;
      primitives_generated = true;
    }

    m_glo[query::keys::SAMPLES_PASSED] = 0;
    m_glo[query::keys::ANY_SAMPLES_PASSED] = 0;
    m_glo[query::keys::TIME_ELAPSED] = 0;
    m_glo[query::keys::PRIMITIVES_GENERATED] = 0;

    if(samples)
    {
      glGenQueries(1, (GLuint*)&m_glo[query::keys::SAMPLES_PASSED]);
    }

    if(any_samples)
    {
      glGenQueries(1, (GLuint*)&m_glo[query::keys::ANY_SAMPLES_PASSED]);
    }

    if(time_elapsed)
    {
      glGenQueries(1, (GLuint*)&m_glo[query::keys::TIME_ELAPSED]);
    }

    if(primitives_generated)
    {
      glGenQueries(1, (GLuint*)&m_glo[query::keys::PRIMITIVES_GENERATED]);
    }
  }

  void query::begin()
  {
    MGL_CORE_ASSERT(m_ctx->is_current(), "Context not current");
    if(m_glo[query::keys::SAMPLES_PASSED])
    {
      glBeginQuery(GL_SAMPLES_PASSED, m_glo[query::keys::SAMPLES_PASSED]);
    }

    if(m_glo[query::keys::ANY_SAMPLES_PASSED])
    {
      glBeginQuery(GL_ANY_SAMPLES_PASSED, m_glo[query::keys::ANY_SAMPLES_PASSED]);
    }

    if(m_glo[query::keys::TIME_ELAPSED])
    {
      glBeginQuery(GL_TIME_ELAPSED, m_glo[query::keys::TIME_ELAPSED]);
    }

    if(m_glo[query::keys::PRIMITIVES_GENERATED])
    {
      glBeginQuery(GL_PRIMITIVES_GENERATED, m_glo[query::keys::PRIMITIVES_GENERATED]);
    }
  }

  void query::end()
  {
    MGL_CORE_ASSERT(m_ctx->is_current(), "Context not current");
    if(m_glo[query::keys::SAMPLES_PASSED])
    {
      glEndQuery(GL_SAMPLES_PASSED);
    }

    if(m_glo[query::keys::ANY_SAMPLES_PASSED])
    {
      glEndQuery(GL_ANY_SAMPLES_PASSED);
    }

    if(m_glo[query::keys::TIME_ELAPSED])
    {
      glEndQuery(GL_TIME_ELAPSED);
    }

    if(m_glo[query::keys::PRIMITIVES_GENERATED])
    {
      glEndQuery(GL_PRIMITIVES_GENERATED);
    }
  }

  void query::begin_render()
  {
    MGL_CORE_ASSERT(m_ctx->is_current(), "Context not current");
    if(m_glo[query::keys::ANY_SAMPLES_PASSED])
    {
      glBeginConditionalRender(m_glo[query::keys::ANY_SAMPLES_PASSED], GL_QUERY_NO_WAIT);
    }
    else if(m_glo[query::keys::SAMPLES_PASSED])
    {
      glBeginConditionalRender(m_glo[query::keys::SAMPLES_PASSED], GL_QUERY_NO_WAIT);
    }
    else
    {
      MGL_CORE_ERROR("no samples");
    }
  }

  void query::end_render()
  {
    MGL_CORE_ASSERT(m_ctx->is_current(), "Context not current");
    glEndConditionalRender();
  }

  int query::samples()
  {
    MGL_CORE_ASSERT(m_glo[query::keys::SAMPLES_PASSED], "No samples query");
    MGL_CORE_ASSERT(m_ctx->is_current(), "Context not current");
    int samples = 0;
    glGetQueryObjectiv(m_glo[query::keys::SAMPLES_PASSED], GL_QUERY_RESULT, &samples);
    return samples;
  }

  int query::primitives()
  {
    MGL_CORE_ASSERT(m_ctx->is_current(), "Context not current");
    MGL_CORE_ASSERT(m_glo[query::keys::PRIMITIVES_GENERATED], "No primitives query");
    int primitives = 0;
    glGetQueryObjectiv(m_glo[query::keys::PRIMITIVES_GENERATED], GL_QUERY_RESULT, &primitives);
    return primitives;
  }

  int query::elapsed()
  {
    MGL_CORE_ASSERT(m_ctx->is_current(), "Context not current");
    MGL_CORE_ASSERT(m_glo[query::keys::TIME_ELAPSED], "No time query");
    int elapsed = 0;
    glGetQueryObjectiv(m_glo[query::keys::TIME_ELAPSED], GL_QUERY_RESULT, &elapsed);
    return elapsed;
  }

} // namespace  mgl::opengl
