#include "mgl_graphics/atlas/font.hpp"

#include "glm/glm.hpp"
namespace mgl::graphics
{
  font_atlas::font_atlas(const mgl::registry::font_ref& font, int32_t pixel_height, int32_t padding)
      : m_font(font)
      , m_pixel_height(pixel_height)
  {
    MGL_CORE_ASSERT(font, "Font is null");

    int32_t atlas_width = m_pixel_height * 255;
    int32_t atlas_height = m_pixel_height + padding * 2;

    m_bitmap = mgl::create_ref<mgl::registry::image>(atlas_width, atlas_height, 1);

    int32_t y = 0;
    auto glyphs = font->draw_sdf_glyph_range(0, y, 0, 255, m_pixel_height, *m_bitmap, padding);

    m_glyphs.reserve(glyphs.size());

    for(auto& glyph : glyphs)
    {
      glyph.y += y;
      glyph.u0 = static_cast<float>(glyph.x) / atlas_width;
      glyph.v0 = static_cast<float>(glyph.y) / atlas_height;
      glyph.u1 = static_cast<float>(glyph.x + glyph.width) / atlas_width;
      glyph.v1 = static_cast<float>(glyph.y + glyph.height) / atlas_height;
      m_glyphs.push_back(glyph);
    }
  }

  glm::vec4* font_atlas::text_to_vertices(
      const glm::vec2& pos, const std::string& text, float sx, float sy, int32_t& vertices) const
  {
    glm::vec4* coords = new glm::vec4[6 * text.size()];

    float scale = m_font->get_scale_for_pixel_height(m_pixel_height);
    int32_t base_line = m_pixel_height - (m_font->get_ascent() * scale);

    float x = pos.x;
    float y = pos.y + base_line * sy;

    int32_t i = 0;
    vertices = 0;
    for(auto& c : text)
    {
      if(c == '\n')
      {
        x = 0;
        y += base_line * sy;
        continue;
      }

      auto g = m_glyphs[c];

      if(!g.width || !g.height)
      {
        x += g.x_advance * sx;
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
    glm::vec4* coords = text_to_vertices(pos, text, sx, sy, vertices);

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
    glm::vec4* coords = text_to_vertices(pos, text, sx, sy, vertices);
    buffer->write(reinterpret_cast<const uint8_t*>(coords), sizeof(glm::vec4) * 6 * text.size());
    delete[] coords;
  }

} // namespace mgl::graphics
