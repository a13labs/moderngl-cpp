#pragma once

#include "glm/glm.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_registry/resource.hpp"
#include "mgl_registry/resources/image.hpp"

namespace mgl::registry
{
  class font_loader;

  struct glyph
  {
    uint16_t x0, y1;
    uint16_t width, height;
    float x_offset, y_offset;
    float x_advance;
    glm::vec2 uv0, uv1;
  };

  struct font_face
  {
    image_ref bitmap;
    float scale;
    int ascent, descent, line_gap;
    std::vector<glyph> glyphs;
  };

  using font_face_ref = mgl::ref<font_face>;

  class font : public resource
  {
public:
    font() { }

    virtual ~font() = default;

    font(const font&) = delete;
    font(font&&) = delete;
    font& operator=(const font&) = delete;
    font& operator=(font&&) = delete;

    virtual resource::type get_type() const override { return resource::type::font; }

    virtual font_face_ref
    bitmap(uint16_t start_char, uint16_t num_chars, u_int16_t pixel_height) const = 0;

private:
  };

  using font_ref = mgl::ref<font>;

} // namespace mgl::registry
