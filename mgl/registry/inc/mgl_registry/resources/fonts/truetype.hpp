#pragma once

#include "mgl_registry/loaders/truetype.hpp"
#include "mgl_registry/resources/font.hpp"

struct stbtt_fontinfo;

namespace mgl::registry
{

  class truetype_font : public font
  {
public:
    truetype_font(const uint8_buffer& data);
    truetype_font(const uint8_t* data, size_t size);
    virtual ~truetype_font();

    virtual int32_t get_ascent() const override final { return m_ascent; }

    virtual int32_t get_descent() const override final { return m_descent; }

    virtual int32_t get_line_gap() const override final { return m_line_gap; }

    virtual float get_scale_for_pixel_height(int32_t pixel_height) const override final;

    virtual glyph get_glyph(uint16_t codepoint, int32_t pixel_height) const override final;

    virtual image_ref
    draw_text(int32_t x, int32_t y, const std::string& text, int32_t pixel_height) override final;

    virtual mgl::list<glyph> get_glyphs(int32_t x,
                                        int32_t y,
                                        uint16_t first_codepoint,
                                        uint16_t last_codepoint,
                                        int32_t pixel_height,
                                        image& bmp,
                                        int32_t padding = 3) override final;

private:
    size measure_text(const std::string& text, int32_t pixel_height) const;

private:
    stbtt_fontinfo* m_font;
    uint8_buffer m_data;
    int32_t m_ascent;
    int32_t m_descent;
    int32_t m_line_gap;
    uint32_t m_unique_id;
  };

  using truetype_font_ref = mgl::ref<truetype_font>;
} // namespace mgl::registry