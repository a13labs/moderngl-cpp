#include "mgl_graphics/commands/state.hpp"
#include "mgl_graphics/application.hpp"

#include "mgl_core/debug.hpp"
namespace mgl::graphics
{

  void enable_state::execute()
  {
    auto& render = mgl::graphics::current_render();
    auto ctx = render.context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");
    ctx->enable(m_flags);
  }

  void disable_state::execute()
  {
    auto& render = mgl::graphics::current_render();
    auto ctx = render.context();
    MGL_CORE_ASSERT(ctx != nullptr, "Context is null");
    ctx->disable(m_flags);
  }

} // namespace mgl::graphics
