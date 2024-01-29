#pragma once

#include "glm/glm.hpp"
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
      uint16_t x0, y1;
      uint16_t width, height;
      float x_offset, y_offset;
      float x_advance;
      glm::vec2 uv0, uv1;
    };

    using glyph_list = mgl::list<glyph>;

    struct glyph_range
    {
      uint16_t start;
      uint16_t end;
      glyph_list glyphs;
    };

    using glyph_range_list = mgl::list<glyph_range>;

    class glyph_lookup_table
    {
  public:
      glyph_lookup_table() = default;

      glyph_lookup_table(const glyph_lookup_table& other)
          : m_lookup_table(other.m_lookup_table)
      { }

      ~glyph_lookup_table() = default;

      const glyph& operator[](uint16_t codepoint)
      {
        for(auto& range : m_lookup_table)
        {
          if(codepoint >= range.start && codepoint <= range.end)
          {
            return range.glyphs[codepoint - range.start];
          }
        }
        MGL_CORE_ASSERT(false, "Codepoint not found in glyph lookup table");
        return m_lookup_table[0].glyphs[0];
      }

      glyph_lookup_table& operator=(const glyph_lookup_table& other)
      {
        m_lookup_table = other.m_lookup_table;
        return *this;
      }

      void resize(size_t size) { m_lookup_table.resize(size); }

      void add_range(const glyph_range& range) { m_lookup_table.push_back(range); }

      void push_glyph(uint16_t codepoint, const glyph& glyph)
      {
        for(auto& range : m_lookup_table)
        {
          if(codepoint >= range.start && codepoint <= range.end)
          {
            range.glyphs.push_back(glyph);
            return;
          }
        }
        MGL_CORE_ASSERT(false, "Codepoint not found in glyph lookup table");
      }

      void update_glyphs_coordinates(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
      {
        for(auto& range : m_lookup_table)
        {
          for(auto& glyph : range.glyphs)
          {
            glyph.x0 += x;
            glyph.y1 += y;
            glyph.uv0.x += static_cast<float>(x) / static_cast<float>(width);
            glyph.uv0.y += static_cast<float>(y) / static_cast<float>(height);
            glyph.uv1.x += static_cast<float>(x) / static_cast<float>(width);
            glyph.uv1.y += static_cast<float>(y) / static_cast<float>(height);
          }
        }
      }

  private:
      glyph_range_list m_lookup_table;
    };

    using glyph_lookup_table_ref = mgl::ref<glyph_lookup_table>;

    struct face
    {
      image_ref bitmap;
      float scale;
      int ascent, descent, line_gap;
      glyph_lookup_table glyph_lookup_table;
    };

    using face_ref = mgl::ref<face>;

    font() { }

    virtual ~font() = default;

    font(const font&) = delete;
    font(font&&) = delete;
    font& operator=(const font&) = delete;
    font& operator=(font&&) = delete;

    virtual resource::type get_type() const override { return resource::type::font; }

    virtual face_ref get_face() const = 0;

    virtual uint16_t get_pixel_height() const = 0;

private:
  };

  using font_ref = mgl::ref<font>;

} // namespace mgl::registry
