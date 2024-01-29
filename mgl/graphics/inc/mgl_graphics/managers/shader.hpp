#pragma once

#include "mgl_graphics/manager.hpp"
#include "mgl_graphics/shader.hpp"

namespace mgl::graphics
{
  class shader_manager : public manager<shader_ref>
  {
public:
    shader_manager();
    ~shader_manager();

    virtual bool on_add(const shader_ref& shader) override final;
    virtual void on_remove(const shader_ref& shader) override final;

    static shader_manager& instance();
  };

  inline shader_manager& current_shader_manager()
  {
    return shader_manager::instance();
  }
} // namespace mgl::graphics