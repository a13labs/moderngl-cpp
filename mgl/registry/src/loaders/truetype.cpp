

#include "mgl_registry/loaders/truetype.hpp"
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
  // The default font loader options. ASCII characters from 0x20 to 0xFF and a pixel height of 96.
  static const font_loader_options default_font_loader_options = { { { 0x0020, 0x00FF } }, 96 };

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

    return mgl::create_ref<truetype_font>(buffer, *opts);
  }

  truetype_font::truetype_font(const uint8_buffer& data, const font_loader_options& options)
      : m_data(data)
      , m_options(options)
  { }

  font::face_ref truetype_font::get_face() const
  {
    stbtt_fontinfo font;
    if(!stbtt_InitFont(&font, m_data.data(), 0))
    {
      MGL_CORE_ERROR("Failed to load font");
      return nullptr;
    }

    font::face_ref f = mgl::create_ref<font::face>();

    f->scale = stbtt_ScaleForPixelHeight(&font, m_options.pixel_height);

    // Calculate the ascent, descent and line gap.
    stbtt_GetFontVMetrics(&font, &f->ascent, &f->descent, &f->line_gap);

    int baseline = static_cast<int32_t>(f->ascent * f->scale);

    // Calculate the size of the texture atlas.
    int32_t bmp_width = 0;
    int32_t bmp_height = 0;
    int32_t num_chars = 0;
    for(const auto& range : m_options.glyph_ranges)
    {
      for(int32_t i = 0; i < range.end - range.start; ++i, ++num_chars)
      {
        int x0, y0, x1, y1;
        const int glyph_index_in_font = stbtt_FindGlyphIndex(&font, range.start + i);
        stbtt_GetCodepointBitmapBox(
            &font, glyph_index_in_font, f->scale, f->scale, &x0, &y0, &x1, &y1);
        int advance, leftSideBearing;
        stbtt_GetCodepointHMetrics(&font, range.start + i, &advance, &leftSideBearing);
        bmp_width += static_cast<int32_t>(advance * f->scale);
        bmp_height = std::max(bmp_height, y1 - y0);
      }
    }

    // Calculate the size of the texture atlas.
    bmp_width = mgl::math::next_power_of_two(bmp_width);
    bmp_height = mgl::math::next_power_of_two(bmp_height);

    // Create the glyph list.
    f->glyph_lookup_table.resize(m_options.glyph_ranges.size());
    f->bitmap = mgl::create_ref<image>(bmp_width, bmp_height, 4);

    // Fill the texture atlas.
    uint32_t x = 0;
    for(const auto& range : m_options.glyph_ranges)
    {
      f->glyph_lookup_table.add_range({ range.start, range.end, {} });
      for(int32_t i = 0; i < range.end - range.start; ++i)
      {
        int x0, y0, x1, y1;
        const int glyph_index_in_font = stbtt_FindGlyphIndex(&font, range.start + i);
        stbtt_GetCodepointBitmapBoxSubpixel(
            &font, glyph_index_in_font, f->scale, f->scale, 0.0f, 0.0f, &x0, &y0, &x1, &y1);

        int advance, leftSideBearing;
        stbtt_GetCodepointHMetrics(&font, range.start + i, &advance, &leftSideBearing);

        int width, height;
        int x_offset, y_offset;

        uint8_t* glyph_8bit = stbtt_GetCodepointBitmap(
            &font, f->scale, f->scale, range.start + i, &width, &height, &x_offset, &y_offset);

        stbtt_MakeCodepointBitmapSubpixel(&font,
                                          glyph_8bit,
                                          width,
                                          height,
                                          width,
                                          f->scale,
                                          f->scale,
                                          0.0f,
                                          0.0f,
                                          range.start + i);

        image glyph_32bit(width, height, 4);
        for(int32_t j = 0; j < height; ++j)
        {
          for(int32_t k = 0; k < width; ++k)
          {
            float alpha = glyph_8bit[j * width + k] / 255.0f;
            glyph_32bit.put_pixel(k, j, glm::vec4(1.0f, 1.0f, 1.0f, alpha));
          }
        }

        delete[] glyph_8bit;

        f->bitmap->blit(x, bmp_height - height, glyph_32bit);
        glyph g;
        g.x0 = x;
        g.y1 = bmp_height - height;
        g.width = width;
        g.height = height;
        g.x_offset = static_cast<float>(x_offset);
        g.y_offset = static_cast<float>(y_offset);
        g.x_advance = static_cast<float>(advance * f->scale);
        g.uv0 = glm::vec2(static_cast<float>(x) / bmp_width,
                          static_cast<float>(bmp_height - height) / bmp_height);
        g.uv1 = glm::vec2(static_cast<float>(x + width) / bmp_width, 1.0f);
        f->glyph_lookup_table.push_glyph(range.start + i, g);

        x += static_cast<int32_t>(advance * f->scale);
      }
    }
    return f;
  }

} // namespace mgl::registry::loaders