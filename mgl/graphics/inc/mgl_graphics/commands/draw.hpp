#pragma once

#include "mgl_graphics/batch.hpp"
#include "mgl_graphics/command.hpp"

#include "mgl_platform/api/buffers.hpp"
#include "mgl_platform/api/render_api.hpp"

namespace mgl::graphics
{
  class draw_command : public render_command
  {
public:
    draw_command(const mgl::platform::api::vertex_buffer_ref& vertex_buffer,
                 const mgl::platform::api::index_buffer_ref& index_buffer = nullptr,
                 render_mode mode = render_mode::TRIANGLES,
                 size_t count = 0,
                 size_t offset = 0,
                 size_t instance_count = 0)
        : m_vertex_buffer(vertex_buffer)
        , m_index_buffer(index_buffer)
        , m_mode(mode)
        , m_count(count)
        , m_offset(offset)
        , m_instance_count(instance_count)
    { }

    void execute() override final
    {
      mgl::platform::api::render_api::render_call(
          m_vertex_buffer, m_index_buffer, m_count, m_offset, m_mode);
    }

private:
    mgl::platform::api::vertex_buffer_ref m_vertex_buffer;
    mgl::platform::api::index_buffer_ref m_index_buffer;
    render_mode m_mode;
    size_t m_count;
    size_t m_offset;
    size_t m_instance_count;
  };

  class draw_batch_command : public render_command
  {
public:
    draw_batch_command(const batch_ref& batch)
        : m_batch(batch)
    { }

    void execute() override final
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

private:
    batch_ref m_batch;
  };

} // namespace mgl::graphics