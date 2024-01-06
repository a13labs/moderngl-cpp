#pragma once

#include "mgl_graphics/render.hpp"

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
    set_blend_func_command(render::blend_factor srcRGB,
                           render::blend_factor dstRGB,
                           render::blend_factor srcAlpha,
                           render::blend_factor dstAlpha)
        : m_srcRGB(srcRGB)
        , m_dstRGB(dstRGB)
        , m_srcAlpha(srcAlpha)
        , m_dstAlpha(dstAlpha)
    { }

    void execute() override final;

private:
    render::blend_factor m_srcRGB;
    render::blend_factor m_dstRGB;
    render::blend_factor m_srcAlpha;
    render::blend_factor m_dstAlpha;
  };

  class set_blend_equation_command : public render_command
  {
public:
    set_blend_equation_command(render::blend_equation_mode modeRGB,
                               render::blend_equation_mode modeAlpha)
        : m_modeRGB(modeRGB)
        , m_modeAlpha(modeAlpha)
    { }

    void execute() override final;

private:
    render::blend_equation_mode m_modeRGB;
    render::blend_equation_mode m_modeAlpha;
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