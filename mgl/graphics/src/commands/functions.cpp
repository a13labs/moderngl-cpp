#include "mgl_graphics/commands/functions.hpp"
#include "mgl_core/debug.hpp"
#include <glm/gtx/string_cast.hpp>

namespace mgl::graphics
{
  void clear_command::execute()
  {
    mgl::window::api::clear(m_color);
  }

  void viewport_command::execute()
  {
    mgl::window::api::set_viewport(m_position, m_size);
  }

  void set_view_command::execute()
  {
    mgl::window::api::set_view_matrix(m_view);
  }

  void set_projection_command::execute()
  {
    mgl::window::api::set_projection_matrix(m_projection);
  }

  void set_blend_func_command::execute()
  {
    mgl::window::api::set_blend_func(m_srcRGB, m_dstRGB, m_srcAlpha, m_dstAlpha);
  }

  void set_blend_equation_command::execute()
  {
    mgl::window::api::set_blend_equation(m_modeRGB, m_modeAlpha);
  }

  void clear_samplers_command::execute()
  {
    mgl::window::api::clear_samplers(m_start, m_end);
  }

} // namespace mgl::graphics