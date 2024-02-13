#pragma once

#include "mgl_graphics/command.hpp"
#include "mgl_graphics/textures.hpp"

#include "mgl_platform/api/render_api.hpp"

namespace mgl::graphics
{
  class enable_texture : public render_command
  {
public:
    enable_texture(uint32_t slot, const texture_ref& texture)
        : m_slot(slot)
        , m_texture(texture)
    { }

    void execute() override final
    {
      mgl::platform::api::render_api::bind_texture(m_slot, m_texture->api());
    }

private:
    texture_ref m_texture;
    uint32_t m_slot;
  };

} // namespace mgl::graphics