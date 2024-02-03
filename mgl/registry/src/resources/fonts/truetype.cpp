#include "mgl_registry/resources/fonts/truetype.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/math.hpp"
#include "mgl_core/memory.hpp"

#define STBTT_fabs(x) mgl::math::abs(x)
#define STBTT_ifloor(x) ((int)mgl::math::floor(x))
#define STBTT_iceil(x) ((int)mgl::math::ceil(x))

#define STB_TRUETYPE_IMPLEMENTATION 1
#include "stb/stb_truetype.h"

namespace mgl::registry
{
  truetype_font::truetype_font(const uint8_buffer& data)
      : m_data(std::move(data))
  {
    m_font = new stbtt_fontinfo;
    if(!stbtt_InitFont(m_font, m_data.data(), 0))
    {
      MGL_CORE_ERROR("Failed to load font");
      m_font = nullptr;
      return;
    }

    stbtt_GetFontVMetrics(m_font, &m_ascent, &m_descent, &m_line_gap);
  }

  truetype_font::truetype_font(const uint8_t* data, size_t size)
      : m_data(0)
  {
    m_font = new stbtt_fontinfo;
    if(!stbtt_InitFont(m_font, data, 0))
    {
      MGL_CORE_ERROR("Failed to load font");
      m_font = nullptr;
      return;
    }

    stbtt_GetFontVMetrics(m_font, &m_ascent, &m_descent, &m_line_gap);
  }

  truetype_font::~truetype_font()
  {
    if(m_font)
    {
      delete m_font;
    }
    m_data.clear();
  }

  float truetype_font::get_scale_for_pixel_height(int32_t pixel_height) const
  {
    MGL_CORE_ASSERT(m_font, "Font not loaded");
    return stbtt_ScaleForPixelHeight(m_font, pixel_height);
  }

  mgl::registry::font::glyph truetype_font::get_glyph(uint16_t codepoint,
                                                      int32_t pixel_height) const
  {
    MGL_CORE_ASSERT(m_font, "Font not loaded");

    float scale = stbtt_ScaleForPixelHeight(m_font, pixel_height);

    int x0, y0, x1, y1;
    const int glyph_index_in_font = stbtt_FindGlyphIndex(m_font, codepoint);
    stbtt_GetCodepointBitmapBox(m_font, glyph_index_in_font, scale, scale, &x0, &y0, &x1, &y1);

    int advance, leftSideBearing;
    stbtt_GetCodepointHMetrics(m_font, codepoint, &advance, &leftSideBearing);

    return { static_cast<uint16_t>(x0),
             static_cast<uint16_t>(y1),
             static_cast<uint16_t>(x1 - x0),
             static_cast<uint16_t>(y1 - y0),
             static_cast<float>(leftSideBearing),
             static_cast<float>(y0),
             static_cast<float>(advance * scale),
             0.0f,
             0.0f,
             0.0f,
             0.0f };
  }

  mgl::registry::font::glyph truetype_font::draw_glyph(
      int32_t x, int32_t y, uint16_t codepoint, int32_t pixel_height, image& bmp)
  {
    MGL_CORE_ASSERT(m_font, "Font not loaded");

    float scale = stbtt_ScaleForPixelHeight(m_font, pixel_height);

    int advance, leftSideBearing;
    stbtt_GetCodepointHMetrics(m_font, codepoint, &advance, &leftSideBearing);

    int width, height;
    int x_offset, y_offset;

    uint8_t* glyph_8bit = stbtt_GetCodepointBitmap(
        m_font, scale, scale, codepoint, &width, &height, &x_offset, &y_offset);

    stbtt_MakeCodepointBitmapSubpixel(
        m_font, glyph_8bit, width, height, width, scale, scale, 0.0f, 0.0f, codepoint);

    image glyph_32bit(width, height, 4);
    for(int32_t j = 0; j < height; ++j)
    {
      for(int32_t k = 0; k < width; ++k)
      {
        float alpha = glyph_8bit[j * width + k] / 255.0f;
        glyph_32bit.put_pixel(k, j, glm::vec4(1.0f, 1.0f, 1.0f, alpha));
      }
    }

    stbtt_FreeBitmap(glyph_8bit, nullptr);

    bmp.blit(x, y, glyph_32bit);
    glyph g;
    g.x = x;
    g.y = y;
    g.width = width;
    g.height = height;
    g.x_offset = static_cast<float>(x_offset);
    g.y_offset = static_cast<float>(y_offset);
    g.x_advance = static_cast<float>(advance * scale);
    return g;
  }

