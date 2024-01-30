#pragma once

#include "mgl_graphics/manager.hpp"
#include "mgl_graphics/texture.hpp"
#include "mgl_registry/resources/font.hpp"

namespace mgl::graphics
{

  class font_manager : public manager<mgl::registry::font_ref>
  {
public:
    font_manager();
    ~font_manager();

    virtual void on_add(const mgl::registry::font_ref& font) override final;
    virtual void on_remove(const mgl::registry::font_ref& font) override final;

    static font_manager& instance();
  };

  inline font_manager& current_font_manager()
  {
    return font_manager::instance();
  }

} // namespace mgl::graphics