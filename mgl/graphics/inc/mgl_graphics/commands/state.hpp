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

} // namespace mgl::graphics