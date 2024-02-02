#pragma once

#include "mgl_graphics/command.hpp"
namespace mgl::graphics
{
  class clear_command : public render_command
  {
public:
    clear_command(const glm::vec4& color)
        : m_color(color)
    { }

    void execute() override final;

private:
    glm::vec4 m_color;
  };

  class viewport_command : public render_command
  {
public:
    viewport_command(const glm::vec2& position, const glm::vec2& size)
        : m_position(position)
        , m_size(size)
    { }

    void execute() override final;

private:
    glm::vec2 m_position;
    glm::vec2 m_size;
  };

  class set_view_command : public render_command
  {
public:
    set_view_command(const glm::mat4& view)
        : m_view(view)
    { }

    void execute() override final;

private:
    glm::mat4 m_view;
  };

  class set_projection_command : public render_command
  {
public:
    set_projection_command(const glm::mat4& projection)
        : m_projection(projection)
    { }

    void execute() override final;

private:
    glm::mat4 m_projection;
  };

  class set_blend_func_command : public render_command
  {
public:
    set_blend_func_command(blend_factor srcRGB,
                           blend_factor dstRGB,
                           blend_factor srcAlpha,
                           blend_factor dstAlpha)
        : m_srcRGB(srcRGB)
        , m_dstRGB(dstRGB)
        , m_srcAlpha(srcAlpha)
        , m_dstAlpha(dstAlpha)
    { }

    void execute() override final;

private:
    blend_factor m_srcRGB;
    blend_factor m_dstRGB;
    blend_factor m_srcAlpha;
    blend_factor m_dstAlpha;
  };

  class set_blend_equation_command : public render_command
  {
public:
    set_blend_equation_command(blend_equation_mode modeRGB, blend_equation_mode modeAlpha)
        : m_modeRGB(modeRGB)
        , m_modeAlpha(modeAlpha)
    { }

    void execute() override final;

private:
    blend_equation_mode m_modeRGB;
    blend_equation_mode m_modeAlpha;
  };

  class clear_samplers_command : public render_command
  {
public:
    clear_samplers_command(int start, int end)
        : m_start(start)
        , m_end(end)
    { }

    void execute() override final;

private:
    int m_start;
    int m_end;
  };

} // namespace mgl::graphics