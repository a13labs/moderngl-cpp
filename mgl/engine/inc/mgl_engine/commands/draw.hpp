#pragma once

#include "mgl_core/containers.hpp"
#include "mgl_engine/buffers.hpp"
#include "mgl_engine/renderer.hpp"

namespace mgl::engine
{
  class draw_command : public render_command
  {
public:
    draw_command(const vertex_buffer_ref& vertex_buffer,
                 const index_buffer_ref& index_buffer,
                 renderer::draw_mode mode)
        : render_command(type::DRAW)
        , m_vertex_buffer(vertex_buffer)
        , m_index_buffer(index_buffer)
        , m_mode(mode)
    { }

    void execute() override;

private:
    vertex_buffer_ref m_vertex_buffer;
    index_buffer_ref m_index_buffer;
    renderer::draw_mode m_mode;
  };
} // namespace mgl::engine