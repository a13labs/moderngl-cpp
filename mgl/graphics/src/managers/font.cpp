#include "mgl_graphics/managers/font.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_window/api/opengl.hpp"

namespace mgl::graphics
{
  struct atlas_entry
  {
    int32_t x0, y0;
    int32_t width, height;
    mgl::registry::font::face_ref face;
  };

  static mgl::window::api::texture_2d_ref s_texture_atlas = nullptr;
  static mgl::registry::image_ref s_offline_image = nullptr;
  static mgl::unordered_map<mgl::registry::font_ref, atlas_entry> s_font_atlas_entries;

  font_manager::font_manager() { }

  font_manager::~font_manager()
  {
    s_texture_atlas = nullptr;
    s_offline_image = nullptr;
    s_font_atlas_entries.clear();
  }

  bool font_manager::on_add(const mgl::registry::font_ref& font)
  {
    MGL_CORE_ASSERT(font != nullptr, "Font is nullptr");
    if(s_font_atlas_entries.find(font) != s_font_atlas_entries.end())
    {
      return false;
    }

    auto face = font->get_face();

    // If the atlas image is not created yet, create it. Otherwise, resize it. Then, blit the
    // current glyph to the atlas image.
    if(s_offline_image == nullptr)
    {
      s_offline_image = face->bitmap->clone();
    }
    else
    {
      int32_t new_width = std::max(s_offline_image->width(), face->bitmap->width());
      int32_t new_height = s_offline_image->height() + face->bitmap->height();

      s_offline_image->resize(new_width, new_height);
      s_offline_image->blit(0, s_offline_image->height() - face->bitmap->height(), *face->bitmap);
    }

    // If the texture atlas is not created yet, create it.
    if(s_texture_atlas == nullptr)
    {
      s_texture_atlas = mgl::window::api::create_texture_2d(s_offline_image->width(),
                                                            s_offline_image->height(),
                                                            s_offline_image->channels(),
                                                            face->bitmap->buffer());
    }
    else
    {
      s_texture_atlas->resize(s_offline_image->width(),
                              s_offline_image->height(),
                              s_offline_image->channels(),
                              s_offline_image->buffer());
    }

    // Update the glyph lookup table to match the new texture atlas.
    face->glyph_lookup_table.update_glyphs_coordinates(0,
                                                       s_offline_image->height() -
                                                           face->bitmap->height(),
                                                       s_offline_image->width(),
                                                       s_offline_image->height());

    // Update the atlas entry.
    s_font_atlas_entries[font] = atlas_entry{ 0,
                                              s_offline_image->height() - face->bitmap->height(),
                                              face->bitmap->width(),
                                              face->bitmap->height(),
                                              face };

    return true;
  }

  void font_manager::on_remove(const mgl::registry::font_ref& font)
  {
    MGL_CORE_ASSERT(font != nullptr, "Font is nullptr");
    if(s_font_atlas_entries.find(font) != s_font_atlas_entries.end())
    {
      return;
    }

    auto atlas_entry = s_font_atlas_entries[font];

    auto new_offline_atlas =
        mgl::create_ref<mgl::registry::image>(s_offline_image->width() - atlas_entry.width,
                                              s_offline_image->height() - atlas_entry.height,
                                              s_offline_image->channels());

    // Blit the top part of the atlas image.
    new_offline_atlas->blit(
        0,
        0,
        s_offline_image,
        { 0, 0, atlas_entry.x0 + atlas_entry.width, atlas_entry.y0 + atlas_entry.height });

    if(atlas_entry.y0 + atlas_entry.height < s_offline_image->height())
    {
      // Blit the bottom part of the atlas image.
      new_offline_atlas->blit(atlas_entry.x0,
                              atlas_entry.y0,
                              s_offline_image,
                              { atlas_entry.x0 + atlas_entry.width,
                                atlas_entry.y0 + atlas_entry.height,
                                s_offline_image->width(),
                                s_offline_image->height() });
    }

    bool do_coordinates_update = false;
    for(auto& [font, entry] : s_font_atlas_entries)
    {
      if(font == font)
      {
        do_coordinates_update = true;
      }

      if(do_coordinates_update)
      {
        entry.y0 -= atlas_entry.height;
        entry.face->glyph_lookup_table.update_glyphs_coordinates(
            0, entry.y0, s_offline_image->width(), s_offline_image->height());
      }
    }

    // Update the texture atlas.
    s_texture_atlas->resize(new_offline_atlas->width(),
                            new_offline_atlas->height(),
                            new_offline_atlas->channels(),
                            new_offline_atlas->buffer());

    // Remove the font from the atlas entries.
    s_font_atlas_entries.erase(font);
  }

  font_manager& font_manager::instance()
  {
    static font_manager instance;
    return instance;
  }
} // namespace mgl::graphics