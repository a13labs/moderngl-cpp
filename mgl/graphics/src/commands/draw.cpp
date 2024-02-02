#include "mgl_graphics/commands/draw.hpp"
#include "glm/glm.hpp"
#include "mgl_core/debug.hpp"

namespace mgl::graphics
{
  void draw_command::execute()
  {
    if(m_index_buffer)
    {
      mgl::window::api::geom_data geom = { m_vertex_buffer->api(),
                                           m_vertex_buffer->layout(),
                                           m_index_buffer->api(),
                                           m_index_buffer->element_size(),
                                           m_mode };
      geom.allocate();
      mgl::window::api::draw(geom, m_model_view, m_count, m_offset, m_instance_count);
      geom.deallocate();
    }
    else
    {
      mgl::window::api::geom_data geom = {
        m_vertex_buffer->api(), m_vertex_buffer->layout(), nullptr, 4, m_mode
      };
      geom.allocate();
      mgl::window::api::draw(geom, m_model_view, m_count, m_offset, m_instance_count);
      geom.deallocate();
    }
  }

  void draw_batch_command::execute()
  {
    if(m_batch->index_buffer())
    {
      mgl::window::api::geom_data geom = { m_batch->vertex_buffer()->api(),
                                           m_batch->vertex_buffer()->layout(),
                                           m_batch->index_buffer()->api(),
                                           m_batch->index_buffer()->element_size(),
                                           m_batch->mode() };
      geom.allocate();
      mgl::window::api::draw_batch(geom, m_batch->get());
      geom.deallocate();
    }
    else
    {
      mgl::window::api::geom_data geom = { m_batch->vertex_buffer()->api(),
                                           m_batch->vertex_buffer()->layout(),
                                           nullptr,
                                           4,
                                           m_batch->mode() };

      geom.allocate();
      mgl::window::api::draw_batch(geom, m_batch->get());
      geom.deallocate();
    }
  }

} // namespace mgl::graphics