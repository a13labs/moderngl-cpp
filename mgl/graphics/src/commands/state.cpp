#include "mgl_graphics/commands/state.hpp"
#include "mgl_core/debug.hpp"

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

} // namespace mgl::graphics
