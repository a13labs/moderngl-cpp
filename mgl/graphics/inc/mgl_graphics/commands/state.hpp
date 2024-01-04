#pragma once

#include "mgl_graphics/render.hpp"

namespace mgl::graphics
{
  class enable_state : public render_command
  {
public:
    enable_state(mgl::graphics::render::state state)
        : render_command(type::ENABLE_STATE)
        , m_state(state)
    { }

    void execute() override;

private:
    mgl::graphics::render::state m_state;
  };

  class disable_state : public render_command
  {
public:
    disable_state(mgl::graphics::render::state state)
        : render_command(type::DISABLE_STATE)
        , m_state(state)
    { }

    void execute() override;

private:
    mgl::graphics::render::state m_state;
  };

} // namespace mgl::graphics