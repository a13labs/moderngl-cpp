#pragma once

#include "mgl_engine/renderer.hpp"

namespace mgl::engine
{

  class clear_command : public render_command
  {
public:
    clear_command(const glm::vec4& color)
        : render_command(type::CLEAR)
        , m_color(color)
    { }

    void execute() override;

private:
    glm::vec4 m_color;
  };

  class set_viewport_command : public render_command
  {
public:
    set_viewport_command(const glm::vec2& position, const glm::vec2& size)
        : render_command(type::SET_VIEWPORT)
        , m_position(position)
        , m_size(size)
    { }

    void execute() override;

private:
    glm::vec2 m_position;
    glm::vec2 m_size;
  };

  class set_view_command : public render_command
  {
public:
    set_view_command(const glm::mat4& view)
        : render_command(type::SET_VIEW)
        , m_view(view)
    { }

    void execute() override;

private:
    glm::mat4 m_view;
  };

  class set_projection_command : public render_command
  {
public:
    set_projection_command(const glm::mat4& projection)
        : render_command(type::SET_PROJECTION)
        , m_projection(projection)
    { }

    void execute() override;

private:
    glm::mat4 m_projection;
  };

} // namespace mgl::engine