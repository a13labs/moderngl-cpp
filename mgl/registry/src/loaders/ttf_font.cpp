

#include "mgl_registry/loaders/ttf_font.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_core/math.hpp"
#include "mgl_core/memory.hpp"

#define STBTT_fabs(x) mgl::math::abs(x)
#define STBTT_ifloor(x) ((int)mgl::math::floor(x))
#define STBTT_iceil(x) ((int)mgl::math::ceil(x))

#define STB_TRUETYPE_IMPLEMENTATION 1
#include "stb/stb_truetype.h"

namespace mgl::registry::loaders
{
  static font_loader_options default_font_loader_options;

  mgl::string_list font_loader::get_extensions() const
  {
    return { ".ttf", ".otf" };
  }

  resource_ref font_loader::load(const location_ref& location,
                                 const std::string& path,
                                 const loader_options& options)
  {
    if(!location->exists(path))
    {
      MGL_CORE_ERROR("Failed to load font: {0}", path);
      return nullptr;
    }

    const font_loader_options* opts = dynamic_cast<const font_loader_options*>(&options);

    if(!opts)
    {
      opts = &default_font_loader_options;
    }

    uint8_buffer buffer;
    stbtt_fontinfo font;

    // Read the font file into a buffer and check if it's a valid font file.
    location->read(path, buffer);
    if(!stbtt_InitFont(&font, buffer.data(), 0))
    {
      MGL_CORE_ERROR("Failed to load font: {0}", path);
      return nullptr;
    }

    return mgl::create_ref<ttf_font>(buffer);
  }

  font_face_ref
  ttf_font::bitmap(uint16_t start_char, uint16_t num_chars, uint16_t pixel_height) const
  {
    stbtt_fontinfo font;
    if(!stbtt_InitFont(&font, m_data.data(), 0))
    {
      MGL_CORE_ERROR("Failed to load font");
      return nullptr;
    }

    auto f = new font_face();

    f->scale = stbtt_ScaleForPixelHeight(&font, pixel_height);

    // Calculate the ascent, descent and line gap.
    stbtt_GetFontVMetrics(&font, &f->ascent, &f->descent, &f->line_gap);

    int baseline = static_cast<int>(f->ascent * f->scale);

    // Calculate the size of the texture atlas.
    uint16_t bmp_width = 0;
    uint16_t bmp_height = 0;
    for(uint16_t i = 0; i < num_chars; ++i)
    {
      int x0, y0, x1, y1;
      const int glyph_index_in_font = stbtt_FindGlyphIndex(&font, start_char + i);
      stbtt_GetCodepointBitmapBox(
          &font, glyph_index_in_font, f->scale, f->scale, &x0, &y0, &x1, &y1);
      int advance, leftSideBearing;
      stbtt_GetCodepointHMetrics(&font, start_char + i, &advance, &leftSideBearing);
      bmp_width += x1 - x0;
      bmp_height = std::max(bmp_height, static_cast<uint16_t>(y1 - y0));
    }

    // Calculate the size of the texture atlas.
    bmp_width = mgl::math::next_power_of_two(bmp_width);
    bmp_height = mgl::math::next_power_of_two(bmp_height);

    // Create the glyph list.
    f->glyphs.resize(num_chars);
    f->bitmap = mgl::create_ref<image>(bmp_width, bmp_height, 4);

    // Fill the texture atlas.
    uint16_t x = 0;
    for(uint16_t i = 0; i < num_chars; ++i)
    {
      int x0, y0, x1, y1;
      const int glyph_index_in_font = stbtt_FindGlyphIndex(&font, start_char + i);
      stbtt_GetCodepointBitmapBoxSubpixel(
          &font, glyph_index_in_font, f->scale, f->scale, 0.0f, 0.0f, &x0, &y0, &x1, &y1);

      int advance, leftSideBearing;
      stbtt_GetCodepointHMetrics(&font, start_char + i, &advance, &leftSideBearing);

      int width, height;
      int x_offset, y_offset;

      uint8_t* glyph_8bit = stbtt_GetCodepointBitmap(
          &font, f->scale, f->scale, start_char + i, &width, &height, &x_offset, &y_offset);

      stbtt_MakeCodepointBitmapSubpixel(
          &font, glyph_8bit, width, height, width, f->scale, f->scale, 0.0f, 0.0f, start_char + i);

      image glyph_32bit(width, height, 4);
      for(uint16_t j = 0; j < height; ++j)
      {
        for(uint16_t k = 0; k < width; ++k)
        {
          float alpha = glyph_8bit[j * width + k] / 255.0f;
          glyph_32bit.put_pixel(k, j, glm::vec4(1.0f, 1.0f, 1.0f, alpha));
        }
      }

      delete[] glyph_8bit;

      f->bitmap->blit(x, bmp_height - height, glyph_32bit);
      f->glyphs[i].x0 = x;
      f->glyphs[i].y1 = bmp_height - height;
      f->glyphs[i].width = width;
      f->glyphs[i].height = height;
      f->glyphs[i].x_offset = static_cast<float>(x_offset);
      f->glyphs[i].y_offset = static_cast<float>(y_offset);
      f->glyphs[i].x_advance = static_cast<float>(advance);
      f->glyphs[i].uv0 = glm::vec2(static_cast<float>(x) / bmp_width,
                                   static_cast<float>(bmp_height - height) / bmp_height);
      f->glyphs[i].uv1 = glm::vec2(static_cast<float>(x + width) / bmp_width, 1.0f);

      x += width;
    }

    return mgl::ref<font_face>(f);
  }

  ttf_font::ttf_font(const uint8_buffer& data)
      : m_data(data)
  { }

} // namespace mgl::registry::loaders