#include "mgl_graphics/commands/draw.hpp"
#include "glm/glm.hpp"
#include "mgl_core/debug.hpp"

namespace mgl::graphics
{
  void draw_command::execute()
  {
    mgl::platform::api::render_api::render_call(
        m_vertex_buffer, m_index_buffer, m_count, m_offset, m_mode);
  }

  void draw_batch_command::execute()
  {
    MGL_CORE_ASSERT(false, "Not implemented");
    // if(m_batch->index_buffer())
    // {
    // mgl::platform::api::vertex_array_ref vao =
    //     mgl::platform::api::create_vertex_array(m_batch->vertex_buffer()->api(),
    //                                             m_batch->vertex_buffer()->layout(),
    //                                             m_batch->index_buffer()->api(),
    //                                             m_batch->index_buffer()->element_size(),
    //                                             m_batch->mode());
    // vao->allocate();
    // mgl::platform::api::draw_batch(vao, m_batch->get());
    // vao->deallocate();
    // }
    // else
    // {
    // mgl::platform::api::vertex_array_ref vao =
    //     mgl::platform::api::create_vertex_array(m_batch->vertex_buffer()->api(),
    //                                             m_batch->vertex_buffer()->layout(),
    //                                             nullptr,
    //                                             4,
    //                                             m_batch->mode());

    // vao->allocate();
    // mgl::platform::api::draw_batch(vao, m_batch->get());
    // vao->deallocate();
    // }
  }

} // namespace mgl::graphics