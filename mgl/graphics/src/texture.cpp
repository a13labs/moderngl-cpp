#include "mgl_graphics/texture.hpp"
#include "mgl_core/debug.hpp"

namespace mgl::graphics
{

  void texture::bind(int index)
  {
    MGL_CORE_ASSERT(m_texture != nullptr, "Texture is not loaded")
    m_texture->use(index);
  }

  void texture::unload()
  {
    MGL_CORE_ASSERT(m_texture != nullptr, "Texture is not loaded")
    m_texture->release();
  }
} // namespace mgl::graphics