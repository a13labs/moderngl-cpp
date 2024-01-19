#pragma once

#include "mgl_graphics/manager.hpp"
#include "mgl_graphics/texture.hpp"

namespace mgl::graphics
{

  class texture_manager : public manager<texture_ref>
  {
public:
    texture_manager();
    ~texture_manager();

    virtual void on_add(const texture_ref& texture) override final;
    virtual void on_remove(const texture_ref& texture) override final;

    static texture_manager& instance();
  };

  inline texture_manager& current_texture_manager()
  {
    return texture_manager::instance();
  }

} // namespace mgl::graphics