#include "mgl_engine/commands/material.hpp"
#include "mgl_engine/application.hpp"

#include "mgl_core/debug.hpp"

namespace mgl::engine
{
  void enable_material::execute()
  {
    auto renderer = mgl::engine::current_renderer();
    MGL_CORE_ASSERT(renderer != nullptr, "Renderer is null");
    MGL_CORE_ASSERT(renderer->context() != nullptr, "Context is null");
    // Enable the material, which will set the current program, view and projection matrices
    renderer->current_state().current_shader = m_material;
    m_material->enable();
    // Prepare the material, which will set the material specific uniforms and textures
    m_material->prepare();
  }

  void disable_material::execute()
  {
    auto renderer = mgl::engine::current_renderer();
    MGL_CORE_ASSERT(renderer != nullptr, "Renderer is null");
    MGL_CORE_ASSERT(renderer->context() != nullptr, "Context is null");

    if(renderer->current_state().current_shader == nullptr)
    {
      return;
    }

    // Disable the material, which will unbind the current program
    renderer->current_state().current_shader->disable();
    renderer->current_state().current_shader = nullptr;
  }
} // namespace mgl::engine