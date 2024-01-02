#include "mgl_engine/shader.hpp"
#include "mgl_engine/application.hpp"

#include "mgl_core/debug.hpp"

namespace mgl::engine
{
  void shader::enable()
  {
    MGL_CORE_ASSERT(program() != nullptr, "Program is null");
    auto renderer = mgl::engine::current_renderer();
    MGL_CORE_ASSERT(renderer != nullptr, "Renderer is null");
    MGL_CORE_ASSERT(renderer->context() != nullptr, "Context is null");

    // Set the state of the renderer, including the current program, view and projection matrices
    renderer->current_state().view_uniform = program()->uniform("view");
    renderer->current_state().projection_uniform = program()->uniform("projection");
    renderer->context()->bind_program(program());

    // Set the view and projection matrices if the uniforms exist
    if(renderer->current_state().view_uniform != nullptr)
    {
      renderer->current_state().view_uniform->set_value(renderer->current_state().view_matrix);
    }

    if(renderer->current_state().projection_uniform != nullptr)
    {
      renderer->current_state().projection_uniform->set_value(
          renderer->current_state().projection_matrix);
    }
  }

  void shader::disable()
  {
    auto renderer = mgl::engine::current_renderer();
    MGL_CORE_ASSERT(renderer != nullptr, "Renderer is null");
    // Unbind the program, and set the current program to null and the view and projection uniforms to
    // null
    renderer->context()->unbind_program();
    renderer->current_state().current_shader = nullptr;
    renderer->current_state().view_uniform = nullptr;
    renderer->current_state().projection_uniform = nullptr;
  }
} // namespace mgl::engine