#pragma once

#include "mgl_graphics/render.hpp"

namespace mgl::graphics
{
  class enable_material : public render_command
  {
public:
    enable_material(mgl::graphics::material_ref material)
        : m_material(material)
    { }

    void execute() override final;

private:
    mgl::graphics::material_ref m_material;
  };

  class disable_material : public render_command
  {
public:
    disable_material() { }

    void execute() override final;
  };

} // namespace mgl::graphics
