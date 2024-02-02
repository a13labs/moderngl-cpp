#include "mgl_graphics/managers/font.hpp"
#include "mgl_core/debug.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"

namespace mgl::graphics
{
  void font_manager::on_add(const mgl::registry::font_ref& font, const std::string& name)
  {
    MGL_CORE_ASSERT(m_font_cache.find(name) == m_font_cache.end(), "Font already exists");

    auto atlas = mgl::create_ref<font_atlas>(font);
    m_font_cache[name] = { atlas, nullptr };

    // Create the texture
    m_font_cache[name].texture = mgl::create_ref<texture2d>(atlas->bitmap());
    m_font_cache[name].texture->load();
  }

  void font_manager::on_remove(const mgl::registry::font_ref& font, const std::string& name)
  {
    MGL_CORE_ASSERT(m_font_cache.find(name) != m_font_cache.end(), "Font does not exist");
    m_font_cache[name].atlas = nullptr;
    m_font_cache[name].texture->unload();
    m_font_cache[name].texture = nullptr;
    m_font_cache.erase(name);
  }

  const font_atlas_ref& font_manager::get_atlas(const std::string& name) const
  {
    MGL_CORE_ASSERT(m_font_cache.find(name) != m_font_cache.end(), "Font does not exist");
    return m_font_cache.at(name).atlas;
  }

  const texture2d_ref& font_manager::get_texture(const std::string& name) const
  {
    MGL_CORE_ASSERT(m_font_cache.find(name) != m_font_cache.end(), "Font does not exist");
    return m_font_cache.at(name).texture;
  }
} // namespace mgl::graphics