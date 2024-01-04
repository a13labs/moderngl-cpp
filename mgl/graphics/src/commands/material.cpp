#include "mgl_graphics/commands/material.hpp"
#include "mgl_graphics/application.hpp"

#include "mgl_core/debug.hpp"

namespace mgl::graphics
{
  void enable_material::execute()
  {
    auto& render = mgl::graphics::current_render();
    MGL_CORE_ASSERT(render != nullptr, "Renderer is null");
    MGL_CORE_ASSERT(render->context() != nullptr, "Context is null");
    // Enable the material, which will set the current program, view and projection matrices
    render->current_state().current_shader = m_material;
    // Prepare the material, which will set the material specific uniforms and textures
    m_material->bind();
    m_material->prepare();
  }

  void disable_material::execute()
  {
    auto& render = mgl::graphics::current_render();
    MGL_CORE_ASSERT(render != nullptr, "Renderer is null");
    MGL_CORE_ASSERT(render->context() != nullptr, "Context is null");

    if(render->current_state().current_shader == nullptr)
    {
      return;
    }

    // Disable the material, which will unbind the current program
    render->current_state().current_shader->unbind();
    render->current_state().current_shader = nullptr;
  }
} // namespace mgl::graphics