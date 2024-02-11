#include "mgl_graphics/commands/state.hpp"
#include "mgl_graphics/managers/font.hpp"

#include "mgl_core/debug.hpp"

#include "mgl_platform/api/render_api.hpp"

namespace mgl::graphics
{

  void enable_state::execute()
  {
    mgl::platform::api::render_api::enable_state(m_flags);
  }

  void disable_state::execute()
  {
    mgl::platform::api::render_api::disable_state(m_flags);
  }

} // namespace mgl::graphics
