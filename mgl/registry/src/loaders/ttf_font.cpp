#include "mgl_registry/loaders/ttf_font.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_core/math.hpp"
#include "mgl_core/memory.hpp"
#include "stb/stb_truetype.h"

namespace mgl::registry::loaders
{
  static font_loader_options default_font_loader_options;

  mgl::string_list font_loader::get_extensions() const
  {
    return { "ttf", "otf" };
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

  font_bitmap_ref
  ttf_font::bitmap(uint16_t start_char, uint16_t num_chars, uint16_t font_size) const
  {
    stbtt_fontinfo font;
    if(!stbtt_InitFont(&font, m_data.data(), 0))
    {
      MGL_CORE_ERROR("Failed to load font");
      return nullptr;
    }

    // Calculate the size of the texture atlas.
    uint16_t bmp_width = 0;
    uint16_t bmp_height = 0;
    for(uint16_t i = 0; i < num_chars; ++i)
    {
      int width, height, x_offset, y_offset;
      stbtt_GetCodepointBitmapBox(
          &font, start_char + i, font_size, font_size, &x_offset, &y_offset, &width, &height);
      bmp_width += width;
      bmp_height = std::max(bmp_height, static_cast<uint16_t>(height));
    }

    // Calculate the size of the texture atlas.
    bmp_width = mgl::math::next_power_of_two(bmp_width);
    bmp_height = mgl::math::next_power_of_two(bmp_height);

    // Create the texture atlas.
    uint8_buffer atlas(bmp_width * bmp_height);
    std::fill(atlas.begin(), atlas.end(), 0);

    auto font_bmp = new font_bitmap();

    // Create the glyph list.
    font_bmp->font_size = font_size;
    font_bmp->glyphs.resize(num_chars);
    font_bmp->image = mgl::create_ref<image>(bmp_width, bmp_height, 4);

    // Fill the texture atlas.
    uint16_t x = 0;
    for(uint16_t i = 0; i < num_chars; ++i)
    {
      int width, height, x_offset, y_offset;

      stbtt_GetCodepointBitmapBox(
          &font, start_char + i, font_size, font_size, &x_offset, &y_offset, &width, &height);

      image glyph_32bit(width, height, 4);
      uint8_t glyph_8bit[width * height];

      stbtt_MakeCodepointBitmap(
          &font, glyph_8bit, width, height, width, 1.0f, 1.0f, start_char + i);

      for(uint16_t j = 0; j < height; ++j)
      {
        for(uint16_t k = 0; k < width; ++k)
        {
          glyph_32bit.put_pixel(
              k, j, glm::vec4(1.0f, 1.0f, 1.0f, glyph_8bit[j * width + k] / 255.0f));
        }
      }

      font_bmp->image->blit(x, bmp_height - height, glyph_32bit);
      font_bmp->glyphs[i].x0 = x;
      font_bmp->glyphs[i].y1 = bmp_height - height;
      font_bmp->glyphs[i].width = width;
      font_bmp->glyphs[i].height = height;
      font_bmp->glyphs[i].x_offset = static_cast<float>(x_offset);
      font_bmp->glyphs[i].y_offset = static_cast<float>(y_offset);
      font_bmp->glyphs[i].x_advance =
          stbtt_GetCodepointKernAdvance(&font, start_char + i, start_char + i + 1);
      font_bmp->glyphs[i].uv0 = glm::vec2(static_cast<float>(x) / bmp_width,
                                          static_cast<float>(bmp_height - height) / bmp_height);
      font_bmp->glyphs[i].uv1 = glm::vec2(static_cast<float>(x + width) / bmp_width,
                                          static_cast<float>(bmp_height) / bmp_height);

      x += width;
    }

    return mgl::ref<font_bitmap>(font_bmp);
  }

  ttf_font::ttf_font(const uint8_buffer& data)
      : m_data(data)
  { }

} // namespace mgl::registry::loaders