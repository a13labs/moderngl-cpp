#pragma once

#include "mgl_graphics/manager.hpp"
#include "mgl_graphics/shader.hpp"

namespace mgl::graphics
{
  class shader_manager : public manager<shader_ref>
  {
public:
    shader_manager() = default;
    ~shader_manager() = default;

    virtual void on_add(const shader_ref& shader, const std::string& name) override final
    {
      shader->load();
    }
    virtual void on_remove(const shader_ref& shader, const std::string& name) override final
    {
      shader->unload();
    }

    static shader_manager& instance()
    {
      static shader_manager instance;
      return instance;
    }
  };

  inline shader_manager& shaders()
  {
    return shader_manager::instance();
  }
} // namespace mgl::graphics