#pragma once

#include "mgl_core/string.hpp"
#include "mgl_graphics/shader.hpp"
#include "mgl_window/api/opengl.hpp"

namespace mgl::graphics::builtins
{
  class text_shader : public mgl::graphics::shader
  {
public:
    text_shader() = default;

    virtual void prepare() override final;
    virtual mgl::string_list& attributes() override final
    {
      static mgl::string_list attributes = { "vertex" };
      return attributes;
    }
    virtual void load() override final;
  };

} // namespace mgl::graphics::builtins