#pragma once

#include "mgl_graphics/batch.hpp"
#include "mgl_graphics/buffer.hpp"
#include "mgl_graphics/command.hpp"

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
                 size_t offset = 0,
                 size_t instance_count = 1)
        : m_vertex_buffer(vertex_buffer)
        , m_index_buffer(index_buffer)
        , m_mode(mode)
        , m_model_view(model_view)
        , m_count(count)
        , m_offset(offset)
        , m_instance_count(instance_count)
    { }

    void execute() override;

private:
    vertex_buffer_ref m_vertex_buffer;
    index_buffer_ref m_index_buffer;
    render_mode m_mode;
    glm::mat4 m_model_view;
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

  class draw_text_command : public render_command
  {
public:
    draw_text_command(const std::string& text,
                      const glm::vec2& position,
                      const glm::vec4& color,
                      float scale = 1.0f)
        : m_text(text)
        , m_position(position)
        , m_color(color)
        , m_scale(scale)
    { }

    void execute() override;

private:
    std::string m_text;
    glm::vec2 m_position;
    glm::vec4 m_color;
    float m_scale;
  };

} // namespace mgl::graphics