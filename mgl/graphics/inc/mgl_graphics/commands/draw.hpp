#pragma once

#include "mgl_graphics/batch.hpp"
#include "mgl_graphics/command.hpp"

#include "mgl_platform/api/buffers.hpp"

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

    void execute() override;

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

    void execute() override;

private:
    batch_ref m_batch;
  };

} // namespace mgl::graphics