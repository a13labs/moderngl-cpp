#pragma once

#include "mgl_graphics/pipeline.hpp"

namespace mgl::graphics::builtins
{
  class gui_pipeline : public mgl::graphics::pipeline
  {
public:
    gui_pipeline() = default;

    virtual void prepare() override final;
    virtual void load() override final;
  };

} // namespace mgl::graphics::builtins