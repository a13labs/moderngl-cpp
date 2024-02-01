#include "mgl_graphics/render.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_graphics/commands/draw.hpp"
#include "mgl_graphics/commands/functions.hpp"
#include "mgl_graphics/commands/shader.hpp"
#include "mgl_graphics/commands/state.hpp"
#include "mgl_graphics/commands/texture.hpp"
#include "mgl_registry/resources/fonts/truetype.hpp"

#include "mgl_graphics/fonts/default.hpp"

namespace mgl::graphics
{
  render_registry::render_registry()
  {
    auto font = mgl::create_ref<mgl::registry::truetype_font>(default_font, default_font_len);
    m_font_manager.add_item("default", font);
  }

  void render_registry::release()
  {
    m_buffer_manager.clear();
    m_shader_manager.clear();
    m_texture_manager.clear();
    m_font_manager.clear();
  }
} // namespace mgl::graphics