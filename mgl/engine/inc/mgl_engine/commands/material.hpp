#pragma once

#include "mgl_engine/renderer.hpp"

namespace mgl::engine
{
  class enable_material : public render_command
  {
public:
    enable_material(mgl::engine::material_ref material)
        : render_command(type::ENABLE_MATERIAL)
        , m_material(material)
    { }

    void execute() override final;

private:
    mgl::engine::material_ref m_material;
  };

  class disable_material : public render_command
  {
public:
    disable_material()
        : render_command(type::DISABLE_MATERIAL)
    { }

    void execute() override final;
  };

} // namespace mgl::engine
