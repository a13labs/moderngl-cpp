#pragma once

#include "mgl_engine/render.hpp"

namespace mgl::engine
{
  class enable_material : public render_command
  {
public:
    enable_material(mgl::engine::material_ref material)
        : render_command(type::ENABLE_SHADER)
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
        : render_command(type::DISABLE_SHADER)
    { }

    void execute() override final;
  };

} // namespace mgl::engine
