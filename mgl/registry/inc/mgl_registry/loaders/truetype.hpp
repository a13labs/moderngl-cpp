#pragma once

#include "mgl_registry/loader.hpp"
#include "mgl_registry/resources/font.hpp"

namespace mgl::registry::loaders
{
  class font_loader;

  struct font_loader_options : public loader_options
  {
    struct glyph_range
    {
      uint16_t start;
      uint16_t end;
    };

    mgl::list<glyph_range> glyph_ranges;
    uint16_t pixel_height;

    font_loader_options(const mgl::list<glyph_range>& glyph_ranges, uint16_t pixel_height)
        : glyph_ranges(glyph_ranges)
        , pixel_height(pixel_height)
    { }

    virtual ~font_loader_options() = default;
  };

  class font_loader : public loader
  {
public:
    font_loader() = default;
    virtual ~font_loader() = default;

    virtual resource::type get_type() const override { return resource::type::font; }

    virtual mgl::string_list get_extensions() const override;

    virtual resource_ref load(const location_ref& location,
                              const std::string& path,
                              const loader_options& options) override;
  };

  class truetype_font : public font
  {
public:
    truetype_font(const uint8_buffer& data, const font_loader_options& options);
    virtual ~truetype_font() = default;

    virtual font::face_ref get_face() const override;

    virtual uint16_t get_pixel_height() const override { return m_options.pixel_height; }

private:
    font_loader_options m_options;
    uint8_buffer m_data;
  };

  using truetype_font_ref = mgl::ref<truetype_font>;
  using font_loader_ref = mgl::scope<font_loader>;
} // namespace mgl::registry::loaders