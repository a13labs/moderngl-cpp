#include "mgl_graphics/commands/common.hpp"
#include "mgl_graphics/application.hpp"

#include "mgl_core/debug.hpp"

#include <glm/gtx/string_cast.hpp>

namespace mgl::graphics
{
  void clear_command::execute()
  {
    auto& render = mgl::graphics::current_render();
    MGL_CORE_ASSERT(render != nullptr, "Renderer is null");
    MGL_CORE_ASSERT(render->context() != nullptr, "Context is null");
    render->context()->clear(m_color);
  }

  void set_view_command::execute()
  {
    auto& render = mgl::graphics::current_render();
    MGL_CORE_ASSERT(render != nullptr, "Renderer is null");
    render->current_state().view_matrix = m_view;
    if(render->current_state().view_uniform != nullptr)
    {
      render->current_state().view_uniform->set_value(m_view);
    }
  }

  void set_projection_command::execute()
  {
    auto& render = mgl::graphics::current_render();
    MGL_CORE_ASSERT(render != nullptr, "Renderer is null");
    render->current_state().projection_matrix = m_projection;
    if(render->current_state().projection_uniform != nullptr)
    {
      render->current_state().projection_uniform->set_value(m_projection);
    }
  }
} // namespace mgl::graphics