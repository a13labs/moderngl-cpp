#include "mgl_core/debug.hpp"
#include "mgl_graphics/commands/draw.hpp"
#include "mgl_graphics/commands/functions.hpp"
#include "mgl_graphics/commands/shader.hpp"
#include "mgl_graphics/commands/state.hpp"
#include "mgl_graphics/commands/texture.hpp"
#include "mgl_graphics/graphics.hpp"
#include "mgl_registry/resources/fonts/truetype.hpp"

#include "mgl_graphics/fonts/default.hpp"

namespace mgl::graphics
{
  void init()
  {
    auto font = mgl::create_ref<mgl::registry::truetype_font>(default_font, default_font_len);
    register_font("default", font);
  }

  void shutdown() { }

} // namespace mgl::graphics