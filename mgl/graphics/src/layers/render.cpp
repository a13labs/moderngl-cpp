#include "mgl_graphics/layers/render.hpp"
#include "mgl_graphics/application.hpp"

#include "mgl_core/debug.hpp"

namespace mgl::graphics::layers
{
  render_layer::render_layer(const std::string& name)
      : layer(name)
  { }

  void render_layer::on_attach() { }

  void render_layer::on_detach() { }

  void render_layer::on_update(float time, float frame_time)
  {
    auto& render = mgl::graphics::current_render();
    render.begin();
    render_prepare();
    render.end();
  }

  void render_layer::on_event(mgl::window::event& event) { }
} // namespace mgl::graphics::layers
