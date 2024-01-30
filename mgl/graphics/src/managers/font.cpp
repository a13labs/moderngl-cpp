#include "mgl_graphics/managers/font.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_window/api/opengl.hpp"

namespace mgl::graphics
{
  static const int16_t s_max_texture_size = 8192;
  static const int16_t s_pixel_height = 32;
  static const uint16_t s_glyphs_per_row = 256;

  static mgl::window::api::texture_2d_ref s_texture_atlas = nullptr;
  static uint32_t s_last_glyph_position = 0;

  font_manager::font_manager() { }

  font_manager::~font_manager()
  {
    s_texture_atlas = nullptr;
  }

  void font_manager::on_add(const mgl::registry::font_ref& font)
  {
    MGL_CORE_ASSERT(font != nullptr, "Font is nullptr");
    if(s_texture_atlas == nullptr)
    {
      MGL_CORE_INFO("Crating texture atlas with size: {0}x{0}", s_max_texture_size);
      s_texture_atlas =
          mgl::window::api::create_texture_2d(s_max_texture_size, s_max_texture_size, 4);
    }
  }

  void font_manager::on_remove(const mgl::registry::font_ref& font)
  {
    MGL_CORE_ASSERT(font != nullptr, "Font is nullptr");
  }

  font_manager& font_manager::instance()
  {
    static font_manager instance;
    return instance;
  }
} // namespace mgl::graphics