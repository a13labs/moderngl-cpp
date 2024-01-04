#include "mgl_graphics/commands/draw.hpp"
#include "mgl_graphics/application.hpp"

#include "mgl_core/debug.hpp"

#include "glm/glm.hpp"

namespace mgl::graphics
{
  void draw_command::execute()
  {
    auto& render = mgl::graphics::current_render();
    MGL_CORE_ASSERT(render != nullptr, "Renderer is null");
    render->current_state().current_batch->push(m_vertex_buffer, m_index_buffer, m_mode);
  }

} // namespace mgl::graphics