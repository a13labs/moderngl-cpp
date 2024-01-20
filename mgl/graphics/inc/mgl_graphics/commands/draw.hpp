#pragma once

#include "mgl_core/containers.hpp"
#include "mgl_graphics/batch_render.hpp"
#include "mgl_graphics/buffer.hpp"
#include "mgl_graphics/render.hpp"

namespace mgl::graphics
{
  class draw_command : public render_command
  {
public:
    draw_command(const vertex_buffer_ref& vertex_buffer,
                 const index_buffer_ref& index_buffer = nullptr,
                 render_mode mode = render_mode::TRIANGLES,
                 glm::mat4 model_view = glm::mat4(1.0f),
                 size_t count = 0,
                 size_t offset = 0)
        : m_vertex_buffer(vertex_buffer)
        , m_index_buffer(index_buffer)
        , m_mode(mode)
        , m_model_view(model_view)
        , m_count(count)
        , m_offset(offset)
    { }

    void execute() override;

private:
    vertex_buffer_ref m_vertex_buffer;
    index_buffer_ref m_index_buffer;
    render_mode m_mode;
    glm::mat4 m_model_view;
    size_t m_count;
    size_t m_offset;
  };

  class draw_batch_command : public render_command
  {
public:
    draw_batch_command(const batch_render_ref& batch)
        : m_batch(batch)
    { }

    void execute() override;

private:
    batch_render_ref m_batch;
  };

} // namespace mgl::graphics