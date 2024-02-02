#include "mgl_graphics/atlas/font.hpp"

#include "glm/glm.hpp"
namespace mgl::graphics
{

  font_atlas::font_atlas(const mgl::registry::font_ref& font, mgl::list<int32_t> pixel_heights)
      : m_font(font)
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

    m_bitmap = mgl::create_ref<mgl::registry::image>(atlas_width, atlas_height, 1);

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

  glm::vec4* font_atlas::text_to_vertices(const glm::vec2& pos,
                                          const std::string& text,
                                          float sx,
                                          float sy,
                                          int32_t pixel_height,
                                          int32_t& vertices) const
  {
    glm::vec4* coords = new glm::vec4[6 * text.size()];

    auto it = m_sizes_info.find(pixel_height);
    MGL_CORE_ASSERT(it != m_sizes_info.end(), "Font size not found");
    auto& glyphs = it->second.glyphs;

    float scale = m_font->get_scale_for_pixel_height(pixel_height);
    int32_t base_line = pixel_height - (m_font->get_ascent() * scale);

    float x = pos.x;
    float y = pos.y + base_line;

    int32_t i = 0;
    vertices = 0;
    for(auto& c : text)
    {
      if(c == '\n')
      {
        x = 0;
        y += pixel_height * sy;
        continue;
      }

      auto g = glyphs[c];

      if(!g.width || !g.height)
      {
        x += g.x_advance;
        continue;
      }

      float x0 = x + g.x_offset * sx;
      float y0 = y - (g.height + g.y_offset) * sy;
      float x1 = x0 + g.width * sx;
      float y1 = y0 + g.height * sy;

      coords[i++] = { x0, y0, g.u0, g.v1 };
      coords[i++] = { x0, y1, g.u0, g.v0 };
      coords[i++] = { x1, y1, g.u1, g.v0 };
      coords[i++] = { x1, y1, g.u1, g.v0 };
      coords[i++] = { x1, y0, g.u1, g.v1 };
      coords[i++] = { x0, y0, g.u0, g.v1 };

      x += g.x_advance * sx;
      vertices += 6;
    }

    return coords;
  }

  void font_atlas::text_to_vertices(
      const glm::vec2& pos, const std::string& text, float32_buffer& out, float sx, float sy) const
  {
    int32_t vertices;
    glm::vec4* coords = text_to_vertices(pos, text, sx, sy, 32, vertices);

    std::copy(reinterpret_cast<const uint8_t*>(coords),
              reinterpret_cast<const uint8_t*>(coords) + sizeof(glm::vec4) * vertices,
              out.data());

    delete[] coords;
  }

  void font_atlas::text_to_vertices(const glm::vec2& pos,
                                    const std::string& text,
                                    vertex_buffer_ref& buffer,
                                    int32_t& vertices,
                                    float sx,
                                    float sy) const
  {
    glm::vec4* coords = text_to_vertices(pos, text, sx, sy, 32, vertices);
    buffer->write(reinterpret_cast<const uint8_t*>(coords), sizeof(glm::vec4) * 6 * text.size());
    delete[] coords;
  }

} // namespace mgl::graphics
