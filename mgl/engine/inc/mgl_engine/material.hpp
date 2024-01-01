#pragma once

#include "mgl_core/memory.hpp"
#include "mgl_window/api.hpp"

namespace mgl::engine
{
  class material
  {
public:
    virtual ~material() = default;

    virtual void prepare() = 0;

    virtual const mgl::window::program_ref& program() = 0;
  };

  using material_ref = mgl::ref<material>;

} // namespace mgl::engine