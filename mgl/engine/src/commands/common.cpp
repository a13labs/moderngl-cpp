#include "mgl_engine/commands/common.hpp"

#include "mgl_core/debug.hpp"

#include <glm/gtx/string_cast.hpp>

namespace mgl::engine
{
  void clear_command::execute()
  {
    MGL_CORE_ASSERT(renderer() != nullptr, "Renderer is null");
    MGL_CORE_ASSERT(renderer()->context() != nullptr, "Context is null");
    renderer()->context()->clear(m_color);
  }

  void set_view_command::execute()
  {
    MGL_CORE_ASSERT(renderer() != nullptr, "Renderer is null");
    renderer()->current_state().view_matrix = m_view;
    if(renderer()->current_state().view_uniform != nullptr)
    {
      renderer()->current_state().view_uniform->set_value(m_view);
    }
  }

  void set_projection_command::execute()
  {
    MGL_CORE_ASSERT(renderer() != nullptr, "Renderer is null");
    renderer()->current_state().projection_matrix = m_projection;
    if(renderer()->current_state().projection_uniform != nullptr)
    {
      renderer()->current_state().projection_uniform->set_value(m_projection);
    }
  }
} // namespace mgl::engine