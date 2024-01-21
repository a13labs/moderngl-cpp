#include "mgl_graphics/render.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_graphics/commands/draw.hpp"
#include "mgl_graphics/commands/functions.hpp"
#include "mgl_graphics/commands/shader.hpp"
#include "mgl_graphics/commands/state.hpp"
#include "mgl_graphics/commands/texture.hpp"
namespace mgl::graphics
{
  static mgl::scope<render> s_instance = nullptr;

  render::render(const mgl::window::api::context_ref& context)
      : m_context(context)
  {
    MGL_CORE_ASSERT(s_instance == nullptr, "Render already exists");
    s_instance = mgl::scope<render>(this);
  }

  render::~render()
  {
    MGL_CORE_ASSERT(s_instance != nullptr, "Render does not exists");
    s_instance.release();
  }

  render& render::current_render()
  {
    MGL_CORE_ASSERT(s_instance != nullptr, "Render does not exists");
    return *s_instance;
  }

  void render::release()
  {
    MGL_CORE_ASSERT(s_instance != nullptr, "Render does not exists");
    m_shader_manager.clear();
  }
} // namespace mgl::graphics