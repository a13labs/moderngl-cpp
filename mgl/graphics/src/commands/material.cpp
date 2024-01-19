#include "mgl_graphics/commands/material.hpp"
#include "mgl_core/debug.hpp"

namespace mgl::graphics
{
  void enable_material::execute()
  {
    auto& render = mgl::graphics::current_render();
    render.current_state().current_shader = m_material;
    m_material->bind();
    m_material->prepare();
  }

  void disable_material::execute()
  {
    auto& render = mgl::graphics::current_render();

    if(render.current_state().current_shader == nullptr)
    {
      return;
    }

    // Disable the material, which will unbind the current program
    render.current_state().current_shader->unbind();
    render.current_state().current_shader = nullptr;
  }
} // namespace mgl::graphics