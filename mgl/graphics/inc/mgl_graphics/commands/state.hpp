#pragma once

#include "mgl_graphics/command.hpp"

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
    set_font_command(const std::string& font)
        : m_font(font)
    { }

    void execute() override;

private:
    std::string m_font;
  };

} // namespace mgl::graphics