#pragma once

#include "mgl_core/containers.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_graphics/buffers/vertex.hpp"
#include "mgl_registry/resource.hpp"
#include "mgl_registry/resources/font.hpp"
#include "mgl_registry/resources/image.hpp"

namespace mgl::graphics
{
  class font_atlas
  {
    struct size_info
    {
      int32_t top;
      mgl::list<mgl::registry::font::glyph> glyphs;
    };

public:
    font_atlas(const mgl::registry::font_ref& font, mgl::list<int32_t> pixel_heights = { 32 });

    font_atlas(const font_atlas&) = delete;

    font_atlas(font_atlas&&) = delete;

    font_atlas& operator=(const font_atlas&) = delete;

    font_atlas& operator=(font_atlas&&) = delete;

    int32_t width() const { return m_width; }

    int32_t height() const { return m_height; }

    const mgl::registry::image_ref& bitmap() const { return m_bitmap; }

    int32_t glypys_top(int32_t pixel_height) const;

    const mgl::list<mgl::registry::font::glyph>& glyphs(int32_t pixel_height) const;

    void text_to_vertices(const glm::vec2& pos,
                          const std::string& text,
                          float32_buffer& out,
                          float sx = 1.0,
                          float sy = 1.0) const;

    void text_to_vertices(const glm::vec2& pos,
                          const std::string& text,
                          vertex_buffer_ref& buffer,
                          int32_t& vertices,
                          float sx = 1.0,
                          float sy = 1.0) const;

private:
    glm::vec4* text_to_vertices(const glm::vec2& pos,
                                const std::string& text,
                                float sx,
                                float sy,
                                int32_t pixel_height,
                                int32_t& vertices) const;

private:
    mgl::registry::font_ref m_font;
    int32_t m_width, m_height;
    mgl::registry::image_ref m_bitmap;
    mgl::unordered_map<int32_t, font_atlas::size_info> m_sizes_info;
  };

  using font_atlas_ref = mgl::ref<font_atlas>;

} // namespace mgl::graphics
