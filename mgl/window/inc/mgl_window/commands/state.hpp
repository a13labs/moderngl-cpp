#pragma once

#include "mgl_window/renderer.hpp"

namespace mgl::window
{

  class enable_state : public render_command
  {
public:
    enable_state(mgl::window::renderer::state state)
        : render_command(type::ENABLE_STATE)
        , m_state(state)
    { }

    void execute() override;

private:
    mgl::window::renderer::state m_state;
  };

  class disable_state : public render_command
  {
public:
    disable_state(mgl::window::renderer::state state)
        : render_command(type::DISABLE_STATE)
        , m_state(state)
    { }

    void execute() override;

private:
    mgl::window::renderer::state m_state;
  };

} // namespace mgl::window