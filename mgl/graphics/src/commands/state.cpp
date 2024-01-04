#include "mgl_graphics/commands/state.hpp"
#include "mgl_graphics/application.hpp"

#include "mgl_core/debug.hpp"
namespace mgl::graphics
{

  void enable_state::execute()
  {
    auto& render = mgl::graphics::current_render();
    MGL_CORE_ASSERT(render != nullptr, "Renderer is null");
    MGL_CORE_ASSERT(render->context() != nullptr, "Context is null");

    auto ctx = render->context();

    switch(m_state)
    {
      case render::state::DEPTH_TEST: break;
      case render::state::SCISSOR_TEST: break;
      case render::state::STENCIL_TEST: break;
      case render::state::ALPHA_TEST: break;
      case render::state::BLEND: break;
      case render::state::CULL_FACE: break;
      case render::state::POLYGON_OFFSET_FILL: break;
      default: break;
    }
  }

  void disable_state::execute()
  {
    auto& render = mgl::graphics::current_render();
    MGL_CORE_ASSERT(render != nullptr, "Renderer is null");
    MGL_CORE_ASSERT(render->context() != nullptr, "Context is null");

    auto ctx = render->context();

    switch(m_state)
    {
      case render::state::DEPTH_TEST: break;
      case render::state::SCISSOR_TEST: break;
      case render::state::STENCIL_TEST: break;
      case render::state::ALPHA_TEST: break;
      case render::state::BLEND: break;
      case render::state::CULL_FACE: break;
      case render::state::POLYGON_OFFSET_FILL: break;
      default: break;
    }
  }

} // namespace mgl::graphics