  mgl::list<font::glyph> truetype_font::draw_glyph_range(int32_t x,
                                                         int32_t y,
                                                         uint16_t first_codepoint,
                                                         uint16_t last_codepoint,
                                                         int32_t pixel_height,
                                                         image& bmp)
  {
    MGL_CORE_ASSERT(bmp.channels() == 1, "Image must be 1 channel")
    MGL_CORE_ASSERT(m_font, "Font not loaded");

    float scale = stbtt_ScaleForPixelHeight(m_font, pixel_height);

    int advance, leftSideBearing;
    stbtt_GetCodepointHMetrics(m_font, first_codepoint, &advance, &leftSideBearing);

    int width, height;
    int x_offset, y_offset;

    mgl::list<glyph> glyphs;
    for(uint16_t c = 0; c <= last_codepoint - first_codepoint; ++c)
    {
      uint8_t* glyph_8bit = stbtt_GetCodepointBitmap(
          m_font, scale, scale, first_codepoint + c, &width, &height, &x_offset, &y_offset);

      glyph g;
      g.x = x;
      g.y = y;
      g.width = width;
      g.height = height;
      g.x_offset = static_cast<float>(x_offset);
      g.y_offset = static_cast<float>(y_offset);
      g.x_advance = static_cast<float>(advance * scale);
      g.u0 = static_cast<float>(x) / bmp.width();
      g.v0 = static_cast<float>(y) / bmp.height();
      g.u1 = static_cast<float>(x + width) / bmp.width();
      g.v1 = static_cast<float>(y + height) / bmp.height();
      glyphs.push_back(g);

      if(glyph_8bit == nullptr)
      {
        continue;
      }

      stbtt_MakeCodepointBitmapSubpixel(
          m_font, glyph_8bit, width, height, width, scale, scale, 0.0f, 0.0f, first_codepoint + c);

      MGL_CORE_ASSERT(x + width <= bmp.width() && y + height <= bmp.height(),
                      "Glyph does not fit in the image");
      for(int32_t j = 0; j < height; ++j)
      {
        std::copy(glyph_8bit + j * width,
                  glyph_8bit + (j + 1) * width,
                  bmp.data() + (y + j) * bmp.width() + x);
      }

      stbtt_FreeBitmap(glyph_8bit, nullptr);

      x += static_cast<int32_t>(advance * scale);
    }

    return glyphs;
  }

  void truetype_font::draw_text(
      int32_t x, int32_t y, const std::string& text, int32_t pixel_height, image& bmp)
  {
    MGL_CORE_ASSERT(m_font, "Font not loaded");

    float scale = stbtt_ScaleForPixelHeight(m_font, pixel_height);

    int advance, leftSideBearing;
    stbtt_GetCodepointHMetrics(m_font, text[0], &advance, &leftSideBearing);

    int start_x = x;
    int width, height;
    int x_offset, y_offset;
    int32_t row_height = static_cast<int32_t>((m_ascent - m_descent + m_line_gap) * scale);
    int32_t base_line = y + static_cast<int32_t>(m_ascent * scale);

    for(const auto& c : text)
    {
      if(c == '\n')
      {
        y += static_cast<int32_t>(pixel_height * scale);
        x = start_x;
        continue;
      }

      uint8_t* glyph_8bit =
          stbtt_GetCodepointBitmap(m_font, scale, scale, c, &width, &height, &x_offset, &y_offset);

      stbtt_MakeCodepointBitmapSubpixel(
          m_font, glyph_8bit, width, height, width, scale, scale, 0.0f, 0.0f, c);

      image glyph_32bit(width, height, 4);
      for(int32_t j = 0; j < height; ++j)
      {
        for(int32_t k = 0; k < width; ++k)
        {
          float alpha = glyph_8bit[j * width + k] / 255.0f;
          glyph_32bit.put_pixel(k, j, glm::vec4(1.0f, 1.0f, 1.0f, alpha));
        }
      }

      stbtt_FreeBitmap(glyph_8bit, nullptr);

      bmp.blit(x + x_offset, base_line + y_offset, glyph_32bit);
      x += static_cast<int32_t>(advance * scale);
    }
  }

  mgl::size truetype_font::get_size(const std::string& text, int32_t pixel_height) const
  {
    MGL_CORE_ASSERT(m_font, "Font not loaded");

    float scale = stbtt_ScaleForPixelHeight(m_font, pixel_height);

    int32_t width = 0;
    int32_t row_height = static_cast<int32_t>((m_ascent - m_descent + m_line_gap) * scale);
    int32_t height = row_height;

    int32_t c_width = 0;
    for(const auto& c : text)
    {
      if(c == '\n')
      {
        height += row_height;
        c_width = 0;
        continue;
      }

      int advance;
      stbtt_GetCodepointHMetrics(m_font, c, &advance, nullptr);

      c_width += static_cast<int32_t>(advance * scale);
      width = std::max(width, c_width);
    }

    return { width, height };
  }

  mgl::size truetype_font::get_size(uint16_t first_codepoint,
                                    uint16_t last_codepoint,
                                    int32_t pixel_height) const
  {
    MGL_CORE_ASSERT(m_font, "Font not loaded");

    float scale = stbtt_ScaleForPixelHeight(m_font, pixel_height);

    int32_t width = 0;
    int32_t row_height = static_cast<int32_t>((m_ascent - m_descent + m_line_gap) * scale);

    for(uint16_t c = 0; c <= last_codepoint - first_codepoint; ++c)
    {
      int advance;
      stbtt_GetCodepointHMetrics(m_font, first_codepoint + c, &advance, nullptr);
      width += static_cast<int32_t>(advance * scale);
    }

    return { width, row_height };
  }

