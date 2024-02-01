#include "mgl_graphics/managers/font.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"

namespace mgl::graphics
{
  struct font_info
  {
    mgl::registry::font_atlas_ref atlas;
    mgl::window::api::texture_2d_ref texture;
  };

  mgl::unordered_map<std::string, font_info> s_font_cache;

  font_manager::font_manager() { }

  font_manager::~font_manager() { }

  void font_manager::on_add(const mgl::registry::font_ref& font, const std::string& name)
  {
    MGL_CORE_ASSERT(s_font_cache.find(name) == s_font_cache.end(), "Font already exists");

    auto atlas = mgl::create_ref<mgl::registry::font_atlas>(font);
    s_font_cache[name] = { atlas, nullptr };

    // Create the texture
    s_font_cache[name].texture = mgl::window::api::create_texture_2d(
        atlas->width(), atlas->height(), 4, atlas->bitmap()->buffer());
  }

  void font_manager::on_remove(const mgl::registry::font_ref& font, const std::string& name)
  {
    MGL_CORE_ASSERT(s_font_cache.find(name) != s_font_cache.end(), "Font does not exist");
    s_font_cache[name].texture = nullptr;
    s_font_cache.erase(name);
  }

  font_manager& font_manager::instance()
  {
    static font_manager instance;
    return instance;
  }
} // namespace mgl::graphics