#pragma once

#include "mgl_graphics/atlas/font.hpp"
#include "mgl_graphics/manager.hpp"
#include "mgl_graphics/textures.hpp"
#include "mgl_registry/resources/font.hpp"

namespace mgl::graphics
{

  class font_manager : public manager<mgl::registry::font_ref>
  {
private:
    struct font_info
    {
      font_atlas_ref atlas;
      texture2d_ref texture;
    };

public:
    font_manager() = default;
    ~font_manager() = default;

    virtual void on_add(const mgl::registry::font_ref& font,
                        const std::string& name) override final;
    virtual void on_remove(const mgl::registry::font_ref& font,
                           const std::string& name) override final;

    const font_atlas_ref& get_atlas(const std::string& name) const;
    const texture2d_ref& get_texture(const std::string& name) const;

    static font_manager& instance()
    {
      static font_manager instance;
      return instance;
    }

private:
    mgl::unordered_map<std::string, font_manager::font_info> m_font_cache;
  };

  inline font_manager& fonts()
  {
    return font_manager::instance();
  }

} // namespace mgl::graphics