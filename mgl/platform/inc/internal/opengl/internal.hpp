#pragma once
#include "mgl_opengl/enums.hpp"
#include "mgl_platform/enums.hpp"

namespace mgl::platform::internal
{
  mgl::opengl::blend_factor to_api(mgl::platform::blend_factor factor);

  mgl::opengl::blend_equation_mode to_api(mgl::platform::blend_equation_mode mode);

  mgl::opengl::render_mode to_api(mgl::platform::render_mode mode);

  mgl::opengl::compare_func to_api(mgl::platform::compare_func func);

} // namespace mgl::platform::internal