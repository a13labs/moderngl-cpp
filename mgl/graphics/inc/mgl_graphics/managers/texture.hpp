#pragma once

#include "mgl_graphics/manager.hpp"
#include "mgl_graphics/texture.hpp"

namespace mgl::graphics
{

  class texture_manager : public manager<texture_ref>
  {
public:
    texture_manager() = default;
    ~texture_manager() = default;

    virtual void on_add(const texture_ref& texture, const std::string& name) override final
    {
      texture->load();
    }
    virtual void on_remove(const texture_ref& texture, const std::string& name) override final
    {
      texture->unload();
    }

    static texture_manager& instance()
    {
      static texture_manager instance;
      return instance;
    }
  };

  inline texture_manager& textures()
  {
    return texture_manager::instance();
  }

} // namespace mgl::graphics