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

  image_ref
  truetype_font::draw_text(int32_t x, int32_t y, const std::string& text, int32_t pixel_height)
  {
    MGL_CORE_ASSERT(m_font, "Font not loaded");

    float scale = stbtt_ScaleForPixelHeight(m_font, pixel_height);

    int32_t start_x = x;
    int32_t width, height, x_offset, y_offset, advance, leftSideBearing;
    int32_t row_height = static_cast<int32_t>((m_ascent - m_descent + m_line_gap) * scale);
    int32_t base_line = y + static_cast<int32_t>(m_ascent * scale);

    auto measure = measure_text(text, pixel_height);
    image_ref bmp = mgl::create_ref<image>(measure.width, measure.height, 4);

    for(const auto& c : text)
    {
      if(c == '\n')
      {
        y += static_cast<int32_t>(pixel_height * scale);
        x = start_x;
        continue;
      }

      stbtt_GetCodepointHMetrics(m_font, c, &advance, &leftSideBearing);

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

      bmp->blit(x + x_offset, base_line + y_offset, glyph_32bit);
      x += static_cast<int32_t>(advance * scale);
    }

    return bmp;
  }

  mgl::list<font::glyph> truetype_font::get_glyphs(int32_t x,
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

    int width, height;
    int x_offset, y_offset;

    mgl::list<glyph> glyphs;
    for(uint16_t c = 0; c <= last_codepoint - first_codepoint; ++c)
    {
      width = 0;
      height = 0;
      x_offset = 0;
      y_offset = 0;

      stbtt_GetCodepointHMetrics(m_font, first_codepoint + c, &advance, &leftSideBearing);

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

  mgl::size truetype_font::measure_text(const std::string& text, int32_t pixel_height) const
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

} // namespace mgl::registry