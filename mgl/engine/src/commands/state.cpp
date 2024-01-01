#include "mgl_engine/commands/state.hpp"

#include "mgl_core/debug.hpp"
namespace mgl::engine
{

  void enable_state::execute()
  {
    MGL_CORE_ASSERT(renderer() != nullptr, "Renderer is null");
    MGL_CORE_ASSERT(renderer()->context() != nullptr, "Context is null");

    auto ctx = renderer()->context();

    switch(m_state)
    {
      case renderer::state::DEPTH_TEST: break;
      case renderer::state::SCISSOR_TEST: break;
      case renderer::state::STENCIL_TEST: break;
      case renderer::state::ALPHA_TEST: break;
      case renderer::state::BLEND: break;
      case renderer::state::CULL_FACE: break;
      case renderer::state::POLYGON_OFFSET_FILL: break;
      default: break;
    }
  }

  void disable_state::execute()
  {
    MGL_CORE_ASSERT(renderer() != nullptr, "Renderer is null");
    MGL_CORE_ASSERT(renderer()->context() != nullptr, "Context is null");

    auto ctx = renderer()->context();

    switch(m_state)
    {
      case renderer::state::DEPTH_TEST: break;
      case renderer::state::SCISSOR_TEST: break;
      case renderer::state::STENCIL_TEST: break;
      case renderer::state::ALPHA_TEST: break;
      case renderer::state::BLEND: break;
      case renderer::state::CULL_FACE: break;
      case renderer::state::POLYGON_OFFSET_FILL: break;
      default: break;
    }
  }

} // namespace mgl::engine
