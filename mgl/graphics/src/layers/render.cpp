#include "mgl_graphics/layers/render.hpp"
#include "mgl_core/debug.hpp"
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
    render_script script;
    render_prepare(script);
    script.execute();
  }

  void render_layer::on_event(mgl::window::event& event) { }
} // namespace mgl::graphics::layers
