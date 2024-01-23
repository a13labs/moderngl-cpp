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
    virtual ~gui() = default;

    virtual void prepare() override final;
    virtual mgl::string_list& attributes() override final;
    virtual void load() override final;
  };

} // namespace mgl::graphics::shaders