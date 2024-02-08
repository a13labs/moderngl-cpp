#pragma once

#include "mgl_core/memory.hpp"

namespace mgl::opengl
{
  class conditional_render
  {
public:
    ~conditional_render() = default;

    void release();

private:
    friend class context;
    conditional_render();
  };

  using conditional_render_ref = mgl::ref<conditional_render>;
} // namespace  mgl::opengl