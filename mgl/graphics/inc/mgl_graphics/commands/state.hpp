#pragma once

#include "mgl_graphics/render.hpp"

namespace mgl::graphics
{
  class enable_state : public render_command
  {
public:
    enable_state(int flags)
        : m_flags(flags)
    { }

    void execute() override;

private:
    int m_flags;
  };

  class disable_state : public render_command
  {
public:
    disable_state(int flags)
        : m_flags(flags)
    { }

    void execute() override;

private:
    int m_flags;
  };

  class set_font_command : public render_command
  {
public:
    set_font_command(const std::string& font, int32_t size)
        : m_font(font)
        , m_size(size)
    { }

    void execute() override;

private:
    std::string m_font;
    int32_t m_size;
  };

} // namespace mgl::graphics