#include "mgl_graphics/atlas/font.hpp"

namespace mgl::graphics
{
  font_atlas::font_atlas(const mgl::registry::font_ref& font, mgl::list<int32_t> pixel_heights)
  {
    MGL_CORE_ASSERT(font, "Font is null");

    int32_t max_pixel_height = pixel_heights.back();
    MGL_CORE_ASSERT(max_pixel_height <= 128, "Font size is too big");

    mgl::size area = font->get_size(0, 255, max_pixel_height);
    int32_t atlas_width = area.width;
    int32_t atlas_height = 0;
    for(auto& size : pixel_heights)
    {
      atlas_height += size;
    }

    m_bitmap = mgl::create_ref<mgl::registry::image>(atlas_width, atlas_height, 4);

    int32_t y = 0;
    for(auto& size : pixel_heights)
    {
      auto glyphs = font->draw_glyph_range(0, y, 0, 255, size, *m_bitmap);

      font_atlas::size_info info;
      info.top = y;
      info.glyphs.reserve(glyphs.size());

      for(auto& glyph : glyphs)
      {
        glyph.y += y;
        glyph.u0 = static_cast<float>(glyph.x) / atlas_width;
        glyph.v0 = static_cast<float>(glyph.y) / atlas_height;
        glyph.u1 = static_cast<float>(glyph.x + glyph.width) / atlas_width;
        glyph.v1 = static_cast<float>(glyph.y + glyph.height) / atlas_height;
        info.glyphs.push_back(glyph);
      }

      m_sizes_info[size] = info;
      y += size;
    }
  }

  int32_t font_atlas::glypys_top(int32_t pixel_height) const
  {
    auto it = m_sizes_info.find(pixel_height);
    MGL_CORE_ASSERT(it != m_sizes_info.end(), "Font size not found");
    return it->second.top;
  }

  const mgl::list<mgl::registry::font::glyph>& font_atlas::glyphs(int32_t pixel_height) const
  {
    auto it = m_sizes_info.find(pixel_height);
    MGL_CORE_ASSERT(it != m_sizes_info.end(), "Font size not found");
    return it->second.glyphs;
  }

} // namespace mgl::graphics
