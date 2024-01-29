#pragma once

#include "mgl_registry/loader.hpp"
#include "mgl_registry/resources/font.hpp"

namespace mgl::registry::loaders
{
  class font_loader;

  struct font_loader_options : public loader_options
  {
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

  class ttf_font : public font
  {
public:
    ttf_font(const uint8_buffer& data);
    virtual ~ttf_font() = default;

    virtual font_face_ref
    bitmap(uint16_t start_char, uint16_t num_chars, uint16_t pixel_height) const override;

private:
    friend class font_loader;

    uint8_buffer m_data;
  };

  using ttf_font_ref = mgl::ref<ttf_font>;
  using font_loader_ref = mgl::scope<font_loader>;
} // namespace mgl::registry::loaders