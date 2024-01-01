#include "mgl_engine/layers/render.hpp"
#include "mgl_engine/application.hpp"

#include "mgl_core/debug.hpp"

namespace mgl::engine::layers
{
  render_layer::render_layer(const std::string& name)
      : layer(name)
  { }

  void render_layer::on_attach() { }

  void render_layer::on_detach() { }

  void render_layer::on_draw(float time, float frame_time)
  {
    MGL_CORE_ASSERT(mgl::engine::current_renderer() != nullptr, "Renderer is null");
    auto renderer = mgl::engine::current_renderer();
    renderer->begin();
    render_prepare();
    renderer->end();
  }

  void render_layer::on_event(mgl::window::event& event) { }
} // namespace mgl::engine::layers
