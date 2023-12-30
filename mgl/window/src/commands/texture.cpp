#include "mgl_window/commands/texture.hpp"

namespace mgl::window
{
  void enable_texture::execute()
  {
    if(renderer() == nullptr)
      return;

    auto ctx = renderer()->context();

    ctx->bind_texture(m_texture);
  }

  void disable_texture::execute()
  {
    if(renderer() == nullptr)
      return;

    auto ctx = renderer()->context();

    ctx->unbind_texture();
  }
} // namespace mgl::window