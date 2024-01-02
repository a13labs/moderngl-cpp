#pragma once

#include "mgl_core/memory.hpp"
#include "mgl_window/api.hpp"

namespace mgl::engine
{
  class shader
  {
public:
    virtual ~shader() = default;
    virtual void enable();
    virtual void disable();
    virtual void prepare() = 0;
    virtual const mgl::window::program_ref& program() = 0;
  };

  using shader_ref = mgl::ref<shader>;
} // namespace mgl::engine
