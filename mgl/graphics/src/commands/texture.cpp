#include "mgl_graphics/commands/texture.hpp"
#include "mgl_graphics/application.hpp"

#include "mgl_core/debug.hpp"

namespace mgl::graphics
{
  void enable_texture::execute()
  {
    auto& render = mgl::graphics::current_render();
    MGL_CORE_ASSERT(render != nullptr, "Renderer is null");
    MGL_CORE_ASSERT(render->context() != nullptr, "Context is null");
    render->context()->bind_texture(m_texture, m_slot);
  }

  void disable_texture::execute()
  {
    auto& render = mgl::graphics::current_render();
    MGL_CORE_ASSERT(render != nullptr, "Renderer is null");
    MGL_CORE_ASSERT(render->context() != nullptr, "Context is null");
    render->context()->unbind_texture(m_slot);
  }
} // namespace mgl::graphics