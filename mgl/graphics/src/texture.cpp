#include "mgl_graphics/texture.hpp"
#include "mgl_core/debug.hpp"

namespace mgl::graphics
{

  void texture::bind(int index)
  {
    MGL_CORE_ASSERT(this->native() != nullptr, "Texture is not loaded")
    this->native()->use(index);
  }

} // namespace mgl::graphics