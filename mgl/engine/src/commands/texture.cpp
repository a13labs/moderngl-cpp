#include "mgl_engine/commands/texture.hpp"
#include "mgl_engine/application.hpp"

#include "mgl_core/debug.hpp"

namespace mgl::engine
{
  void enable_texture::execute()
  {
    auto& render = mgl::engine::current_render();
    MGL_CORE_ASSERT(render != nullptr, "Renderer is null");
    MGL_CORE_ASSERT(render->context() != nullptr, "Context is null");
    render->context()->bind_texture(m_texture, m_slot);
  }

  void disable_texture::execute()
  {
    auto& render = mgl::engine::current_render();
    MGL_CORE_ASSERT(render != nullptr, "Renderer is null");
    MGL_CORE_ASSERT(render->context() != nullptr, "Context is null");
    render->context()->unbind_texture(m_slot);
  }
} // namespace mgl::engine