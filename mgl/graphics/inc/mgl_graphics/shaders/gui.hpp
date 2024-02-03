#pragma once

#include "mgl_core/string.hpp"
#include "mgl_graphics/shader.hpp"
#include "mgl_platform/api/opengl.hpp"

namespace mgl::graphics::builtins
{
  class gui_shader : public mgl::graphics::shader
  {
public:
    gui_shader() = default;

    virtual void prepare() override final;
    virtual mgl::string_list& attributes() override final
    {
      static mgl::string_list attributes = { "Position", "UV", "Color" };
      return attributes;
    }
    virtual void load() override final;
  };

} // namespace mgl::graphics::builtins