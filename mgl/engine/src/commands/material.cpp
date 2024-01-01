#include "mgl_engine/commands/material.hpp"

#include "mgl_core/debug.hpp"

namespace mgl::engine
{
  void enable_material::execute()
  {
    MGL_CORE_ASSERT(renderer() != nullptr, "Renderer is null");
    MGL_CORE_ASSERT(renderer()->context() != nullptr, "Context is null");

    // Set the state of the renderer, including the current program, view and projection matrices
    renderer()->current_state().current_program = m_material->program();
    renderer()->current_state().view_uniform = m_material->program()->uniform("view");
    renderer()->current_state().projection_uniform = m_material->program()->uniform("projection");
    renderer()->context()->bind_program(m_material->program());

    // Set the view and projection matrices if the uniforms exist
    if(renderer()->current_state().view_uniform != nullptr)
    {
      renderer()->current_state().view_uniform->set_value(renderer()->current_state().view_matrix);
    }

    if(renderer()->current_state().projection_uniform != nullptr)
    {
      renderer()->current_state().projection_uniform->set_value(
          renderer()->current_state().projection_matrix);
    }

    // Prepare the material, which will set the material specific uniforms and textures
    m_material->prepare();
  }

  void disable_material::execute()
  {
    MGL_CORE_ASSERT(renderer() != nullptr, "Renderer is null");
    // Unbind the program, and set the current program to null and the view and projection uniforms to
    // null
    renderer()->context()->unbind_program();
    renderer()->current_state().current_program = nullptr;
    renderer()->current_state().view_uniform = nullptr;
    renderer()->current_state().projection_uniform = nullptr;
  }
} // namespace mgl::engine