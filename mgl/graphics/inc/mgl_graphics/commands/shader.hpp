#pragma once

#include "mgl_graphics/command.hpp"
#include "mgl_graphics/shader.hpp"

namespace mgl::graphics
{
  class enable_shader : public render_command
  {
public:
    enable_shader(mgl::graphics::shader_ref material)
        : m_shader(material)
    { }

    void execute() override final;

private:
    mgl::graphics::shader_ref m_shader;
  };

  class disable_shader : public render_command
  {
public:
    disable_shader() { }

    void execute() override final;
  };

} // namespace mgl::graphics
