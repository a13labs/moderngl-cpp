#pragma once

#include "mgl_engine/renderer.hpp"

namespace mgl::engine
{
  class enable_state : public render_command
  {
public:
    enable_state(mgl::engine::renderer::state state)
        : render_command(type::ENABLE_STATE)
        , m_state(state)
    { }

    void execute() override;

private:
    mgl::engine::renderer::state m_state;
  };

  class disable_state : public render_command
  {
public:
    disable_state(mgl::engine::renderer::state state)
        : render_command(type::DISABLE_STATE)
        , m_state(state)
    { }

    void execute() override;

private:
    mgl::engine::renderer::state m_state;
  };

} // namespace mgl::engine