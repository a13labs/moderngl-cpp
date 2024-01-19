#include "mgl_graphics/commands/draw.hpp"
#include "glm/glm.hpp"
#include "mgl_core/debug.hpp"

namespace mgl::graphics
{
  void draw_command::execute()
  {
    auto& render = mgl::graphics::current_render();
    render.current_state().current_batch->push(
        m_vertex_buffer, m_index_buffer, m_mode, m_model_view, m_count, m_offset);
  }

} // namespace mgl::graphics