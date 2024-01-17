#pragma once

#include "mgl_graphics/render.hpp"

namespace mgl::graphics
{
  class enable_texture : public render_command
  {
public:
    enable_texture(uint32_t slot, const texture_ref& texture)
        : m_slot(slot)
        , m_texture(texture)
    { }

    void execute() override final;

private:
    texture_ref m_texture;
    uint32_t m_slot;
  };

} // namespace mgl::graphics