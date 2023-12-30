#pragma once

#include "mgl_window/renderer.hpp"

namespace mgl::window
{
  class enable_texture : public render_command
  {
public:
    enable_texture(mgl::window::texture texture)
        : render_command(type::ENABLE_TEXTURE)
        , m_texture(texture)
    { }

    void execute() override;

private:
    mgl::window::texture m_texture;
  };

  class disable_texture : public render_command
  {
public:
    disable_texture()
        : render_command(type::DISABLE_TEXTURE)
    { }

    void execute() override;
  };

} // namespace mgl::window