  mgl::size truetype_font::get_sdf_size(uint16_t first_codepoint,
                                        uint16_t last_codepoint,
                                        int32_t pixel_height,
                                        int32_t padding) const
  {
    MGL_CORE_ASSERT(m_font, "Font not loaded");

    float scale = stbtt_ScaleForPixelHeight(m_font, pixel_height);

    int32_t width = 0;
    int32_t row_height = 0;

    int32_t x0, y0, x1, y1;
    for(uint16_t c = 0; c <= last_codepoint - first_codepoint; ++c)
    {
      int32_t glyph = stbtt_FindGlyphIndex(m_font, first_codepoint + c);
      stbtt_GetGlyphBitmapBoxSubpixel(m_font, glyph, scale, scale, 0.0f, 0.0f, &x0, &y0, &x1, &y1);

      if(x0 == x1 || y0 == y1)
      {
        width += pixel_height;
        continue;
      }

      x0 -= padding;
      y0 -= padding;
      x1 += padding;
      y1 += padding;

      int32_t w = (x1 - x0);
      int32_t h = (y1 - y0);

      width += w;
      row_height = std::max(row_height, h);
    }

    return { width, row_height };
  }

  void truetype_font::draw_sdf_glyph(
      int32_t x, int32_t y, uint16_t codepoint, int32_t pixel_height, image& bmp, int32_t padding)
  {
    MGL_CORE_ASSERT(m_font, "Font not loaded");

    float scale = stbtt_ScaleForPixelHeight(m_font, pixel_height);

    int advance, leftSideBearing;
    stbtt_GetCodepointHMetrics(m_font, codepoint, &advance, &leftSideBearing);

    int width, height;
    int x_offset, y_offset;

    uint8_t* glyph_8bit = stbtt_GetCodepointSDF(m_font,
                                                scale,
                                                codepoint,
                                                padding,
                                                128,
                                                pixel_height,
                                                &width,
                                                &height,
                                                &x_offset,
                                                &y_offset);

    image glyph_32bit(width, height, 4);
    for(int32_t j = 0; j < height; ++j)
    {
      for(int32_t k = 0; k < width; ++k)
      {
        float alpha = glyph_8bit[j * width + k] / 255.0f;
        glyph_32bit.put_pixel(k, j, glm::vec4(1.0f, 1.0f, 1.0f, alpha));
      }
    }

    stbtt_FreeSDF(glyph_8bit, nullptr);

    bmp.blit(x, y, glyph_32bit);
  }

  mgl::list<font::glyph> truetype_font::draw_sdf_glyph_range(int32_t x,
                                                             int32_t y,
                                                             uint16_t first_codepoint,
                                                             uint16_t last_codepoint,
                                                             int32_t pixel_height,
                                                             image& bmp,
                                                             int32_t padding)
  {
    MGL_CORE_ASSERT(bmp.channels() == 1, "Image must be 1 channel")
    MGL_CORE_ASSERT(m_font, "Font not loaded");

    float scale = stbtt_ScaleForPixelHeight(m_font, pixel_height);

    int advance, leftSideBearing;
    stbtt_GetCodepointHMetrics(m_font, first_codepoint, &advance, &leftSideBearing);

    int width, height;
    int x_offset, y_offset;

    mgl::list<glyph> glyphs;
    for(uint16_t c = 0; c <= last_codepoint - first_codepoint; ++c)
    {
      width = 0;
      height = 0;
      x_offset = 0;
      y_offset = 0;

      uint8_t* glyph_8bit = stbtt_GetCodepointSDF(m_font,
                                                  scale,
                                                  first_codepoint + c,
                                                  padding,
                                                  128,
                                                  pixel_height,
                                                  &width,
                                                  &height,
                                                  &x_offset,
                                                  &y_offset);

      glyph g;
      g.x = x;
      g.y = y;
      g.width = width;
      g.height = height;
      g.x_offset = static_cast<float>(x_offset);
      g.y_offset = static_cast<float>(y_offset);
      g.x_advance = static_cast<float>(advance * scale);
      g.u0 = static_cast<float>(x) / bmp.width();
      g.v0 = static_cast<float>(y) / bmp.height();
      g.u1 = static_cast<float>(x + width) / bmp.width();
      g.v1 = static_cast<float>(y + height) / bmp.height();
      glyphs.push_back(g);

      if(glyph_8bit == nullptr)
      {
        continue;
      }

      MGL_CORE_ASSERT(x + width <= bmp.width() && y + height <= bmp.height(),
                      "Glyph does not fit in the image");
      for(int32_t j = 0; j < std::min(height, bmp.height()); ++j)
      {
        std::copy(glyph_8bit + j * width,
                  glyph_8bit + (j + 1) * width,
                  bmp.data() + (y + j) * bmp.width() + x);
      }

      stbtt_FreeSDF(glyph_8bit, nullptr);

      x += static_cast<int32_t>(advance * scale) + 2 * padding;
    }

    return glyphs;
  }

} // namespace mgl::registry