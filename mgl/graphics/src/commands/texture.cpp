#include "mgl_graphics/commands/texture.hpp"
#include "mgl_core/debug.hpp"

namespace mgl::graphics
{
  void enable_texture::execute()
  {
    auto& render = mgl::graphics::current_render();
    m_texture->use(m_slot);
  }
} // namespace mgl::graphics