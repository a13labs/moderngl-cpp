#include "mgl_engine/commands/draw.hpp"
#include "mgl_engine/application.hpp"

#include "mgl_core/debug.hpp"

#include "glm/glm.hpp"

namespace mgl::engine
{
  void draw_command::execute()
  {
    auto renderer = mgl::engine::current_renderer();
    MGL_CORE_ASSERT(renderer != nullptr, "Renderer is null");
    MGL_CORE_ASSERT(renderer->context() != nullptr, "Context is null");

    if(renderer->current_state().current_batch == nullptr)
    {
      renderer->current_state().current_batch = mgl::create_ref<batch_list>();
    }

    renderer->current_state().current_batch->push(m_vertex_buffer, m_index_buffer, m_mode);
  }

} // namespace mgl::engine