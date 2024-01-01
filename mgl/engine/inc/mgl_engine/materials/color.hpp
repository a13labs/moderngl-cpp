#pragma once

#include "mgl_engine/material.hpp"

namespace mgl::engine
{
  class color_material : public material
  {
public:
    color_material(const glm::vec4& color);

    virtual void prepare() override final;

    virtual const mgl::window::program_ref& program() override final;

private:
    glm::vec4 m_color;
  };

} // namespace mgl::engine
