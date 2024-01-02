#pragma once
#include "shader.hpp"

#include "mgl_core/memory.hpp"
#include "mgl_window/api.hpp"

namespace mgl::engine
{
  class material : public shader
  {
public:
    virtual ~material() = default;
  };

  using material_ref = mgl::ref<material>;

} // namespace mgl::engine