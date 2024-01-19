#include "mgl_graphics/commands/state.hpp"
#include "mgl_core/debug.hpp"

namespace mgl::graphics
{

  void enable_state::execute()
  {
    auto& render = mgl::graphics::current_render();
    render.enable_state(m_flags);
  }

  void disable_state::execute()
  {
    auto& render = mgl::graphics::current_render();
    render.disable_state(m_flags);
  }

} // namespace mgl::graphics
