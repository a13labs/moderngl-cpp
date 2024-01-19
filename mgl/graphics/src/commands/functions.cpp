#include "mgl_graphics/commands/functions.hpp"
#include "mgl_core/debug.hpp"
#include <glm/gtx/string_cast.hpp>

namespace mgl::graphics
{
  void clear_command::execute()
  {
    auto& render = mgl::graphics::current_render();
    render.clear(m_color);
  }

  void viewport_command::execute()
  {
    auto& render = mgl::graphics::current_render();
    render.set_viewport(m_position, m_size);
  }

  void set_view_command::execute()
  {
    auto& render = mgl::graphics::current_render();
    render.current_state().view_matrix = m_view;
    if(render.current_state().view_uniform != nullptr)
    {
      render.current_state().view_uniform->set_value(m_view);
    }
  }

  void set_projection_command::execute()
  {
    auto& render = mgl::graphics::current_render();
    render.current_state().projection_matrix = m_projection;
    if(render.current_state().projection_uniform != nullptr)
    {
      render.current_state().projection_uniform->set_value(m_projection);
    }
  }

  void set_blend_func_command::execute()
  {
    auto& render = mgl::graphics::current_render();
    render.set_blend_func(m_srcRGB, m_dstRGB, m_srcAlpha, m_dstAlpha);
  }

  void set_blend_equation_command::execute()
  {
    auto& render = mgl::graphics::current_render();
    render.set_blend_equation(m_modeRGB, m_modeAlpha);
  }

  void clear_samplers_command::execute()
  {
    auto& render = mgl::graphics::current_render();
    render.clear_samplers(m_start, m_end);
  }

} // namespace mgl::graphics