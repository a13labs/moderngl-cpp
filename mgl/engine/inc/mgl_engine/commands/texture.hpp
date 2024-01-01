#pragma once

#include "mgl_engine/renderer.hpp"

namespace mgl::engine
{
  class enable_texture : public render_command
  {
public:
    enable_texture(uint32_t slot, mgl::window::texture texture)
        : render_command(type::ENABLE_TEXTURE)
        , m_slot(slot)
        , m_texture(texture)
    { }

    void execute() override;

private:
    mgl::window::texture m_texture;
    uint32_t m_slot;
  };

  class disable_texture : public render_command
  {
public:
    disable_texture(uint32_t slot)
        : render_command(type::DISABLE_TEXTURE)
        , m_slot(slot)
    { }

    void execute() override;

private:
    uint32_t m_slot;
  };

} // namespace mgl::engine