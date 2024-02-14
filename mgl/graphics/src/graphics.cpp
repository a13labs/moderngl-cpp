#include "mgl_graphics/graphics.hpp"
#include "mgl_graphics/commands/draw.hpp"
#include "mgl_graphics/commands/functions.hpp"
#include "mgl_graphics/commands/shader.hpp"
#include "mgl_graphics/commands/state.hpp"
#include "mgl_graphics/commands/texture.hpp"
#include "mgl_graphics/fonts/default.hpp"
#include "mgl_graphics/shaders/text.hpp"

#include "mgl_platform/api/render_api.hpp"

#include "mgl_registry/resources/fonts/truetype.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/profiling.hpp"

#define TEXT_BUFFER_SIZE 4096 * sizeof(float) * 6 * 4

namespace mgl::graphics
{
  void init()
  {
    MGL_PROFILE_FUNCTION("GRAPHICS INIT");
    MGL_CORE_INFO("Loading default font.");
    auto font = mgl::create_ref<mgl::registry::truetype_font>(default_font, default_font_len);
    register_font("default", font);
    MGL_CORE_INFO("Creating vertex buffer and shader for text rendering.");
    register_buffer("text_vb",
                    mgl::platform::api::render_api::create_vertex_buffer(
                        TEXT_BUFFER_SIZE, "2f 2f", { "i_position", "i_uv" }, true));
    register_shader("text_shader", mgl::create_ref<builtins::text_shader>());
  }

  void shutdown() { }

} // namespace mgl::graphics