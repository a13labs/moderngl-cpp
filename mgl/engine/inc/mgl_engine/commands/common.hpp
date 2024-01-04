#pragma once

#include "mgl_engine/render.hpp"

namespace mgl::engine
{

  class clear_command : public render_command
  {
public:
    clear_command(const glm::vec4& color)
        : render_command(type::CLEAR)
        , m_color(color)
    { }

    void execute() override final;

private:
    glm::vec4 m_color;
  };

  class set_view_command : public render_command
  {
public:
    set_view_command(const glm::mat4& view)
        : render_command(type::SET_VIEW)
        , m_view(view)
    { }

    void execute() override final;

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

    void execute() override final;

private:
    glm::mat4 m_projection;
  };

} // namespace mgl::engine