#pragma once

#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"
#include "mgl_window/api.hpp"

namespace mgl::graphics
{
  class shader
  {
public:
    virtual ~shader() = default;

    virtual void bind();
    virtual void unbind();

    virtual void prepare() = 0;
    virtual const mgl::window::api::program_ref& native() = 0;
    virtual const mgl::string_list& attributes() = 0;
    virtual void load() = 0;
    virtual void unload() = 0;
  };

  using shader_ref = mgl::ref<shader>;
} // namespace mgl::graphics
