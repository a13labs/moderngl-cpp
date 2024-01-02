#include "mgl_engine/commands/texture.hpp"
#include "mgl_engine/application.hpp"

#include "mgl_core/debug.hpp"

namespace mgl::engine
{
  void enable_texture::execute()
  {
    auto renderer = mgl::engine::current_renderer();
    MGL_CORE_ASSERT(renderer != nullptr, "Renderer is null");
    MGL_CORE_ASSERT(renderer->context() != nullptr, "Context is null");
    renderer->context()->bind_texture(m_texture, m_slot);
  }

  void disable_texture::execute()
  {
    auto renderer = mgl::engine::current_renderer();
    MGL_CORE_ASSERT(renderer != nullptr, "Renderer is null");
    MGL_CORE_ASSERT(renderer->context() != nullptr, "Context is null");
    renderer->context()->unbind_texture(m_slot);
  }
} // namespace mgl::engine