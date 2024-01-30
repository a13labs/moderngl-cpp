#pragma once

#include "mgl_core/memory.hpp"
#include "mgl_registry/resource.hpp"
#include "mgl_registry/resources/image.hpp"

namespace mgl::registry
{
  class font_loader;

  class font : public resource
  {
public:
    struct glyph
    {
      uint16_t x, y;
      uint16_t width, height;
      float x_offset, y_offset;
      float x_advance;
    };

    font() = default;
    virtual ~font() = default;

    font(const font&) = delete;
    font(font&&) = delete;
    font& operator=(const font&) = delete;
    font& operator=(font&&) = delete;

    virtual resource::type get_type() const override { return resource::type::font; }

    virtual int32_t get_ascent() const = 0;

    virtual int32_t get_descent() const = 0;

    virtual int32_t get_line_gap() const = 0;

    virtual glyph get_glyph(uint16_t codepoint, int32_t pixel_height) const = 0;

    virtual glyph
    draw_glyph(int32_t x, int32_t y, uint16_t codepoint, int32_t pixel_height, image& bmp) = 0;

    virtual mgl::list<glyph> draw_glyph_range(int32_t x,
                                              int32_t y,
                                              uint16_t first_codepoint,
                                              uint16_t last_codepoint,
                                              int32_t pixel_height,
                                              image& bmp) = 0;

    virtual void
    draw_text(int32_t x, int32_t y, const std::string& text, int32_t pixel_height, image& bmp) = 0;

    virtual size get_size(const std::string& text, int32_t pixel_height) const = 0;

    virtual size
    get_size(uint16_t first_codepoint, uint16_t last_codepoint, int32_t pixel_height) const = 0;

    glyph draw_glyph(int32_t x, int32_t y, uint16_t codepoint, int32_t pixel_height, image_ref& bmp)
    {
      return draw_glyph(x, y, codepoint, pixel_height, *bmp);
    }

    mgl::list<glyph> draw_glyph_range(int32_t x,
                                      int32_t y,
                                      uint16_t first_codepoint,
                                      uint16_t last_codepoint,
                                      int32_t pixel_height,
                                      image_ref& bmp)
    {
      return draw_glyph_range(x, y, first_codepoint, last_codepoint, pixel_height, *bmp);
    }

    void
    draw_text(int32_t x, int32_t y, const std::string& text, int32_t pixel_height, image_ref& bmp)
    {
      draw_text(x, y, text, pixel_height, *bmp);
    }

private:
  };

  using font_ref = mgl::ref<font>;

} // namespace mgl::registry
