#pragma once

#include "mgl_graphics/command.hpp"

#include "mgl_platform/api/render_api.hpp"

namespace mgl::graphics
{
  class enable_state : public render_command
  {
public:
    enable_state(int flags)
        : m_flags(flags)
    { }

    void execute() override final { mgl::platform::api::render_api::enable_state(m_flags); }

private:
    int m_flags;
  };

  class disable_state : public render_command
  {
public:
    disable_state(int flags)
        : m_flags(flags)
    { }

    void execute() override final { mgl::platform::api::render_api::disable_state(m_flags); }

private:
    int m_flags;
  };

} // namespace mgl::graphics