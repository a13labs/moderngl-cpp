#include "mgl_graphics/layers/render.hpp"
#include "mgl_core/profiling.hpp"

namespace mgl::graphics::layers
{
  render_layer::render_layer(const std::string& name)
      : layer(name)
  { }

  void render_layer::on_attach() { }

  void render_layer::on_detach() { }

  void render_layer::on_update(float time, float frame_time)
  {
    MGL_PROFILE_FUNCTION("RENDER_LAYER");
    command_buffer script;
    script.begin();
    render_prepare(script);
    script.end();
  }

  void render_layer::on_event(mgl::platform::event& event) { }
} // namespace mgl::graphics::layers
