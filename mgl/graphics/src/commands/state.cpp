#include "mgl_graphics/commands/state.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_graphics/managers/font.hpp"

namespace mgl::graphics
{

  void enable_state::execute()
  {
    mgl::window::api::enable_state(m_flags);
  }

  void disable_state::execute()
  {
    mgl::window::api::disable_state(m_flags);
  }

  void set_font_command::execute() { }

} // namespace mgl::graphics
