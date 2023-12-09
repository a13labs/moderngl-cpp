#pragma once

#include "mgl_core/containers.hpp"

namespace mgl::opengl
{
  struct color_mask
  {
    bool r, g, b, a;
  };

  using color_masks = mgl::core::list<color_mask>;

} // namespace mgl::opengl
