#pragma once

#include "mgl_graphics/pipeline.hpp"

namespace mgl::graphics::builtins
{
  class text_pipeline : public mgl::graphics::pipeline
  {
public:
    text_pipeline() = default;

    virtual void prepare() override final;
    virtual void load() override final;
  };

} // namespace mgl::graphics::builtins