#include "mgl_graphics/commands/draw.hpp"
#include "glm/glm.hpp"
#include "mgl_core/debug.hpp"

namespace mgl::graphics
{
  void draw_command::execute()
  {
    if(m_index_buffer)
    {
      mgl::window::api::draw(m_vertex_buffer->api(),
                             m_vertex_buffer->layout(),
                             m_index_buffer->api(),
                             m_index_buffer->element_size(),
                             m_mode,
                             m_model_view,
                             m_count,
                             m_offset,
                             m_instance_count);
    }
    else
    {
      mgl::window::api::draw(m_vertex_buffer->api(),
                             m_vertex_buffer->layout(),
                             nullptr,
                             4,
                             m_mode,
                             m_model_view,
                             m_count,
                             m_offset,
                             m_instance_count);
    }
  }

  void draw_batch_command::execute()
  {
    if(m_batch->index_buffer())
    {
      mgl::window::api::draw_batch(m_batch->vertex_buffer()->api(),
                                   m_batch->vertex_buffer()->layout(),
                                   m_batch->index_buffer()->api(),
                                   m_batch->index_buffer()->element_size(),
                                   m_batch->mode(),
                                   m_batch->get());
    }
    else
    {
      mgl::window::api::draw_batch(m_batch->vertex_buffer()->api(),
                                   m_batch->vertex_buffer()->layout(),
                                   nullptr,
                                   4,
                                   m_batch->mode(),
                                   m_batch->get());
    }
  }

} // namespace mgl::graphics