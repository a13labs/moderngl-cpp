#include "mgl_engine/commands/state.hpp"

namespace mgl::engine
{

  void enable_state::execute()
  {
    if(renderer() == nullptr)
      return;

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
    if(renderer() == nullptr)
      return;

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
