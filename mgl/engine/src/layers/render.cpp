#include "mgl_engine/layers/render.hpp"

namespace mgl::engine::layers
{
  render_layer::render_layer(const std::string& name)
      : layer(name)
  { }

  void render_layer::on_attach() { }

  void render_layer::on_detach() { }

  void render_layer::on_draw(float time, float frame_time) { }

  void render_layer::on_event(mgl::window::event& event) { }
} // namespace mgl::engine::layers
