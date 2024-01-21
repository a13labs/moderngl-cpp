#pragma once

#include "mgl_core/string.hpp"
#include "mgl_graphics/shader.hpp"
#include "mgl_window/api/opengl.hpp"

namespace mgl::graphics::shaders
{
  class gui : public mgl::graphics::shader
  {
public:
    gui() = default;

    virtual void prepare() override final;
    virtual mgl::window::api::program_ref& native() override final;
    virtual mgl::string_list& attributes() override final;
    virtual void unload() override final;
    virtual void load() override final;

private:
    mgl::window::api::program_ref m_program;
  };

} // namespace mgl::graphics::shaders