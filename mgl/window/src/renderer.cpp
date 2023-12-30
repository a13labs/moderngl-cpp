#include "mgl_window/renderer.hpp"

namespace mgl::window
{
  void renderer::flush()
  {
    auto ctx = context();

    ctx->enter();

    ctx->clear(m_clear_color.r, m_clear_color.g, m_clear_color.b);

    for(auto& command : m_render_queue)
    {
      command->execute();
    }

    ctx->exit();
  }

} // namespace mgl::window