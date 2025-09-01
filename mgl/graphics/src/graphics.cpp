#include "mgl_graphics/graphics.hpp"
#include "mgl_graphics/fonts/default.hpp"
#include "mgl_graphics/pipelines/text.hpp"

#include "mgl_platform/gpu.hpp"

#include "mgl_registry/resources/fonts/truetype.hpp"

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
    MGL_CORE_INFO("Creating vertex buffer and pipeline for text rendering.");
    register_buffer("text_vb",
                    mgl::platform::gpu::create_vertex_buffer(
                        TEXT_BUFFER_SIZE, "2f 2f", { "i_position", "i_uv" }, true));
    register_pipeline("text_pipeline", mgl::create_ref<builtins::text_pipeline>());
  }

  void shutdown() { }

} // namespace mgl::graphics