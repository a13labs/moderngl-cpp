#include "mgl_engine/commands/texture.hpp"

namespace mgl::engine
{
  void enable_texture::execute()
  {
    if(renderer() == nullptr)
      return;

    auto ctx = renderer()->context();

    ctx->bind_texture(m_texture, m_slot);
  }

  void disable_texture::execute()
  {
    if(renderer() == nullptr)
      return;

    auto ctx = renderer()->context();

    ctx->unbind_texture(m_slot);
  }
} // namespace mgl::engine