#include "mgl_graphics/commands/texture.hpp"
#include "mgl_core/debug.hpp"

namespace mgl::graphics
{
  void enable_texture::execute()
  {
    m_texture->bind(m_slot);
  }
} // namespace mgl::graphics