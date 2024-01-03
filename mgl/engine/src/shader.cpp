#include "mgl_engine/shader.hpp"
#include "mgl_engine/application.hpp"

#include "mgl_core/debug.hpp"

namespace mgl::engine
{
  void shader::enable()
  {
    auto prg = program();
    MGL_CORE_ASSERT(prg != nullptr, "Program is null");

    auto renderer = mgl::engine::current_renderer();
    MGL_CORE_ASSERT(renderer != nullptr, "Renderer is null");

    auto& state = renderer->current_state();

    // Set the state of the renderer, including the current program, view and projection matrices
    state.view_uniform = prg->uniform("view");
    state.projection_uniform = prg->uniform("projection");
    state.model_uniform = prg->uniform("model");
    prg->bind();

    // Set the view and projection matrices if the uniforms exist
    if(state.view_uniform != nullptr)
    {
      state.view_uniform->set_value(state.view_matrix);
    }

    if(state.projection_uniform != nullptr)
    {
      state.projection_uniform->set_value(state.projection_matrix);
    }

    // If the current batch is empty, we create a new batch
    if(state.current_batch == nullptr)
    {
      state.current_batch = mgl::create_ref<batch_render>();
    }
    else
    {
      // If the current batch is not empty, we commit all the draw calls in the batch
      state.current_batch->commit();
    }
  }

  void shader::disable()
  {
    auto prg = program();
    MGL_CORE_ASSERT(prg != nullptr, "Program is null");

    auto renderer = mgl::engine::current_renderer();
    MGL_CORE_ASSERT(renderer != nullptr, "Renderer is null");

    if(renderer->current_state().current_batch != nullptr)
    {
      renderer->current_state().current_batch->commit();
    }

    // Unbind the program, and set the current program to null and the view and projection uniforms to
    // null
    prg->unbind();

    // clean-up the current shader, view and projection uniforms
    renderer->current_state().current_shader = nullptr;
    renderer->current_state().view_uniform = nullptr;
    renderer->current_state().projection_uniform = nullptr;
    renderer->current_state().model_uniform = nullptr;
  }
} // namespace mgl::engine