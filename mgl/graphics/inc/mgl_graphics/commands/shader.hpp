#pragma once

#include "mgl_graphics/render.hpp"

namespace mgl::graphics
{
  class enable_shader : public render_command
  {
public:
    enable_shader(mgl::graphics::shader_ref material)
        : m_material(material)
    { }

    void execute() override final;

private:
    mgl::graphics::shader_ref m_material;
  };

  class disable_shader : public render_command
  {
public:
    disable_shader() { }

    void execute() override final;
  };

} // namespace mgl::graphics
