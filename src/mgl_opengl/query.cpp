
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
#include "query.hpp"
#include "context.hpp"
#include "mgl_core/log.hpp"

namespace mgl::opengl
{
  void query::begin()
  {
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

    if(m_query_obj[query::keys::SAMPLES_PASSED])
    {
      gl.BeginQuery(GL_SAMPLES_PASSED, m_query_obj[query::keys::SAMPLES_PASSED]);
    }

    if(m_query_obj[query::keys::ANY_SAMPLES_PASSED])
    {
      gl.BeginQuery(GL_ANY_SAMPLES_PASSED, m_query_obj[query::keys::ANY_SAMPLES_PASSED]);
    }

    if(m_query_obj[query::keys::TIME_ELAPSED])
    {
      gl.BeginQuery(GL_TIME_ELAPSED, m_query_obj[query::keys::TIME_ELAPSED]);
    }

    if(m_query_obj[query::keys::PRIMITIVES_GENERATED])
    {
      gl.BeginQuery(GL_PRIMITIVES_GENERATED, m_query_obj[query::keys::PRIMITIVES_GENERATED]);
    }
  }

  void query::end()
  {
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

    if(m_query_obj[query::keys::SAMPLES_PASSED])
    {
      gl.EndQuery(GL_SAMPLES_PASSED);
    }

    if(m_query_obj[query::keys::ANY_SAMPLES_PASSED])
    {
      gl.EndQuery(GL_ANY_SAMPLES_PASSED);
    }

    if(m_query_obj[query::keys::TIME_ELAPSED])
    {
      gl.EndQuery(GL_TIME_ELAPSED);
    }

    if(m_query_obj[query::keys::PRIMITIVES_GENERATED])
    {
      gl.EndQuery(GL_PRIMITIVES_GENERATED);
    }
  }

  void query::begin_render()
  {
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();

    if(m_query_obj[query::keys::ANY_SAMPLES_PASSED])
    {
      gl.BeginConditionalRender(m_query_obj[query::keys::ANY_SAMPLES_PASSED], GL_QUERY_NO_WAIT);
    }
    else if(m_query_obj[query::keys::SAMPLES_PASSED])
    {
      gl.BeginConditionalRender(m_query_obj[query::keys::SAMPLES_PASSED], GL_QUERY_NO_WAIT);
    }
    else
    {
      MGL_CORE_ERROR("no samples");
    }
  }

  void query::end_render()
  {
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();
    gl.EndConditionalRender();
  }

  int query::samples()
  {
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();
    int samples = 0;
    gl.GetQueryObjectiv(m_query_obj[query::keys::SAMPLES_PASSED], GL_QUERY_RESULT, &samples);
    return samples;
  }

  int query::primitives()
  {
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();
    int primitives = 0;
    gl.GetQueryObjectiv(m_query_obj[query::keys::PRIMITIVES_GENERATED], GL_QUERY_RESULT, &primitives);
    return primitives;
  }

  int query::elapsed()
  {
    MGL_CORE_ASSERT(m_context, "No context");
    MGL_CORE_ASSERT(!m_context->released(), "Context already released");
    const GLMethods& gl = m_context->gl();
    int elapsed = 0;
    gl.GetQueryObjectiv(m_query_obj[query::keys::TIME_ELAPSED], GL_QUERY_RESULT, &elapsed);
    return elapsed;
  }

} // namespace  mgl::opengl
